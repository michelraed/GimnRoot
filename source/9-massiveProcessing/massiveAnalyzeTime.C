
void massiveTimeAnalyze(
    TString DataFolder ="/home/gimn6/Documentos/Michel/DOUTORADO_CARACTERIZACAO/RootOutputs/PreProcessed/Caracterization/OV_QDC_COIN",
    std::string data_out ="processedDatatime.csv"
 )
{
   // /// Here you can configure your acquisition folder in order to process the data massively




//    TSt°ring DataFolder = {"/home/gimn6/Documentos/Michel/DOUTORADO_CARACTERIZACAO/RootOutputs/PreProcessed/Caracterization/OV_QDC_COIN"};

   TSystemDirectory dir(DataFolder.Data(), DataFolder.Data());
   TList *filesList = dir.GetListOfFiles();

   TSystemFile *file;
   TString fileName;
   TIter next(filesList);
   DIFERENTIATOR = 1;


   std::vector<TString> listOfFiles;

   // iterates throug all files
   while ((file = (TSystemFile *)next()))
   {

       // gets file name
       fileName = file->GetName();
       std::cout<<fileName<<std::endl;
       //checks if the file ends with "_group.root" ,and if it ends with it then ...
       if (!file->IsDirectory() && TString(fileName).EndsWith("_CalibratedCoin.root", TString::kIgnoreCase))
       {
           TString fullPath = DataFolder + "/" + fileName;
           // if verbose is activated then it will print the root name and processing.
           if (VERBOSE){
               std::cout << "ROOT FILE FOUND : " << fullPath.Data() << std::endl;
               std::cout << "Processing ... " << std::endl;
           }
           std::cout<<"processing file : "<<fileName<<std::endl;
           // creates the parameter vector input
           
           std::string d =fullPath.Data();
           std::cout<<d<<std::endl;
           analyzeTime(d,data_out);

           }
   }
   //gROOT->ProcessLine(".q");
}