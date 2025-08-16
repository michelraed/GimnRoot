#include <TFile.h>
#include <TTree.h>
#include <TDirectory.h>

/**
 * @brief GimnWrite será a classe responsável pela escrita dos dados processados e dos histogramas gerados
 * Esta irá gerar um arquivo do tipo *.root, cntendo em seu interior duas arvores, uma contendo os eventos separados individualmente
 * e outra contendo as coincidencias. Além disso no interior desta classe serão geradas pastas contendo todos os histogramas produzidos
 * por canal e globais.
 */
class GimnWriteClass
{
private:
    std::shared_ptr<TFile> File;           //= nullptr;
    std::shared_ptr<TTree> coincidences;   //= nullptr;
    std::shared_ptr<TTree> singles;        //= nullptr;
    std::shared_ptr<TTree> pulses;         //= nullptr;
    std::shared_ptr<TTree> configurations; //= nullptr;

    // std::shared_ptr<TTree> tomographicSingles;      //= nullptr;
    // std::shared_ptr<TTree> tomographicCoincidences; //= nullptr;

    // TomographicCoincidenceEvent internalTomoCoin;
    // TomographicSingleEvent internalTomoSingle;
    GimnAQD internalaqd;
    GimnEvent internal;
    GimnPulse internalp;

public:
    GimnWriteClass()
    {
        SetBranchesCoincidences();
        SetBranchesSingles();
        SetBranchesPulses();
        SetBranchesAQD();
        // SetBranchesCoincidencesTomo();
        // SetBranchesSinglesTomo();
    }

    GimnWriteClass(const char *fileName)
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
        coincidences = std::make_shared<TTree>("Coincidences", Form("Coincidences-%d", DIFERENTIATOR));
        SetBranchesCoincidences();
        if (VERBOSE)
            std::cout << "setting coincidence tree" << std::endl;
        singles = std::make_shared<TTree>("Singles", Form("Singles-%d", DIFERENTIATOR));
        SetBranchesSingles();
        if (VERBOSE)
            std::cout << "setting Pulses tree" << std::endl;
        pulses = std::make_shared<TTree>("Pulses", Form("Pulses-%d", DIFERENTIATOR));
        SetBranchesPulses();
        if (VERBOSE)
            std::cout << "setting configurations tree" << std::endl;
        configurations = std::make_shared<TTree>("Configurations", Form("Configurations-%d", DIFERENTIATOR));
        SetBranchesAQD();
        
        // if (VERBOSE)
        //     std::cout << "setting Tomographic Coincidences tree" << std::endl;
        // tomographicCoincidences = std::make_shared<TTree>("tomographicCoincidences", Form("tomographicCoincidences-%d", DIFERENTIATOR));
        // SetBranchesCoincidencesTomo();

        // if (VERBOSE)
        //     std::cout << "setting Tomographic Singles tree" << std::endl;
        // tomographicSingles = std::make_shared<TTree>("tomographicSingles", Form("tomographicSingles-%d", DIFERENTIATOR));
        // SetBranchesSinglesTomo();
    }
    /**
     * @brief Este método irá carregar os dados processados em cada evento para as arvores que serão escritas no arquivo processado
     *
     * @param ev Estrutura do tipo GimnEvent que irá possuir informações a cerca da posição energia e tempo do evento
     */
    void putData(const GimnEvent &ev)
    {

        internal.Energy_1 = ev.Energy_1;
        internal.time_1 = ev.time_1;
        internal.channel_1 = ev.channel_1;
        internal.xmm_1 = ev.xmm_1;
        internal.ymm_1 = ev.ymm_1;
        internal.doi_1 = ev.doi_1;
        internal.Port_1 = ev.Port_1;
        internal.coincidenceID = ev.coincidenceID;
        internal.sipmX_1 = ev.sipmX_1;
        internal.sipmY_1 = ev.sipmY_1;
        internal.Energy_2 = ev.Energy_2;
        internal.time_2 = ev.time_2;
        internal.channel_2 = ev.channel_2;
        internal.xmm_2 = ev.xmm_2;
        internal.ymm_2 = ev.ymm_2;
        internal.doi_2 = ev.doi_2;
        internal.Port_2 = ev.Port_2;
        internal.coincidenceID = ev.coincidenceID;
        internal.sipmX_2 = ev.sipmX_2;
        internal.sipmY_2 = ev.sipmY_2;
        internal.CTR = ev.CTR;
        internal.Energy = ev.Energy;
        internal.time = ev.time;
        internal.channel = ev.channel;
        internal.xmm = ev.xmm;
        internal.ymm = ev.ymm;
        internal.doi = ev.doi;
        internal.Port = ev.Port;
        internal.coincidenceID = ev.coincidenceID;
        internal.sipmX = ev.sipmX;
        internal.sipmY = ev.sipmY;
        internal.angle = ev.angle;
        internal.distance = ev.distance;
        internal.slice = ev.slice;

        // std::cout<<"internal energy"<<internal.Energy_1<<std::endl;

        coincidences->Fill();
        singles->Fill();
    }

    /**
     * @brief Este método irá carregar os dados processados em cada evento para as arvores que serão escritas no arquivo processado
     *
     * @param ev Estrutura do tipo GimnEvent que irá possuir informações a cerca da posição energia e tempo do evento
     */
    void putData(const GimnEvent &ev, GimnPulse &p)
    {
        p.copyEnergyTo(internalp.energyChanels);
        p.copyTimeTo(internalp.timeChanels);
        internalp.chipID = p.chipID;
        internalp.coincidenceID = p.coincidenceID;
        internalp.Port = p.Port;
        internalp.first_e = p.first_e;
        internalp.first_t = p.first_t;
        internalp.first_c = p.first_c;
        internalp.sipm_xmm = p.sipm_ymm;
        internalp.sipm_ymm = p.sipm_ymm;

        internalp.EnergyVec.clear();
        internalp.TimeVec.clear();
        for (int i = 0; i < 64; i++)
        {
            internalp.EnergyVec.push_back(p.energyChanels[i]);
            internalp.TimeVec.push_back(p.timeChanels[i]);
        }

        internal.Energy_1 = ev.Energy_1;
        internal.time_1 = ev.time_1;
        internal.channel_1 = ev.channel_1;
        internal.xmm_1 = ev.xmm_1;
        internal.ymm_1 = ev.ymm_1;
        internal.doi_1 = ev.doi_1;
        internal.Port_1 = ev.Port_1;
        internal.coincidenceID = ev.coincidenceID;
        internal.sipmX_1 = ev.sipmX_1;
        internal.sipmY_1 = ev.sipmY_1;
        internal.Energy_2 = ev.Energy_2;
        internal.time_2 = ev.time_2;
        internal.channel_2 = ev.channel_2;
        internal.xmm_2 = ev.xmm_2;
        internal.ymm_2 = ev.ymm_2;
        internal.doi_2 = ev.doi_2;
        internal.Port_2 = ev.Port_2;
        internal.coincidenceID = ev.coincidenceID;
        internal.sipmX_2 = ev.sipmX_2;
        internal.sipmY_2 = ev.sipmY_2;
        internal.CTR = ev.CTR;
        internal.Energy = ev.Energy;
        internal.time = ev.time;
        internal.channel = ev.channel;
        internal.xmm = ev.xmm;
        internal.ymm = ev.ymm;
        internal.doi = ev.doi;
        internal.Port = ev.Port;
        internal.coincidenceID = ev.coincidenceID;
        internal.sipmX = ev.sipmX;
        internal.sipmY = ev.sipmY;
        internal.angle = ev.angle;
        internal.distance = ev.distance;
        internal.slice = ev.slice;

        // std::cout<<"internal energy"<<internal.Energy_1<<std::endl;

        coincidences->Fill();
        singles->Fill();
        pulses->Fill();
    }

    /**
     * @brief Ajusta as referencias internas para cada folha da arvore de coincidencias
     *
     */
    void SetBranchesCoincidences()
    {
        if (VERBOSE)
            std::cout << "setting branches for coincidences" << endl;
        coincidences->Branch("Energy1", &internal.Energy_1);
        coincidences->Branch("time1", &internal.time_1);
        coincidences->Branch("channel1", &internal.channel_1);
        coincidences->Branch("xmm1", &internal.xmm_1);
        coincidences->Branch("ymm1", &internal.ymm_1);
        coincidences->Branch("doi1", &internal.doi_1);
        coincidences->Branch("Port1", &internal.Port_1);
        coincidences->Branch("sipmX1", &internal.sipmX_1);
        coincidences->Branch("sipmY1", &internal.sipmY_1);
        coincidences->Branch("Energy2", &internal.Energy_2);
        coincidences->Branch("time2", &internal.time_2);
        coincidences->Branch("channel2", &internal.channel_2);
        coincidences->Branch("xmm2", &internal.xmm_2);
        coincidences->Branch("ymm2", &internal.ymm_2);
        coincidences->Branch("doi2", &internal.doi_2);
        coincidences->Branch("Port2", &internal.Port_2);
        coincidences->Branch("sipmX2", &internal.sipmX_2);
        coincidences->Branch("sipmY2", &internal.sipmY_2);
        coincidences->Branch("CTR", &internal.CTR);
        coincidences->Branch("angle", &internal.angle);
        coincidences->Branch("distance", &internal.distance);
        coincidences->Branch("slice", &internal.slice);
        coincidences->Branch("coincidenceID", &internal.coincidenceID);
    }

    /**
     * @brief Ajusta as folhas da arvore de singles
     *
     */
    void SetBranchesSingles()
    {
        if (VERBOSE)
            std::cout << "setting branches for Singles" << endl;
        singles->Branch("Energy", &internal.Energy);
        singles->Branch("time", &internal.time);
        singles->Branch("channel", &internal.channel);
        singles->Branch("xmm", &internal.xmm);
        singles->Branch("ymm", &internal.ymm);
        singles->Branch("doi", &internal.doi);
        singles->Branch("Port", &internal.Port);
        singles->Branch("coincidenceID", &internal.coincidenceID);
        singles->Branch("sipmX", &internal.sipmX);
        singles->Branch("sipmY", &internal.sipmY);
    }

    /**
     * @brief Ajusta as referencias internas para cada folha da arvore de coincidencias
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
        pulses->Branch("sipm_xmm", &internalp.sipm_xmm);          // channel for the highest Energy
        pulses->Branch("sipm_ymm", &internalp.sipm_ymm);          // channel for the highest Energy
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

    void writeObject(GimnData &Histos, std::vector<std::shared_ptr<TCanvas>> Canvas)
    {

        File->cd();

        coincidences->Write();
        singles->Write();
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
        if (VERBOSE)
            std::cout << " Saving Canvas ..." << std::endl;

        TDirectory *Canvases = File->mkdir("Canvas");
        for (int i = 0; i < Canvas.size(); i++)
        {
            std::string cvName = "Canvas -" + std::to_string(i);
            Canvases->WriteObject(Canvas[i].get(), cvName.c_str());
            if (VERBOSE)
                std::cout << cvName << std::endl;
        }

        // File->Write();
    }

    void writeObject(GimnData &Histos)
    {

        File->cd();

        coincidences->Write();
        singles->Write();
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

    /**
     * @brief Guarda os dados dos diversos histogramas coletados em pastas dentro do arquivo root gerado
     *
     * @param chanellHistogram vetor de vetores de TH1F correspondendo aos histogramas de energia de cada canal de cada detector
     * @param GlobalEnergy vetor de TH1F corresponde aos histograma de energia global de cada detector
     * @param depthOfInteraction vetor TH2F que corresponde à profundidade de interação dos fotons detectados (valido para monolitico) por detector
     * @param FloodHist vetor de TH2F que corresponde ao mapa de flood de cada detector
     * @param detectorName vetor contendo os nomes dos detectores
     * @param chpid vetor contendo os chipID's ativos durante a aquisição
     * @param Canvas vetor de TCanvas contendo as informações da aquisição
     */
    void writeObject(vector<vector<TH1F *>> chanellHistogram, vector<TH1F *> GlobalEnergy, vector<TH1F *> depthOfInteraction, vector<TH2F *> FloodHist, vector<std::string> detectorName, vector<int> chpid, std::vector<TCanvas *> Canvas)
    {
        File->cd();
        coincidences->Write();
        singles->Write();
        pulses->Write();

        TDirectory *Detectors = File->mkdir("Detectors");
        for (int i = 0; i < chpid.size(); i++)
        {
            std::string Name = "ChipID-" + std::to_string(chpid[i]);
            TDirectory *aux = Detectors->mkdir(Name.c_str());
            aux->cd();
            std::string geName = "global Energy -" + Name + "-" + detectorName[i];
            aux->WriteObject(GlobalEnergy[i], geName.c_str());
            std::string fhName = "Flood Histogram-" + Name + "-" + detectorName[i];
            aux->WriteObject(FloodHist[i], fhName.c_str());
            std::string doiName = "DOI -" + Name + "-" + detectorName[i];
            aux->WriteObject(depthOfInteraction[i], doiName.c_str());
            TDirectory *aux2 = aux->mkdir("Channels");
            aux2->cd();

            for (int ch = 0; ch < 64; ch++)
            {
                std::string channelName = "channel-" + std::to_string(ch) + "-" + Name + "-" + detectorName[i];
                aux2->WriteObject(chanellHistogram[i][ch], channelName.c_str());
            }
        }

        TDirectory *Canvases = File->mkdir("Canvas");
        for (int i = 0; i < Canvas.size(); i++)
        {
            std::string cvName = "Canvas -" + std::to_string(i);
            Canvases->WriteObject(Canvas[i], cvName.c_str());
        }

        // File->Write();
    }

    void writeTree()
    {
        File->cd();
        coincidences->Write();
    }

    void writeObject(HistogramCoincidences &Histos, Identificator identificator, std::string method)
    {

        std::cout << "IM HERE hahahahah" << std::endl;
        File->cd();

        coincidences->Write();
        singles->Write();
        pulses->Write();
        configurations->Write();

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
        std::cout << "Closing File!" << std::endl;
        File->Close();
    }



};