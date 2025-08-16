{

    TString CSV_DATA_OUT ="/home/gimn6/Documentos/Michel/produtos_moles/gimnpet/CSV_ANALYZED_DATA";

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    //                                                                                                  /
    //                                                                                                  /
    //                                      OVER VOLTAGE ANALYSIS                                       /
    //                                                                                                  /
    //                                                                                                  /
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // PROCESSING OVER VOLTAGE

    // This part will process the group in order to obtain the calibrated energy for each channel
    // The first step is process the raw data that comes from PETSYS Daq Board.
    // massive pre processing.
    TString OV_RAW_DATA = "/home/gimn6/Documentos/Michel/produtos_moles/gimnpet/DoutoradoCaracterizacao/RAW/aqd_26042025/OV";
    TString OV_PROCESSED_OUT ="/home/gimn6/Documentos/Michel/produtos_moles/gimnpet/DoutoradoCaracterizacao/RAW/aqd_26042025/Pre_Processed/OV";
    // GET Energy Histograms from each channel to get calibration Factors
    //massivePreProcess(OV_RAW_DATA,OV_PROCESSED_OUT);

    // Massive Pre Calibration
    //TString FolderTest ="/home/gimn6/Documentos/Michel/DOUTORADO_CARACTERIZACAO/RootOutputs/PreProcessed/Caracterization/OV_QDC";
    // Same as OV_PROCESSED_OUT
    //PROCES OV DATA INTO CALIBRATION FACTORS
    TString CALIBRATION_FACTORS = "/home/gimn6/Documentos/Michel/produtos_moles/gimnpet/DoutoradoCaracterizacao/RAW/aqd_26042025/Calibration_Factors/OV";
    //massivePreCalibration(OV_PROCESSED_OUT , CALIBRATION_FACTORS);


    //massive Calibration;
    // USE CALIBRATION FACTORS TO CORRECT DATA
    TString OV_CALIBRATED_DATA = "/home/gimn6/Documentos/Michel/produtos_moles/gimnpet/DoutoradoCaracterizacao/RAW/aqd_26042025/Corrected_data/OV";
    massiveCalibration(OV_PROCESSED_OUT,CALIBRATION_FACTORS,OV_CALIBRATED_DATA);

    //Massive Analyze
    //Analyze Corrected Data To Obtain the metrics;
    TString OV_DATA_OUT = CSV_DATA_OUT+"/OV_METRICS_26042025.csv";
    //massiveAnalyze(OV_CALIBRATED_DATA,OV_DATA_OUT.Data());

    // massive time coincidence
    // TString OV_COINCIDENCES="/home/gimn6/Documentos/Michel/DOUTORADO_CARACTERIZACAO/Aqd/OV2/Coin";
    // TString OV_COINCIDENCES_OUT= "/home/gimn6/Documentos/Michel/DOUTORADO_CARACTERIZACAO/RootOutputs/PreProcessed/Caracterization/OVERVOLTAGE_HIGH_COUNT/PreProcessedCoincidence";
    // massiveTimeCoincidences(OV_COINCIDENCES, CALIBRATION_FACTORS,OV_COINCIDENCES_OUT );
    
    
    // TString OV_DATA_TIME_OUT = CSV_DATA_OUT+"/OV_TIME_METRICS.csv";
    // massiveTimeAnalyze(OV_COINCIDENCES_OUT,OV_DATA_TIME_OUT.Data());


 
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    //                                                                                                  /
    //                                                                                                  /
    //                                  ENERGY THRESHOLD ANALYSIS                                       /
    //                                                                                                  /
    //                                                                                                  /
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    
    // PROCESSING TE 
    // This part will process the group in order to obtain the calibrated energy for each channel
    // The first step is process the raw data that comes from PETSYS Daq Board.
    // massive pre processing.
    
    
    TString TE_RAW_DATA = "/home/gimn6/Documentos/Michel/produtos_moles/gimnpet/DoutoradoCaracterizacao/RAW/aqd_26042025/TE";
    TString TE_PROCESSED_OUT ="/home/gimn6/Documentos/Michel/produtos_moles/gimnpet/DoutoradoCaracterizacao/RAW/aqd_26042025/Pre_Processed/TE";
    // GET Energy Histograms from each channel to get calibration Factors
   //massivePreProcess(TE_RAW_DATA,TE_PROCESSED_OUT);

    // Massive Pre Calibration
    //TString FolderTest ="/home/gimn6/Documentos/Michel/DOUTORADO_CARACTERIZACAO/RootOutputs/PreProcessed/Caracterization/OV_QDC";
    // Same as TR_PROCESSED_OUT
    //PROCES OV DATA INTO CALIBRATION FACTORS
    TString CALIBRATION_FACTORS_TE = "/home/gimn6/Documentos/Michel/produtos_moles/gimnpet/DoutoradoCaracterizacao/RAW/aqd_26042025/Calibration_Factors/TE";
    //massivePreCalibration(TE_PROCESSED_OUT , CALIBRATION_FACTORS_TE);

    //massive Calibration;
    // USE CALIBRATION FACTORS TO CORRECT DATA
    TString CALIBRATED_DATA_TE = "/home/gimn6/Documentos/Michel/produtos_moles/gimnpet/DoutoradoCaracterizacao/RAW/aqd_26042025/Corrected_data/TE";
    massiveCalibration(TE_PROCESSED_OUT,CALIBRATION_FACTORS_TE,CALIBRATED_DATA_TE);

    
    //Massive Analyze
    //Analyze Corrected Data To Obtain the metrics;
    TString TE_DATA_OUT = CSV_DATA_OUT+"/TE_Metrics_26042025.csv";
    //massiveAnalyze(CALIBRATED_DATA_TE,TE_DATA_OUT.Data());

    // massive time coincidence
    //TString OV_COINCIDENCES="/home/gimn6/Documentos/Michel/DOUTORADO_CARACTERIZACAO/Aqd/OV/CoinQDC";
    //TString OV_COINCIDENCES_OUT= "/home/gimn6/Documentos/Michel/DOUTORADO_CARACTERIZACAO/RootOutputs/PreProcessed/Caracterization/OV_QDC_COIN";
    
    //TString OV_DATA_TIME_OUT = CSV_DATA_OUT+"/OV_TIME_METRICS.csv"
    
    //massiveTimeCoincidences(OV_COINCIDENCES_RAW, CALIBRATION_FACTORS,OV_COINCIDENCES_OUT );
    //massiveTimeAnalyze(OV_COINCIDENCES_OUT,OV_DATA_TIME_OUT.Data());


    
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    //                                                                                                  /
    //                                                                                                  /
    //                                  T1 and t2 THRESHOLD ANALYSIS                                    /
    //                                                                                                  /
    //                                                                                                  /
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    
    // PROCESSING OVER VOLTAGE
    // This part will process the group in order to obtain the calibrated energy for each channel
    // The first step is process the raw data that comes from PETSYS Daq Board.
    // massive pre processing.
    
    TString T1T2_RAW_DATA = "/home/gimn6/Documentos/Michel/produtos_moles/gimnpet/DoutoradoCaracterizacao/RAW/aqd_26042025/T1T2";
    TString T1T2_PROCESSED_OUT ="/home/gimn6/Documentos/Michel/produtos_moles/gimnpet/DoutoradoCaracterizacao/RAW/aqd_26042025/Pre_Processed/T1T2";
    // GET Energy Histograms from each channel to get calibration Factors
    //massivePreProcess(T1T2_RAW_DATA,T1T2_PROCESSED_OUT);

    // Massive Pre Calibration
    //TString FolderTest ="/home/gimn6/Documentos/Michel/DOUTORADO_CARACTERIZACAO/RootOutputs/PreProcessed/Caracterization/OV_QDC";
    // Same as OV_PROCESSED_OUT
    //PROCES OV DATA INTO CALIBRATION FACTORS
    TString CALIBRATION_FACTORS_T1T2 = "/home/gimn6/Documentos/Michel/produtos_moles/gimnpet/DoutoradoCaracterizacao/RAW/aqd_26042025/Calibration_Factors/T1T2";
    //massivePreCalibration(T1T2_PROCESSED_OUT , CALIBRATION_FACTORS_T1T2);


    //massive Calibration;
    // USE CALIBRATION FACTORS TO CORRECT DATA
    TString CALIBRATED_DATA_T1T2 = "/home/gimn6/Documentos/Michel/produtos_moles/gimnpet/DoutoradoCaracterizacao/RAW/aqd_26042025/Corrected_data/T1T2";
    massiveCalibration(T1T2_PROCESSED_OUT,CALIBRATION_FACTORS_T1T2,CALIBRATED_DATA_T1T2);

    //Massive Analyze
    //Analyze Corrected Data To Obtain the metrics;
    TString T1T2_DATA_OUT = CSV_DATA_OUT+"/T1T2_METRICS_26042025.csv";
    //massiveAnalyze(CALIBRATED_DATA_T1T2,T1T2_DATA_OUT.Data());

    // massive time coincidence
    //TString OV_COINCIDENCES="/home/gimn6/Documentos/Michel/DOUTORADO_CARACTERIZACAO/Aqd/OV/CoinQDC";
    //TString OV_COINCIDENCES_OUT= "/home/gimn6/Documentos/Michel/DOUTORADO_CARACTERIZACAO/RootOutputs/PreProcessed/Caracterization/OV_QDC_COIN";
    
    //TString OV_DATA_TIME_OUT = CSV_DATA_OUT+"/OV_TIME_METRICS.csv"
    
    //massiveTimeCoincidences(OV_COINCIDENCES_RAW, CALIBRATION_FACTORS,OV_COINCIDENCES_OUT );
    //massiveTimeAnalyze(OV_COINCIDENCES_OUT,OV_DATA_TIME_OUT.Data());

}