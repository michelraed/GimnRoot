
void massiveCalibrationPositioning(TString FolderTest="/home/gimn6/Documentos/Michel/DOUTORADO_CARACTERIZACAO/RootOutputs/PreProcessed/Caracterization/OV_QDC" ,
    TString CalibrationFile ="/home/gimn6/Documentos/Michel/DOUTORADO_CARACTERIZACAO/RootOutputs/PreProcessed/Caracterization/OV_QDC_CAL/time_120_t1_20_t2_20_te_15_ov_4_25_HW_OFF_totAquisicao_OV_04_02_2025_group_pre_CalFactorsChannels.root" ,
    TString OutFolder= "/home/gimn6/Documentos/Michel/DOUTORADO_CARACTERIZACAO/RootOutputs/PreProcessed/Caracterization/OV_QDC_FINAL"
){

// /// Here you can configure your acquisition folder in order to process the data massively

    // TString FolderTest = {"/home/gimn6/Documentos/Michel/DOUTORADO_CARACTERIZACAO/RootOutputs/PreProcessed/Caracterization/OV_QDC"};
    // TString CalibrationFactorsFolder = {"/home/gimn6/Documentos/Michel/DOUTORADO_CARACTERIZACAO/RootOutputs/PreProcessed/Caracterization/OV_QDC_CAL"};
    // TString OutFolder = {"/home/gimn6/Documentos/Michel/DOUTORADO_CARACTERIZACAO/RootOutputs/PreProcessed/Caracterization/OV_QDC_FINAL"};


    TList *fileList = getFolderList(FolderTest.Data());
    TSystemFile *file;
    TString fileName;
    TIter next(fileList);
    DIFERENTIATOR = 1;
    // Do not use numbers near the Maximum number i.e if threads = 32 use 28
    const UInt_t nProcesses = 32; // Number of processes


    // begins multi processor 
    MultiThreaderGimn mt (nProcesses);


    // creates the lambda that will call the funcion inside the thread
    auto runCalibration= [](std::vector<TString>files){
        ProcessPulsesSingle(files[0].Data(), files[1].Data(),files[2].Data());
    };

    //ROOT::TThreadExecutor executor(nProcesses);

    std::vector<TString> listOfFiles;

    // iterates throug all files
    while ((file = (TSystemFile *)next()))
    {

        // gets file name
        fileName = file->GetName();
        // checks if the file ends with "_group.root" ,and if it ends with it then ...
        if (!file->IsDirectory() && TString(fileName).EndsWith("_group_pre.root", TString::kIgnoreCase))
        {
            TString fullPath = FolderTest + "/" + fileName;
            // if verbose is activated then it will print the root name and processing.
            if (VERBOSE){
                std::cout << "ROOT FILE FOUND : " << fullPath.Data() << std::endl;
                std::cout << "Processing ... " << std::endl;
            }
            std::cout<<"processing file : "<<fileName<<std::endl;
            // creates the parameter vector input
            std::vector<TString> files {fullPath,CalibrationFile,OutFolder};

            // processPulses(files[0].Data(), files[1].Data(),files[2].Data(),DIFERENTIATOR);
            // cin.get();
            //calibrate(files[0].Data(), files[1].Data());
            // // sleeps the system for 400 ms in order to not overflow the processes and avoid crashing
            gSystem->Sleep(3000);
            // // creates a new thread that will execute the calibration function declared on the lambda run runCalibration
            mt.createThread(runCalibration,files);
            // cin.get();
            DIFERENTIATOR++;
            }
    }

    mt.waitForCompletion();


    // gROOT->ProcessLine(".q");
}