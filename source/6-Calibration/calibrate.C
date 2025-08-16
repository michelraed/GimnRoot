#include <iostream>
#include <TH1F.h>
#include <TCanvas.h>
#include <TRandom.h>
#include <TF1.h>
#include <TMarker.h>

// std::string file1 = "/home/enzo/projects/gimnpet/Testando_pre_caract_group_pre.root";
// std::string file1 = "/home/michelraed/dev/gimnpet/time_30_t1_15_t2_20_te_15_ov_4_0_HW_OFF_qdcCharacterization_06122024_group_pre.root";
std::string file1 = "/home/gimn6/Documentos/Michel/DOUTORADO_CARACTERIZACAO/RootOutputs/PreProcessed/Caracterization/QDC/time_30_t1_5_t2_20_te_19_ov_2_0_HW_OFF_qdcCharacterization_06122024_group_pre.root";

std::map<TString, Float_t> polinomialCorrection(std::map<int, double> peaks)
{
    std::cout<<" on Poli "<<std::endl;

    std::shared_ptr<TGraph> graph(nullptr);

    int out_type = {-1};

    if (peaks.size() == 3)
    {
        Double_t E[3]{0, 511, 1274}; // 1274};
        Double_t Q[3]{0, peaks[511], peaks[1274]};
        graph = std::make_shared<TGraph>(3, Q, E);
        out_type = 1;
    }
    else if (peaks.size() == 2)
    {
        Double_t E[2]{0, 511};
        Double_t Q[2]{0, peaks[511]};
        graph = std::make_shared<TGraph>(3, Q, E);
        out_type = 2;
    }

    std::map<TString, Float_t> out;
    TF1 fitFunc("polinomial", "[0]*pow(x,2)+[1]*x+[2]");

    for (int pa = -10; pa < 10; pa++)
    {
        for (int pb = -10; pb < 10; pb++)
        {
            for (int pc = -10; pc < 100; pc++)
            {
                std::map<TString, Float_t> partial1;
                fitFunc.SetParameter(0, pa); // Valor inicial para c
                fitFunc.SetParameter(1, pb); // Valor inicial para s
                fitFunc.SetParameter(2, pc); // Valor inicial para s

                graph->Fit(&fitFunc, "Q");

                float a = fitFunc.GetParameter(0);
                float b = fitFunc.GetParameter(1);
                float c = fitFunc.GetParameter(2);

                // float T = fitFunc.GetParameter(2);

                partial1.insert(std::make_pair("a", a));
                partial1.insert(std::make_pair("b", b));
                partial1.insert(std::make_pair("c", c));

                float calculation511 = a * pow(peaks[511], 2) + b * peaks[511] + c;
                // std::cout << " the value is " << calculation511 << "  and " << calculation1274 << std::endl;

                if (out_type == 1)
                {
                    float calculation1274 = a * pow(peaks[1274], 2) + b * peaks[1274] + c;

                    if ((calculation511 > 510.999 && calculation511 < 511.001) && ((calculation1274 > 1273.999 && calculation1274 < 1274.001)))
                    {
                        // std::cout << pa << " " << pb << " " << pc << std::endl;

                        partial1["out"] = true;
                        return partial1;
                    }
                }
                else if (out_type == 2)
                {
                    if (calculation511 > 510.9 && calculation511 < 511.01){
                     
                        std::cout << pa << " " << pb << " " << pc << std::endl;
                        return partial1;
                    }
                }
            }
        }
    }
    return out;
}

std::map<TString, Float_t> SaturationCorrectionNadig(std::map<int, double> peaks)
{
    std::cout<<" on Nadig "<<std::endl;


    std::shared_ptr<TGraph> graph(nullptr);

    int out_type = {-1};

    if (peaks.size() == 3)
    {
        Double_t E[3]{0, 511, 1274}; // 1274};
        Double_t Q[3]{0, peaks[511], peaks[1274]};
        graph = std::make_shared<TGraph>(3, Q, E);
        out_type = 1;
    }
    else if (peaks.size() == 2)
    {
        Double_t E[2]{0, 511};
        Double_t Q[2]{0, peaks[511]};
        graph = std::make_shared<TGraph>(3, Q, E);
        out_type = 2;
    }

    std::map<TString, Float_t> out;
    TF1 fitFunc("nadigShug", "[0]*[1]*log(1/(1-x/[1]))");

    bool outBool = false;
    float parameter_a = -10;
    float parameter_b = -10;
    int contador{0};
    while (!outBool)
    {
        std::map<TString, Float_t> partial;
        // std::cout<<"contador "<<contador <<std::endl;

        fitFunc.SetParameter(0, parameter_a); // Valor inicial para c
        fitFunc.SetParameter(1, parameter_b); // Valor inicial para s
        // fitFunc.SetParameter(2, 100); // Valor inicial para s

        graph->Fit(&fitFunc, "Q");

        float C = fitFunc.GetParameter(0);
        float S = fitFunc.GetParameter(1);
        // float T = fitFunc.GetParameter(2);

        partial.insert(std::make_pair("c", C));
        partial.insert(std::make_pair("s", S));
        // out.insert(std::make_pair("t", T));

        parameter_a += 0.1;
        parameter_b += 0.1;
        float calculation511 = partial["c"] * partial["s"] * log(1 / (1 - peaks[511] / partial["s"]));
        // std::cout<<"peak 511 ->"<<peaks[511]<<std::endl;
        // std::cout<<"peak 1274 ->"<<peaks[1274]<<std::endl;
        // std::cout<<calculation511<<std::endl;
        // std::cout<<calculation1274<<std::endl;
        // std::cout<<"PARAMETRO a->"<<parameter_a<<std::endl;
        // std::cout<<"PARAMETRO b->"<<parameter_b<<std::endl;
        // std::cout<<"PARAMETRO C->"<<C<<std::endl;
        // std::cout<<"PARAMETRO S->"<<S<<std::endl;
        if (contador > 1000000)
        {
            std::cout << "got stuck Nadig" << std::endl;
            gROOT->ProcessLine(".q");
        }

        if (out_type == 1)
        {
            float calculation1274 = partial["c"] * partial["s"] * log(1 / (1 - peaks[1274] / partial["s"]));
            if ((calculation511 > 510.999 && calculation511 < 511.001) && ((calculation1274 > 1273.999 && calculation1274 < 1274.001)))
            {
                // std::cout<<" the value is "<<calulation511<<std::endl;
                outBool = true;
                return partial;
            }
        }

        else if (out_type == 2)
        {
            if (calculation511 > 510.9 && calculation511 < 511.01)
            {
                std::cout << " a" << parameter_a << " b " << parameter_b << calculation511 << std::endl;
                outBool = true;
                return partial;
            }
        }
        contador++;
    }

    return out;
}

std::map<TString, Float_t> correctionWang(std::map<int, double> peaks)
{
    std::cout<<" on wang "<<std::endl;
    std::shared_ptr<TGraph> graph(nullptr);

    int out_type = {-1};

    if (peaks.size() == 3)
    {
        Double_t E[3]{0, 511, 1274}; // 1274};
        Double_t Q[3]{0, peaks[511], peaks[1274]};
        graph = std::make_shared<TGraph>(3, Q, E);
        out_type = 1;
    }
    else if (peaks.size() == 2)
    {
        Double_t E[2]{0, 511};
        Double_t Q[2]{0, peaks[511]};
        graph = std::make_shared<TGraph>(3, Q, E);
        out_type = 2;
    }

    std::map<TString, Float_t> out;
    // TF1 fitFunc("wang", "[0]*(1-exp(-[1]*x)+[2])");

    // TF1 fitFunc("wang", "log((x-[2])/[0])*(1/[1])");

    TF1 fitFunc("wang", "log([0]/([0]+[2]-x))*(1/[1])");

    bool outBool = false;

    for (int pa = -1000; pa < 1000; pa += 20)
    {
        for (int pb = -2000; pb < 10000; pb += 100)
        {
            for (int pc = -2000; pc < 1000; pc += 100)
            {
                std::map<TString, Float_t> partial1;
                fitFunc.SetParameter(0, pa); // Valor inicial para c
                fitFunc.SetParameter(1, pb); // Valor inicial para s
                fitFunc.SetParameter(2, pc); // Valor inicial para s

                graph->Fit(&fitFunc, "Q");

                float k1 = fitFunc.GetParameter(0);
                float k2 = fitFunc.GetParameter(1);
                float b = fitFunc.GetParameter(2);

                // float T = fitFunc.GetParameter(2);

                partial1.insert(std::make_pair("k1", k1));
                partial1.insert(std::make_pair("k2", k2));
                partial1.insert(std::make_pair("b", b));

                // float calculation511 = k1 * (1 - exp(-k2 * peaks[511])) + b;
                // float calculation1274 = k1 * (1 - exp(-k2 * peaks[1274])) + b;


                float calculation511 = log(k1 / (k1 + b - peaks[511])) * (1 / k2);
                float calculation1274 = log(k1 / (k1 + b - peaks[1274])) * (1 / k2);
                if (out_type == 1)
                {

                    if ((calculation511 > 510.999 && calculation511 < 511.001) && ((calculation1274 > 1273.999 && calculation1274 < 1274.001)))
                    {
                        std::cout<<"calculated 511  "<<calculation511<<std::endl;
                        // std::cout<<"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"<< std::endl;
                        std::cout<<"k1 :"<<k1<<"k2 "<<k2<<" b " << b<<std::endl;
                        // std::cout << " the value of 511 peak is " << calculation511 << "  and  1274 is" << calculation1274 << std::endl;
                        partial1["out"] = true;
                        outBool = true;
                        return partial1;
                    }
                }

                else if (out_type == 2)
                {
                    if (calculation511 > 510.9 && calculation511 < 511.01){
                        outBool = true;
                        std::cout<<"k1 :"<<k1<<"k2 "<<k2<<" b " << b<<std::endl;
                        break;
                        return partial1;
                    }
                }
            }
        }
    }
    return out;
}

// Funcao petsys;
std::map<TString, Float_t> FindLinearizationPETSYS(std::map<int, double> peaks)
{
    std::cout<<" on Petsys "<<std::endl;

    std::map<TString, Float_t> out;

    std::shared_ptr<TGraph> graph(nullptr);

    int out_type = {-1};

    if (peaks.size() == 3)
    {
        Double_t E[3]{0, 511, 1274}; // 1274};
        Double_t Q[3]{0, peaks[511], peaks[1274]};
        graph = std::make_shared<TGraph>(3, Q, E);
        out_type = 1;
    }
    else if (peaks.size() == 2)
    {
        Double_t E[2]{0, 511};
        Double_t Q[2]{0, peaks[511]};
        graph = std::make_shared<TGraph>(3, Q, E);
        out_type = 2;
    }

    TF1 fitFunc("fitFunc", " [0] * pow([1], pow(x, [2])) + [3] * x - [0]");


    for (int pa = -10; pa < 10; pa++)
    {
        for (int pb = 20; pb < 109; pb++)
        {
            for (int pc = 10; pc < 100; pc++)
            {
                for (int pd = -10; pd < 1; pd++)
                {
                    std::map<TString, Float_t> partial1;

                    fitFunc.SetParameter(0, pa / 10); // Valor inicial para P0
                    fitFunc.SetParameter(1, pb / 10); // Valor inicial para P1
                    fitFunc.SetParameter(2, pc / 10); // Valor inicial para P2
                    fitFunc.SetParameter(3, pd / 10); // Valor inicial para P3

                    graph->Fit(&fitFunc, "Q");

                    float P0 = fitFunc.GetParameter(0);
                    float P1 = fitFunc.GetParameter(1);
                    float P2 = fitFunc.GetParameter(2);
                    float P3 = fitFunc.GetParameter(3);

                    // float T = fitFunc.GetParameter(2);

                    partial1.insert(std::make_pair("P0", fitFunc.GetParameter(0)));
                    partial1.insert(std::make_pair("P1", fitFunc.GetParameter(1)));
                    partial1.insert(std::make_pair("P2", fitFunc.GetParameter(2)));
                    partial1.insert(std::make_pair("P3", fitFunc.GetParameter(3)));

                    float calculation511 = P0 * pow(P1, pow(peaks[511], P2)) + P3 * peaks[511] + P0;

                    if (out_type == 1)
                    {
                        float calculation1274 = P0 * pow(P1, pow(peaks[1274], P2)) + P3 * peaks[1274] + P0;
                        if ((calculation511 > 510.999 && calculation511 < 511.001) && ((calculation1274 > 1273.99 && calculation1274 < 1274.001)))
                        {
                            // std::cout << "PETSYS" << std::endl;
                            // std::cout << " Pa : " << pa << " Pb : " << pb << " Pc : " << pc << " Pd : " << pd << std::endl;
                            // std::cout << pa << " " << pb << " " << pc << std::endl;
                            // std::cout << " P0 " << P0 << " P1 " << P1 << " P2 " << P2 << " P3 " << P3;
                            // std::cout << " the value is " << calculation511 << "  and " << calculation1274 << std::endl;
                            partial1["out"] = true;
                            return partial1;
                        }
                    }

                    else if (out_type == 2)
                    {
                        if (calculation511 > 510.9 && calculation511 < 511.01)
                            std::cout << " Pa : " << pa << " Pb : " << pb << " Pc : " << pc << " Pd : " << pd << std::endl;
                            return partial1;
                    }
                }
            }
        }
    }

    return out;
}

std::map<int, double> FindAndMarkPeaksTot(TH1F *histo, Float_t sigma, Float_t Threshold)
{
    // Float_t sigma {5} , Threshold {0.001};
    std::vector<Float_t> pks;
    TSpectrum *s = new TSpectrum;
    int npeaks = s->Search(histo, sigma, "", Threshold);
    double *peaks = s->GetPositionX();

    std::map<int, double> sodiumPeaks;
    sodiumPeaks[511] = 0.0;
    sodiumPeaks[0] = npeaks;
    bool first = true;
    if (VERBOSE)
        std::cout << "nPeaks " << npeaks << std::endl;

    double maior = 0;
    for (int i = 0; i < npeaks; i++)
    {
        if (i == 0)
        {
            sodiumPeaks[511] = peaks[i];
        }
        if (VERBOSE)
            std::cout << "pico encontrado em " << peaks[i] << std::endl;
    }

    delete s;
    return sodiumPeaks;
}

std::map<int, double> FindAndMarkPeaks(TH1F *histo, Float_t sigma, Float_t Threshold)
{
    // Float_t sigma {5} , Threshold {0.001};
    std::vector<Float_t> pks;
    TSpectrum *s = new TSpectrum;
    int npeaks = s->Search(histo, sigma, "", Threshold);
    double *peaks = s->GetPositionX();

    //double maxPeak = histo->GetXaxis()->GetXmax();

    std::map<int, double> sodiumPeaks;
    sodiumPeaks[511] = 0.0;
    sodiumPeaks[1274] = 0.0;
    sodiumPeaks[0] = npeaks;
    bool first = true;
    if (VERBOSE)
        std::cout << "nPeaks " << npeaks << std::endl;

    double maior = 0;
    //double menor = 99999;
    for (int i = 0; i < npeaks; i++)
    {
        //std::cout<<menor<<std::endl;
        //if (peaks[i]>maxPeak/10){
        //    if (peaks[i]< menor)
        //        menor = peaks[i];
        if (i==0){
            sodiumPeaks[511]=peaks[i];
        }
        if (peaks[i] > maior)
            maior = peaks[i];
        if (VERBOSE)
            std::cout << "pico encontrado em " << peaks[i] << std::endl;
    }
    //sodiumPeaks[511] = menor;
    sodiumPeaks[1274] = maior;

    delete s;
    return sodiumPeaks;
}

std::vector<std::string> getKeysFromResult(std::map<std::string, std::vector<std::shared_ptr<TH1F>>> result)
{
    std::vector<std::string> output;

    for (auto &p : result)
    {
        output.push_back(p.first);
    }
    return output;
}

std::vector<std::vector<std::shared_ptr<TH1F>>> getValuesFromResult(std::map<std::string, std::vector<std::shared_ptr<TH1F>>> result)
{
    std::vector<std::vector<std::shared_ptr<TH1F>>> output;
    for (auto &p : result)
    {
        output.push_back(p.second);
    }
    return output;
}

std::map<std::string, std::vector<TH1F *>> GetMapHistosRoot(std::string file_name)
{
    //  FILE
    //   |--Detectors
    //   |  |--ChipID
    //   |  |   |--Channel_energy
    //   |  |
    //   |
    // std::string file_name = "/home/michelraed/dev/gimnpet/GimnROOT/Beg_27112024_600_group_pre.root";

    // reads file
    TFile *file = new TFile(file_name.c_str(), "Open");

    // Vetor dos canais
    // std::vector<std::vector<std::shared_ptr<TH1F>>> histograms;

    // Map dos canais
    std::map<std::string, std::vector<TH1F *>> histograms;

    // Nomes dos ChipIDs
    std::vector<TString> chipIDNames;

    // std::map<std::pair<TString, std::vector<std::shared_ptr<TH1F>>> histoData;

    // Gets the name of the files inside the
    TList *keysFile = file->GetListOfKeys();

    // Iterate over each key in the directory
    for (int i = 0; i < keysFile->GetSize(); ++i)
    {
        // Get keys
        TKey *key = (TKey *)keysFile->At(i);

        std::string name = key->GetName();

        // Print the name of the key with prefix for hierarchy
        if (VERBOSE)
            std::cout << name << std::endl;

        // Get detector Folder
        if (name.find("Detectors") != -1 && key->IsFolder())
        {
            if (VERBOSE)
                std::cout << "detecores encontrado e 'e uma pasta " << std::endl;
            TDirectory *detector = (TDirectory *)file->Get(name.c_str());
            auto *keysDetectors = detector->GetListOfKeys();
            for (int j = 0; j < keysDetectors->GetSize(); j++)
            {
                TKey *key2 = (TKey *)keysDetectors->At(j);
                std::string name2 = key2->GetName();
                if (VERBOSE)
                    std::cout << name2 << std::endl;

                if (name2.find("ChipID") != -1 && key->IsFolder())
                {

                    chipIDNames.push_back(name2);
                    //  int num = extractNumber(name2);
                    //  int p  = (int)num/2+1;
                    //  std::string portName =  "PORT-"+std::to_str(p);
                    //  this->portNames.push_back(portName);
                    if (VERBOSE)
                        std::cout << "chipID encontrado e 'e uma pasta " << std::endl;

                    TDirectory *chipID = (TDirectory *)detector->Get(name2.c_str());
                    auto *keysChipID = chipID->GetListOfKeys();
                    // std::cout<<(int)keysChipID->GetSize()<<" fimim"<<std::endl;
                    // std::vector<TH1F *> detAux;
                    std::vector<TH1F *> detAux(64, nullptr);

                    for (int k = 0; k < keysChipID->GetSize(); k++)
                    {

                        TKey *key3 = (TKey *)keysChipID->At(k);
                        std::string name3 = key3->GetName();
                        if (name3.find("Channel_energy") != -1 && key3->IsFolder())
                        {
                            if (VERBOSE)
                                std::cout << "Pasta de canais  encontrada" << std::endl;
                            TDirectory *channelEnergy = (TDirectory *)chipID->Get(name3.c_str());
                            auto *keysChannelsE = channelEnergy->GetListOfKeys();

                            for (int m = 0; m < keysChannelsE->GetSize(); m++)
                            {
                                TKey *key4 = (TKey *)keysChannelsE->At(m);
                                std::string name4 = key4->GetName();

                                TH1F *aux_chan = (TH1F *)(channelEnergy->Get(name4.c_str()));

                                // Extracts channel Name
                                std::string chname = aux_chan->GetName();
                                chname = splitString(chname, '-')[1];
                                chname = removePart(chname, "detector");
                                auto cnl = std::stoi(chname);
                                detAux[cnl] = aux_chan;
                                // std::shared_ptr<TH1F> canal (aux_chan);
                                // canal = std::make_shared<TH1F>(h);
                                // std::cout << name4 << std::endl;
                                // canal = std::make_shared<TH1F>();
                                // TCanvas * c2  = new TCanvas;
                                // c2->cd();
                                // canal.get()->Draw();
                                // c2->Draw();
                                // detAux.push_back(canal);
                            }
                            histograms[name2] = detAux;
                        }
                    }
                }
            }
        }
    }

    return histograms;
}



// std::map<std::string, std::vector<TH1F *>>
std::map<std::string, std::vector<TH1F *>> calibrate(std::string fileName, std::string outPath, Float_t sigma = 2, Float_t Threshold = 0.01, int det = 30)
{
    static int cnt = 0;
    cnt++;
    std::cout << " Processing File " << "[" << cnt << "] :" << fileName << std::endl;

    // splits filenamen in order to get the information from aqd
    auto name = splitString(fileName, '/');
    auto name_pos = name.size();
    std::string outName = name[name_pos - 1];
    name = splitString(outName, '.');
    name_pos = name.size();
    outName = name[0];
    outName = outName + "_CalFactorsChannels.root";

    outName = outPath + "/" + outName;

    // Creates the file
    std::shared_ptr<TFile> calOut;
    calOut = std::make_shared<TFile>(outName.c_str(), "RECREATE");

    // Creates the tree to store the calibration Factors
    std::shared_ptr<TTree> calFactors;
    calFactors = std::make_shared<TTree>("calFactorChannel", "calFactorChannel");

    Int_t chan{-1};
    Double_t C, S, Pos511, Pos1274;
    Int_t cpN;
    Double_t k1, k2, b;
    Double_t pa, pb, pc;
    Double_t P0, P1, P2, P3;

    calFactors->Branch("channel", &chan);
    calFactors->Branch("c", &C);
    calFactors->Branch("s", &S);
    calFactors->Branch("511Pos", &Pos511);
    calFactors->Branch("1274Pos", &Pos1274);
    calFactors->Branch("chipID", &cpN);
    calFactors->Branch("k1", &k1);
    calFactors->Branch("k2", &k2);
    calFactors->Branch("b", &b);
    calFactors->Branch("pa", &pa);
    calFactors->Branch("pb", &pb);
    calFactors->Branch("pc", &pc);
    calFactors->Branch("P0", &P0);
    calFactors->Branch("P1", &P1);
    calFactors->Branch("P2", &P2);
    calFactors->Branch("P3", &P3);

    // Aqui voce tinha colocado auto, mas eu alterei a funç~ao para retornar um map
    // esse map vai conter <std::String ChipID, std::vector<std::shared_ptr<TH1F>>>
    std::map<std::string, std::vector<TH1F *>> result = GetMapHistosRoot(fileName);

    std::string chipName = "name";

    int counter = 0;
    pair<std::string, std::vector<TH1F *>> de;

    // std::cout << "GETS HERE" << std::endl;
    for (auto &detector : result)
    {
        chipName = detector.first;
        // std::cout<<"XX CHIP ID IS "<< chipName <<std::endl;
        auto histograms = detector.second;
        int ch = 0;
        for (auto &histogram : histograms)
        {
            std::cout << histogram->GetName() << std::endl;
            bool out = false;

            // auto sodiumPeaks = FindAndMarkPeaks(histogram, sigma, Threshold);
            //  std::cout<<"+++++++++++++++++++++++++"<<std::endl;

            auto sodiumPeaks = FindAndMarkPeaks(histogram, sigma, Threshold);
            // std::cout << "511 " << sodiumPeaks[511] << std::endl;
            // std::cout << "1274 " << sodiumPeaks[1274] << std::endl;
            // std::cout << sodiumPeaks[0] << std::endl;
            Float_t modifiedSigma = sigma;

            if (fileName.find("qdc") != -1)
            {
                while ((sodiumPeaks[0] < 3) || (sodiumPeaks[511] == sodiumPeaks[1274]) || (sodiumPeaks[511] < 2))
                {
                    modifiedSigma += 0.001;
                    sodiumPeaks = FindAndMarkPeaks(histogram, modifiedSigma, Threshold);
                    // std::cout << modifiedSigma << std::endl;
                    if (modifiedSigma > 10)
                    {
                        modifiedSigma = 0;
                    }
                }
            }
            else if (fileName.find("tot") != -1)
            {

                sodiumPeaks = FindAndMarkPeaksTot(histogram, modifiedSigma, Threshold);
            }
            else {

                cout<<" NOT TOT NOR QDC"<<endl;

            }

            std::cout << "sodium Peaks n " << sodiumPeaks.size() << std::endl;
            std::cout<<sodiumPeaks[0]<<std::endl;
            std::cout<<"511 " <<sodiumPeaks[511]<<std::endl;
            std::cout<< "1274 "<<sodiumPeaks[1274]<<std::endl;
            /// test to see whats happens with channel 39 ... Why?


            std::map<TString, Float_t> corNadig;
            std::map<TString, Float_t> corWang;
            std::map<TString, Float_t> corPoli;

            std::map<TString, Float_t> corPetsys;

            corNadig = SaturationCorrectionNadig(sodiumPeaks);
            // std::cout << "Nadig" << std::endl;
            corWang = correctionWang(sodiumPeaks);
            // std::cout << "Wang" << std::endl;
            corPoli = polinomialCorrection(sodiumPeaks);
            // std::cout << "Poli" << std::endl;
            corPetsys = FindLinearizationPETSYS(sodiumPeaks);
            // std::cout << "Petsys" << std::endl;

            // std::cout << "++++++++Ajuste Nadig++++++++++++++" << std::endl;
            // std::cout << "valor  de c " << corNadig["c"] << std::endl;
            // std::cout << "valor  de s " << corNadig["s"] << std::endl;
            // std::cout<<"energia calculada para o pico de 511 "<< <<std::endl;
            // std::cout << counter << std::endl;
            // if (corNadig["c"] * corNadig["s"] * log(1 / (1 - sodiumPeaks[511] / corNadig["s"])) != 511)
            //{
            //    std::cout << "deu pau " << corNadig["c"] * corNadig["s"] * log(1 / (1 - sodiumPeaks[511] / corNadig["s"])) << std::endl;
            //    std::cout << " pico de 511 em " << sodiumPeaks[511] << " calculado " << corNadig["c"] * corNadig["s"] * log(1 / (1 - sodiumPeaks[511] / corNadig["s"])) << std::endl;
            //    std::cout << " pico de 1274 em " << " calculado " << corNadig["c"] * corNadig["s"] * log(1 / (1 - sodiumPeaks[1274] / corNadig["s"])) << std::endl;
            //}
            auto id = std::stoi(removePart(chipName, "ChipID-").c_str());
            // std::cout << ch << std::endl;
            // std::cout << chipName << std::endl;
            cpN = id;
            chan = ch;
            C = corNadig["c"];
            S = corNadig["s"];
            k1 = corWang["k1"];
            k2 = corWang["k2"];
            b = corWang["b"];
            pa = corPoli["a"];
            pb = corPoli["b"];
            pc = corPoli["c"];
            P0 = corPetsys["P0"];
            P1 = corPetsys["P1"];
            P2 = corPetsys["P2"];
            P3 = corPetsys["P3"];
            // std::cout<<" P0 "<<P0<<" P1 "<<P1<<" P2 "<<P2<<" P3 "<<P3;

            if (fileName.find("qdc") != -1)
            {
                Pos511 = sodiumPeaks[511];
                Pos1274 = sodiumPeaks[1274];
            }
            else
            {
                Pos511 = sodiumPeaks[511];
            }
            
            ch++;
            calFactors->Fill();
            // std::cout << "ends" << std::endl;
        }
        de = detector;
    }
    calOut->WriteObject(calFactors.get(), "CalibrationFactors");
    chipName = de.first;
    // result[chipName][det]->Draw();
    // detectorsData[chipName] = auxChip;
    calOut->Close();
    return result;
}


