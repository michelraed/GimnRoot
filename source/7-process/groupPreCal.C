#include <iostream>
#include <string>
#include <TTree.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TFile.h>
#include <TCanvas.h>
#include <vector>
#include <TMatrixT.h>

#include <mutex>

// std::mutex mtx; // Mutex for synchronizing access

// int MinimumGroupSize = 64;
using namespace std;

// std::vector<std::shared_ptr<TCanvas>>
void groupPreCal(std::string fileName, std::string outFolder)
{
    Float_t cut = 1.0;
    static int counter = 0;
    counter++;
    int event_counter = 0;
    std::cout << " Processing File " << "[" << counter << "] :" << fileName << std::endl;
    // ROOT::EnableThreadSafety();
    //  Simulate processing

    int MinimumGroupSize = 1;
    // cria as variaveis nescessárias à leitura do arquivo root
    UShort_t mh_n{0}, mh_j{0};
    UInt_t channelID{0};
    Float_t energy{0.0};
    Long64_t time{0};
    std::vector<TString> ports;

    Float_t first_e{0.0};
    Float_t first_t{0.0};
    int first_c{-1};
    int chan{-1};
    // abre o arquivo root
    std::unique_ptr<TFile> file(new TFile(fileName.c_str(), "read"));
    std::unique_ptr<TTree> tree((TTree *)file->Get("data"));

    // cria os vetores que armazenarão as informações de tempo e energia dos eventos no grupo para calculo do timing
    std::vector<Long64_t> timeChanels(64, 0);
    std::vector<Float_t> energyChanels(64, 0);

    // set branches
    tree->SetBranchAddress("mh_n", &mh_n);
    tree->SetBranchAddress("mh_j", &mh_j);
    tree->SetBranchAddress("channelID", &channelID);
    tree->SetBranchAddress("energy", &energy);
    tree->SetBranchAddress("time", &time);

    std::vector<int> chipIDs;

    Identificator Id;
    Id.LoadIdentificators("/home/gimn6/Documentos/Michel/produtos_moles/gimnpet/GimnROOT/petsys_conf/identificators.conf");

    auto inCoincidenceWith = Id.getInCoincidenceWith();
    auto reference = Id.getRef();


    // for (auto &val : inCoincidenceWith)
    // {
    //     std::cout << "in coincidence with : " << std::endl;
    //     for (auto &val2 : val)
    //         std::cout << " " << val2 << std::endl;
    // }

    SiPM sipm_tools(64, 8, 8, 3.36, Id, counter);
    // Processa o caminho, para obter somente o nome do arquivo
    auto name = splitString(fileName, '/');
    auto name_pos = name.size();
    string outName = name[name_pos - 1];
    name = splitString(outName, '.');
    name_pos = name.size();
    outName = name[0];
    outName = outName + "_pre.root";

    // cria a classe que irá salvar os arquivos processados no HDD
    outName = outFolder + "/" + outName;

    GimnWriteSimple Output(outName.c_str());

    int pos = 0;
    int auxPos = 0; // posicao do elemento dentro do vetor de detectores e configurações
    float esum = 0; // soma da energia do grupo

    // inicializa a barra de progresso ...
    std::string loading = "[--------------------------------------------------]";
    int load_pos = 1;

    const int entries = tree->GetEntries();

    // Begins the iteration through the events
    for (Int_t event = 0; event <= entries; event++)
    {
        tree->GetEntry(event);
        // std::cout << " *******************************************  " << std::endl;
        // std::cout << " Event  " << event << std::endl;
        // std::cout << " mh_n   " << mh_n << std::endl;
        // std::cout << " mh_j   " << mh_j << std::endl;

        Int_t chipID = (int)channelID / 64;
        Int_t channel = channelID - 64 * chipID;

        // std::cout << " chipID   " << chipID << std::endl;
        // std::cout << " ChannelID   " << channel << std::endl;

        //============================================================================================
        // Loading bar
        if (VERBOSE)
        {
            if (event > ((entries / 50.0) * load_pos))
            {
                auto porcento = ((double)event / (double)entries) * 100;
                auto clear = system("clear");
                loading[load_pos] = '=';
                std::cout << loading << porcento << "%" << std::endl;
                load_pos += 1;
            }
        }
        // =============================================================================================

        // aqui o programa busca qual a posição do chipID no vetor chpid de configuração
        // Primeiramente o programa ira buscar pelo chip ID no vetor de chipID's, se nao encontrar, irá criar um novo;

        auto it = std::find(Id.chipID.begin(), Id.chipID.end(), chipID);

        // Se ele nao entrar aqui ele vai perder a conta colocar o else depois
        if (it != Id.chipID.end())
            pos = std::distance(Id.chipID.begin(), it);

        // se numero de eventos no grupo maior do que um numero minimo


        // CHecar se o chipID é a referencia
        // if (chipID==reference[pos]){

        //     std::cout<<"this chip ID is the reference"<<std::endl;
        //     std::cout<<"chip ID : "<<chipID<<" reference : "<<reference[pos]<<std::endl;
        //     cin.get();
        // }

        if ((mh_n >= MinimumGroupSize) && (!(chipID==reference[pos])))
        {
            
            auto pos_xy = sipm_tools.CorrectChannelToPixelPs(channel);
            // x esta para colunas [0]
            // y para linhas [1]
            // APENAS PARA O PRIMEIRO EVENTO

            // eventos do grupo // Roda também pelo primeiro evento
            if (mh_j < (mh_n - 1))
            {
                if (mh_j == 0) // Pega o primeiro evento dentro do grupo
                {
                    event_counter++;
                    // std::cout << "------------------------" << std::endl;
                    // std::cout << "first event" << std::endl;
                    // std::cout << "------------------------" << std::endl;
                    if (energy > cut)
                    {
                        sipm_tools.Histos.channelEnergyHistogram[pos][channel]->Fill(energy);
                    }
                    // Fills first Event;
                    first_e = energy;
                    first_t = time;
                    first_c = 8 * pos_xy[1] + pos_xy[0];
                    chan = channel;
                }
                sipm_tools.Sipm_e[pos](pos_xy[0], pos_xy[1]) = +energy;
                timeChanels[channel] = time;
                Float_t timeInSeconds = time / 1e12;
                sipm_tools.Sipm_t[pos](pos_xy[0], pos_xy[1]) = +timeInSeconds;
                energyChanels[channel] = energy;
                esum += energy;
            }
            // ultimo evento
            else if (mh_j == mh_n - 1)
            {
                if (mh_j == 0) // Pega o primeiro evento dentro do grupo
                {
                    event_counter++;
                    // std::cout << "------------------------" << std::endl;
                    // std::cout << "first event" << std::endl;
                    // std::cout << "------------------------" << std::endl;
                    if (energy > cut)
                    {
                        sipm_tools.Histos.channelEnergyHistogram[pos][channel]->Fill(energy);
                    }
                    // Fills first Event;
                    first_e = energy;
                    first_t = time;
                    first_c = 8 * pos_xy[1] + pos_xy[0];
                    chan = channel;
                }

                if (first_e > cut)
                {
                    // std::cout << "------------------------" << std::endl;
                    // std::cout << "Last event" << std::endl;
                    // std::cout << "------------------------" << std::endl;
                    sipm_tools.Sipm_e[pos](pos_xy[0], pos_xy[1]) = +energy;
                    esum += energy;

                    timeChanels[channel] = time;
                    Float_t timeInSeconds = time / 1e12;
                    sipm_tools.Sipm_t[pos](pos_xy[0], pos_xy[1]) = +timeInSeconds;
                    energyChanels[channel] = energy;

                    auto Pos3D = anger(sipm_tools.Sipm_e[pos]);

                    sipm_tools.Histos.FloodHist[pos]->Fill(Pos3D[0],Pos3D[1]);
                    // corrige pelo fator de correção encontrado
                    // divideMatrix(Sipm[pos], correctionFactor[pos]);

                    GimnPulse p;
                    p.first_e = first_e; // Highest energy
                    p.first_t = first_t; // time from highest energy event
                    p.first_c = first_c; // Position in the matrix of the highest energy
                    p.chan = chan;       // channel of the highest Energy
                    p.chipID = chipID;
                    p.Port = (int)chipID / 2;
                    p.fillEnergyFromTmatrixT(sipm_tools.Sipm_e[pos]);
                    p.fillTimeFromTmatrixT(sipm_tools.Sipm_t[pos]);
                    Output.putData(p);
                }
                sipm_tools.Sipm_e[pos].Zero();
                sipm_tools.Sipm_t[pos].Zero();
            }
        }
    }

    int cnt = 0;
    for (auto &CID : Id.chipID)
    {

        GimnAQD aqd;
        aqd.chipID = CID;
        aqd.port = Id.port[cnt];
        Output.putDataAQD(aqd);
        cnt++;
    }

    Output.writeObject(sipm_tools.Histos);
    Output.close();

    if (VERBOSE)
        std::cout << "finish" << std::endl;
    std::cout << "o programa detectou " << event_counter << std::endl;
}
// 1748681