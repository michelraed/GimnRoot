void Utils() {}

/**
 * @brief Esta função irá dividir todos os elementos de uma matriz por um determinado valor
 *
 * @param mat matriz a ser dividida
 * @param divisor valor pelo qual se deseja dividir os elementos da matriz
 */
void divideMatrix(TMatrixT<double> &mat, double divisor)
{

    for (int i = 0; i < mat.GetNrows(); i++)
    {
        for (int j = 0; j < mat.GetNcols(); j++)
        {
            mat[i][j] = mat[i][j] / divisor;
        }
    }
}

/**
 * @brief Esta função é utilizada quando se deseja dividir duas matrizes de tamanhos iguals os elementos correspondentes de uma matriz
 * pelos elementos correspondentes de outra desta forma será feito mat1[i][j]/mat2[i][j]
 *
 * @param mat Matriz do numerador
 * @param mat2 Matriz denominador
 */
void divideMatrix(TMatrixT<double> &mat, TMatrixT<double> &mat2)
{
    for (int i = 0; i < mat.GetNrows(); i++)
    {
        for (int j = 0; j < mat.GetNcols(); j++)
        {
            auto answer = mat[i][j] / mat2[i][j];
            if (!std::isnan(answer))
                mat[i][j] = answer;
            else
                mat[i][j] = 0;
        }
    }
}

/**
 * @brief Esta função irá juntar um vetor de strings em uma unica string, usando um separador que é um caractere fornecido pelo usuário
 *
 * @param toJoin vetor de strings contendo os dados que se desejam juntar
 * @param separator caractere que será utilizado para juntar os elementos do vetor em uma unica string
 * @return std::string
 */
std::string joinString(std::vector<std::string> toJoin, char separator)
{
    std::string out;
    for (auto item : toJoin)
    {
        out += item + separator;
    }
    return out;
}

/**
 * @brief splitString é uma função que irá separar em um vetor de strings uma determinada string a partir de um separador dado.
 * exemplo :  a menina comeu banana. Se for utilizado como separador ' ' isso resultará na seguinte saida : ['a','menina','comeu','banana']
 * Esta função pode ser muito util para processar caminhos de arquivos;
 *
 * @param string
 * @param separator
 * @return std::vector<std::string>
 */
std::vector<std::string> splitString(std::string string, char separator)
{
    int aux{0};
    int beg{0};
    int lastSize{0};
    // std::cout << string << std::endl;
    std::vector<std::string> out;
    for (int position = 0; position < string.size(); position++)
    {
        if (string[position] == separator)
        {
            int newSize = position - beg;
            std::string splited;
            aux = beg;
            beg = position + 1;
            for (int internalPos = 0; internalPos < newSize; internalPos++)
            {
                splited += string[aux + internalPos];
            }
            lastSize = newSize;
            out.push_back(splited);
        }
    }
    int newSize = string.size() - beg;
    // std::cout << newSize << std::endl;
    std::string splited;
    for (int internalPos = 0; internalPos < newSize; internalPos++)
    {
        splited += string[aux + lastSize + 1 + internalPos];
    }
    out.push_back(splited);
    return out;
}

/**
 * @brief CorrectChannelToPixelPS irá converter um determinado valor de canal das sipms ketek 3325-WB0808 em sua posição em (x,y)
 *
 * @param channel numero do canal
 * @return std::vector<Int_t>  retorna como saida um vetor contendo na posição 0 o valor da posição de x e na posição 1 o valor referente à posição em y
 */
std::vector<Int_t> CorrectChannelToPixelPs(Int_t channel)
{

    int CORRECTION_MAP[64][3]{
        // {Channel,X,Y}
        {0, 1, 7},
        {1, 1, 6},
        {2, 2, 4},
        {3, 0, 6},
        {4, 0, 5},
        {5, 3, 7},
        {6, 0, 4},
        {7, 2, 6},
        {8, 1, 5},
        {9, 3, 3},
        {10, 0, 7},
        {11, 3, 4},
        {12, 3, 6},
        {13, 2, 5},
        {14, 2, 7},
        {15, 1, 4},
        {16, 6, 6},
        {17, 7, 7},
        {18, 6, 7},
        {19, 7, 6},
        {20, 4, 7},
        {21, 4, 6},
        {22, 5, 6},
        {23, 5, 7},
        {24, 7, 4},
        {25, 7, 5},
        {26, 6, 4},
        {27, 3, 5},
        {28, 6, 5},
        {29, 5, 5},
        {30, 5, 4},
        {31, 4, 4},
        {32, 4, 5},
        {33, 4, 3},
        {34, 4, 2},
        {35, 5, 2},
        {36, 5, 3},
        {37, 6, 2},
        {38, 3, 2},
        {39, 6, 3},
        {40, 7, 2},
        {41, 7, 3},
        {42, 4, 0},
        {43, 4, 1},
        {44, 5, 0},
        {45, 6, 1},
        {46, 5, 1},
        {47, 6, 0},
        {48, 7, 0},
        {49, 7, 1},
        {50, 0, 3},
        {51, 3, 0},
        {52, 3, 1},
        {53, 2, 1},
        {54, 2, 0},
        {55, 1, 0},
        {56, 1, 1},
        {57, 0, 2},
        {58, 2, 2},
        {59, 1, 3},
        {60, 0, 0},
        {61, 1, 2},
        {62, 2, 3},
        {63, 0, 1},
    };

    // RETORNA X,Y
    // Retorna Linha , Coluna
    std::vector<Int_t> aux{CORRECTION_MAP[channel][1], CORRECTION_MAP[channel][2]};
    return aux;
}

/**
 * @brief Irá gerar a soma em linhas e colunas de uma dada matriz fornecida, este irá passar por referência os valores da soma das linhas e das colunas
 * para os vetores rowSum e colSum
 *
 * @param matrix Matriz de interesse
 * @param rowSums vetor vazio onde será colocada a soma das linhas
 * @param colSums vetor vazio onde serão colocadas as somas das colunas
 */
void getRowAndColumnSums(const TMatrixT<double> &matrix, std::vector<double> &rowSums, std::vector<double> &colSums)
{
    int numRows = matrix.GetNrows();
    int numCols = matrix.GetNcols();

    for (int i = 0; i < numRows; i++)
    {
        double rowSum = 0;
        for (int j = 0; j < numCols; j++)
        {
            rowSum += matrix(i, j);
        }
        rowSums[i] = rowSum;
    }

    for (int j = 0; j < numCols; j++)
    {
        double colSum = 0;
        for (int i = 0; i < numRows; i++)
        {
            colSum += matrix(i, j);
        }
        colSums[j] = colSum;
    }
}

/**
 * @brief Irá gerar a soma em linhas e colunas de uma dada matriz fornecida, este irá passar por referência os valores da soma das linhas e das colunas
 * para os vetores rowSum e colSum
 *
 * @param matrix Matriz de interesse
 * @param rowSums vetor vazio onde será colocada a soma das linhas
 * @param colSums vetor vazio onde serão colocadas as somas das colunas
 */
void getRowAndColumnSums(const TMatrixT<Float_t> &matrix, std::vector<Float_t> &rowSums, std::vector<Float_t> &colSums)
{
    int numRows = matrix.GetNrows();
    int numCols = matrix.GetNcols();

    for (int i = 0; i < numRows; i++)
    {
        Float_t rowSum = 0;
        for (int j = 0; j < numCols; j++)
        {
            rowSum += matrix(i, j);
        }
        rowSums[i] = rowSum;
    }

    for (int j = 0; j < numCols; j++)
    {
        Float_t colSum = 0;
        for (int i = 0; i < numRows; i++)
        {
            colSum += matrix(i, j);
        }
        colSums[j] = colSum;
    }
}
/**
 * @brief showFiles irá mostrar todos os arquivos presentes em uma determinada folder
 *
 * @param path Caminho para a folder desejada
 */
void showFiles(std::string path)
{
    TSystemDirectory dir(path.c_str(), path.c_str());
    TList *files = dir.GetListOfFiles();
    if (files)
    {
        TSystemFile *file;
        TString fname;
        int i = 0;
        TIter next(files);
        while ((file = (TSystemFile *)next()))
        {
            fname = file->GetName();
            std::cout << i << ": " << fname.Data() << std::endl;
            i++;
        }
    }
}

/**
 * @brief Obtem um vetor contendo a lista de arquivos presentes em uma determinada folder e devolve o arquivo de indice "index"
 *
 * @param path caminho desejado
 * @param index indice, de posição do arquivo desejado na lista gerada
 * @return std::string nome do arquivo é devolvido como uma string
 */
std::string getFiles(std::string path, int index)
{
    TSystemDirectory dir(path.c_str(), path.c_str());
    TList *files = dir.GetListOfFiles();
    if (files)
    {
        TSystemFile *file;
        TString fname;
        int i = 0;
        TIter next(files);
        while ((file = (TSystemFile *)next()))
        {
            fname = file->GetName();
            if (i == index)
            {
                return fname.Data();
            }
            i++;
        }
    }
    return "";
}

TList* getFolderList(const std::string &folder)
{

    TSystemDirectory dir(folder.c_str(), folder.c_str());
    TList *filesList = dir.GetListOfFiles();

    return filesList;
}

void LoadLib(TString param , TString fileName)
{

    /// Must end with .C because its a root file;
    if (TString(fileName).EndsWith(".C", TString::kIgnoreCase))
    {
        std::cout << fileName.Data() << std::endl;
        TString line = param + " " + fileName;
        std::cout << line.Data() << std::endl;
        gROOT->ProcessLine(line.Data());
    }
}

void CloseAllCanvases() {
    // Get the list of all canvases as TSeqCollection
    TSeqCollection* canvases = gROOT->GetListOfCanvases();
    
    // Iterate over the collection and close each canvas
    TIter next(canvases);
    TCanvas* canvas;
    while ((canvas = static_cast<TCanvas*>(next()))) {
        canvas->Close();  // Close the canvas
    }
}

int extractNumber(const std::string& str) {
    std::string numberStr; // To hold the extracted number as a string

    // Iterate through each character in the string
    for (char ch : str) {
        // Check if the character is a digit
        if (isdigit(ch)) {
            numberStr += ch; // Append the digit to the number string
        }
    }

    // Convert the extracted string to an integer
    int number = 0;
    if (!numberStr.empty()) {
        number = std::stoi(numberStr); // Convert to integer
    }

    return number; // Return the extracted number
}

std::string removePart(const std::string &str, const std::string &part)
{
    size_t pos = str.find(part);
    if (pos != std::string::npos)
    {
        return str.substr(0, pos) + str.substr(pos + part.size());
    }
    else
    {
        return str; // part not found, return original string
    }
}

bool arquivoExiste(const std::string& nomeArquivo) {
    std::ifstream arquivo(nomeArquivo.c_str());
    return arquivo.good();
}


void MergeTrees(const char* inputDir, const char* outputFile) {
    // Create TChain for target tree
    TChain chain("tomographicCoincidences");
    
    // Get list of files in directory
    void* dirPtr = gSystem->OpenDirectory(inputDir);
    if (!dirPtr) {
        std::cerr << "Error: Failed to open directory '" << inputDir << "'" << std::endl;
        return;
    }

    const char* entry;
    int fileCount = 0;
    TSystemDirectory dir(inputDir, inputDir);
    TList* files = dir.GetListOfFiles();
    
    if (files) {
        TSystemFile* file;
        TIter next(files);
        while ((file = (TSystemFile*)next())) {
            TString fileName = file->GetName();
            if (file->IsDirectory() || !fileName.EndsWith(".root")) continue;
            
            TString fullPath = TString::Format("%s/%s", inputDir, fileName.Data());
            if (chain.Add(fullPath)) {
                std::cout << "Added: " << fullPath << std::endl;
                fileCount++;
            }
        }
        delete files;
    }

    // Check if files were found
    if (fileCount == 0) {
        std::cerr << "Error: No valid ROOT files found in '" << inputDir << "'" << std::endl;
        return;
    }

    // Create output file and clone tree
    TFile outFile(outputFile, "RECREATE");
    if (!outFile.IsOpen()) {
        std::cerr << "Error: Failed to create output file '" << outputFile << "'" << std::endl;
        return;
    }

    // Clone tree using fast method
    TTree* mergedTree = chain.CloneTree(-1, "fast");
    if (!mergedTree) {
        std::cerr << "Error: Tree cloning failed!" << std::endl;
        outFile.Close();
        return;
    }

    mergedTree->Write();
    outFile.Close();
}



/**
 * Retorna todas as subpastas diretas dentro de uma pasta fornecida.
 *
 * @param targetFolder Caminho da pasta onde procurar subdiretórios.
 * @return std::vector<TString> contendo o caminho completo de cada subpasta encontrada.
 */
std::vector<TString> ListSubdirectories(const TString& targetFolder) {
    std::vector<TString> subfolders;

    // Limpa a barra final, se houver
    TString cleanFolder = targetFolder.Strip(TString::kTrailing, '/');

    // Abre a pasta
    void* dir = gSystem->OpenDirectory(cleanFolder);
    if (!dir) {
        std::cerr << "[❌] Não foi possível abrir a pasta: " << cleanFolder << std::endl;
        return subfolders;
    }

    const char* entry;
    while ((entry = gSystem->GetDirEntry(dir))) {
        TString entryName(entry);

        // Ignora "." e ".."
        if (entryName == "." || entryName == "..") continue;

        // Caminho completo da entrada
        TString fullPath = cleanFolder + "/" + entryName;

        // Verifica se é diretório
        FileStat_t statbuf;
        if (gSystem->GetPathInfo(fullPath, statbuf) == 0 && R_ISDIR(statbuf.fMode)) {
            subfolders.push_back(fullPath);
        }
    }

    gSystem->FreeDirectory(dir);
    return subfolders;
}





/**
 * Realiza o merge dos arquivos ROOT de uma pasta de entrada, salvando o resultado
 * na pasta de saída com o nome da pasta de entrada. Se o arquivo já existir, ele é deletado.
 *
 * @param inputDirStr  Caminho da pasta contendo os arquivos a serem mesclados
 * @param outputDirStr Caminho da pasta onde o arquivo mesclado será salvo
 */
void MergeAndReplaceOutput(const std::string& inputDirStr, const std::string& outputDirStr) {
    TString inputDir = TString(inputDirStr).Strip(TString::kTrailing, '/');
    TString outputDir = TString(outputDirStr).Strip(TString::kTrailing, '/');

    // Verifica se pasta de entrada existe
    if (gSystem->AccessPathName(inputDir, kFileExists)) {
        std::cerr << "[❌] Pasta de entrada não encontrada: " << inputDir << std::endl;
        return;
    }

    // Extrai o nome da pasta de entrada (para nome do arquivo de saída)
    TString inputFolderName = gSystem->BaseName(inputDir);

    // Monta o caminho completo do arquivo de saída
    TString outputFilePath = outputDir + "/" + inputFolderName + ".root";

    // Deleta o arquivo existente, se houver
    if (!gSystem->AccessPathName(outputFilePath, kFileExists)) {
        std::cout << "[Info] Arquivo de saída já existe: " << outputFilePath << std::endl;
        std::cout << "[Info] Deletando arquivo antigo..." << std::endl;

        int delStatus = gSystem->Unlink(outputFilePath);
        if (delStatus != 0) {
            std::cerr << "[❌] Falha ao deletar o arquivo existente: " << outputFilePath << std::endl;
            return;
        } else {
            std::cout << "[✅] Arquivo antigo deletado com sucesso." << std::endl;
        }
    }

    // Cria a TChain com o nome da árvore a ser mesclada
    TChain chain("tomographicCoincidences");

    // Lista arquivos da pasta de entrada
    TSystemDirectory dir(inputDir, inputDir);
    TList* files = dir.GetListOfFiles();
    if (!files) {
        std::cerr << "[Erro] Falha ao acessar os arquivos da pasta: " << inputDir << std::endl;
        return;
    }

    // Adiciona os arquivos à chain
    int fileCount = 0;
    TIter next(files);
    TSystemFile* file;
    while ((file = (TSystemFile*)next())) {
        TString name = file->GetName();
        if (file->IsDirectory() || !name.EndsWith(".root")) continue;

        TString fullPath = inputDir + "/" + name;
        if (chain.Add(fullPath)) {
            std::cout << "➕ Adicionado: " << fullPath << std::endl;
            fileCount++;
        }
    }
    delete files;

    if (fileCount == 0) {
        std::cerr << "[❌] Nenhum arquivo ROOT válido encontrado em: " << inputDir << std::endl;
        return;
    }

    // Cria arquivo de saída e escreve a árvore mesclada
    TFile outFile(outputFilePath, "RECREATE");
    if (!outFile.IsOpen()) {
        std::cerr << "[❌] Não foi possível criar o arquivo de saída: " << outputFilePath << std::endl;
        return;
    }

    TTree* mergedTree = chain.CloneTree(-1, "fast");
    if (!mergedTree) {
        std::cerr << "[❌] Falha ao clonar a árvore 'tomographicCoincidences'" << std::endl;
        outFile.Close();
        return;
    }

    mergedTree->Write();
    outFile.Close();

    std::cout << "[✅ Finalizado] Arquivo de merge salvo em: " << outputFilePath << std::endl;
}













/**
 * Gera cópias de arquivos ROOT de background para diferentes ângulos.
 * 
 * A função busca arquivos com "_background_" no nome (um de coincidência e um de grupo),
 * e os replica para diferentes ângulos, salvando os novos arquivos em uma pasta "Background"
 * dentro da pasta de saída fornecida.
 *
 * @param inputFolderStr  Caminho da pasta onde os arquivos *_background_*.root estão localizados
 * @param outputFolderStr Caminho da pasta onde os arquivos serão copiados
 * @param angleStep       Passo angular (em graus) entre as cópias geradas [default: 15°]
 */
void GenerateBackgroundCopiesFromTemplate(const TString& inputFolderStr,
                                          const TString& outputFolderStr,
                                          int angleStep = 15) {
    // Convertendo para TString e limpando barras finais
    TString inputFolder = TString(inputFolderStr).Strip(TString::kTrailing, '/');
    TString outputFolder = TString(outputFolderStr).Strip(TString::kTrailing, '/');

    // Abrindo a pasta de entrada
    void* dir = gSystem->OpenDirectory(inputFolder);
    if (!dir) {
        std::cerr << "[❌] Pasta de entrada não encontrada: " << inputFolder << std::endl;
        return;
    }

    const char* filename;
    TString backgroundCoincFile, backgroundGroupFile;

    // Busca por arquivos contendo "_background_" e terminando com ".root"
    while ((filename = gSystem->GetDirEntry(dir))) {
        TString name = filename;

        // Ignora "." e ".."
        if (name == "." || name == "..") continue;

        if (name.Contains("_background_") && name.EndsWith(".root")) {
            if (name.Contains("coinc"))
                backgroundCoincFile = name;
            else if (name.Contains("group"))
                backgroundGroupFile = name;
        }
    }
    gSystem->FreeDirectory(dir);

    // Verifica se encontrou os arquivos de background necessários
    if (backgroundCoincFile.IsNull() && backgroundGroupFile.IsNull()) {
        std::cerr << "[Erro] Nenhum arquivo background encontrado (nem de coincidência nem de grupo)." << std::endl;
        return;
    }

    if (!backgroundCoincFile.IsNull())
        std::cout << "[Info] Arquivo de coincidência: " << backgroundCoincFile << std::endl;
    if (!backgroundGroupFile.IsNull())
        std::cout << "[Info] Arquivo de grupo:        " << backgroundGroupFile << std::endl;

    // Garante que a subpasta Background exista na pasta de saída
    TString backgroundOutputFolder = outputFolder + "/Background";
    if (gSystem->AccessPathName(backgroundOutputFolder, kFileExists)) {
        std::cout << "[Info] Pasta 'Background' não existe em " << outputFolder << ". Criando..." << std::endl;
        if (gSystem->mkdir(backgroundOutputFolder, kTRUE) != 0) {
            std::cerr << "[❌] Falha ao criar a pasta 'Background'." << std::endl;
            return;
        }
    }

    // Loop de replicação para os ângulos desejados
    for (int angle = 0; angle < 180; angle += angleStep) {
        TString srcCoincPath, dstCoincPath;
        TString srcGroupPath, dstGroupPath;

        // Cria cópia do arquivo de coincidência
        if (!backgroundCoincFile.IsNull()) {
            TString suffixCoinc = backgroundCoincFile;
            suffixCoinc.ReplaceAll("_background_", "_");
            TString newNameCoinc = Form("Angle_%d_0%s", angle, suffixCoinc.Data());
            srcCoincPath = inputFolder + "/" + backgroundCoincFile;
            dstCoincPath = backgroundOutputFolder + "/" + newNameCoinc;

            int res = gSystem->CopyFile(srcCoincPath, dstCoincPath, kTRUE);
            if (res == 0)
                std::cout << "[✅] Cópia coincidência: ângulo " << angle << "° → " << newNameCoinc << std::endl;
            else
                std::cerr << "[❌] Cópia coincidência: ângulo " << angle << std::endl;
        }

        // Cria cópia do arquivo de grupo
        if (!backgroundGroupFile.IsNull()) {
            TString suffixGroup = backgroundGroupFile;
            suffixGroup.ReplaceAll("_background_", "_");
            TString newNameGroup = Form("Angle_%d_0%s", angle, suffixGroup.Data());
            srcGroupPath = inputFolder + "/" + backgroundGroupFile;
            dstGroupPath = backgroundOutputFolder + "/" + newNameGroup;

            int res = gSystem->CopyFile(srcGroupPath, dstGroupPath, kTRUE);
            if (res == 0)
                std::cout << "[✅] Cópia grupo: ângulo " << angle << "° → " << newNameGroup << std::endl;
            else
                std::cerr << "[❌] Cópia grupo: ângulo " << angle << std::endl;
        }
    }

    std::cout << "[Finalizado] Geração de cópias de background concluída." << std::endl;
}


/**
 * Verifica se uma pasta existe, e caso não exista, cria a pasta (incluindo diretórios intermediários).
 *
 * @param folderPath Caminho da pasta a ser verificada/criada.
 */
void ensureFolderExists(const TString& folderPath) {
    TString cleanPath = folderPath.Strip(TString::kTrailing, '/');

    // Verifica se a pasta já existe
    if (!gSystem->AccessPathName(cleanPath, kFileExists)) {
        std::cout << "📁 Pasta já existe: " << cleanPath << std::endl;
        return;
    }

    // Tenta criar a pasta (e intermediárias, se necessário)
    int status = gSystem->mkdir(cleanPath, kTRUE);
    if (status == 0) {
        std::cout << "✅ Pasta criada com sucesso: " << cleanPath << std::endl;
    } else {
        std::cerr << "❌ Erro ao criar a pasta: " << cleanPath << std::endl;
    }
}

/**
 * Recursivamente percorre a árvore de diretórios e escreve apenas o resumo de pastas.
 */
void WriteFolderTreeSummary(std::ofstream& out, const TString& path, int level = 0) {
    void* dir = gSystem->OpenDirectory(path);
    if (!dir) return;

    const char* entry;
    std::vector<TString> subdirs;

    while ((entry = gSystem->GetDirEntry(dir))) {
        TString name(entry);
        if (name == "." || name == "..") continue;

        TString fullPath = path + "/" + name;

        FileStat_t statbuf;
        if (gSystem->GetPathInfo(fullPath, statbuf) == 0 && R_ISDIR(statbuf.fMode)) {
            subdirs.push_back(name);
        }
    }

    gSystem->FreeDirectory(dir);

    for (const auto& d : subdirs) {
        out << std::string(level * 2, ' ') << "- 📁 " << d << "\n";
        WriteFolderTreeSummary(out, path + "/" + d, level + 1);
    }
}

/**
 * Recursivamente escreve os arquivos dentro das pastas (estrutura detalhada).
 */
void WriteFolderTreeRecursive(std::ofstream& out, const TString& path, int level = 0) {
    void* dir = gSystem->OpenDirectory(path);
    if (!dir) return;

    const char* entry;
    std::vector<TString> subdirs;
    std::vector<TString> files;

    while ((entry = gSystem->GetDirEntry(dir))) {
        TString name(entry);
        if (name == "." || name == "..") continue;

        TString fullPath = path + "/" + name;

        FileStat_t statbuf;
        if (gSystem->GetPathInfo(fullPath, statbuf) == 0) {
            if (R_ISDIR(statbuf.fMode))
                subdirs.push_back(name);
            else
                files.push_back(name);
        }
    }

    gSystem->FreeDirectory(dir);

    // Escreve arquivos
    for (const auto& f : files) {
        out << std::string(level * 2, ' ') << "- 📄 " << f << "\n";
    }

    // Recorre para as subpastas
    for (const auto& d : subdirs) {
        out << std::string(level * 2, ' ') << "- 📁 " << d << "\n";
        WriteFolderTreeRecursive(out, path + "/" + d, level + 1);
    }
}

/**
 * Gera um arquivo Markdown (.md) com:
 * 1. Resumo hierárquico de pastas
 * 2. Estrutura detalhada com arquivos
 */
void GenerateFolderStructureMarkdown(const TString& baseFolder, const TString& outputMarkdownFile = "folder_structure.md") {
    TString cleanBase = baseFolder.Strip(TString::kTrailing, '/');

    std::ofstream out(outputMarkdownFile.Data());
    if (!out.is_open()) {
        std::cerr << "[Erro] Não foi possível criar arquivo Markdown: " << outputMarkdownFile << std::endl;
        return;
    }

    TString rootName = gSystem->BaseName(cleanBase);

    // Título principal
    out << "# 📂 Estrutura de diretórios: `" << cleanBase << "`\n\n";

    // Sessão de Resumo
    out << "## 🧭 Resumo de Pastas\n\n";
    out << "- 📁 " << rootName << "\n";
    WriteFolderTreeSummary(out, cleanBase, 1);

    // Sessão detalhada
    out << "\n---\n\n";
    out << "## 🗂️ Estrutura Completa com Arquivos\n\n";
    out << "- 📁 " << rootName << "\n";
    WriteFolderTreeRecursive(out, cleanBase, 1);

    out.close();
    std::cout << "[✅] Estrutura de diretórios salva em: " << outputMarkdownFile << std::endl;
}


/**
 * @brief Lista os arquivos de uma pasta e permite ao usuário selecionar um pelo índice.
 *
 * @param folderPath Caminho da pasta a ser explorada (TString).
 * @return TString Caminho completo do arquivo selecionado.
 */
TString SelectFileFromFolder(const TString& folderPath)
{
    // Remove barra final, se houver
    TString cleanPath = folderPath;
    cleanPath = cleanPath.Strip(TString::kTrailing, '/');

    TSystemDirectory dir("target", cleanPath);
    TList* files = dir.GetListOfFiles();
    if (!files) {
        std::cout << "❌ Erro ao abrir a pasta: " << cleanPath << std::endl;
        return "";
    }

    std::vector<TString> fileNames;
    std::cout << "📁 Conteúdo da pasta: " << cleanPath << "\n\n";

    int index = 0;
    TIter next(files);
    while (TObject* obj = next()) {
        TSystemFile* file = dynamic_cast<TSystemFile*>(obj);
        if (!file) continue;

        TString fname = file->GetName();

        if (fname == "." || fname == "..") continue;

        TString fullPath = cleanPath + "/" + fname;

        if (file->IsDirectory()) {
            std::cout << "[" << index << "] 📁 " << fname << " (pasta)\n";
        } else {
            std::cout << "[" << index << "] 📄 " << fname << "\n";
        }

        fileNames.push_back(fullPath);
        index++;
    }

    if (fileNames.empty()) {
        std::cout << "📭 Nenhum arquivo encontrado.\n";
        return "";
    }

    std::cout << "\n🔢 Digite o índice do arquivo desejado: ";
    int userChoice = -1;
    std::cin >> userChoice;

    if (userChoice < 0 || userChoice >= (int)fileNames.size()) {
        std::cout << "❌ Índice inválido. Abortando.\n";
        return "";
    }
    
    if (userChoice==-1){
        return ".q";
    }

    TString selected = fileNames[userChoice];
    std::cout << "✅ Arquivo selecionado: " << selected << "\n";
    return selected;
}


/**
 * @brief Lista as subpastas de uma pasta e permite ao usuário selecionar uma delas.
 *
 * @param basePath Caminho da pasta a ser explorada (TString).
 * @return TString Caminho completo da subpasta selecionada.
 */
TString SelectFolderFromDirectory(const TString& basePath)
{
    // Remove barra final, se houver
    TString cleanPath = basePath;
    cleanPath = cleanPath.Strip(TString::kTrailing, '/');

    TSystemDirectory dir("target", cleanPath);
    TList* entries = dir.GetListOfFiles();
    if (!entries) {
        std::cout << "❌ Erro ao abrir a pasta: " << cleanPath << std::endl;
        return "";
    }

    std::vector<TString> folderPaths;
    std::cout << "📁 Subpastas em: " << cleanPath << "\n\n";

    int index = 0;
    TIter next(entries);
    while (TObject* obj = next()) {
        TSystemFile* entry = dynamic_cast<TSystemFile*>(obj);
        if (!entry) continue;

        TString name = entry->GetName();

        // Ignora "." e ".."
        if (name == "." || name == "..") continue;

        // Verifica se é diretório
        if (entry->IsDirectory()) {
            TString fullPath = cleanPath + "/" + name;
            std::cout << "[" << index << "] 📂 " << name << std::endl;
            folderPaths.push_back(fullPath);
            index++;
        }
    }

    if (folderPaths.empty()) {
        std::cout << "📭 Nenhuma subpasta encontrada.\n";
        return "";
    }

    std::cout << "\n🔢 Digite o índice da pasta desejada: ";
    int userChoice = -1;
    std::cin >> userChoice;

    if (userChoice < 0 || userChoice >= (int)folderPaths.size()) {
        std::cout << "❌ Índice inválido. Abortando.\n";
        return "";
    }

    TString selected = folderPaths[userChoice];
    std::cout << "✅ Pasta selecionada: " << selected << "\n";
    return selected;
}


/**
 * Pergunta ao usuário se deseja continuar e interpreta a resposta.
 *
 * @return true se a resposta for afirmativa (sim, yes, s, y), false se negativa (nao, no, n)
 */
bool askUserToContinue()
{
    std::string resposta;

    while (true)
    {
        std::cout << "Deseja continuar? (s/n): ";
        std::getline(std::cin, resposta);

        // Converter para minúsculas
        std::transform(resposta.begin(), resposta.end(), resposta.begin(), ::tolower);

        // Remover espaços
        resposta.erase(remove_if(resposta.begin(), resposta.end(), ::isspace), resposta.end());

        // Verificações
        if (resposta == "sim" || resposta == "yes" || resposta == "s" || resposta == "y")
        {
            return true;
        }
        else if (resposta == "nao" || resposta == "não" || resposta == "no" || resposta == "n")
        {
            return false;
        }
        else
        {
            std::cout << "❓ Resposta inválida. Respostas possíveis: 'sim','nao', 'yes', 'no', 's', 'y' ,'n'." << std::endl;
        }
    }
}

// Função que verifica as pastas essenciais e o arquivo Identificators.conf
// e retorna as subpastas de TomographyAQD
bool checkNeededFiles(TString aimFolder)
{
    std::vector<TString> Essentials = {"BackgroundAQD", "CalibrationAQD", "TomographyAQD"};

    // Remove barra final, se existir
    aimFolder = aimFolder.Strip(TString::kTrailing, '/');

    // Verificação da existência do arquivo Identificators.conf
    TString idFilePath = aimFolder + "/identificators.conf";
    if (gSystem->AccessPathName(idFilePath, kFileExists))
    {
        std::cerr << "❌ Arquivo essencial faltando: identificators.conf" << std::endl;
        std::cerr << "Por favor, adicione o arquivo \"identificators.conf\" em: " << aimFolder << std::endl;
        return false;
    }

    // Verificação da existência de pastas essenciais
    for (const auto &folderName : Essentials)
    {
        TString fullPath = aimFolder + "/" + folderName;
        void *dirCheck = gSystem->OpenDirectory(fullPath);
        if (!dirCheck)
        {
            std::cerr << "❌ Pasta essencial faltando: " << folderName << std::endl;
            std::cerr << "Por favor, adicione a pasta \"" << folderName << "\" em: " << aimFolder << std::endl;
            return false; // retorna vazio em caso de erro
        }
        gSystem->FreeDirectory(dirCheck);
    }

    std::cout << "✅ Estruturas de pastas Nescessários para o processamento Presentes " << std::endl;

    return true;
}

std::vector<TString> checkTomographyAQDS(TString aimFolder)
{
    std::vector<TString> tomographySubfolders;
    // Busca subpastas dentro de TomographyAQD
    TString tomographyPath = aimFolder + "/TomographyAQD";
    void *dir = gSystem->OpenDirectory(tomographyPath);

    if (!dir)
    {
        std::cerr << "❌ Erro ao acessar a pasta TomographyAQD." << std::endl;
        return {};
    }

    const char *entry;
    std::cout << "📁 Pastas encontradas em TomographyAQD:\n";
    while ((entry = gSystem->GetDirEntry(dir)))
    {
        TString entryName(entry);

        // Ignorar "." e ".."
        if (entryName == "." || entryName == "..")
            continue;

        TString fullSubPath = tomographyPath + "/" + entryName;

        // Verifica se é diretório
        FileStat_t statbuf;
        if (gSystem->GetPathInfo(fullSubPath, statbuf) == 0 && R_ISDIR(statbuf.fMode))
        {
            tomographySubfolders.push_back(fullSubPath);
            std::cout << " - " << fullSubPath << std::endl;
        }
    }

    gSystem->FreeDirectory(dir);
    return tomographySubfolders;
}

/**
 * Função que garante a existência da estrutura de pastas necessárias para o processamento de dados.
 * Se alguma pasta não existir, ela será criada automaticamente.
 */
void checkProcessingPaths(const TString &baseFolder)
{
    std::string dataMain = "OutputData";
    std::vector<TString> Essentials = {"Processed", "CalibrationParameters", "Results", "Plots","IndividualFiles"};

    // Remove barra final, se existir
    TString cleanBase = baseFolder;
    cleanBase = cleanBase.Strip(TString::kTrailing, '/');

    // Caminho completo para a pasta OutputData
    TString outputPath = cleanBase + "/" + TString(dataMain);

    // Verifica se a pasta OutputData existe, caso não, cria
    if (gSystem->AccessPathName(outputPath, kFileExists))
    {
        std::cout << "🔧 Pasta '" << outputPath << "' não encontrada. Criando..." << std::endl;
        if (gSystem->mkdir(outputPath, kTRUE) != 0)
        {
            std::cerr << "❌ Erro ao criar a pasta: " << outputPath << std::endl;
            return;
        }
    }

    // Verifica/cria cada pasta essencial dentro de OutputData
    for (const auto &folder : Essentials)
    {
        TString fullPath = outputPath + "/" + folder;
        if (gSystem->AccessPathName(fullPath, kFileExists))
        {
            std::cout << "🔧 Pasta essencial '" << folder << "' não encontrada. Criando em: " << fullPath << std::endl;
            if (gSystem->mkdir(fullPath, kTRUE) != 0)
            {
                std::cerr << "❌ Erro ao criar a pasta: " << fullPath << std::endl;
            }
        }
    }

    std::cout << "✅ Estrutura de pastas validada e/ou criada com sucesso em: " << outputPath << std::endl;
}

/**
 * Procura por arquivos ROOT na pasta fornecida que contenham
 * "group" e "calibration"/"Calibration" no nome.
 *
 * @param targetFolder Pasta onde os arquivos serão procurados.
 * @return Caminho completo do arquivo encontrado, ou "" se não encontrado.
 */
TString findCalibrationFile(const TString &targetFolder, TString aqdData)
{
    TString cleanFolder = targetFolder.Strip(TString::kTrailing, '/');

    void *dir = gSystem->OpenDirectory(cleanFolder);
    if (!dir)
    {
        std::cerr << "❌ Não foi possível abrir a pasta: " << cleanFolder << std::endl;
        return "";
    }

    const char *entry;
    while ((entry = gSystem->GetDirEntry(dir)))
    {
        TString filename(entry);

        // Ignorar "." e ".."
        if (filename == "." || filename == "..")
            continue;

        // Verifica se é um arquivo ROOT
        if (filename.EndsWith(".root"))
        {

            // Verifica se contém "group" e "calibration" (case-insensitive)
            bool hasGroup = filename.Contains(aqdData.Data(), TString::kIgnoreCase);
            bool hasCalibration = filename.Contains("calibration", TString::kIgnoreCase);

            if (hasGroup && hasCalibration)
            {
                gSystem->FreeDirectory(dir);
                TString fullPath = cleanFolder + "/" + filename;
                std::cout << "✅ Arquivo encontrado: " << fullPath << std::endl;
                return fullPath;
            }
        }
    }

    gSystem->FreeDirectory(dir);
    std::cerr << "❌ Nenhum arquivo '.root' contendo 'group' e 'calibration' foi encontrado em: "
              << cleanFolder << std::endl;
    return "";
}

/**
 * Procura dentro de uma pasta por um arquivo ROOT cujo nome contenha uma substring fornecida.
 *
 * @param folderPath Caminho da pasta a ser verificada
 * @param nameToFind Substring a ser buscada no nome dos arquivos
 * @return TString com o caminho completo do arquivo encontrado, ou "" se nenhum for encontrado
 */
TString fileWithNameExists(const TString &folderPath, const TString &nameToFind)
{
    TString cleanFolder = folderPath.Strip(TString::kTrailing, '/');

    void *dir = gSystem->OpenDirectory(cleanFolder);
    if (!dir)
    {
        std::cerr << "❌ Não foi possível abrir a pasta: " << cleanFolder << std::endl;
        return "";
    }

    const char *entry;
    while ((entry = gSystem->GetDirEntry(dir)))
    {
        TString filename(entry);

        // Ignora "." e ".."
        if (filename == "." || filename == "..")
            continue;

        // Verifica se termina com .root
        if (filename.EndsWith(".root"))
        {

            // Verifica se o nome contém a substring desejada
            if (filename.Contains(nameToFind))
            {
                gSystem->FreeDirectory(dir);
                TString fullPath = cleanFolder + "/" + filename;
                return fullPath;
            }
        }
    }

    gSystem->FreeDirectory(dir);
    return ""; // não encontrou
}


/**
 * Garante a existência da subpasta "Background" dentro da pasta alvo fornecida.
 * Cria a subpasta caso não exista.
 *
 * @param targetFolder Pasta base onde a subpasta "Background" deve existir.
 */
void ensureBackgroundFolderExists(const TString& targetFolder) {
    // Remove barra no final, se houver
    TString baseFolder = targetFolder.Strip(TString::kTrailing, '/');

    // Caminho completo para a subpasta Background
    TString backgroundPath = baseFolder + "/Background";

    // Verifica se a subpasta já existe
    if (gSystem->AccessPathName(backgroundPath, kFileExists)) {
        std::cout << "🔧 Subpasta 'Background' não encontrada. Criando em: " << backgroundPath << std::endl;
        if (gSystem->mkdir(backgroundPath, kTRUE) != 0) {
            std::cerr << "❌ Erro ao criar a subpasta 'Background'." << std::endl;
        } else {
            std::cout << "✅ Subpasta 'Background' criada com sucesso." << std::endl;
        }
    } else {
        std::cout << "📁 Subpasta 'Background' já existe em: " << backgroundPath << std::endl;
    }
}

TString getValidFolder(const std::string& prompt) {
    while (true) {
        std::string input;
        std::cout << prompt << " (ou digite .q para sair): ";
        std::getline(std::cin, input);

        if (input == ".q") {
            std::cout << "🚪 Saindo da operação atual.\n";
            return "";
        }

        TString folder = input;
        folder = folder.Strip(TString::kTrailing, '/');

        if (!gSystem->AccessPathName(folder, kFileExists)) {
            std::cout << "✅ Pasta válida: " << folder << "\n";
            return folder;
        } else {
            std::cout << "❌ Pasta inválida ou não encontrada. Tente novamente.\n";
        }
    }
}