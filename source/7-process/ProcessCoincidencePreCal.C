

void processCoincidencePreCal(std::string fileName = "/home/gimn6/Documentos/Michel/DOUTORADO_CARACTERIZACAO/Aqd/OV/CoinQDC/time_120_t1_20_t2_20_te_15_ov_4_25_HW_OFF_qdcAquisicao_OV_04_02_2025_coinc.root",
                              std::string outFolder = "/home/gimn6/Documentos/Michel/produtos_moles/gimnpet/GimnROOT")
{

    int ReferenceChannel = 33;

    int groupSize = 1;

    // fileName = "/home/gimn6/Documentos/Michel/PETSYS/GIMNroot/data_test/caracterization/qdc_20_20_15_4_1800s_coinc.root";
    //  PC Michel
    int invalid = 0;
    // cria as variaveis nescessárias à leitura do arquivo root
    UShort_t mh_n1{0}, mh_j1{0}, mh_n2{0}, mh_j2{0};
    UInt_t channelID1{0}, channelID2{0};
    Float_t energy1{0.0}, energy2{0};
    Long64_t time1{0}, time2{0};

    std::vector<Long64_t> timeChanels1(64, 0);
    std::vector<Float_t> energyChannels1(64, 0);
    std::vector<Long64_t> timeChanels2(64, 0);
    std::vector<Float_t> energyChannels2(64, 0);

    // abre o arquivo root
    std::shared_ptr<TFile> file(new TFile(fileName.c_str(), "read"));
    std::shared_ptr<TTree> tree((TTree *)file->Get("data"));

    // set branches
    // Group 1
    tree->SetBranchAddress("mh_n1", &mh_n1);
    tree->SetBranchAddress("mh_j1", &mh_j1);
    tree->SetBranchAddress("channelID1", &channelID1);
    tree->SetBranchAddress("energy1", &energy1);
    tree->SetBranchAddress("time1", &time1);
    // Group 2
    tree->SetBranchAddress("mh_n2", &mh_n2);
    tree->SetBranchAddress("mh_j2", &mh_j2);
    tree->SetBranchAddress("channelID2", &channelID2);
    tree->SetBranchAddress("energy2", &energy2);
    tree->SetBranchAddress("time2", &time2);

    // AQUI TEM PROBLEMA
    Identificator Id;
    Id.LoadIdentificators("/home/gimn6/Documentos/Michel/Doutorado/PETSYS/GIMNroot/petsys_conf/identificators.conf");

    SiPM sipm_tools(64, 8, 8, 3.36, Id);

    int pos1 = 0;
    int pos2 = 0;
    float esum1 = 0;
    float esum2 = 0.0;
    Long64_t counting = 0;
    std::string loading = "[--------------------------------------------------]";
    int load_pos = 1;
    bool first = true;
    const int entries = tree->GetEntries();
    int initial_event{0}, ini_n1{0}, ini_n2{0};

    // Processa o nome da string de entrada
    auto name = splitString(fileName, '/');
    auto name_pos = name.size();
    string outName = name[name_pos - 1];
    name = splitString(outName, '.');
    name_pos = name.size();
    outName = name[0];
    outName = outName + "_CalibratedCoin.root";

    outName = outFolder+"/"+outName;
    // cria a classe responsável pela escrita
    GimnWriteSimple Output(outName.c_str());

    float first_e1 = 0;
    float first_e2 = 0;
    float first_t1 = 0;
    float first_t2 = 0;
    int first_c1 = 0;
    int first_c2 = 0;
    int chan1{0}, chan2{0};

    unsigned int coincidenceID{1};

    for (Int_t event = 0; event <= entries; event++)
    {
        tree->GetEntry(event);
        // Collects the chipID and channel information from global channel
        // for both detectors of a given coincidence
        Int_t chipID1 = (int)channelID1 / 64;
        Int_t channel1 = channelID1 - 64 * chipID1;

        Int_t chipID2 = (int)channelID2 / 64;
        Int_t channel2 = channelID2 - 64 * chipID2;

        // Check if the group size is greater than minimum groupsize;
        if ((mh_n1 >= groupSize) && (mh_n2 >= groupSize))
        {

            // It will check on loaded data from identificators where this chip ID should Fit
            bool isValid = true;
            auto it = std::find(Id.chipID.begin(), Id.chipID.end(), chipID1);
            if (it != Id.chipID.end())
                pos1 = std::distance(Id.chipID.begin(), it);
            else
                isValid = false;

            // gets the position of chipID2 on the identificator data.
            it = std::find(Id.chipID.begin(), Id.chipID.end(), chipID2);
            if (it != Id.chipID.end())
                pos2 = std::distance(Id.chipID.begin(), it);
            else
                isValid = false;

            // Check if both detectors are in an aceptable coincidenece,
            // in case of using several coincidence options
            if (!Id.checkInCoincidence(pos1, chipID2))
            {
                isValid = false;
            }

            if (isValid)
            {

                auto pos_xy1 = sipm_tools.CorrectChannelToPixelPs(channel1);
                auto pos_xy2 = sipm_tools.CorrectChannelToPixelPs(channel2);

                if (mh_j1 == 0 && mh_j2 == 0) // Primeiro Evento
                {
                    // the first event will be the one which have mh_j1 and j2 = 0
                    // in a group this event will be the one with highest energy

                    // Now We take the "energy winner" -> mh_j* = 0 -> highest energy
                    sipm_tools.Histos.channelEnergyHistogram[pos1][channel1]->Fill(energy1);
                    sipm_tools.Histos.channelEnergyHistogram[pos2][channel2]->Fill(energy2);

                    coincidenceID++;

                    initial_event = event;
                    ini_n1 = mh_n1;
                    ini_n2 = mh_n2;

                    first_e1 = energy1;
                    first_e2 = energy2;

                    sipm_tools.Sipm_e[pos1](pos_xy1[1], pos_xy1[0]) = +energy1;
                    sipm_tools.Sipm_e[pos2](pos_xy2[1], pos_xy2[0]) = +energy2;

                    timeChanels1[channel1] = time1;
                    energyChannels1[channel1] = energy1;
                    timeChanels2[channel2] = time2;
                    energyChannels2[channel2] = energy2;

                    Float_t timeInSeconds1 = (float)time1 / 1.0e12;
                    Float_t timeInSeconds2 = (float)time2 / 1.0e12;

                    Long64_t dt = time1-time2;
                    Float_t timeDiference =(Float_t)dt/ 1.0e12;
                    
                    sipm_tools.Sipm_t[pos1](pos_xy1[1], pos_xy1[0]) = +timeInSeconds1;
                    sipm_tools.Sipm_t[pos2](pos_xy2[1], pos_xy2[0]) = +timeInSeconds2;

                    first_t1 = timeInSeconds1;
                    first_t2 = timeInSeconds2;
                    first_c1 = 8 * pos_xy1[0] + pos_xy1[1];
                    first_c2 = 8 * pos_xy2[0] + pos_xy2[1];
                    chan1 = channel1;
                    chan2 = channel2;

                    esum1 += energy1;
                    esum2 += energy2;


                    sipm_tools.Histos.channelTimeSkew[pos2][channel2]->Fill(timeDiference);
                    sipm_tools.Histos.channelTimeWalk[pos1][channel1]->Fill(energy1 * timeDiference, timeDiference);
                    sipm_tools.Histos.channelTimeWalk[pos2][channel2]->Fill(energy2 * timeDiference, timeDiference);
                    sipm_tools.Histos.channelTimeSkew[pos1][channel1]->Fill(timeDiference);

                    // if (channel1 == ReferenceChannel)
                    // {
                    // }
                    // if (channel2 == ReferenceChannel)
                    // {
                    // }
                    // std::cout<<" saving segmented event in a tree"<<std::endl;
                }

                else if (mh_j1 != 0 && mh_j2 == 0) // Intermediary event group1 goes until last event
                {
                    sipm_tools.Sipm_e[pos1](pos_xy1[1], pos_xy1[0]) = +energy1;
                    timeChanels1[channel1] = time1;
                    energyChannels1[channel1] = energy1;
                    Float_t timeInSeconds1 = time1 / 1.0e12;
                    sipm_tools.Sipm_t[pos1](pos_xy1[1], pos_xy1[0]) = +timeInSeconds1;
                    esum1 += energy1;
                }

                else if (mh_j1 == 0 && mh_j2 != 0) // Intermediary event group2 goes until last event
                {
                    sipm_tools.Sipm_e[pos2](pos_xy2[1], pos_xy2[0]) = +energy2;
                    timeChanels1[channel2] = time2;
                    energyChannels1[channel2] = energy2;
                    Float_t timeInSeconds2 = time2 / 1.0e12;
                    sipm_tools.Sipm_t[pos2](pos_xy2[1], pos_xy2[0]) = +timeInSeconds2;
                    esum2 += energy2;
                }

                // last event itself
                if (event == (initial_event + (ini_n1 + ini_n2) - 2))
                {

                    if (first_e1 > 0 && first_e2 > 0)
                    {
                        GimnPulse p1;
                        p1.first_e = first_e1; // Highest energy
                        p1.first_t = first_t1; // time from highest energy event
                        p1.first_c = first_c1; // Position in the matrix of the highest energy
                        p1.coincidenceID = coincidenceID;
                        p1.chan = chan1; // channel of the highest Energy
                        p1.chipID = chipID1;
                        p1.Port = (int)chipID1 / 2;
                        p1.fillEnergyFromTmatrixT(sipm_tools.Sipm_e[pos1]);
                        p1.fillTimeFromTmatrixT(sipm_tools.Sipm_t[pos1]);
                        sipm_tools.Histos.GlobalEnergy[pos1]->Fill(esum1);
                        Output.putData(p1);

                        GimnPulse p2;
                        p2.first_e = first_e2; // Highest energy
                        p2.first_t = first_t2; // time from highest energy event
                        p2.first_c = first_c2; // Position in the matrix of the highest energy
                        p2.coincidenceID = coincidenceID;
                        p2.chan = chan2; // channel of the highest Energy
                        p2.chipID = chipID2;
                        p2.Port = (int)chipID2 / 2;
                        p2.fillEnergyFromTmatrixT(sipm_tools.Sipm_e[pos2]);
                        p2.fillTimeFromTmatrixT(sipm_tools.Sipm_t[pos2]);
                        sipm_tools.Histos.GlobalEnergy[pos2]->Fill(esum2);

                        Output.putData(p2);
                    }

                    sipm_tools.Sipm_e[pos1].Zero();
                    sipm_tools.Sipm_e[pos2].Zero();
                    sipm_tools.Sipm_t[pos1].Zero();
                    sipm_tools.Sipm_t[pos2].Zero();
                }
            }
            else
            {
                //std::cout<<"ChipID1 "<<chipID1<<" ChipID2 "<<chipID2<<std::endl;
                invalid++;
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
    std::cout << "there was " << invalid << "invalid events" << std::endl;

    // event's loop end
}
