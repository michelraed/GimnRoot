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

std::string fileName2 = {"/home/gimn6/Documentos/Michel/produtos_moles/gimnpet/GimnTools/Test_Data/Beg_27112024_600_group.root"};
std::string newFileName = {"/home/gimn6/Documentos/Michel/produtos_moles/gimnpet/GimnTools/Test_Data/Test_first_group.root"};
std::string filename3 = {"/home/gimn6/Documentos/Michel/produtos_moles/gimnpet/GimnTools/Test_Data/Test_first_30s_group.root"};
std::string filename4 = {"/home/gimn6/Documentos/Michel/produtos_moles/gimnpet/GimnTools/Test_Data/Testando_pre_caract_group.root"};
std::string filename5 = {"/home/gimn6/Documentos/Michel/produtos_moles/gimnpet/GimnTools/Test_Data/Teste_manha_group.root"};
// std::string fileName2 = {"/home/michelraed/dev/gimnpet/Beg_27112024_600_group.root"};
std::string fnam = "/home/gimn6/Documentos/Michel/produtos_moles/gimnpet/GimnROOT/Teste_pre_aqd_2_group.root";
// std::vector<std::shared_ptr<TCanvas>>
void processGroupPre(std::string fileName, std::string outFolder)
{
    static int counter = 0;
    counter++;


    std::cout<<" Processing File "<<"["<<counter<<"] :"<<fileName<<std::endl;
    ROOT::EnableThreadSafety();
    // Simulate processing
    
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

    // AQUI TEM PROBLEMA
    


    /// REMOVER UTILIZACAO DISTO 
    Identificator Id;
    Id.LoadIdentificators("/home/gimn6/Documentos/Michel/produtos_moles/gimnpet/GimnROOT/petsys_conf/identificators.conf");


    SiPM sipm_tools(64, 8, 8, 3.36, Id,counter);
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

    GimnWriteClass Output(outName.c_str());

    int pos = 0;    // posicao do elemento dentro do vetor de detectores e configurações
    float esum = 0; // soma da energia do grupo

    // inicializa a barra de progresso ...
    std::string loading = "[--------------------------------------------------]";
    int load_pos = 1;

    const int entries = tree->GetEntries();
    for (Int_t event = 0; event <= entries; event++)
    {
        tree->GetEntry(event);
        // std::cout << " *******************************************  " << std::endl;
        // std::cout << " Event  " << event << std::endl;
        // std::cout << " mh_n   " << mh_n << std::endl;
        // std::cout << " mh_j   " << mh_j << std::endl;

        Int_t chipID = (int)channelID / 64;
        Int_t channel = channelID - 64 * chipID;

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

        // aqui o programa busca qual a posição do chipID no vetor chpid de configuração
        auto it = std::find(Id.chipID.begin(), Id.chipID.end(), chipID);
        if (it != Id.chipID.end())
            pos = std::distance(Id.chipID.begin(), it);

        // se monolitico
        if (Id.crystalType[pos] == "mono")
        {

            // se numero de eventos no grupo maior do que um numero minimo
            if (mh_n >= MinimumGroupSize)
            {
                auto pos_xy = sipm_tools.CorrectChannelToPixelPs(channel);

                if (mh_j == 0)
                {
                    if (energy > 0.75)
                    {
                        sipm_tools.Histos.channelEnergyHistogram[pos][channel]->Fill(energy);
                        // Fills first Event;
                        Float_t timeInSeconds = time / 1e12;
                        sipm_tools.Sipm_t[pos](pos_xy[1], pos_xy[0]) = +timeInSeconds;
                        first_e = energy;
                        first_t = time;
                        first_c = pos_xy[0] * 8 + pos_xy[1];
                        chan = channel;
                    }
                }

                // eventos do grupo
                if (mh_j < (mh_n - 1))
                {

                    if (energy > 0)
                    {
                        sipm_tools.Sipm_e[pos](pos_xy[1], pos_xy[0]) = +energy;
                        timeChanels[channel] = time;
                        Float_t timeInSeconds = time / 1e12;
                        sipm_tools.Sipm_t[pos](pos_xy[1], pos_xy[0]) = +timeInSeconds;
                        energyChanels[channel] = energy;
                        esum += energy;
                    }
                }
                // ultimo evento
                else if (mh_j == mh_n - 1)
                {

                    if (energy > 0)
                    {

                        sipm_tools.Sipm_e[pos](pos_xy[1], pos_xy[0]) = +energy;
                        esum += energy;

                        timeChanels[channel] = time;
                        Float_t timeInSeconds = time / 1e12;
                        sipm_tools.Sipm_t[pos](pos_xy[1], pos_xy[0]) = +timeInSeconds;
                        energyChanels[channel] = energy;

                        // corrige pelo fator de correção encontrado
                        // divideMatrix(Sipm[pos], correctionFactor[pos]);

                        GimnEvent ev;
                        GimnPulse p;
                        p.first_e = first_e; // Highest energy
                        p.first_t = first_t; // time from highest energy event
                        p.first_c = first_c; // Position in the matrix of the highest energy
                        p.chan = chan;       // channel of the highest Energy
                        p.chipID = chipID;
                        p.Port = (int)chipID / 2;
                        p.fillEnergyFromTmatrixT(sipm_tools.Sipm_e[pos]);
                        p.fillTimeFromTmatrixT(sipm_tools.Sipm_t[pos]);

                        auto Positions3D = anger(sipm_tools.Sipm_e[pos]);
                        ev.channel = 0;
                        ev.Energy = esum;
                        ev.time = tea(timeChanels, energyChanels);
                        ev.xmm = Positions3D[0];
                        ev.ymm = Positions3D[1];
                        ev.doi = Positions3D[2];
                        ev.Port = Id.chipID[pos];
                        ev.sipmX = 0;
                        ev.sipmY = 0;

                        sipm_tools.Histos.GlobalEnergy[pos]->Fill(ev.Energy);
                        sipm_tools.Histos.depthOfInteraction[pos]->Fill(ev.doi);
                        sipm_tools.Histos.FloodHist[pos]->Fill(ev.xmm, ev.ymm);

                        bool out = false;
                        if ((ev.xmm > 13.44 || ev.xmm < -13.44) && (ev.ymm > 13.44 || ev.ymm < -13.44))
                            out = true;

                        if (!out)
                        {
                            Output.putData(ev, p);
                        }
                    }

                    esum = 0;
                    sipm_tools.Sipm_e[pos].Zero();
                    sipm_tools.Sipm_t[pos].Zero();
                }
            }
        }

        // This will fall appart
        else if (Id.crystalType[pos] == "seg")
        {
            if (mh_j == 0)
            {
                sipm_tools.Histos.channelEnergyHistogram[pos][channel]->Fill(energy);

                auto pos_xy = sipm_tools.CorrectChannelToPixelPs(channel);
                sipm_tools.Sipm_e[pos](pos_xy[1], pos_xy[0]) = +energy;

                std::vector<Float_t> xPos = sipm_tools.getPositionsX();
                std::vector<Float_t> yPos = sipm_tools.getPositionsY();

                GimnPulse p;

                GimnEvent ev;
                ev.channel = 0;
                ev.Energy = energy;
                ev.time = time;
                ev.xmm = xPos[pos_xy[0]];
                ev.ymm = yPos[pos_xy[1]];
                ev.doi = 0;
                ev.Port = Id.chipID[pos];
                ev.sipmX = 0;
                ev.sipmY = 0.;
                if (ev.Energy > 0)
                {
                    sipm_tools.Histos.GlobalEnergy[pos]->Fill(ev.Energy);
                    sipm_tools.Histos.depthOfInteraction[pos]->Fill(ev.doi);
                    sipm_tools.Histos.FloodHist[pos]->Fill(ev.xmm, ev.ymm);
                    Output.putData(ev);
                }
            }
        }
    }

    // declaracao dos histogramas;
    // auto c1 =

    std::string d1= "Detector1-"+std::to_string(counter);
    std::string d2= "Detector2-"+std::to_string(counter);
    std::string ge= "global energy-"+std::to_string(counter);
    std::string f= "floods-"+std::to_string(counter);
    std::string d= "doi-"+std::to_string(counter);

    // Instantiating the vector as a vector of shared ptrs of type TCanvas;
    //std::vector<std::shared_ptr<TCanvas> Canvas;
    //
    //// Here I fill my TCanvas. 
    //Canvas.push_back(std::make_shared<TCanvas>("Detector1", d1.c_str(), 4000, 4000));
    //Canvas.push_back(std::make_shared<TCanvas>("Detector2", d1.c_str(), 4000, 4000));

   //std::vector<TCanvas> Canvas;
   //TCanvas c1 ("Detector1", d1.c_str(), 4000, 4000) ;
   //TCanvas c2 ("Detector2", d2.c_str(), 4000, 4000);
   ////Canvas.emplace_back(c2);
   //// Canvas.push_back(new TCanvas("Detector3", "Detector3", 4000, 4000));
   //// Canvas.push_back(new TCanvas("Detector4", "Detector4", 4000, 4000));
   ////Canvas.emplace_back(c3);
//
//   Canvas.push_back(TCanvas("Global Energy", ge.c_str(), 4000, 4000));
//   // auto c4 = ;
//   Canvas.push_back(TCanvas("Floods", f.c_str(), 4000, 4000));
//   // auto c5 = ;
//   Canvas.push_back(TCanvas("Doi", d.c_str(), 4000, 4000));
////
////
    //for (Int_t i = 0; i < Id.detNum; i++)
    //{
    //    // std::string canvasName = (std::string)ports[i].Data()+ "Channel MAP";
    //    // Canvas.push_back(new TCanvas(ports[i].Data(),canvasName.c_str(),4000,4000));
    //    if (i==0)
    //        c1.Divide(8, 8);
    //    else if (i==1)
    //        c2.Divide(8,8);
    //    //Canvas[i]->Divide(8,8);
    //    for (Int_t channels = 0; channels < 64; channels++)
    //    {
    //        auto pos_yx = CorrectChannelToPixelPs(channels);
//
    //    if (i==0)
    //        c1.cd((pos_yx[0] * 8 + pos_yx[1]) + 1);
    //    else if (i==1)
    //        c2.cd((pos_yx[0] * 8 + pos_yx[1]) + 1);            
    //        
    //       // Canvas[i].cd((pos_yx[0] * 8 + pos_yx[1]) + 1);
    //        sipm_tools.Histos.channelEnergyHistogram[i][channels]->Draw("HIST");
    //        // std::cout<<sipm_tools.Histos.channelEnergyHistogram[i][channels]->GetMaximum()<<std::endl;
    //    }
    //    // std::string saveAs = (std::string)ports[i].Data()+ "ChannelMAP.png";
    //    // Canvas[i]->Update(); // Print(saveAs.c_str());
    //    // Canvas[i]->SaveAs(Form("matrix_of_histograms%d.png",i));
    //    //Canvas[i].Write();
    //    //Canvas[i].Close();
    //    if (i==0)
    //        c1.Write();
    //    else if (i==1)
    //        c2.Write();
    //     }
//
    //c1.Close();
    //c2.Close();
    //gSystem->ProcessEvents();
    ////Canvas[2].Divide(2, 2);
    ////Canvas[2].cd(1);
    ////sipm_tools.Histos.GlobalEnergy[0]->Draw("L");
    //Canvas[2].cd(2);
    //sipm_tools.Histos.GlobalEnergy[1]->Draw("L");
    //Canvas[2].cd(3);
    //sipm_tools.Histos.FloodHist[0]->Draw("COLZ");
    //// GlobalEnergy[2]->Draw("L");
    //Canvas[2].cd(4);
    //sipm_tools.Histos.FloodHist[1]->Draw("COLZ");
    // GlobalEnergy[3]->Draw("L");
    // Canvas[2]->Update();
   
    // Canvas[3]->Divide(2, 2);
    // Canvas[3]->cd(1);
    // FloodHist[0]->Draw("COLZ");
    // Canvas[3]->cd(2);
    // FloodHist[1]->Draw("COLZ");
    // Canvas[3]->cd(3);
    // FloodHist[2]->Draw("COLZ");
    // Canvas[3]->cd(4);
    // FloodHist[3]->Draw("COLZ");
    // Canvas[3]->Draw();
   
    //Canvas[4].Divide(2, 1);
    //Canvas[4].cd(1);
    //sipm_tools.Histos.depthOfInteraction[0]->Draw("L");
    //Canvas[4].cd(2);
    //sipm_tools.Histos.depthOfInteraction[1]->Draw("L");
    // Canvas[4]->cd(3);
    // depthOfInteraction[2]->Draw("L");
    // Canvas[4]->cd(4);
    // depthOfInteraction[3]->Draw("L");
    // Canvas[4]->Update();
   // Salva as configurações da AQD;
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
    // CloseAllCanvases();
    //  Canvas[0]->Close();
    //  Canvas[1]->Close();
    //  Canvas[2]->Close();
    //  Canvas[3]->Close();
    //  Canvas[4]->Close();

    // return Canvas;
}
