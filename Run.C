{
    //GLOBAL VARIABLES
    int DIFERENTIATOR = 0;
    bool VERBOSE = false;
    std::mutex mtx; // Mutex for synchronizing access
    
    using ROOT::VecOps::RVec;
    //CONFIGURATION FOLDERS
    static TString outputFolder = {"/home/gimn6/Documentos/Michel/DOUTORADO_CARACTERIZACAO/RootOutputs/PreProcessed/Caracterization/Test"};
    static std::string correctionFactorsFolder = "/home/aquisicao/Documentos/PET_SYS/ROOT/CorFac/";
    static std::string aqdFolder = "/home/aquisicao/Documentos/PET_SYS/Caracterizacao_2024/caracterizacao";
    std::string file_name{"/home/gimn6/Documentos/Michel/DOUTORADO_CARACTERIZACAO/RootOutputs/PreProcessed/Caracterization/Test/time_30_t1_15_t2_20_te_15_ov_4_0_HW_OFF_qdcCharacterization_06122024_group.root"};
    
    
    /**
     * @brief Função utilizada para a leitura dos arquivos peresentes em uma determinada pasta
     *
     */
    auto listarArquivosELoad = [](const std::string &pasta)
    {
        TSystemDirectory dir(pasta.c_str(), pasta.c_str());
        TList *filesList = dir.GetListOfFiles();

        if (filesList)
        {
            TSystemFile *file;
            TString filename;
            TIter next(filesList);

            while ((file = (TSystemFile *)next()))
            {
                filename = file->GetName();
                if (!file->IsDirectory() && TString(filename).EndsWith(".C", TString::kIgnoreCase))
                {
                    std::cout <<"⏳"<< filename.Data() << std::endl;
                    std::string line = ".L " + pasta + filename.Data();
                    std::cout << line << std::endl;
                    gROOT->ProcessLine(line.c_str());
                }
            }
        }
    };

    /**
     * @brief Esta função será responsavel por mostrar todas as pastas presentes dentro de uma pasta
     * 
     */
    auto obterSubpastas = [](const std::string &pastaBase)
    {
        std::vector<std::string> subpastas;

        TSystemDirectory dir(pastaBase.c_str(), pastaBase.c_str());
        TList *filesList = dir.GetListOfFiles();

        if (filesList)
        {
            TSystemFile *file;
            TString filename;
            TIter next(filesList);

            while ((file = (TSystemFile *)next()))
            {
                filename = file->GetName();
                if (file->IsDirectory() && !filename.BeginsWith("."))
                {
                    subpastas.push_back(std::string(filename.Data()));
                }
            }
        }

        // Ordenar as subpastas em ordem alfabética
        std::sort(subpastas.begin(), subpastas.end());

        return subpastas;
    };


    // vasculha por arquivos .C dentro das pastas para carregar na memoria
    std::string projectFolder = gSystem->pwd();
    std::string pastaBase = gSystem->pwd();
    std::vector<std::string> subpastas = obterSubpastas(pastaBase);

    for (const auto &subpasta : subpastas)
    {
        std::string subPasta = projectFolder + "/" + subpasta;
        std::vector<std::string> subsubpastas = obterSubpastas(subPasta);
        std::cout<<"🔍 searching in : "<<std::endl;
        std::cout << subPasta << std::endl;
        for (const auto &subsubpasta : subsubpastas)
        {
            std::string subsubsubPasta = subPasta + "/" + subsubpasta+"/";
            listarArquivosELoad(subsubsubPasta);
            std::cout<<"⏳ Loading in : "<<std::endl;
            std::cout << subsubsubPasta << std::endl;
        }
    }

}
