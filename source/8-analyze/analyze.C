HistosOut analyze(std::string fileName = "/home/gimn6/Documentos/Michel/DOUTORADO_CARACTERIZACAO/RootOutputs/PreProcessed/Caracterization/OV_QDC_FINAL/time_120_t1_20_t2_20_te_15_ov_2_5_HW_OFF_qdcAquisicao_OV_04_02_2025_calibrated.root",
    std::string data_out ="processedDatatime.csv")
{

    auto myData = GetHistosFromCalibrated(fileName);

    std::vector<std::string> corrections = {"Nadig", "Wang", "Poli", "PetSys"};

    std::map<int, std::map<int, std::map<std::string, std::map<std::string, double>>>> processedData;

    std::map<int, std::map<std::string, std::map<std::string, double>>> GlobHist;

    int chanNum = 64;

    for (auto &chipID : myData.chipIDVector)
    {
        for (auto &correction : corrections)
        {
            auto globalHist = myData.GlobalHistos[chipID][correction];
            auto FloodHist = myData.Floods[chipID][correction];

            // This is My Histo;
            GlobHist[chipID][correction]["ParFWHM"] = ajustParable(globalHist);
            GlobHist[chipID][correction]["gaussFWHM"] = Calculate511keVFWHM(globalHist);

            auto output = CalculateFloodQuality(FloodHist);
            GlobHist[chipID][correction]["K"] = output["k"];
            GlobHist[chipID][correction]["K_std"] = output["stdev"];

            for (int ch = 0; ch < chanNum; ch++)
            {

                auto myHist = myData.data[chipID][ch][correction];
                auto fwhm = ajustParable(myHist);
                processedData[chipID][ch][correction]["ParFWHM"] = fwhm;
                auto fwhm_gauss = Calculate511keVFWHM(myHist);
                processedData[chipID][ch][correction]["gaussFWHM"] = fwhm_gauss;
            }
        }
    }

    //    "chipID", "channel", "correction", "parFWHM",

    fstream outfile;

    std::string parameters_out = "/home/gimn6/Documentos/Michel/produtos_moles/gimnpet/CSV_ANALYZED_DATA/OV_METRICS.csv";

    bool existe = arquivoExiste(parameters_out);
    if (existe)
    {
        outfile.open(data_out.c_str(), std::ios::app);
    }
    else
    {
        outfile.open(data_out.c_str(), std::ios::out);
    }

    if (outfile.is_open() && outfile.good())
    {
        if (!existe)
            outfile << "chipID,channel,correction,parFWHM,gaussFWHM,globalPar,globalGauss,k,kErr,file\n";

        for (auto &chipID : myData.chipIDVector)
        {
            for (int ch = 0; ch < chanNum; ch++)
            {
                for (auto &correction : corrections)
                {
                    // std::cout << chipID << "," << ch << "," << correction << "," << processedData[chipID][ch][correction]["ParFWHM"] << "," << processedData[chipID][ch][correction]["gaussFWHM"] << "," << fileName << "\n";
                    outfile << chipID << "," << ch << "," << correction << "," << processedData[chipID][ch][correction]["ParFWHM"] << "," << processedData[chipID][ch][correction]["gaussFWHM"] << "," << GlobHist[chipID][correction]["ParFWHM"] << "," << GlobHist[chipID][correction]["gaussFWHM"] << "," << GlobHist[chipID][correction]["K"] << "," << GlobHist[chipID][correction]["K_std"] << "," << fileName << "\n";
                }
            }
        }
        outfile.close();
    }
    else
    {
        cout << "Erro ao abrir o arquivo" << endl;
    }

    return myData;
}