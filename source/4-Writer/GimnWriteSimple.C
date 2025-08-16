class GimnWriteSimple
{

private:
    GimnPulse internalp;
    GimnAQD internalaqd;

    
    std::shared_ptr<TFile> File; //= nullptr;
    std::shared_ptr<TTree> pulses;         //= nullptr;
    std::shared_ptr<TTree> configurations; //= nullptr;

public:

    GimnWriteSimple(const char *fileName)
    {

        ROOT::EnableThreadSafety();
        // Simulate processing
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        // Lock only when accessing shared resources
        {
            std::lock_guard<std::mutex> lock(mtx); // Lock mutex for thread safety
        }
        // std::cout<<"DIFF "<<DIFERENTIATOR<<std::endl;
        // std::cin.get();

        File = std::make_shared<TFile>(fileName, "RECREATE");
        if (VERBOSE)
            std::cout << "setting coincidence tree" << std::endl;

        if (VERBOSE)
            std::cout << "setting Pulses tree" << std::endl;
        pulses = std::make_shared<TTree>("Pulses", Form("Pulses-%d", DIFERENTIATOR));
        SetBranchesPulses();

        if (VERBOSE)
            std::cout << "setting configurations tree" << std::endl;
        configurations = std::make_shared<TTree>("Configurations", Form("Configurations-%d", DIFERENTIATOR));
        SetBranchesAQD();
    }

    void putData( GimnPulse &p)
    {
        p.copyEnergyTo(internalp.energyChanels);
        p.copyTimeTo(internalp.timeChanels);
        
        internalp.chipID = p.chipID;
        internalp.coincidenceID = p.coincidenceID;
        internalp.Port = p.Port;
        internalp.chan = p.chan;
        internalp.first_e = p.first_e;
        internalp.first_t = p.first_t;
        internalp.first_c = p.first_c;
        internalp.first_c = p.first_c;
        internalp.sipm_xmm=p.sipm_xmm;
        internalp.sipm_ymm=p.sipm_ymm;
        internalp.esum= p.esum;
    
        internalp.EnergyVec.clear();
        internalp.TimeVec.clear();
        for (int i = 0; i < 64; i++)
        {
            internalp.EnergyVec.push_back(p.energyChanels[i]);
            internalp.TimeVec.push_back(p.timeChanels[i]);

        }

        pulses->Fill();
    }

    /**
     * @brief Ajusta as referencias internas para cada folha da arvore de Pulsos
     *
     */
    void SetBranchesPulses()
    {
        if (VERBOSE)
            std::cout << "setting branches for pulses" << std::endl;

        pulses->Branch("chipID", &internalp.chipID);              // chip ID from the detector
        pulses->Branch("portID", &internalp.Port);                // Port where the detector is connected
        pulses->Branch("coicidenceID", &internalp.coincidenceID); // Coincidence ID
        pulses->Branch("channel", &internalp.chan);               // channel for the highest Energy
        pulses->Branch("EnergyVec", &internalp.EnergyVec);        // Energy vector, stores the energy vector containing  the energy of each event inside a group
        pulses->Branch("TimeVec", &internalp.TimeVec);            // Time vector, is a vector that contains the time of all detected events inside a group
        pulses->Branch("h_e", &internalp.first_e);                // Highest Energy
        pulses->Branch("h_t", &internalp.first_t);                // Time for the highest Energy
        pulses->Branch("h_c", &internalp.first_c);                // channel for the highest Energy
        pulses->Branch("x_mm",&internalp.sipm_xmm);
        pulses->Branch("y_mm",&internalp.sipm_ymm);
        pulses->Branch("esum",&internalp.esum);
    
    }


    void SetBranchesAQD()
    {
        if (VERBOSE)
            std::cout << "setting branches for configurations" << std::endl;

        configurations->Branch("chipID", &internalaqd.chipID);
        configurations->Branch("Port", &internalaqd.port);
    }

    void putDataAQD(GimnAQD &a)
    {
        internalaqd.chipID = a.chipID;
        internalaqd.port = a.port;

        if (VERBOSE)
        {
            std::cout << " Internal ChipID" << internalaqd.chipID << std::endl;
            std::cout << " Internal port" << internalaqd.port << std::endl;
        }
        configurations->Fill();
    }

    void writeObject(GimnData &Histos)
    {

        File->cd();
        pulses->Write();
        configurations->Write();

        TDirectory *Detectors = File->mkdir("Detectors");
        for (int i = 0; i < Histos.Id.chipID.size(); i++)
        {

            std::string Name = "ChipID-" + std::to_string(Histos.Id.chipID[i]);
            if (VERBOSE)
                std::cout << " Saving " << Name << std::endl;
            TDirectory *aux = (Detectors->mkdir(Name.c_str()));
            aux->cd();

            std::string geName = "global Energy -" + Name + "-" + Histos.Id.detectorName[i];
            aux->WriteObject(Histos.GlobalEnergy[i].get(), geName.c_str());
            std::string fhName = "Flood Histogram-" + Name + "-" + Histos.Id.detectorName[i];
            aux->WriteObject(Histos.FloodHist[i].get(), fhName.c_str());
            std::string doiName = "DOI -" + Name + "-" + Histos.Id.detectorName[i];
            aux->WriteObject(Histos.depthOfInteraction[i].get(), doiName.c_str());

            TDirectory *aux2 = aux->mkdir("Channel_energy");
            aux2->cd();

            if (VERBOSE)
                std::cout << " Saving channels ..." << std::endl;
            for (int ch = 0; ch < 64; ch++)
            {
                std::string channelName = "channel-" + std::to_string(ch) + "-" + Name + "-" + Histos.Id.detectorName[i];
                if (VERBOSE)
                    std::cout << " Saving channel " << channelName << std::endl;

                aux2->WriteObject(Histos.channelEnergyHistogram[i][ch].get(), channelName.c_str());
            }

            // Moves Back
            aux->cd();

            TDirectory *aux3 = aux->mkdir("Channel_timeSKew");
            aux3->cd();
            if (VERBOSE)
                std::cout << " Saving TimeSkew ..." << std::endl;

            for (int ch = 0; ch < 64; ch++)
            {
                std::string channelName = "channel-" + std::to_string(ch) + "-" + Name + "-" + Histos.Id.detectorName[i];
                if (VERBOSE)
                    std::cout << channelName << std::endl;

                aux3->WriteObject(Histos.channelTimeSkew[i][ch].get(), channelName.c_str());
            }
            aux->cd();
            TDirectory *aux4 = aux->mkdir("Channel_timeWalk");
            aux4->cd();
            if (VERBOSE)
                std::cout << " Saving Time Walk ..." << std::endl;
            for (int ch = 0; ch < 64; ch++)
            {
                std::string channelName = "channel-" + std::to_string(ch) + "-" + Name + "-" + Histos.Id.detectorName[i];
                if (VERBOSE)
                    std::cout << channelName << std::endl;
                aux4->WriteObject(Histos.channelTimeWalk[i][ch].get(), channelName.c_str());
            }
        }

        // File->Write();
    }
    void close()
    {

        File->Close();
    }
};