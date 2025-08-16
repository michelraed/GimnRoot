
void massivePreProcess(TString FolderTest ="/home/gimn6/Documentos/Michel/DOUTORADO_CARACTERIZACAO/Aqd/OV/QDC",TString outputFolder="/home/gimn6/Documentos/Michel/DOUTORADO_CARACTERIZACAO/RootOutputs/PreProcessed/Caracterization/OV_QDC" ){


    // GEnerates a list to keep the file names as a TList
    TList *fileList = getFolderList(FolderTest.Data());
    TSystemFile *file;
    TString fileName;
    TIter next(fileList);

    const UInt_t nProcesses = 32; // Number of processes in paralells

    // Instantiates our Multi threading class
    MultiThreaderGimn mt (nProcesses);

    // Initiates the lambda that we will use to execute the function
    // groupPreCal in each core 
    auto runGimn= [](std::vector<TString>files){
        groupPreCal(files[0].Data(), files[1].Data());
    };

    // Creates a vector of Strings for storing the name of the files that will be passed as a parameter
    // to our multiproecssing function
    std::vector<TString> listOfFiles;

    //Initiates the iteraction through  the files in the system
    while ((file = (TSystemFile *)next()))
    {
        fileName = file->GetName();
        // check if he file is a directory and if our file is a raw file ending wit _group.root
        if (!file->IsDirectory() && TString(fileName).EndsWith("_group.root", TString::kIgnoreCase))
        {
            // Concatenates the name to get the full path
            TString fullPath = FolderTest + "/" + fileName;
            if (VERBOSE){
            //if verbose is activated will print this
            std::cout << "ROOT FILE FOUND : " << fullPath.Data() << std::endl;
            std::cout << "Processing ... " << std::endl;
            }
            // fills the parameters that will be passed to our thread
            std::vector<TString> files {fullPath,outputFolder};
            // get a rest
            gSystem->Sleep(400);
            
            // initiates a new thread with the function groupPreCal that will process the data
            mt.createThread(runGimn,files);

        }
    }

    mt.waitForCompletion();

    // gROOT->ProcessLine(".q");
}