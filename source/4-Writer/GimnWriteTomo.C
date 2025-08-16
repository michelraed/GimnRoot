class GimnWriteTomo
{
private:
    std::shared_ptr<TFile> File;                    //= nullptr;
    std::shared_ptr<TTree> tomographicSingles;      //= nullptr;
    std::shared_ptr<TTree> tomographicCoincidences; //= nullptr;

    TomographicCoincidenceEvent internalTomoCoin;
    TomographicSingleEvent internalTomoSingle;

public:
    GimnWriteTomo()
    {

        SetBranchesCoincidencesTomo();
        SetBranchesSinglesTomo();
    }

    GimnWriteTomo(const char *fileName)
    {

        ROOT::EnableThreadSafety();
        // Simulate processing
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        // Lock only when accessing shared resources
        {
            std::lock_guard<std::mutex> lock(mtx); // Lock mutex for thread safety
        }
        // std::cout<<"DIFF "<<DIFERENTIATOR<<std::endl;

        File = std::make_shared<TFile>(fileName, "RECREATE");
        if (VERBOSE)
            std::cout << "setting coincidence tree" << std::endl;
        if (VERBOSE)
            std::cout << "setting Tomographic Coincidences tree" << std::endl;
        tomographicCoincidences = std::make_shared<TTree>("tomographicCoincidences", Form("tomographicCoincidences-%d", DIFERENTIATOR));
        SetBranchesCoincidencesTomo();

        if (VERBOSE)
            std::cout << "setting Tomographic Singles tree" << std::endl;
        tomographicSingles = std::make_shared<TTree>("tomographicSingles", Form("tomographicSingles-%d", DIFERENTIATOR));
        SetBranchesSinglesTomo();
    }

    void SetBranchesCoincidencesTomo()
    {
        if (VERBOSE)
            std::cout << "setting branches for tomographic Coincidences" << std::endl;
        std::lock_guard<std::mutex> lock(mtx);
        tomographicCoincidences->Branch("mh_n_1", &internalTomoCoin.mh_n1);
        tomographicCoincidences->Branch("chipID_1", &internalTomoCoin.chipID1);
        tomographicCoincidences->Branch("channelID_1", &internalTomoCoin.channelID1);
        tomographicCoincidences->Branch("energy_1", &internalTomoCoin.energy1);
        tomographicCoincidences->Branch("esum_1", &internalTomoCoin.esum1);
        tomographicCoincidences->Branch("slice_1", &internalTomoCoin.slice1);
        tomographicCoincidences->Branch("posI_1", &internalTomoCoin.posI1);
        tomographicCoincidences->Branch("posJ_1", &internalTomoCoin.posJ1);
        tomographicCoincidences->Branch("SiPMPosX_1", &internalTomoCoin.SiPMPosX1);
        tomographicCoincidences->Branch("SiPMPosY_1", &internalTomoCoin.SiPMPosY1);
        tomographicCoincidences->Branch("angerPosX_1", &internalTomoCoin.angerPosX1);
        tomographicCoincidences->Branch("angerPosY_1", &internalTomoCoin.angerPosY1);
        tomographicCoincidences->Branch("angleSino_1", &internalTomoCoin.angleSino1);
        tomographicCoincidences->Branch("rSino_1", &internalTomoCoin.rSino1);
        tomographicCoincidences->Branch("angleGantry_1", &internalTomoCoin.angleGantry1);
        tomographicCoincidences->Branch("timeI_1", &internalTomoCoin.timeI1);
        tomographicCoincidences->Branch("timeSec_1", &internalTomoCoin.timeSec1);
        tomographicCoincidences->Branch("energyCorrected_1", &internalTomoCoin.energyCorrected1);

        tomographicCoincidences->Branch("globalPosXpre_1", &internalTomoCoin.globalPosXpre1);
        tomographicCoincidences->Branch("globalPosYpre_1", &internalTomoCoin.globalPosYpre1);
        tomographicCoincidences->Branch("globalPosZpre_1", &internalTomoCoin.globalPosZpre1);

        tomographicCoincidences->Branch("globalPosX_1", &internalTomoCoin.globalPosX1);
        tomographicCoincidences->Branch("globalPosY_1", &internalTomoCoin.globalPosY1);
        tomographicCoincidences->Branch("globalPosZ_1", &internalTomoCoin.globalPosZ1);

        tomographicCoincidences->Branch("globalPosXpreAnger_1", &internalTomoCoin.globalPosXpreAnger1);
        tomographicCoincidences->Branch("globalPosYpreAnger_1", &internalTomoCoin.globalPosYpreAnger1);
        tomographicCoincidences->Branch("globalPosZpreAnger_1", &internalTomoCoin.globalPosZpreAnger1);

        tomographicCoincidences->Branch("globalPosXAnger_1", &internalTomoCoin.globalPosXAnger1);
        tomographicCoincidences->Branch("globalPosYAnger_1", &internalTomoCoin.globalPosYAnger1);
        tomographicCoincidences->Branch("globalPosZAnger_1", &internalTomoCoin.globalPosZAnger1);

        tomographicCoincidences->Branch("mh_n_2", &internalTomoCoin.mh_n2);
        tomographicCoincidences->Branch("chipID_2", &internalTomoCoin.chipID2);
        tomographicCoincidences->Branch("channelID_2", &internalTomoCoin.channelID2);
        tomographicCoincidences->Branch("energy_2", &internalTomoCoin.energy2);
        tomographicCoincidences->Branch("esum_2", &internalTomoCoin.esum2);
        tomographicCoincidences->Branch("slice_2", &internalTomoCoin.slice2);
        tomographicCoincidences->Branch("posI_2", &internalTomoCoin.posI2);
        tomographicCoincidences->Branch("posJ_2", &internalTomoCoin.posJ2);
        tomographicCoincidences->Branch("SiPMPosX_2", &internalTomoCoin.SiPMPosX2);
        tomographicCoincidences->Branch("SiPMPosY_2", &internalTomoCoin.SiPMPosY2);
        tomographicCoincidences->Branch("angerPosX_2", &internalTomoCoin.angerPosX2);
        tomographicCoincidences->Branch("angerPosY_2", &internalTomoCoin.angerPosY2);
        tomographicCoincidences->Branch("angleSino_2", &internalTomoCoin.angleSino2);
        tomographicCoincidences->Branch("rSino_2", &internalTomoCoin.rSino2);
        tomographicCoincidences->Branch("angleGantry_2", &internalTomoCoin.angleGantry2);
        tomographicCoincidences->Branch("timeI_2", &internalTomoCoin.timeI2);
        tomographicCoincidences->Branch("timeSec_2", &internalTomoCoin.timeSec2);
        tomographicCoincidences->Branch("energyCorrected_2", &internalTomoCoin.energyCorrected2);

        tomographicCoincidences->Branch("globalPosXpre_2", &internalTomoCoin.globalPosXpre2);
        tomographicCoincidences->Branch("globalPosYpre_2", &internalTomoCoin.globalPosYpre2);
        tomographicCoincidences->Branch("globalPosZpre_2", &internalTomoCoin.globalPosZpre2);

        tomographicCoincidences->Branch("globalPosX_2", &internalTomoCoin.globalPosX2);
        tomographicCoincidences->Branch("globalPosY_2", &internalTomoCoin.globalPosY2);
        tomographicCoincidences->Branch("globalPosZ_2", &internalTomoCoin.globalPosZ2);

        tomographicCoincidences->Branch("globalPosXpreAnger_2", &internalTomoCoin.globalPosXpreAnger2);
        tomographicCoincidences->Branch("globalPosYpreAnger_2", &internalTomoCoin.globalPosYpreAnger2);
        tomographicCoincidences->Branch("globalPosZpreAnger_2", &internalTomoCoin.globalPosZpreAnger2);

        tomographicCoincidences->Branch("globalPosXAnger_2", &internalTomoCoin.globalPosXAnger2);
        tomographicCoincidences->Branch("globalPosYAnger_2", &internalTomoCoin.globalPosYAnger2);
        tomographicCoincidences->Branch("globalPosZAnger_2", &internalTomoCoin.globalPosZAnger2);

        tomographicCoincidences->Branch("angleSinoAnger_1", &internalTomoCoin.angleSinoAnger1);
        tomographicCoincidences->Branch("angleSinoAnger_2", &internalTomoCoin.angleSinoAnger2);
        tomographicCoincidences->Branch("rSinoAnger_1", &internalTomoCoin.rSinoAnger1);
        tomographicCoincidences->Branch("rSinoAnger_2", &internalTomoCoin.rSinoAnger2);




    }

    void SetBranchesSinglesTomo()
    {
        if (VERBOSE)
            std::cout << "setting branches for coincidences" << endl;
        std::lock_guard<std::mutex> lock(mtx);
        tomographicSingles->Branch("mh_n", &internalTomoSingle.mh_n);
        tomographicSingles->Branch("chipID", &internalTomoSingle.chipID);
        tomographicSingles->Branch("channelID", &internalTomoSingle.channelID);
        tomographicSingles->Branch("energy", &internalTomoSingle.energy);
        tomographicSingles->Branch("slice", &internalTomoSingle.slice);
        tomographicSingles->Branch("posI", &internalTomoSingle.posI);
        tomographicSingles->Branch("posJ", &internalTomoSingle.posJ);
        tomographicSingles->Branch("esum", &internalTomoSingle.esum);
        tomographicSingles->Branch("SiPMPosX", &internalTomoSingle.SiPMPosX);
        tomographicSingles->Branch("SiPMPosY", &internalTomoSingle.SiPMPosY);
        tomographicSingles->Branch("angerPosX", &internalTomoSingle.angerPosX);
        tomographicSingles->Branch("angerPosY", &internalTomoSingle.angerPosY);
        tomographicSingles->Branch("angleSino", &internalTomoSingle.angleSino);
        tomographicSingles->Branch("rSino", &internalTomoSingle.rSino);
        tomographicSingles->Branch("angleGantry", &internalTomoSingle.angleGantry);
        tomographicSingles->Branch("timeI", &internalTomoSingle.timeI);
        tomographicSingles->Branch("timeSec", &internalTomoSingle.timeSec);
        tomographicSingles->Branch("energyCorrected", &internalTomoSingle.energyCorrected);

        tomographicSingles->Branch("globalPosXpre", &internalTomoSingle.globalPosXpre);
        tomographicSingles->Branch("globalPosYpre", &internalTomoSingle.globalPosYpre);
        tomographicSingles->Branch("globalPosZpre", &internalTomoSingle.globalPosZpre);

        tomographicSingles->Branch("globalPosX", &internalTomoSingle.globalPosX);
        tomographicSingles->Branch("globalPosY", &internalTomoSingle.globalPosY);
        tomographicSingles->Branch("globalPosZ", &internalTomoSingle.globalPosZ);

        tomographicSingles->Branch("globalPosXpreAnger", &internalTomoSingle.globalPosXpreAnger);
        tomographicSingles->Branch("globalPosYpreAnger", &internalTomoSingle.globalPosYpreAnger);
        tomographicSingles->Branch("globalPosZpreAnger", &internalTomoSingle.globalPosZpreAnger);

        tomographicSingles->Branch("globalPosXAnger", &internalTomoSingle.globalPosXAnger);
        tomographicSingles->Branch("globalPosYAnger", &internalTomoSingle.globalPosYAnger);
        tomographicSingles->Branch("globalPosZAnger", &internalTomoSingle.globalPosZAnger);

           tomographicSingles->Branch("angleSinoAnger", &internalTomoSingle.angleSinoAnger);
        tomographicSingles->Branch("rSinoAnger", &internalTomoSingle.rSinoAnger);
    }


    void putData(const TomographicCoincidenceEvent &tce)
    {
        //std::lock_guard<std::mutex> lock(mtx);
        internalTomoCoin.mh_n1 = tce.mh_n1;
        internalTomoCoin.chipID1 = tce.chipID1;
        internalTomoCoin.channelID1 = tce.channelID1;
        internalTomoCoin.energy1 = tce.energy1;
        internalTomoCoin.esum1 = tce.esum1;
        internalTomoCoin.slice1 = tce.slice1;
        internalTomoCoin.posI1 = tce.posI1;
        internalTomoCoin.posJ1 = tce.posJ1;
        internalTomoCoin.SiPMPosX1 = tce.SiPMPosX1;
        internalTomoCoin.SiPMPosY1 = tce.SiPMPosY1;
        internalTomoCoin.angerPosX1 = tce.angerPosX1;
        internalTomoCoin.angerPosY1 = tce.angerPosY1;
        internalTomoCoin.angleSino1 = tce.angleSino1;
        internalTomoCoin.rSino1 = tce.rSino1;
        internalTomoCoin.angleGantry1 = tce.angleGantry1;
        internalTomoCoin.timeI1 = tce.timeI1;
        internalTomoCoin.timeSec1 = tce.timeSec1;
        internalTomoCoin.energyCorrected1 = tce.energyCorrected1;

        internalTomoCoin.globalPosXpre1 = tce.globalPosXpre1;
        internalTomoCoin.globalPosYpre1 = tce.globalPosYpre1;
        internalTomoCoin.globalPosZpre1 = tce.globalPosZpre1;

        internalTomoCoin.globalPosX1 = tce.globalPosX1;
        internalTomoCoin.globalPosY1 = tce.globalPosY1;
        internalTomoCoin.globalPosZ1 = tce.globalPosZ1;

        internalTomoCoin.globalPosXpreAnger1 = tce.globalPosXpreAnger1;
        internalTomoCoin.globalPosYpreAnger1 = tce.globalPosYpreAnger1;
        internalTomoCoin.globalPosZpreAnger1 = tce.globalPosZpreAnger1;

        internalTomoCoin.globalPosXAnger1 = tce.globalPosXAnger1;
        internalTomoCoin.globalPosYAnger1 = tce.globalPosYAnger1;
        internalTomoCoin.globalPosZAnger1 = tce.globalPosZAnger1;

        internalTomoCoin.mh_n2 = tce.mh_n2;
        internalTomoCoin.chipID2 = tce.chipID2;
        internalTomoCoin.channelID2 = tce.channelID2;
        internalTomoCoin.energy2 = tce.energy2;
        internalTomoCoin.esum2 = tce.esum2;
        internalTomoCoin.slice2 = tce.slice2;
        internalTomoCoin.posI2 = tce.posI2;
        internalTomoCoin.posJ2 = tce.posJ2;
        internalTomoCoin.SiPMPosX2 = tce.SiPMPosX2;
        internalTomoCoin.SiPMPosY2 = tce.SiPMPosY2;
        internalTomoCoin.angerPosX2 = tce.angerPosX2;
        internalTomoCoin.angerPosY2 = tce.angerPosY2;
        internalTomoCoin.angleSino2 = tce.angleSino2;
        internalTomoCoin.rSino2 = tce.rSino2;
        internalTomoCoin.angleGantry2 = tce.angleGantry2;
        internalTomoCoin.timeI2 = tce.timeI2;
        internalTomoCoin.timeSec2 = tce.timeSec2;
        internalTomoCoin.energyCorrected2 = tce.energyCorrected2;

        internalTomoCoin.globalPosXpre2 = tce.globalPosXpre2;
        internalTomoCoin.globalPosYpre2 = tce.globalPosYpre2;
        internalTomoCoin.globalPosZpre2 = tce.globalPosZpre2;

        internalTomoCoin.globalPosX2 = tce.globalPosX2;
        internalTomoCoin.globalPosY2 = tce.globalPosY2;
        internalTomoCoin.globalPosZ2 = tce.globalPosZ2;

        internalTomoCoin.globalPosXpreAnger2 = tce.globalPosXpreAnger2;
        internalTomoCoin.globalPosYpreAnger2 = tce.globalPosYpreAnger2;
        internalTomoCoin.globalPosZpreAnger2 = tce.globalPosZpreAnger2;

        internalTomoCoin.globalPosXAnger2 = tce.globalPosXAnger2;
        internalTomoCoin.globalPosYAnger2 = tce.globalPosYAnger2;
        internalTomoCoin.globalPosZAnger2 = tce.globalPosZAnger2;

        internalTomoCoin.angleSinoAnger1 = tce.angleSinoAnger1;
        internalTomoCoin.rSinoAnger1 = tce.rSinoAnger1;

        internalTomoCoin.angleSinoAnger2 = tce.angleSinoAnger2;
        internalTomoCoin.rSinoAnger2 = tce.rSinoAnger2;

        tomographicCoincidences->Fill();
    }

    void putData(const TomographicSingleEvent &tse)
    {
        //std::lock_guard<std::mutex> lock(mtx);
        internalTomoSingle.mh_n = tse.mh_n;
        internalTomoSingle.chipID = tse.chipID;
        internalTomoSingle.channelID = tse.channelID;
        internalTomoSingle.energy = tse.energy;
        internalTomoSingle.energyCorrected = tse.energyCorrected;

        internalTomoSingle.esum = tse.esum;
        internalTomoSingle.slice = tse.slice;
        internalTomoSingle.posI = tse.posI;
        internalTomoSingle.posJ = tse.posJ;
        internalTomoSingle.SiPMPosX = tse.SiPMPosX;
        internalTomoSingle.SiPMPosY = tse.SiPMPosY;
        internalTomoSingle.angerPosX = tse.angerPosX;
        internalTomoSingle.angerPosY = tse.angerPosY;
        internalTomoSingle.angleSino = tse.angleSino;
        internalTomoSingle.rSino = tse.rSino;
        internalTomoSingle.chipID = tse.angleGantry;
        internalTomoSingle.timeI = tse.timeI;
        internalTomoSingle.timeSec = tse.timeSec;

        internalTomoSingle.globalPosXpre = tse.globalPosXpre;
        internalTomoSingle.globalPosYpre = tse.globalPosYpre;
        internalTomoSingle.globalPosZpre = tse.globalPosZpre;

        internalTomoSingle.globalPosX = tse.globalPosX;
        internalTomoSingle.globalPosY = tse.globalPosY;
        internalTomoSingle.globalPosZ = tse.globalPosZ;

        internalTomoSingle.globalPosXpreAnger = tse.globalPosXpreAnger;
        internalTomoSingle.globalPosYpreAnger = tse.globalPosYpreAnger;
        internalTomoSingle.globalPosZpreAnger = tse.globalPosZpreAnger;

        internalTomoSingle.globalPosXAnger = tse.globalPosXAnger;
        internalTomoSingle.globalPosYAnger = tse.globalPosYAnger;
        internalTomoSingle.globalPosZAnger = tse.globalPosZAnger;



        internalTomoSingle.angleSinoAnger = tse.angleSinoAnger;
        internalTomoSingle.rSinoAnger = tse.rSinoAnger;

        
        tomographicSingles->Fill();
    }

    void writeObject(HistogramCoincidences &Histos, Identificator identificator, std::string method)
    {
     
        std::lock_guard<std::mutex> lock(mtx); // Lock mutex for thread safety


        File->cd();

        tomographicSingles->Write();
        tomographicCoincidences->Write();

        TDirectory *Detectors = File->mkdir("Detectors");
        for (int i = 0; i < identificator.chipID.size(); i++)
        {

            int chpID = identificator.chipID[i];

            std::string Name = "ChipID-" + std::to_string(identificator.chipID[i]);
            if (VERBOSE)
                std::cout << " Saving " << Name << std::endl;
            TDirectory *aux = (Detectors->mkdir(Name.c_str()));
            aux->cd();

            std::string geName1 = "global Energy Corrected -" + method + " - " + Name + "-" + identificator.detectorName[i];
            aux->WriteObject(Histos.globalEnergyHistogramCorrected[chpID].get(), geName1.c_str());

            std::string geName2 = "global Energy Raw-" + method + " - " + Name + "-" + identificator.detectorName[i];
            aux->WriteObject(Histos.globalEnergyHistogramRaw[chpID].get(), geName2.c_str());

            std::string fhName = "Flood Raw -" + Name + "-" + identificator.detectorName[i];
            aux->WriteObject(Histos.floodRaw[chpID].get(), fhName.c_str());

            std::string fhName2 = "Flood Histogram -" + method + " - " + Name + "-" + identificator.detectorName[i];
            aux->WriteObject(Histos.floodCorrected[chpID].get(), fhName2.c_str());

            TDirectory *aux2 = aux->mkdir("Channel_energy");
            aux2->cd();

            if (VERBOSE)
                std::cout << " Saving channels ..." << std::endl;
            for (int ch = 0; ch < 64; ch++)
            {
                std::string channelNameRaw = "channel-" + std::to_string(ch) + "-" + Name + "-" + identificator.detectorName[i];
                std::string channelNameCor = "channel-" + std::to_string(ch) + "-" + method + " - " + Name + "-" + identificator.detectorName[i];

                std::cout << i << " channel " << ch << std::endl;
                if (true)
                {
                    std::cout << " Saving channel " << channelNameCor << std::endl;
                    std::cout << " Saving channel " << channelNameRaw << std::endl;
                    std::cout << " Name " << Histos.rawEnergyChannel[chpID][ch]->GetName() << std::endl;
                    std::cout << " mean " << Histos.rawEnergyChannel[chpID][ch]->GetMean() << std::endl;
                }

                aux2->WriteObject(Histos.rawEnergyChannel[chpID][ch].get(), channelNameRaw.c_str());
                aux2->WriteObject(Histos.correctedEnergyChannel[chpID][ch].get(), channelNameCor.c_str());
            }

            // Moves Back
            aux->cd();

            TDirectory *aux3 = aux->mkdir("Channel_timeSKew");
            aux3->cd();
            if (VERBOSE)
                std::cout << " Saving TimeSkew ..." << std::endl;

            for (int ch = 0; ch < 64; ch++)
            {
                std::string channelName = "channel-" + std::to_string(ch) + "-" + Name + "-" + identificator.detectorName[i];
                if (VERBOSE)
                    std::cout << channelName << std::endl;

                aux3->WriteObject(Histos.timeSkew[chpID][ch].get(), channelName.c_str());
            }
            aux->cd();
            TDirectory *aux4 = aux->mkdir("Channel_timeWalk");
            aux4->cd();
            if (VERBOSE)
                std::cout << " Saving Time Walk ..." << std::endl;
            for (int ch = 0; ch < 64; ch++)
            {
                std::string channelName = "channel-" + std::to_string(ch) + "-" + Name + "-" + identificator.detectorName[i];
                if (VERBOSE)
                    std::cout << channelName << std::endl;
                aux4->WriteObject(Histos.timeWalk[chpID][ch].get(), channelName.c_str());
            }
        }

        // File->Write();
    }

    void close()
    {
        std::lock_guard<std::mutex> lock(mtx); // Lock mutex for thread safety

        std::cout << "Closing File!" << std::endl;
        File->Close();
    }
};