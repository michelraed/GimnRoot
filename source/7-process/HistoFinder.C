
struct HistosOut
{
    std::map<int, std::map<int, std::map<std::string, std::shared_ptr<TH1F>>>> data;
    std::map<int, std::map<std::string, std::shared_ptr<TH1F>>> GlobalHistos;
    std::map<int, std::map<std::string, std::shared_ptr<TH2F>>> Floods;
    std::map<int, std::map<int, std::shared_ptr<TH1F>>> timeHistos;

    std::vector<int> chipIDVector;
};

HistosOut GetHistosFromCalibrated(std::string fileName = "/home/gimn6/Documentos/Michel/DOUTORADO_CARACTERIZACAO/RootOutputs/PreProcessed/Caracterization/OV_QDC_FINAL/time_120_t1_20_t2_20_te_15_ov_4_25_HW_OFF_qdcAquisicao_OV_04_02_2025_calibrated.root")
{

    bool VERBOSE = false;
    TFile *file = new TFile(fileName.c_str(), "READ");
    if (!file || file->IsZombie())
    {
        cerr << "Erro ao abrir arsquivo: " << fileName << endl;
    }

    std::map<int, std::map<int, std::map<std::string, std::shared_ptr<TH1F>>>> data;
    std::map<int, std::map<int, std::shared_ptr<TH1F>>> timeHistos;

    std::map<int, std::map<std::string, std::shared_ptr<TH1F>>> GlobalHistos;
    std::map<int, std::map<std::string, std::shared_ptr<TH2F>>> Floods;

    std::vector<std::string> corrections = {"Nadig", "Wang", "Raw", "Poli", "PetSys"};
    TList *keysFile = file->GetListOfKeys();

    std::vector<int> chipIDVector;

    for (int i = 0; i < keysFile->GetSize(); ++i)
    {
        /**
         * Here the function will check item by item inside the keys from the main
         * folder searching for the detector's folder
         */

        // Get keys
        TKey *key = (TKey *)keysFile->At(i);

        // stores the name of the key
        std::string name = key->GetName();

        // Print the name of the key with prefix for hierarchy
        if (VERBOSE)
            std::cout << name << std::endl;
        std::cout << " Here  1" << std::endl;
        // Get detector Folder
        if (name.find("Detectors") != -1 && key->IsFolder())
        {

            // if verbose false don't print
            if (VERBOSE)
                std::cout << "detecores encontrado e 'e uma pasta " << std::endl;

            // gets the folder name
            TDirectory *detector = (TDirectory *)file->Get(name.c_str());

            // gets the keys inside the Folder
            auto *keysDetectors = detector->GetListOfKeys();

            std::cout << " Here 2 " << std::endl;

            // Iterates through the files
            for (int j = 0; j < keysDetectors->GetSize(); j++)
            {
                // gets the name of the subfolder

                TKey *key2 = (TKey *)keysDetectors->At(j);
                std::string name2 = key2->GetName();

                std::string chpID = removePart(name2, "ChipID-");
                int chipIDint = stoi(chpID);
                chipIDVector.push_back(chipIDint);

                if (VERBOSE)
                    std::cout << name2 << std::endl;

                // Search for chipID Folders
                if (name2.find("ChipID") != -1 && key->IsFolder())
                {
                    /**
                     *   Checks for the existence of a ChipID Folder
                     *  and if it exists then it will search inside of this folder
                     * in order to obtain the histograms.
                     */

                    // chipIDNames.push_back(name2);
                    if (VERBOSE)
                        std::cout << "chipID encontrado e 'e uma pasta " << std::endl;

                    std::cout << " Here  3" << std::endl;
                    // if it has found a chipID folder, then it will get it and put in a
                    // TDirectoru variable

                    std::cout << name2 << std::endl;
                    TDirectory *chipID = (TDirectory *)detector->Get(name2.c_str());

                    // Then it will take the keys of all the items inside the chipID file
                    auto *keysChipID = chipID->GetListOfKeys();
                    // std::cout<<(int)keysChipID->GetSize()<<" fimim"<<std::endl;

                    // and will create a vector to put the detectors
                    std::vector<TH1F *> detAux; // Hold everyone;

                    for (int k = 0; k < keysChipID->GetSize(); k++)
                    {
                        // Inside this folder there will be the Global Histograms and the Flood Histogram

                        TKey *key3 = (TKey *)keysChipID->At(k);
                        std::string name3 = key3->GetName();

                        if (name3.find("Flood") != -1)
                        {
                            for (auto &correction : corrections)
                            {
                                if (name3.find(correction) != -1)
                                {
                                    std::shared_ptr<TH2F> aux_flood((TH2F *)(chipID->Get(name3.c_str())));
                                    Floods[chipIDint][correction] = aux_flood;
                                }
                            }
                        }

                        if (name3.find("Global") != -1)
                        {
                            for (auto &correction : corrections)
                            {
                                if (name3.find(correction) != -1)
                                {

                                    std::shared_ptr<TH1F> aux_global((TH1F *)(chipID->Get(name3.c_str())));
                                    GlobalHistos[chipIDint][correction] = aux_global;
                                }
                            }
                        }

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

                                for (auto &correction : corrections)
                                {

                                    if (name4.find(correction) != -1)
                                    {
                                        // Remove the name part that have the correction method name and the chipID number and name
                                        std::cout << correction << "Found" << std::endl;
                                        std::string partToBeRemoved = "-" + name2 + "-" + correction;
                                        std::string channelClean = removePart(name4, partToBeRemoved);
                                        channelClean = removePart(channelClean, "channel-");
                                        int channelNumber = stoi(channelClean);
                                        std::cout << "Channel as an int " << channelNumber << " chipID as an int : " << chipIDint << std::endl;
                                        std::shared_ptr<TH1F> aux_chan((TH1F *)(channelEnergy->Get(name4.c_str())));

                                        data[chipIDint][channelNumber][correction] = aux_chan;
                                    }
                                }
                            }
                        }
                        if (name3.find("Channel_timeSKew") != -1 && key3->IsFolder())
                        {
                            if (VERBOSE)
                                std::cout << "Pasta de tempo por canais  encontrada" << std::endl;
                            TDirectory *channelTimeSkew = (TDirectory *)chipID->Get(name3.c_str());
                            auto *keysChannelsTW = channelTimeSkew->GetListOfKeys();

                            for (int m = 0; m < keysChannelsTW->GetSize(); m++)
                            {
                                TKey *key5 = (TKey *)keysChannelsTW->At(m);
                                std::string name5 = key5->GetName();
                                std::string chanNum = splitString(name5, '-')[1];
                                int channelNumber = stoi(chanNum);
                                std::cout << "Channel as an int " << channelNumber << " chipID as an int : " << chipIDint << std::endl;
                                std::shared_ptr<TH1F> aux_chan2((TH1F *)(channelTimeSkew->Get(name5.c_str())));
                                timeHistos[chipIDint][channelNumber] = aux_chan2;
                            }
                        }
                    }
                }
            }
        }
    }


HistosOut output;
output.timeHistos = timeHistos;
output.data = data;
output.chipIDVector = chipIDVector;
output.GlobalHistos = GlobalHistos;
output.Floods = Floods;
return output;
}