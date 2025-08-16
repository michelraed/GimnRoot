

void massiveAnalyze(TString DataFolder ="/home/gimn6/Documentos/Michel/DOUTORADO_CARACTERIZACAO/RootOutputs/PreProcessed/Caracterization/OV_QDC_FINAL",
TString csvOut = "processedDatatime.csv"){
   // /// Here you can configure your acquisition folder in order to process the data massively


//    TString DataFolder = {"/home/gimn6/Documentos/Michel/DOUTORADO_CARACTERIZACAO/RootOutputs/PreProcessed/Caracterization/OV_QDC_FINAL"};


   TSystemDirectory dir(DataFolder.Data(), DataFolder.Data());
   TList *filesList = dir.GetListOfFiles();

   TSystemFile *file;
   TString fileName;
   TIter next(filesList);
   DIFERENTIATOR = 1;
   // Do not use numbers near the Maximum number i.e if threads = 32 use 28
   const UInt_t nProcesses = 1; // Number of processes


   std::vector<TString> listOfFiles;

   // iterates throug all files
   while ((file = (TSystemFile *)next()))
   {

       // gets file name
       fileName = file->GetName();
       std::cout<<fileName<<std::endl;
       //checks if the file ends with "_group.root" ,and if it ends with it then ...
       if (!file->IsDirectory() && TString(fileName).EndsWith("_calibrated.root", TString::kIgnoreCase))
       {
           TString fullPath = DataFolder + "/" + fileName;
           // if verbose is activated then it will print the root name and processing.
           if (VERBOSE){
               std::cout << "ROOT FILE FOUND : " << fullPath.Data() << std::endl;
               std::cout << "Processing ... " << std::endl;
           }
           std::cout<<"processing file : "<<fileName<<std::endl;
           // creates the parameter vector input
           //cin.get();
           std::string d =fullPath.Data();
           std::cout<<d<<std::endl;
           analyze(d,csvOut.Data());

           }
   }
   //gROOT->ProcessLine(".q");
}