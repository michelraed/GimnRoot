
void ProcessPulsesSingle(std::string fileName = "/home/gimn6/Documentos/Michel/DOUTORADO_CARACTERIZACAO/RootOutputs/PreProcessed/Caracterization/OV_QDC/time_120_t1_20_t2_20_te_15_ov_2_0_HW_OFF_qdcAquisicao_OV_04_02_2025_group_pre.root",
                   std::string calibrationParameters = "/home/gimn6/Documentos/Michel/DOUTORADO_CARACTERIZACAO/RootOutputs/PreProcessed/Caracterization/OV_QDC_CAL/time_120_t1_20_t2_20_te_15_ov_2_0_HW_OFF_qdcAquisicao_OV_04_02_2025_group_pre_CalFactorsChannels.root",
                   std::string OutputFolder = "/home/gimn6/Documentos/Michel/DOUTORADO_CARACTERIZACAO/RootOutputs/PreProcessed/Caracterization/OV_QDC_FINAL")
{

    // TString fileName= "/home/gimn6/Documentos/Michel/DOUTORADO_CARACTERIZACAO/RootOutputs/PreProcessed/Caracterization/OV_QDC/time_120_t1_20_t2_20_te_15_ov_2_0_HW_OFF_qdcAquisicao_OV_04_02_2025_group_pre.root";
    // calibrationParameters="/home/gimn6/Documentos/Michel/DOUTORADO_CARACTERIZACAO/RootOutputs/PreProcessed/Caracterization/OV_QDC_CAL/time_120_t1_20_t2_20_te_15_ov_2_0_HW_OFF_qdcAquisicao_OV_04_02_2025_group_pre_CalFactorsChannels.root";
    // OutputFolder="/home/gimn6/Documentos/Michel/DOUTORADO_CARACTERIZACAO/RootOutputs/PreProcessed/Caracterization/OV_QDC_FINAL";

    ROOT::EnableImplicitMT();
    ROOT::EnableThreadSafety();
    HistosMaps Histogramas = generateHistos(fileName);
    SiPM aux;
    std::vector<Float_t> xPos = aux.getPositionsX();
    std::vector<Float_t> yPos = aux.getPositionsY();

    std::cout << "Processing aquisition : [" << DIFERENTIATOR << "]" << std::endl;
    std::cout << fileName << std::endl;

    std::string calibrationName = removePart(fileName, ".root");
    auto auxiliaryName = splitString(fileName,'/');
    auxiliaryName.pop_back();
    std::string preName = joinString(auxiliaryName,'/');
    calibrationName = removePart(calibrationName, preName);


    calibrationName = calibrationParameters + "/" + calibrationName + "_CalFactorsChannels.root";

    // if working with the function only uncoment this \/
    //calibrationName = calibrationParameters;//+"/"+calibrationName+"_CalFactorsChannels.root";

    std::cout << "calibration Name " << calibrationName << std::endl;
    // std::cout << "Loading calibration parameters" << std::endl;
    correctionParameters cPar = LoadCalibrationFactors(calibrationName, Histogramas, aux);

    //=========================================creates output ================================
    std::cout << "Processing Name" << std::endl;

    std::string coreName = removePart(fileName, "group_pre.root");
    coreName = coreName + "calibrated.root";
    coreName = removePart(coreName, preName);
    coreName = OutputFolder + "/" + coreName;

    std::cout << "Creating an output Tree" << std::endl;
    std::shared_ptr<TFile> outFile(new TFile(coreName.c_str(), "recreate"));
    if (!outFile || outFile->IsZombie())
    {
        std::cerr << "Error opening file: " << outFile << std::endl;
        return;
    }

    std::shared_ptr<TTree> calibrated;
    calibrated = std::make_shared<TTree>("calibrated", Form("calibrated-%d", DIFERENTIATOR));

    Float_t UN, UW, VN, VW, EnN, EnW, T;
    Float_t UPol, VPol, UPS, VPS, EPol, EPS;
    Int_t chID;

    calibrated->Branch("xPosNadig", &UN);
    calibrated->Branch("xPosWang", &UW);
    calibrated->Branch("xPosPoli", &UPol);
    calibrated->Branch("xPosPetSys", &UPS);
    calibrated->Branch("yPosNadig", &VN);
    calibrated->Branch("yPosWang", &VW);
    calibrated->Branch("yPosPoli", &VPol);
    calibrated->Branch("yPosPetSys", &VPS);
    calibrated->Branch("energyNadig", &EnN);
    calibrated->Branch("energyWang", &EnW);
    calibrated->Branch("energyPoli", &EPol);
    calibrated->Branch("energyPetSys", &EPS);
    calibrated->Branch("time", &T);
    calibrated->Branch("chipID", &chID);

    // ==========================================================================================================================

    //                                         LAMBDAS

    // ==========================================================================================================================

    auto correctEnergyRVec = [&cPar](RVec<Float_t> *EnergyVec, int chipID)
    {
        RVec<Float_t> correctedNadig;
        RVec<Float_t> correctedWang;
        RVec<Float_t> correctedPetSys;
        RVec<Float_t> correctedPoli;

        std::map<std::string, RVec<Float_t>> output;

        correctedNadig.assign(64, 0);
        correctedWang.assign(64, 0);
        correctedPetSys.assign(64, 0);
        correctedPoli.assign(64, 0);

        int auxChan{-1};
        Float_t k1{-1}, k2{-1}, b{-1}, s{-1}, c{-1}, pa{-1}, pb{-1}, pc{-1}, P0{-1}, P1{-1}, P2{-2}, P3{-1};

        for (int chan = 0; chan < 64; chan++)
        {
            // std::cout<<"channel"<<chan<<std::endl;
            auxChan = cPar.chanEquivalent[chan];

            Float_t energy = (*EnergyVec)[auxChan];
            // std::cout<<"energy"<<energy<<std::endl;

            // std::cout<<"Wang"<<chan<<std::endl;
            k1 = cPar.fatoresDeCalibracaoWang[chipID][chan]["k1"];
            k2 = cPar.fatoresDeCalibracaoWang[chipID][chan]["k2"];
            b = cPar.fatoresDeCalibracaoWang[chipID][chan]["b"];
            // std::cout<<"k1 : "<<k1<<" k2 :"<<k2<<" b "<<b<<std::endl;

            s = cPar.fatoresDeCalibracaoNadig[chipID][chan]["s"];
            c = cPar.fatoresDeCalibracaoNadig[chipID][chan]["c"];
            // std::cout<<"Nadig"<<chan<<std::endl;
            // std::cout<<"s: "<<s<<" c :"<<c<<std::endl;

            pa = cPar.fatoresDeCalibracaoPoli[chipID][chan]["pa"];
            pb = cPar.fatoresDeCalibracaoPoli[chipID][chan]["pb"];
            pc = cPar.fatoresDeCalibracaoPoli[chipID][chan]["pc"];
            // std::cout<<"Poli"<<chan<<std::endl;
            // std::cout<<"pa: "<<pa<<" pb :"<<pb<<" pc :"<<pc<<std::endl;
            //
            // std::cout<<"PetSys"<<chan<<std::endl;
            P0 = cPar.fatoresDeCalibracaoPetSys[chipID][chan]["P0"];
            P1 = cPar.fatoresDeCalibracaoPetSys[chipID][chan]["P1"];
            P2 = cPar.fatoresDeCalibracaoPetSys[chipID][chan]["P2"];
            P3 = cPar.fatoresDeCalibracaoPetSys[chipID][chan]["P3"];
            // std::cout<<"P0: "<<P0<<" P1 :"<<P1<<" P2 :"<<P2<<" P3 :"<<P3<<std::endl;

            correctedWang[auxChan] = log(k1 / (k1 + b - energy)) * (1 / k2);
            // correctedWang[chan] = k1 * (1 - exp(-k2 * energy) + b);
            correctedNadig[auxChan] = c * s * log(1 / (1 - energy / s));
            correctedPoli[auxChan] = pa * pow(energy, 2) + pb * energy + pc;
            correctedPetSys[auxChan] = P0 * pow(P1, pow(energy, P2)) + P3 * energy + P0;
            // std::cout<<"energy wang"<<correctedWang[chan]<<std::endl;
            // std::cout<<c * s * log(1 / (1 - energy / s))<<std::endl;
            // std::cout<<"energy nadig"<<correctedNadig[chan]<<std::endl;
            // std::cout<<"energy poli"<<correctedPoli[chan]<<std::endl;
            // std::cout<<"energy petsys"<<correctedPetSys[chan]<<std::endl;
        }

        output["wang"] = correctedWang;
        output["nadig"] = correctedNadig;
        output["poli"] = correctedPoli;
        output["petsys"] = correctedPetSys;

        return output;
    };

    auto AngerSimple = [&xPos, &yPos](RVec<Float_t> EnergyVec)
    {
        TMatrixT<Float_t> energy(8, 8);

        // Fill The Matrix
        int counter = 0;

        for (int col = 0; col < energy.GetNcols(); col++)
        {
            for (int row = 0; row < energy.GetNrows(); row++)
            {
                if (counter < 64)
                {
                    energy(row, col) = EnergyVec[counter];
                }

                counter++;
            }
        }

        auto sum = energy.Sum();

        const auto I = energy.Max();

        int n_rows = 8;
        int n_cols = 8;

        std::vector<Float_t> rowSums(n_rows, 0);
        std::vector<Float_t> colSums(n_cols, 0);

        getRowAndColumnSums(energy, rowSums, colSums);

        // Scrimger and Baker formula
        // N = 2*PI * I * t²
        // N is the integral of scintilation light  (esum)
        // I is the maximum intnsity
        // t is the distance between photon scintilation and the detection plane "(doi)"
        //
        // t  =  sqrt(N/(2*PI*I))

        // const double DOI = sqrt(sum / (2 * TMath::Pi() * I));
        Float_t doi = I / sum;

        Float_t u{0}, v{0};
        Float_t usum{0}, vsum{0};


        for (int i = 0; i < 8; i++)
        {
            u += colSums[i] * xPos[i];
            v += rowSums[i] * yPos[i];
            usum += colSums[i]; 
            vsum += rowSums[i]; 
        
        }
        u = u / usum;
        v = v / vsum;

        std::map<std::string, Float_t> output;
        output["u"] = u;
        output["v"] = v;
        return output;
    };

    //===========================READS FILE====================================
    std::cout << "filename " << fileName << std::endl;
    // std::unique_ptr<TFile> myFile(new TFile(fileName.c_str(), "read"));
    // std::unique_ptr<TTree> pulses((TTree *)myFile->Get("Pulses"));

    std::unique_ptr<TFile> myFile(new TFile(fileName.c_str(), "read"));
    if (!myFile || myFile->IsZombie())
    {
        std::cerr << "Error opening file: " << fileName << std::endl;
        return;
    }

    TTree *tempTree = static_cast<TTree *>(myFile->Get("Pulses"));
    tempTree->SetName(Form("Pulses_%d", DIFERENTIATOR)); // Unique name: Pulses_0, Pulses_1, etc.
    std::unique_ptr<TTree> pulses(tempTree);

    Int_t channel, chipIDp, entries;
    Float_t h_e, h_t;

    ROOT::VecOps::RVec<float> *EnergyVec = nullptr;
    ROOT::VecOps::RVec<float> *TimeVec = nullptr;

    pulses->SetBranchAddress("EnergyVec", &EnergyVec);
    pulses->SetBranchAddress("TimeVec", &TimeVec);
    pulses->SetBranchAddress("channel", &channel);
    pulses->SetBranchAddress("chipID", &chipIDp);
    pulses->SetBranchAddress("h_e", &h_e);
    pulses->SetBranchAddress("h_t", &h_t);
    entries = pulses->GetEntries();

    Float_t k1{-1}, k2{-1}, b{-1}, s{-1}, c{-1}, pa{-1}, pb{-1}, pc{-1}, P0{-1}, P1{-1}, P2{-2}, P3{-1};

    Float_t energia{-1}, corrigidaWang{-1}, corrigidaNadig{-1}, corrigidaPoli{-1}, corrigidaPetSys{-1};

    //==========================================================================================================================
    //
    //
    //                                         Main LOOP
    //
    //
    //
    //==========================================================================================================================

    for (int entry = 0; entry < entries; entry++)
    {
        pulses->GetEntry(entry);

        //  std::cout<<"Processing entry "<<entry<<std::endl;
        //  std::cout<<"chipID :"<<chipIDp<<std::endl;
        //  std::cout<<"channel :"<<channel<<std::endl;

        /*
         *  Retrieves calibration factors for both methods for each channel
         *
         */
        //=====================================================================================
        k1 = cPar.fatoresDeCalibracaoWang[chipIDp][channel]["k1"];
        k2 = cPar.fatoresDeCalibracaoWang[chipIDp][channel]["k2"];
        b = cPar.fatoresDeCalibracaoWang[chipIDp][channel]["b"];

        s = cPar.fatoresDeCalibracaoNadig[chipIDp][channel]["s"];
        c = cPar.fatoresDeCalibracaoNadig[chipIDp][channel]["c"];

        pa = cPar.fatoresDeCalibracaoPoli[chipIDp][channel]["pa"];
        pb = cPar.fatoresDeCalibracaoPoli[chipIDp][channel]["pb"];
        pc = cPar.fatoresDeCalibracaoPoli[chipIDp][channel]["pc"];

        P0 = cPar.fatoresDeCalibracaoPetSys[chipIDp][channel]["P0"];
        P1 = cPar.fatoresDeCalibracaoPetSys[chipIDp][channel]["P1"];
        P2 = cPar.fatoresDeCalibracaoPetSys[chipIDp][channel]["P2"];
        P3 = cPar.fatoresDeCalibracaoPetSys[chipIDp][channel]["P3"];

        //  std::cout<<"k1: "<<k1<<std::endl;
        //  std::cout<<"k2: "<<k2<<std::endl;
        //  std::cout<<"b: "<<b<<std::endl;
        //  std::cout<<"s: "<<s<<std::endl;
        //  std::cout<<"c: "<<c<<std::endl;
        //  std::cout<<"pa: "<<pa<<std::endl;
        //  std::cout<<"pb: "<<pb<<std::endl;
        //  std::cout<<"pc: "<<pc<<std::endl;
        //  std::cout<<"P0: "<<P0<<std::endl;
        //  std::cout<<"P1: "<<P1<<std::endl;
        //  std::cout<<"P2: "<<P2<<std::endl;
        //  std::cout<<"P3: "<<P3<<std::endl;

        // //=====================================================================================
        // /*
        //  *  Corrects the energy that is in DAC units to energy
        //  */
        // //=====================================================================================
        energia = h_e;
        // corrigidaWang   = k1 * (1 - exp(-k2 * energia) + b);
        corrigidaWang = log(k1 / (k1 + b - energia)) * (1 / k2);
        corrigidaNadig = c * s * log(1 / (1 - energia / s));
        corrigidaPoli = pa * pow(energia, 2) + pb * energia + pc;
        corrigidaPetSys = P0 * pow(P1, pow(energia, P2)) + P3 * energia + P0;

        // std::cout<<"energy : "<<energia<<std::endl;
        // std::cout<<"wang : "<<corrigidaWang<<std::endl;
        // std::cout<<"nadig : "<<corrigidaNadig<<std::endl;
        // std::cout<<"Poli : "<<corrigidaPoli<<std::endl;
        // std::cout<<"PetSys : "<<corrigidaPetSys<<std::endl;
        // std::cout<<"Correcting Energies"<<std::endl;

        auto RvecCor = correctEnergyRVec(EnergyVec, chipIDp);

        auto positionsNadig = AngerSimple(RvecCor["nadig"]);

        auto positionsWang = AngerSimple(RvecCor["wang"]);

        auto positionsPoli = AngerSimple(RvecCor["poli"]);

        auto positionsPetSys = AngerSimple(RvecCor["petsys"]);

        // std::cout<<"Positioning Nadig"<<std::endl;
        // auto positionsNadig = AngerSimple(RvecCor["nadig"]);
        // std::cout<<"Positioning wang"<<std::endl;
        // auto positionsWang = AngerSimple(RvecCor["wang"]);
        // std::cout<<"Positioning Poli"<<std::endl;
        // auto positionsPoli = AngerSimple(RvecCor["poli"]);
        // std::cout<<"Positioning PetSys"<<std::endl;
        // auto positionsPetSys = AngerSimple(RvecCor["petsys"]);

        UW = positionsWang["u"];
        UN = positionsNadig["u"];
        VW = positionsWang["v"];
        VN = positionsNadig["v"];

        UPol = positionsPoli["u"];
        VPol = positionsPoli["v"];

        UPS = positionsPetSys["u"];
        VPS = positionsPetSys["v"];

        EPol = corrigidaPoli;
        EPS = corrigidaPetSys;
        EnW = corrigidaWang;
        EnN = corrigidaNadig;
        T = h_t;
        chID = chipIDp;
        calibrated->Fill();

        // // std::cout<<"energia nao corrigida "<<energia<<" Energia corrigida "<<corrigida<<std::endl;
        // //=====================================================================================

        // //=====================================================================================
        // /*
        //  *      Fill Histograms
        //  */
        // //=====================================================================================
        Histogramas.channelRaw[chipIDp][channel]->Fill(h_e);
        Histogramas.canaisCorNadig[chipIDp][channel]->Fill(corrigidaNadig);
        Histogramas.canaisCorWang[chipIDp][channel]->Fill(corrigidaWang);
        Histogramas.canaisCorPoli[chipIDp][channel]->Fill(corrigidaPoli);
        Histogramas.canaisCorPetSys[chipIDp][channel]->Fill(corrigidaPetSys);

        if ((EnN > 425.0) && (EnN < 575.0))
            Histogramas.FloodsNadig[chipIDp]->Fill(positionsNadig["u"], positionsNadig["v"]);
        if ((EnW > 425.0) && (EnW < 575.0))
            Histogramas.FloodsWang[chipIDp]->Fill(positionsWang["u"], positionsWang["v"]);
        if ((EPol > 425.0) && (EPol < 575.0))
            Histogramas.FloodsPoli[chipIDp]->Fill(positionsPoli["u"], positionsPoli["v"]);
        if ((EPS > 425.0) && (EPS < 575.0))
            Histogramas.FloodsPetSys[chipIDp]->Fill(positionsPetSys["u"], positionsPetSys["v"]);

        Histogramas.GlobalHistsNadig[chipIDp]->Fill(corrigidaNadig);
        Histogramas.GlobalHistsWang[chipIDp]->Fill(corrigidaWang);
        Histogramas.GlobalHistsPoli[chipIDp]->Fill(corrigidaPoli);
        Histogramas.GlobalHistsPetSys[chipIDp]->Fill(corrigidaPetSys);

        Histogramas.GlobalHistsRaw[chipIDp]->Fill(h_e);
    }

    //=====================================================================================
    /*
     *      Saves all Data
     */
    //=====================================================================================
    outFile->cd();
    calibrated->Write();
    TDirectory *Detectors = outFile->mkdir("Detectors");

    for (auto &cID : Histogramas.chipIDs)
    {
        std::string dirName = "ChipID-" + std::to_string(cID);
        TDirectory *dir1 = (Detectors->mkdir(dirName.c_str()));
        dir1->cd();
        dir1->WriteObject(Histogramas.FloodsWang[cID].get(), "Floods Wang correction");
        dir1->WriteObject(Histogramas.FloodsNadig[cID].get(), "Floods Nadig correction");

        dir1->WriteObject(Histogramas.FloodsPoli[cID].get(), "Floods Poli correction");
        dir1->WriteObject(Histogramas.FloodsPetSys[cID].get(), "Floods PetSys correction");

        dir1->WriteObject(Histogramas.GlobalHistsWang[cID].get(), "Global Hist Wang correction");
        dir1->WriteObject(Histogramas.GlobalHistsNadig[cID].get(), "Global Hist Nadig correction");

        dir1->WriteObject(Histogramas.GlobalHistsPoli[cID].get(), "Global Hist Poli correction");
        dir1->WriteObject(Histogramas.GlobalHistsPetSys[cID].get(), "Global Hist PetSys correction");

        dir1->WriteObject(Histogramas.GlobalHistsRaw[cID].get(), "Global Hist Raw");

        TDirectory *subdir = dir1->mkdir("Channel_energy");
        subdir->cd();
        for (int ch = 0; ch < 64; ch++)
        {
            std::string channelNameNadig = "channel-" + std::to_string(ch) + "-ChipID-" + std::to_string(cID) + "-Nadig Correction";
            std::string channelNameWang = "channel-" + std::to_string(ch) + "-ChipID-" + std::to_string(cID) + "-Wang Correction";
            std::string channelNamePoli = "channel-" + std::to_string(ch) + "-ChipID-" + std::to_string(cID) + "-Poli Correction";
            std::string channelNamePetSys = "channel-" + std::to_string(ch) + "-ChipID-" + std::to_string(cID) + "-PetSys Correction";
            std::string channelNameRaw = "channel-" + std::to_string(ch) + "-ChipID-" + std::to_string(cID) + "-Raw";

            subdir->WriteObject(Histogramas.canaisCorNadig[cID][ch].get(), channelNameNadig.c_str());
            subdir->WriteObject(Histogramas.canaisCorWang[cID][ch].get(), channelNameWang.c_str());
            subdir->WriteObject(Histogramas.channelRaw[cID][ch].get(), channelNameRaw.c_str());

            subdir->WriteObject(Histogramas.canaisCorPoli[cID][ch].get(), channelNamePoli.c_str());
            subdir->WriteObject(Histogramas.canaisCorPetSys[cID][ch].get(), channelNamePetSys.c_str());
        }
    }
    // TCanvas * c1= new TCanvas ("Comparação correcoes","Comparacao correcoes",1080,790);
    // c1->Divide(2,3);
    // c1->cd(1);
    // GlobalHistsNadig[1]->Draw();
    // c1->cd(2);
    // GlobalHistsWang[1]->Draw();
    // c1->cd(3);
    // canaisCorNadig[1][45]->Draw();
    // c1->cd(4);
    // canaisCorWang[1][45]->Draw();
    // c1->cd(5);
    // FloodsNadig[1]->Draw("COLZ");
    // c1->cd(6);
    // FloodsWang[1]->Draw("COLZ");
    // c1->Draw();
    //

    std::cout << "finished!" << std::endl;
}
