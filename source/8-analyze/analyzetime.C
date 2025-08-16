HistosOut analyzeTime(std::string fileName = "/home/gimn6/Documentos/Michel/DOUTORADO_CARACTERIZACAO/RootOutputs/PreProcessed/Caracterization/OV_QDC_COIN/time_120_t1_20_t2_20_te_15_ov_4_5_HW_OFF_qdcAquisicao_OV_04_02_2025_coinc_CalibratedCoin.root",
std::string data_out ="processedDatatime.csv" )
{

    auto myData = GetHistosFromCalibrated(fileName);
    std::map<int, std::map<int, double>> CTR;
    int chanNum = 64;

    for (auto &chipID : myData.chipIDVector)
    {

            for (int ch = 0; ch < chanNum; ch++)
            {
                auto myHist = myData.timeHistos[chipID][ch];
                CTR[chipID][ch] = CalculateCRT_TSpectrum(myHist);
            }
        
    }

    //    "chipID", "channel", "correction", "parFWHM",

    fstream outfile;

    std::string parameters_out = "/home/gimn6/Documentos/Michel/produtos_moles/gimnpet/GimnROOT/processedDatatime.csv";

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
            outfile << "chipID,channel,CTR,file\n";

        for (auto &chipID : myData.chipIDVector)
        {
            for (int ch = 0; ch < chanNum; ch++)
            {
                    // std::cout << chipID << "," << ch << "," << correction << "," << processedData[chipID][ch][correction]["ParFWHM"] << "," << processedData[chipID][ch][correction]["gaussFWHM"] << "," << fileName << "\n";
                    outfile << chipID << "," << ch << ","<<CTR[chipID][ch] <<"," << fileName << "\n";
         
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