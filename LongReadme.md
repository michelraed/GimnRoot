
# GimnRoot
Uma biblioteca para processamento de dados tomográficos usando o sistema PETSYS.
=======

---

# 📘 Documentação da Biblioteca `GIMNRoot`

Este documento apresenta uma descrição detalhada da arquitetura, classes e funções que compõem a biblioteca `GIMNRoot`, desenvolvida no âmbito do **Grupo de Imagens e Instrumentação em Medicina Nuclear (GIMN)**. A biblioteca tem como finalidade o processamento dos dados adquiridos por sistemas de tomografia por emissão de pósitrons (PET), com ênfase tanto na fase de caracterização de detectores quanto na reconstrução de imagens tomográficas.

O pipeline de software foi desenvolvido para ser compatível com dados oriundos da eletrônica **PETSYS TOFPET2**, amplamente empregada no projeto, e inclui módulos específicos para leitura de dados brutos, agrupamento de pulsos, calibração espacial e energética, análise de desempenho, geração de histogramas, sinogramas e reconstrução tomográfica interativa.

## ⚙️ Execução do Ambiente

Para utilizar as funcionalidades da biblioteca, siga os seguintes passos no ambiente ROOT:

1. Abra um terminal e navegue até o diretório raiz do projeto:

   ```bash
   cd /caminho/para/GimnROOT
   ```

2. Inicie o ambiente interativo do **ROOT**:

   ```bash
   root
   ```

3. Dentro do prompt do ROOT, execute o script de inicialização:

   ```cpp
   .X Run.C
   ```

   Este script carrega automaticamente todos os arquivos `.C` localizados nas subpastas do diretório `source/`, tornando disponíveis as classes e funções da biblioteca para uso imediato na sessão atual.


## 1. Introdução

Este documento fornece uma documentação abrangente e detalhada do código-fonte do sistema PET (Tomografia por Emissão de Pósitrons). O sistema é projetado para processar dados de aquisição de PET, desde o pré-processamento até a análise e calibração, utilizando o framework ROOT C++.

O objetivo desta documentação é explicar o funcionamento de cada classe e função, seus atributos, métodos, parâmetros e valores de retorno, além de fornecer exemplos práticos de utilização para facilitar a compreensão e o desenvolvimento futuro.

## 2. Estrutura de Diretórios

O código-fonte está organizado em módulos funcionais, conforme descrito no `README.md` original. A seguir, uma visão geral da estrutura de diretórios:

| Diretório | Descrição |
|-----------|-----------|
| `0-functions` | Funções utilitárias e métricas de análise |
| `1-base` | Estruturas base de dados e leitura de arquivos PETsys |
| `2-histograms` | Geração de histogramas e mapas de resposta |
| `3-positioning` | Algoritmos de posicionamento de eventos |
| `4-Writer` | Escrita de dados reconstruídos e eventos |
| `5-timing` | Processamento de informações temporais |
| `6-Calibration` | Calibração espacial e energética |
| `7-process` | Pipeline de processamento de aquisição |
| `8-analyze` | Ferramentas de análise de desempenho |
| `9-massiveProcessing` | Processamento massivo de grandes volumes de dados |
| `91-sinogram` | Geração de sinogramas |
| `92-TomoProcessing` | Processamento tomográfico final |
| `93- Menu` | Interface de execução de tarefas |

## 3. Detalhamento das Classes e Funções

Esta seção detalha cada arquivo, classe e função presente no código-fonte, seguindo a estrutura de diretórios apresentada. Para cada elemento, serão fornecidas descrições, atributos, métodos (com parâmetros e retornos) e exemplos de uso.




### 3.1. `source/0-functions/Metrics.C`

Este arquivo contém funções utilitárias para o cálculo de métricas importantes na análise de dados PET, como a distância entre pontos, a Largura Total a Meio Máximo (FWHM) de picos em histogramas e a qualidade de imagens de inundação (flood images).

#### 3.1.1. Estruturas

##### `struct Point2D`

Representa um ponto em um espaço 2D com coordenadas `x` e `y`.

**Atributos:**
- `double x`: Coordenada X do ponto.
- `double y`: Coordenada Y do ponto.

**Exemplo de Uso:**
```cpp
Point2D p1 = {1.0, 2.0};
Point2D p2 = {4.0, 6.0};
```

#### 3.1.2. Funções

##### `double distance(const Point2D &a, const Point2D &b)`

Calcula a distância euclidiana entre dois pontos 2D.

**Parâmetros:**
- `const Point2D &a`: O primeiro ponto.
- `const Point2D &b`: O segundo ponto.

**Retorno:**
- `double`: A distância euclidiana entre os pontos `a` e `b`.

**Exemplo de Uso:**
```cpp
Point2D p1 = {1.0, 2.0};
Point2D p2 = {4.0, 6.0};
double dist = distance(p1, p2); // dist será aproximadamente 5.0
```

##### `double CalculteFWHMPixel(std::shared_ptr<TH2F> hist, char direction, double xPosition, double yPosition)`

Calcula a Largura Total a Meio Máximo (FWHM) de um pico em um histograma 2D (`TH2F`) em uma direção específica (x ou y) e em uma dada posição. Esta função é utilizada para caracterizar a resolução espacial de picos em imagens de inundação.

**Parâmetros:**
- `std::shared_ptr<TH2F> hist`: Ponteiro compartilhado para o histograma 2D de entrada.
- `char direction`: A direção na qual o FWHM será calculado ('x' para projeção no eixo X, 'y' para projeção no eixo Y).
- `double xPosition`: A coordenada X do ponto de interesse para a projeção.
- `double yPosition`: A coordenada Y do ponto de interesse para a projeção.

**Retorno:**
- `double`: O valor do FWHM em pixels para o pico encontrado na projeção especificada.

**Exemplo de Uso:**
```cpp
// Supondo que 'myHist' é um std::shared_ptr<TH2F> válido
double fwhm_x = CalculteFWHMPixel(myHist, 'x', 10.0, 5.0);
double fwhm_y = CalculteFWHMPixel(myHist, 'y', 10.0, 5.0);
```

##### `std::map<std::string, double> CalculateFloodQuality(std::shared_ptr<TH2F> histo)`

Calcula métricas de qualidade para uma imagem de inundação (flood image) representada por um histograma 2D (`TH2F`). A função identifica picos na imagem (cristais) e calcula a razão D/W (distância entre picos / largura dos picos) para cada cristal, retornando a média e o desvio padrão desses valores.

**Parâmetros:**
- `std::shared_ptr<TH2F> histo`: Ponteiro compartilhado para o histograma 2D da imagem de inundação.

**Retorno:**
- `std::map<std::string, double>`: Um mapa contendo duas entradas:
    - `"k"`: O valor médio da razão D/W.
    - `"stdev"`: O desvio padrão da razão D/W.

**Exemplo de Uso:**
```cpp
// Supondo que 'floodImageHist' é um std::shared_ptr<TH2F> válido
std::map<std::string, double> qualityMetrics = CalculateFloodQuality(floodImageHist);
double k_value = qualityMetrics["k"];
double stdev_k = qualityMetrics["stdev"];
```

##### `double Calculate511keVFWHM(std::shared_ptr<TH1F> hist)`

Calcula a Largura Total a Meio Máximo (FWHM) em porcentagem para o pico de 511 keV em um histograma de energia (`TH1F`). A função realiza um ajuste gaussiano na região do pico de 511 keV para determinar sua largura.

**Parâmetros:**
- `std::shared_ptr<TH1F> hist`: Ponteiro compartilhado para o histograma 1D de energia.

**Retorno:**
- `double`: O valor do FWHM em porcentagem para o pico de 511 keV.

**Exemplo de Uso:**
```cpp
// Supondo que 'energyHist' é um std::shared_ptr<TH1F> válido com o pico de 511 keV
double fwhm_511 = Calculate511keVFWHM(energyHist);
```

##### `double ajustParable(std::shared_ptr<TH1F> hist)`

Realiza um ajuste parabólico em um histograma 1D (`TH1F`) para encontrar o vértice e, a partir dele, calcular o FWHM. Esta função é útil para picos que podem não ser bem descritos por uma gaussiana pura, ou para métodos alternativos de cálculo de FWHM.

**Parâmetros:**
- `std::shared_ptr<TH1F> hist`: Ponteiro compartilhado para o histograma 1D de entrada.

**Retorno:**
- `double`: O valor do FWHM calculado a partir do ajuste parabólico.

**Exemplo de Uso:**
```cpp
// Supondo que 'dataHist' é um std::shared_ptr<TH1F> válido
double fwhm_parabolic = ajustParable(dataHist);
```

##### `Double_t CalculateCRT_TSpectrum(std::shared_ptr<TH1F> hist, Bool_t verbose = kTRUE)`

Calcula o Tempo de Resolução de Coincidência (CRT) de um histograma de tempo (`TH1F`) utilizando a classe `TSpectrum` do ROOT para detecção de picos e ajuste gaussiano. A função busca um único pico no histograma e calcula o FWHM desse pico, que representa o CRT.

**Parâmetros:**
- `std::shared_ptr<TH1F> hist`: Ponteiro compartilhado para o histograma 1D de tempo.
- `Bool_t verbose`: (Opcional) Se `kTRUE`, imprime informações detalhadas sobre o processo de cálculo e os resultados. Padrão é `kTRUE`.

**Retorno:**
- `Double_t`: O valor do CRT em nanossegundos (ns). Retorna `-1` em caso de histograma inválido, estatística insuficiente ou número de picos detectados diferente de 1.

**Exemplo de Uso:**
```cpp
// Supondo que 'timeHist' é um std::shared_ptr<TH1F> válido
Double_t crt_value = CalculateCRT_TSpectrum(timeHist, kTRUE);
```




### 3.2. `source/0-functions/Utils.C`

Este arquivo contém uma coleção de funções utilitárias diversas que auxiliam em operações de manipulação de strings, arquivos, diretórios, matrizes e interações com o usuário. São funções de propósito geral que podem ser usadas em várias partes do sistema.

#### 3.2.1. Funções

##### `void divideMatrix(TMatrixT<double> &mat, double divisor)`

Divide todos os elementos de uma matriz `TMatrixT<double>` por um valor escalar `divisor`.

**Parâmetros:**
- `TMatrixT<double> &mat`: A matriz a ser modificada (passada por referência).
- `double divisor`: O valor pelo qual os elementos da matriz serão divididos.

**Exemplo de Uso:**
```cpp
TMatrixT<double> myMatrix(2, 2);
myMatrix(0,0) = 10.0; myMatrix(0,1) = 20.0;
myMatrix(1,0) = 30.0; myMatrix(1,1) = 40.0;
divideMatrix(myMatrix, 10.0); // myMatrix agora será {{1.0, 2.0}, {3.0, 4.0}}
```

##### `void divideMatrix(TMatrixT<double> &mat, TMatrixT<double> &mat2)`

Divide os elementos correspondentes de uma matriz `mat` pelos elementos de outra matriz `mat2` (elemento a elemento). Se a divisão resultar em `NaN` (Not a Number), o elemento resultante é definido como 0.

**Parâmetros:**
- `TMatrixT<double> &mat`: A matriz do numerador, que será modificada com o resultado da divisão.
- `TMatrixT<double> &mat2`: A matriz do denominador.

**Exemplo de Uso:**
```cpp
TMatrixT<double> mat1(2, 2); mat1(0,0) = 10.0; mat1(0,1) = 20.0; mat1(1,0) = 30.0; mat1(1,1) = 40.0;
TMatrixT<double> mat2(2, 2); mat2(0,0) = 2.0; mat2(0,1) = 5.0; mat2(1,0) = 10.0; mat2(1,1) = 8.0;
divideMatrix(mat1, mat2); // mat1 agora será {{5.0, 4.0}, {3.0, 5.0}}
```

##### `std::string joinString(std::vector<std::string> toJoin, char separator)`

Concatena um vetor de strings em uma única string, utilizando um caractere separador especificado.

**Parâmetros:**
- `std::vector<std::string> toJoin`: O vetor de strings a ser concatenado.
- `char separator`: O caractere a ser usado como separador entre as strings.

**Retorno:**
- `std::string`: A string resultante da concatenação.

**Exemplo de Uso:**
```cpp
std::vector<std::string> words = {"hello", "world", "example"};
std::string result = joinString(words, ' '); // result será "hello world example "
```

##### `std::vector<std::string> splitString(std::string string, char separator)`

Divide uma string em um vetor de strings, utilizando um caractere separador especificado. Útil para processar caminhos de arquivos ou dados delimitados.

**Parâmetros:**
- `std::string string`: A string a ser dividida.
- `char separator`: O caractere a ser usado como separador.

**Retorno:**
- `std::vector<std::string>`: Um vetor de strings contendo as partes da string original.

**Exemplo de Uso:**
```cpp
std::string path = "/home/user/documents/file.txt";
std::vector<std::string> parts = splitString(path, '/'); 
// parts será {"", "home", "user", "documents", "file.txt"}

std::string sentence = "a menina comeu banana";
std::vector<std::string> words = splitString(sentence, ' ');
// words será {"a", "menina", "comeu", "banana"}
```

##### `std::vector<Int_t> CorrectChannelToPixelPs(Int_t channel)`

Converte um número de canal de um SiPM Ketek 3325-WB0808 para suas coordenadas (X, Y) em um layout de pixel. A função utiliza um mapa de correção hardcoded para mapear o canal para a posição correta.

**Parâmetros:**
- `Int_t channel`: O número do canal a ser convertido.

**Retorno:**
- `std::vector<Int_t>`: Um vetor contendo dois inteiros: a posição X (coluna) na posição 0 e a posição Y (linha) na posição 1.

**Exemplo de Uso:**
```cpp
std::vector<Int_t> pixelCoords = CorrectChannelToPixelPs(0); // Retorna {1, 7} para o canal 0
Int_t x = pixelCoords[0];
Int_t y = pixelCoords[1];
```

##### `void getRowAndColumnSums(const TMatrixT<double> &matrix, std::vector<double> &rowSums, std::vector<double> &colSums)`

Calcula a soma dos elementos de cada linha e cada coluna de uma matriz `TMatrixT<double>`. Os resultados são passados por referência para os vetores `rowSums` e `colSums`.

**Parâmetros:**
- `const TMatrixT<double> &matrix`: A matriz de entrada.
- `std::vector<double> &rowSums`: Vetor onde as somas das linhas serão armazenadas.
- `std::vector<double> &colSums`: Vetor onde as somas das colunas serão armazenadas.

**Exemplo de Uso:**
```cpp
TMatrixT<double> myMatrix(2, 2); myMatrix(0,0) = 1.0; myMatrix(0,1) = 2.0; myMatrix(1,0) = 3.0; myMatrix(1,1) = 4.0;
std::vector<double> rows(2); 
std::vector<double> cols(2);
getRowAndColumnSums(myMatrix, rows, cols); 
// rows será {3.0, 7.0}
// cols será {4.0, 6.0}
```

##### `void getRowAndColumnSums(const TMatrixT<Float_t> &matrix, std::vector<Float_t> &rowSums, std::vector<Float_t> &colSums)`

Sobrecarga da função `getRowAndColumnSums` para matrizes de tipo `Float_t`. Calcula a soma dos elementos de cada linha e cada coluna de uma matriz `TMatrixT<Float_t>`. Os resultados são passados por referência para os vetores `rowSums` e `colSums`.

**Parâmetros:**
- `const TMatrixT<Float_t> &matrix`: A matriz de entrada.
- `std::vector<Float_t> &rowSums`: Vetor onde as somas das linhas serão armazenadas.
- `std::vector<Float_t> &colSums`: Vetor onde as somas das colunas serão armazenadas.

**Exemplo de Uso:**
```cpp
TMatrixT<Float_t> myMatrix(2, 2); myMatrix(0,0) = 1.0f; myMatrix(0,1) = 2.0f; myMatrix(1,0) = 3.0f; myMatrix(1,1) = 4.0f;
std::vector<Float_t> rows(2); 
std::vector<Float_t> cols(2);
getRowAndColumnSums(myMatrix, rows, cols); 
// rows será {3.0f, 7.0f}
// cols será {4.0f, 6.0f}
```

##### `void showFiles(std::string path)`

Lista e imprime no console todos os arquivos presentes em um diretório especificado.

**Parâmetros:**
- `std::string path`: O caminho para o diretório a ser listado.

**Exemplo de Uso:**
```cpp
showFiles("/home/user/data");
// Saída esperada:
// 0: file1.txt
// 1: image.png
// ...
```

##### `std::string getFiles(std::string path, int index)`

Obtém o nome de um arquivo específico em um diretório, dado seu índice na lista de arquivos. A função lista os arquivos e retorna o nome do arquivo correspondente ao índice fornecido.

**Parâmetros:**
- `std::string path`: O caminho para o diretório.
- `int index`: O índice do arquivo desejado na lista (baseado em 0).

**Retorno:**
- `std::string`: O nome do arquivo encontrado, ou uma string vazia se o índice for inválido ou o diretório não puder ser aberto.

**Exemplo de Uso:**
```cpp
std::string fileName = getFiles("/home/user/data", 0); // Retorna o nome do primeiro arquivo
```

##### `TList* getFolderList(const std::string &folder)`

Retorna um `TList` contendo os arquivos e subdiretórios de uma pasta especificada. Esta função é útil para iteração e manipulação de conteúdo de diretórios no ambiente ROOT.

**Parâmetros:**
- `const std::string &folder`: O caminho para a pasta.

**Retorno:**
- `TList*`: Um ponteiro para um `TList` contendo os objetos `TSystemFile` e `TSystemDirectory` da pasta, ou `nullptr` se a pasta não puder ser aberta.

**Exemplo de Uso:**
```cpp
TList* filesAndFolders = getFolderList("/path/to/my/folder");
if (filesAndFolders) {
    TIter next(filesAndFolders);
    TSystemFile* entry;
    while ((entry = (TSystemFile*)next())) {
        std::cout << entry->GetName() << std::endl;
    }
    delete filesAndFolders; // Liberar memória
}
```

##### `void LoadLib(TString param , TString fileName)`

Carrega uma biblioteca ROOT (geralmente um arquivo `.C` ou `.so`) usando `gROOT->ProcessLine()`. A função verifica se o `fileName` termina com `.C` antes de tentar carregar.

**Parâmetros:**
- `TString param`: Parâmetros adicionais para o comando `gROOT->ProcessLine()` (ex: ".L" para carregar).
- `TString fileName`: O nome do arquivo da biblioteca a ser carregada.

**Exemplo de Uso:**
```cpp
LoadLib(".L", "MyAnalysis.C+"); // Carrega e compila MyAnalysis.C
```

##### `void CloseAllCanvases()`

Fecha todas as instâncias de `TCanvas` abertas no ambiente ROOT. Útil para limpar a interface gráfica após a geração de muitos gráficos.

**Parâmetros:**
- Nenhum.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
// ... (código que cria vários TCanvas)
CloseAllCanvases(); // Fecha todos os canvases abertos
```

##### `int extractNumber(const std::string& str)`

Extrai o primeiro número inteiro encontrado em uma string. Percorre a string e concatena os dígitos encontrados, convertendo-os para um inteiro.

**Parâmetros:**
- `const std::string& str`: A string de onde o número será extraído.

**Retorno:**
- `int`: O número inteiro extraído. Retorna 0 se nenhum número for encontrado.

**Exemplo de Uso:**
```cpp
int num = extractNumber("channel-123-detector"); // num será 123
int noNum = extractNumber("no_numbers_here"); // noNum será 0
```

##### `std::string removePart(const std::string &str, const std::string &part)`

Remove a primeira ocorrência de uma substring `part` de uma string `str`.

**Parâmetros:**
- `const std::string &str`: A string original.
- `const std::string &part`: A substring a ser removida.

**Retorno:**
- `std::string`: A string resultante após a remoção, ou a string original se `part` não for encontrada.

**Exemplo de Uso:**
```cpp
std::string original = "channel-1-ChipID-10";
std::string cleaned = removePart(original, "-ChipID-10"); // cleaned será "channel-1"
```

##### `bool arquivoExiste(const std::string& nomeArquivo)`

Verifica se um arquivo com o nome especificado existe no sistema de arquivos.

**Parâmetros:**
- `const std::string& nomeArquivo`: O caminho completo para o arquivo.

**Retorno:**
- `bool`: `true` se o arquivo existir e puder ser aberto, `false` caso contrário.

**Exemplo de Uso:**
```cpp
if (arquivoExiste("data.root")) {
    std::cout << "Arquivo existe!" << std::endl;
}
```

##### `void MergeTrees(const char* inputDir, const char* outputFile)`

Mescla todos os arquivos ROOT contendo a árvore `tomographicCoincidences` de um diretório de entrada em um único arquivo ROOT de saída. A função cria um `TChain` para adicionar as árvores e, em seguida, clona a árvore mesclada para o arquivo de saída.

**Parâmetros:**
- `const char* inputDir`: O caminho para o diretório contendo os arquivos ROOT de entrada.
- `const char* outputFile`: O caminho e nome do arquivo ROOT de saída onde a árvore mesclada será salva.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
MergeTrees("/path/to/input_data", "merged_output.root");
```

##### `std::vector<TString> ListSubdirectories(const TString& targetFolder)`

Lista todas as subpastas diretas dentro de uma pasta fornecida. Retorna o caminho completo de cada subpasta.

**Parâmetros:**
- `const TString& targetFolder`: O caminho da pasta onde procurar subdiretórios.

**Retorno:**
- `std::vector<TString>`: Um vetor de `TString` contendo o caminho completo de cada subpasta encontrada.

**Exemplo de Uso:**
```cpp
std::vector<TString> subfolders = ListSubdirectories("/home/user/projects");
for (const auto& folder : subfolders) {
    std::cout << folder.Data() << std::endl;
}
```

##### `void MergeAndReplaceOutput(const std::string& inputDirStr, const std::string& outputDirStr)`

Realiza a mesclagem de arquivos ROOT de uma pasta de entrada, salvando o resultado na pasta de saída com o nome da pasta de entrada. Se o arquivo de saída já existir, ele é deletado antes da mesclagem. Esta função é robusta e inclui verificações de existência de diretórios e tratamento de erros.

**Parâmetros:**
- `const std::string& inputDirStr`: Caminho da pasta contendo os arquivos a serem mesclados.
- `const std::string& outputDirStr`: Caminho da pasta onde o arquivo mesclado será salvo.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
MergeAndReplaceOutput("/data/raw/run1", "/data/processed");
```

##### `void GenerateBackgroundCopiesFromTemplate(const TString& inputFolderStr, const TString& outputFolderStr, int angleStep = 15)`

Gera cópias de arquivos ROOT de background para diferentes ângulos. A função busca arquivos com "_background_" no nome (um de coincidência e um de grupo) e os replica para diferentes ângulos, salvando os novos arquivos em uma subpasta "Background" dentro da pasta de saída fornecida.

**Parâmetros:**
- `const TString& inputFolderStr`: Caminho da pasta onde os arquivos `*_background_*.root` estão localizados.
- `const TString& outputFolderStr`: Caminho da pasta onde os arquivos serão copiados.
- `int angleStep`: (Opcional) Passo angular (em graus) entre as cópias geradas. Padrão é 15°.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
GenerateBackgroundCopiesFromTemplate("/data/background_templates", "/data/simulations", 30);
```

##### `void ensureFolderExists(const TString& folderPath)`

Verifica se uma pasta existe e, caso não exista, a cria (incluindo diretórios intermediários, se necessário). Útil para garantir que os diretórios de saída estejam prontos antes de escrever arquivos.

**Parâmetros:**
- `const TString& folderPath`: Caminho da pasta a ser verificada/criada.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
ensureFolderExists("/output/results/run_01");
```

##### `void WriteFolderTreeSummary(std::ofstream& out, const TString& path, int level = 0)`

Função auxiliar recursiva que escreve um resumo hierárquico de pastas em um stream de saída (`std::ofstream`).

**Parâmetros:**
- `std::ofstream& out`: O stream de saída para onde o resumo será escrito.
- `const TString& path`: O caminho da pasta atual a ser processada.
- `int level`: (Opcional) O nível de indentação atual na árvore de diretórios. Padrão é 0.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
// Usada internamente por GenerateFolderStructureMarkdown
```

##### `void WriteFolderTreeRecursive(std::ofstream& out, const TString& path, int level = 0)`

Função auxiliar recursiva que escreve a estrutura detalhada de diretórios e arquivos em um stream de saída (`std::ofstream`).

**Parâmetros:**
- `std::ofstream& out`: O stream de saída para onde a estrutura será escrita.
- `const TString& path`: O caminho da pasta atual a ser processada.
- `int level`: (Opcional) O nível de indentação atual na árvore de diretórios. Padrão é 0.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
// Usada internamente por GenerateFolderStructureMarkdown
```

##### `void GenerateFolderStructureMarkdown(const TString& baseFolder, const TString& outputMarkdownFile = "folder_structure.md")`

Gera um arquivo Markdown (`.md`) que descreve a estrutura de diretórios de uma pasta base. O arquivo gerado inclui um resumo hierárquico de pastas e uma estrutura detalhada com arquivos.

**Parâmetros:**
- `const TString& baseFolder`: O caminho da pasta base cuja estrutura será documentada.
- `const TString& outputMarkdownFile`: (Opcional) O nome do arquivo Markdown de saída. Padrão é `folder_structure.md`.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
GenerateFolderStructureMarkdown("/home/user/my_project", "project_structure.md");
```

##### `TString SelectFileFromFolder(const TString& folderPath)`

Lista os arquivos de uma pasta e permite ao usuário selecionar um pelo índice. A função interage com o usuário via console para obter a escolha.

**Parâmetros:**
- `const TString& folderPath`: Caminho da pasta a ser explorada.

**Retorno:**
- `TString`: Caminho completo do arquivo selecionado, ou uma string vazia se o usuário cancelar ou a escolha for inválida.

**Exemplo de Uso:**
```cpp
TString selectedFile = SelectFileFromFolder("/data/raw");
if (!selectedFile.IsNull()) {
    std::cout << "Você selecionou: " << selectedFile.Data() << std::endl;
}
```

##### `TString SelectFolderFromDirectory(const TString& basePath)`

Lista as subpastas de um diretório e permite ao usuário selecionar uma delas pelo índice. A função interage com o usuário via console para obter a escolha.

**Parâmetros:**
- `const TString& basePath`: Caminho da pasta a ser explorada.

**Retorno:**
- `TString`: Caminho completo da subpasta selecionada, ou uma string vazia se o usuário cancelar ou a escolha for inválida.

**Exemplo de Uso:**
```cpp
TString selectedFolder = SelectFolderFromDirectory("/home/user/data");
if (!selectedFolder.IsNull()) {
    std::cout << "Você selecionou a pasta: " << selectedFolder.Data() << std::endl;
}
```

##### `bool askUserToContinue()`

Pergunta ao usuário se deseja continuar e interpreta a resposta (sim/não). A função é robusta a diferentes formatos de entrada (ex: "s", "sim", "yes", "n", "nao", "no").

**Parâmetros:**
- Nenhum.

**Retorno:**
- `bool`: `true` se a resposta for afirmativa, `false` se for negativa.

**Exemplo de Uso:**
```cpp
if (askUserToContinue()) {
    std::cout << "Continuando..." << std::endl;
} else {
    std::cout << "Encerrando..." << std::endl;
}
```

##### `bool checkNeededFiles(TString aimFolder)`

Verifica a existência de pastas essenciais (`BackgroundAQD`, `CalibrationAQD`, `TomographyAQD`) e do arquivo `identificators.conf` dentro de uma pasta alvo. Esta função é crucial para validar a estrutura de diretórios esperada pelo sistema.

**Parâmetros:**
- `TString aimFolder`: O caminho da pasta alvo a ser verificada.

**Retorno:**
- `bool`: `true` se todas as pastas e o arquivo essencial existirem, `false` caso contrário.

**Exemplo de Uso:**
```cpp
if (checkNeededFiles("/data/my_acquisition")) {
    std::cout << "Estrutura de arquivos OK." << std::endl;
} else {
    std::cerr << "Estrutura de arquivos incompleta!" << std::endl;
}
```

##### `std::vector<TString> checkTomographyAQDS(TString aimFolder)`

Verifica e lista as subpastas dentro do diretório `TomographyAQD` (que deve estar dentro de `aimFolder`). Esta função é usada para identificar diferentes aquisições tomográficas.

**Parâmetros:**
- `TString aimFolder`: O caminho da pasta base que contém `TomographyAQD`.

**Retorno:**
- `std::vector<TString>`: Um vetor de `TString` contendo o caminho completo de cada subpasta encontrada em `TomographyAQD`.

**Exemplo de Uso:**
```cpp
std::vector<TString> tomographyRuns = checkTomographyAQDS("/data/experiments");
for (const auto& run : tomographyRuns) {
    std::cout << "Tomografia encontrada: " << run.Data() << std::endl;
}
```

##### `void checkProcessingPaths(const TString &baseFolder)`

Função que garante a existência da estrutura de pastas necessárias para o processamento de dados de saída. Se alguma pasta não existir (`OutputData`, `Processed`, `CalibrationParameters`, `Results`, `Plots`, `IndividualFiles`), ela será criada automaticamente. Esta função é vital para organizar os resultados do processamento.

**Parâmetros:**
- `const TString &baseFolder`: A pasta base onde a estrutura de saída será criada (ex: `/home/user/gimnpet_output`).

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
checkProcessingPaths("/home/user/my_project_output");
// Isso criará /home/user/my_project_output/OutputData/Processed, etc.
```

##### `TString findCalibrationFile(const TString &targetFolder, TString aqdData)`

Procura por arquivos ROOT em uma pasta que contenham "group" e "calibration" (case-insensitive) no nome, e que também contenham a string `aqdData`. Retorna o caminho completo do primeiro arquivo encontrado que satisfaz os critérios.

**Parâmetros:**
- `const TString &targetFolder`: Pasta onde os arquivos serão procurados.
- `TString aqdData`: Substring adicional a ser buscada no nome do arquivo (ex: "qdc", "tot").

**Retorno:**
- `TString`: Caminho completo do arquivo encontrado, ou uma string vazia se nenhum arquivo for encontrado.

**Exemplo de Uso:**
```cpp
TString calFile = findCalibrationFile("/data/calibration", "qdc");
if (!calFile.IsNull()) {
    std::cout << "Arquivo de calibração QDC encontrado: " << calFile.Data() << std::endl;
}
```

##### `TString fileWithNameExists(const TString &folderPath, const TString &nameToFind)`

Procura dentro de uma pasta por um arquivo ROOT (`.root`) cujo nome contenha uma substring fornecida (`nameToFind`). Retorna o caminho completo do primeiro arquivo encontrado.

**Parâmetros:**
- `const TString &folderPath`: Caminho da pasta a ser verificada.
- `const TString &nameToFind`: Substring a ser buscada no nome dos arquivos.

**Retorno:**
- `TString`: Caminho completo do arquivo encontrado, ou uma string vazia se nenhum for encontrado.

**Exemplo de Uso:**
```cpp
TString dataFile = fileWithNameExists("/data/raw", "run_001");
if (!dataFile.IsNull()) {
    std::cout << "Arquivo de dados encontrado: " << dataFile.Data() << std::endl;
}
```

##### `void ensureBackgroundFolderExists(const TString& targetFolder)`

Garante a existência da subpasta "Background" dentro da pasta alvo fornecida. Cria a subpasta caso não exista. Esta função é específica para a organização de arquivos de background.

**Parâmetros:**
- `const TString& targetFolder`: Pasta base onde a subpasta "Background" deve existir.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
ensureBackgroundFolderExists("/data/simulations");
// Isso criará /data/simulations/Background se não existir
```

##### `TString getValidFolder(const std::string& prompt)`

Solicita ao usuário um caminho de pasta e valida se a pasta existe. Continua solicitando até que um caminho válido seja fornecido ou o usuário digite ".q" para sair.

**Parâmetros:**
- `const std::string& prompt`: A mensagem a ser exibida ao usuário para solicitar a entrada.

**Retorno:**
- `TString`: O caminho válido da pasta inserido pelo usuário, ou uma string vazia se o usuário optar por sair.

**Exemplo de Uso:**
```cpp
TString folder = getValidFolder("Digite o caminho da pasta de entrada");
if (!folder.IsNull()) {
    std::cout << "Pasta selecionada: " << folder.Data() << std::endl;
}
```




### 3.3. `source/1-base/GimnData.C`

Este arquivo define a classe `GimnData`, que é fundamental para o armazenamento e manipulação de dados de histogramas no sistema PET. Ela encapsula um conjunto de histogramas 1D e 2D, fornecendo métodos para inicialização e acesso a esses dados.

#### 3.3.1. Classes

##### `class GimnData`

Gerencia uma coleção de histogramas ROOT (`TH1F` e `TH2F`) usados para armazenar dados de energia, tempo e posicionamento de eventos PET. A classe permite a inicialização desses histogramas com configurações padrão e o acesso a eles.

**Atributos:**
- `TH1F *hEnergy`: Histograma 1D para energia.
- `TH1F *hEnergyRaw`: Histograma 1D para energia bruta.
- `TH1F *hTime`: Histograma 1D para tempo.
- `TH1F *hTimeRaw`: Histograma 1D para tempo bruto.
- `TH1F *hTimeDiff`: Histograma 1D para diferença de tempo.
- `TH2F *hPosition`: Histograma 2D para posicionamento.
- `TH2F *hPositionRaw`: Histograma 2D para posicionamento bruto.
- `TH2F *hPositionEnergy`: Histograma 2D para posicionamento vs. energia.
- `TH2F *hPositionEnergyRaw`: Histograma 2D para posicionamento vs. energia bruta.
- `TH2F *hTimePosition`: Histograma 2D para tempo vs. posicionamento.
- `TH2F *hTimePositionRaw`: Histograma 2D para tempo vs. posicionamento bruto.
- `TH2F *hTimeEnergy`: Histograma 2D para tempo vs. energia.
- `TH2F *hTimeEnergyRaw`: Histograma 2D para tempo vs. energia bruta.
- `Int_t counter`: Contador interno para controle de estado ou iterações.

**Construtor:**
##### `GimnData()`

Construtor padrão da classe `GimnData`. Inicializa o contador interno com 0.

**Exemplo de Uso:**
```cpp
GimnData myData;
```

**Métodos:**

##### `void initializeHistograms()`

Inicializa todos os histogramas membros da classe com configurações padrão (número de bins, ranges). Esta função deve ser chamada após a criação de um objeto `GimnData` para preparar os histogramas para o preenchimento de dados.

**Parâmetros:**
- Nenhum.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
GimnData myData;
myData.initializeHistograms();
// Agora os histogramas estão prontos para serem preenchidos
```

##### `void setCounter(Int_t value)`

Define o valor do contador interno da classe.

**Parâmetros:**
- `Int_t value`: O novo valor para o contador.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
GimnData myData;
myData.setCounter(10);
```

##### `Int_t counter()`

Retorna o valor atual do contador interno da classe.

**Parâmetros:**
- Nenhum.

**Retorno:**
- `Int_t`: O valor atual do contador.

**Exemplo de Uso:**
```cpp
GimnData myData;
myData.setCounter(5);
Int_t currentCount = myData.counter(); // currentCount será 5
```




### 3.4. `source/1-base/GimnLoader.C`

Este arquivo é crucial para o carregamento de dados e configurações do sistema PET. Ele define as classes `Loader`, `Identificator` e `histoConf`, que trabalham em conjunto para ler e interpretar os arquivos de dados brutos e de configuração, preparando as informações para o processamento subsequente.

#### 3.4.1. Classes

##### `class histoConf`

Armazena as configurações para a criação de histogramas, como o número de bins e os limites dos eixos. Esta classe é usada para padronizar a criação de histogramas em diferentes partes do sistema.

**Atributos:**
- `int nbinsx`: Número de bins no eixo X.
- `double xmin`: Limite inferior do eixo X.
- `double xmax`: Limite superior do eixo X.
- `int nbinsy`: Número de bins no eixo Y (para histogramas 2D).
- `double ymin`: Limite inferior do eixo Y.
- `double ymax`: Limite superior do eixo Y.

**Exemplo de Uso:**
```cpp
histoConf config;
config.nbinsx = 100;
config.xmin = 0.0;
config.xmax = 1024.0;

TH1F* myHist = new TH1F("h1", "My Histogram", config.nbinsx, config.xmin, config.xmax);
```

##### `class Identificator`

Representa a identidade de um detector no sistema PET. Armazena informações como o nome do detector, tipo de cristal, ângulo de posicionamento e outras propriedades físicas. Esses objetos são carregados a partir do arquivo `identificators.conf`.

**Atributos:**
- `TString detectorName`: Nome do detector.
- `TString crystalType`: Tipo de cristal usado no detector.
- `int detNum`: Número de identificação do detector.
- `int chipIDs[4]`: IDs dos chips associados ao detector.
- `int ports[4]`: Portas associadas ao detector.
- `double detectorAngle`: Ângulo de posicionamento do detector.
- `double detectorZRotation`: Rotação do detector em torno do eixo Z.
- `bool inCoincidenceWith[10]`: Array booleano que indica com quais outros detectores este detector está em coincidência.
- `bool timing`: Flag que indica se o detector possui informações de tempo.
- `bool positioning`: Flag que indica se o detector possui informações de posicionamento.

**Métodos:**

- **Getters e Setters:** A classe fornece um conjunto completo de métodos `get` e `set` para todos os seus atributos (ex: `getDetectorName()`, `setDetectorName(TString name)`, `getChipIDs()`, `setChipIDs(int ids[4])`, etc.).

**Exemplo de Uso:**
```cpp
Identificator id;
id.setDetectorName("Detector_01");
id.setDetectorAngle(90.0);

TString name = id.getDetectorName();
```

##### `class Loader`

É a classe principal para o carregamento de dados. Ela gerencia a leitura de arquivos de dados brutos (`.root`), associa os dados aos `Identificator` corretos e fornece métodos para acessar as informações lidas. A classe pode operar em diferentes modos (coincidência, grupo, etc.) e gerencia o ponteiro de leitura nos arquivos de dados.

**Atributos:**
- `TFile *f`: Ponteiro para o arquivo ROOT (`.root`) que está sendo lido.
- `TTree *fTree`: Ponteiro para a árvore (`TTree`) dentro do arquivo ROOT.
- `TString fileName`: Nome do arquivo que está sendo lido.
- `long pontLeitura`: Ponteiro de leitura atual na árvore (índice do evento).
- `long entries`: Número total de entradas (eventos) na árvore.
- `int ref`: Referência interna, geralmente o número do detector.
- `std::vector<Identificator> id`: Vetor de objetos `Identificator` que contém as configurações de todos os detectores.

**Construtor:**
##### `Loader()`

Construtor padrão da classe `Loader`. Inicializa os ponteiros `f` e `fTree` como nulos e o ponteiro de leitura `pontLeitura` como 0.

**Exemplo de Uso:**
```cpp
Loader myLoader;
```

**Métodos:**

##### `void loadFile(TString fName)`

Tenta abrir um arquivo ROOT com o nome fornecido. Se bem-sucedido, armazena o ponteiro para o `TFile` e o nome do arquivo nos atributos da classe.

**Parâmetros:**
- `TString fName`: O nome do arquivo ROOT a ser aberto.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
Loader myLoader;
myLoader.loadFile("data_run_01.root");
```

##### `void closeFile()`

Fecha o arquivo ROOT que foi aberto anteriormente. É importante chamar este método para liberar os recursos do arquivo.

**Parâmetros:**
- Nenhum.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
Loader myLoader;
myLoader.loadFile("data.root");
// ... processamento ...
myLoader.closeFile();
```

##### `void LoadIdentificators(TString fName)`

Carrega as informações de identificação dos detectores a partir de um arquivo de configuração (geralmente `identificators.conf`). A função lê o arquivo linha por linha, extrai os parâmetros de cada detector e cria objetos `Identificator` que são armazenados no vetor `id`.

**Parâmetros:**
- `TString fName`: O nome do arquivo de configuração dos identificadores.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
Loader myLoader;
myLoader.LoadIdentificators("identificators.conf");
```

##### `void Load(TString treeName)`

Carrega uma árvore (`TTree`) específica do arquivo ROOT que já foi aberto. A função obtém o ponteiro para a árvore e o número total de entradas, preparando o `Loader` para a leitura dos eventos.

**Parâmetros:**
- `TString treeName`: O nome da árvore a ser carregada do arquivo.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
Loader myLoader;
myLoader.loadFile("data.root");
myLoader.Load("coincidences"); // Carrega a árvore chamada "coincidences"
```

##### `void print()`

Imprime no console as informações de todos os `Identificator` que foram carregados, mostrando os detalhes de cada detector configurado.

**Parâmetros:**
- Nenhum.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
Loader myLoader;
myLoader.LoadIdentificators("identificators.conf");
myLoader.print();
```

- **Outros Getters e Setters:** A classe também fornece métodos `get` e `set` para seus atributos, como `getFileName()`, `setFileName(TString name)`, `getPontLeitura()`, `setPontLeitura(long p)`, etc.




### 3.5. `source/1-base/petSysCoincidences.C`

Este arquivo define a classe `PetsysCoincidences`, que é responsável por gerenciar e acessar dados de coincidências de eventos PET. Ela atua como um wrapper para a leitura de dados de coincidência de um arquivo ROOT, permitindo a iteração sobre as entradas e o acesso aos canais envolvidos na coincidência.

#### 3.5.1. Classes

##### `class PetsysCoincidences`

Gerencia a leitura e o acesso a eventos de coincidência de um `TTree` (árvore ROOT). A classe mantém um ponteiro para a árvore e o índice da entrada atual, permitindo a navegação pelos eventos de coincidência.

**Atributos:**
- `TTree *fTree`: Ponteiro para a árvore ROOT que contém os dados de coincidência.
- `Long64_t fCurrentEntry`: O índice da entrada (evento) atual na árvore.
- `Int_t channel1_`: Canal do primeiro evento na coincidência.
- `Int_t channel2_`: Canal do segundo evento na coincidência.

**Construtor:**
##### `PetsysCoincidences(TTree *tree)`

Construtor da classe `PetsysCoincidences`. Inicializa a classe com um ponteiro para a árvore ROOT que contém os dados de coincidência.

**Parâmetros:**
- `TTree *tree`: Ponteiro para a árvore ROOT de coincidências.

**Exemplo de Uso:**
```cpp
TFile *file = TFile::Open("coincidence_data.root");
TTree *tree = (TTree*)file->Get("coincidences"); // Supondo que a árvore se chama "coincidences"
PetsysCoincidences coincidences(tree);
```

**Métodos:**

##### `Long64_t Entries()`

Retorna o número total de entradas (eventos de coincidência) na árvore.

**Parâmetros:**
- Nenhum.

**Retorno:**
- `Long64_t`: O número total de entradas.

**Exemplo de Uso:**
```cpp
PetsysCoincidences coincidences(tree);
Long64_t numEntries = coincidences.Entries();
std::cout << "Total de coincidências: " << numEntries << std::endl;
```

##### `void switchState(Long64_t entry)`

Define a entrada atual na árvore para o índice especificado e carrega os dados correspondentes (canais) para os atributos internos da classe.

**Parâmetros:**
- `Long64_t entry`: O índice da entrada para a qual a classe deve mudar.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
PetsysCoincidences coincidences(tree);
coincidences.switchState(0); // Mudar para a primeira entrada
```

##### `Int_t channel1()`

Retorna o número do canal do primeiro evento na coincidência da entrada atual.

**Parâmetros:**
- Nenhum.

**Retorno:**
- `Int_t`: O número do canal do primeiro evento.

**Exemplo de Uso:**
```cpp
PetsysCoincidences coincidences(tree);
coincidences.switchState(0);
Int_t ch1 = coincidences.channel1();
```

##### `Int_t channel2()`

Retorna o número do canal do segundo evento na coincidência da entrada atual.

**Parâmetros:**
- Nenhum.

**Retorno:**
- `Int_t`: O número do canal do segundo evento.

**Exemplo de Uso:**
```cpp
PetsysCoincidences coincidences(tree);
coincidences.switchState(0);
Int_t ch2 = coincidences.channel2();
```

##### `void getEntry(Long64_t entry)`

Este método é um alias para `switchState(entry)`, fornecendo uma interface alternativa para mudar para uma entrada específica na árvore e carregar seus dados.

**Parâmetros:**
- `Long64_t entry`: O índice da entrada para a qual a classe deve mudar.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
PetsysCoincidences coincidences(tree);
coincidences.getEntry(5); // Mudar para a sexta entrada
```




### 3.6. `source/1-base/petsysGroup.C`

Este arquivo define a classe `PetsysGroup`, que é utilizada para gerenciar e acessar dados de eventos agrupados (group events) de um arquivo ROOT. Similar à `PetsysCoincidences`, ela atua como um facilitador para a leitura e iteração sobre os dados de grupos, permitindo o acesso ao canal principal do grupo.

#### 3.6.1. Classes

##### `class PetsysGroup`

Gerencia a leitura e o acesso a eventos agrupados de um `TTree` (árvore ROOT). A classe mantém um ponteiro para a árvore e o índice da entrada atual, permitindo a navegação pelos eventos de grupo.

**Atributos:**
- `TTree *fTree`: Ponteiro para a árvore ROOT que contém os dados de grupo.
- `Long64_t fCurrentEntry`: O índice da entrada (evento) atual na árvore.
- `Int_t channel_`: Canal principal do evento agrupado.

**Construtor:**
##### `PetsysGroup(TTree *tree)`

Construtor da classe `PetsysGroup`. Inicializa a classe com um ponteiro para a árvore ROOT que contém os dados de grupo.

**Parâmetros:**
- `TTree *tree`: Ponteiro para a árvore ROOT de grupos.

**Exemplo de Uso:**
```cpp
TFile *file = TFile::Open("group_data.root");
TTree *tree = (TTree*)file->Get("groups"); // Supondo que a árvore se chama "groups"
PetsysGroup group(tree);
```

**Métodos:**

##### `void switchState(Long64_t entry)`

Define a entrada atual na árvore para o índice especificado e carrega os dados correspondentes (canal) para o atributo interno da classe.

**Parâmetros:**
- `Long64_t entry`: O índice da entrada para a qual a classe deve mudar.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
PetsysGroup group(tree);
group.switchState(0); // Mudar para a primeira entrada
```

##### `Int_t channel()`

Retorna o número do canal principal do evento agrupado da entrada atual.

**Parâmetros:**
- Nenhum.

**Retorno:**
- `Int_t`: O número do canal principal do evento agrupado.

**Exemplo de Uso:**
```cpp
PetsysGroup group(tree);
group.switchState(0);
Int_t ch = group.channel();
```

##### `void getEntry(Long64_t entry)`

Este método é um alias para `switchState(entry)`, fornecendo uma interface alternativa para mudar para uma entrada específica na árvore e carregar seus dados.

**Parâmetros:**
- `Long64_t entry`: O índice da entrada para a qual a classe deve mudar.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
PetsysGroup group(tree);
group.getEntry(10); // Mudar para a décima primeira entrada
```




### 3.7. `source/1-base/structures.C`

Este arquivo define estruturas de dados fundamentais para representar eventos individuais e suas propriedades (energia, tempo) no sistema PET. Essas estruturas são usadas para organizar os dados de forma eficiente antes do processamento ou escrita em arquivos.

#### 3.7.1. Estruturas

##### `struct TomographicSingleEvent`

Representa um único evento detectado em um sistema PET, contendo informações sobre energia e tempo para múltiplos canais. Esta estrutura é projetada para armazenar dados brutos ou pré-processados de um evento antes de ser classificado como uma coincidência ou grupo.

**Atributos:**
- `std::vector<double> energyChannels`: Vetor de valores de energia para cada canal.
- `std::vector<double> timeChannels`: Vetor de valores de tempo para cada canal.

**Construtor:**
##### `TomographicSingleEvent()`

Construtor padrão que inicializa os vetores `energyChannels` e `timeChannels` com 64 elementos, todos definidos como 0.0. Isso sugere que a estrutura é otimizada para um sistema com 64 canais por evento.

**Exemplo de Uso:**
```cpp
TomographicSingleEvent event;
// Acessar e modificar canais:
event.energyChannels[0] = 100.5;
event.timeChannels[1] = 20.3;
```

#### 3.7.2. Funções

##### `void clear(TomographicSingleEvent &event)`

Limpa os dados de energia e tempo de um `TomographicSingleEvent`, redefinindo todos os valores para 0.0.

**Parâmetros:**
- `TomographicSingleEvent &event`: A estrutura de evento a ser limpa (passada por referência).

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
TomographicSingleEvent event;
// ... preencher dados ...
clear(event);
// event.energyChannels e event.timeChannels agora contêm apenas 0.0
```

##### `void fill(TomographicSingleEvent &event, int channel, double energy, double time)`

Preenche os valores de energia e tempo para um canal específico dentro de um `TomographicSingleEvent`.

**Parâmetros:**
- `TomographicSingleEvent &event`: A estrutura de evento a ser preenchida.
- `int channel`: O índice do canal a ser atualizado.
- `double energy`: O valor de energia para o canal.
- `double time`: O valor de tempo para o canal.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
TomographicSingleEvent event;
fill(event, 5, 511.0, 10.0);
// event.energyChannels[5] será 511.0, event.timeChannels[5] será 10.0
```

##### `void FillEnergyChanelsWith(TomographicSingleEvent &event, double value)`

Preenche todos os canais de energia de um `TomographicSingleEvent` com um valor específico.

**Parâmetros:**
- `TomographicSingleEvent &event`: A estrutura de evento a ser modificada.
- `double value`: O valor com o qual os canais de energia serão preenchidos.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
TomographicSingleEvent event;
FillEnergyChanelsWith(event, 0.0);
```

##### `void FillTimeChanelsWith(TomographicSingleEvent &event, double value)`

Preenche todos os canais de tempo de um `TomographicSingleEvent` com um valor específico.

**Parâmetros:**
- `TomographicSingleEvent &event`: A estrutura de evento a ser modificada.
- `double value`: O valor com o qual os canais de tempo serão preenchidos.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
TomographicSingleEvent event;
FillTimeChanelsWith(event, 0.0);
```

##### `void copyEnergyTo(TomographicSingleEvent &destination, const TomographicSingleEvent &source)`

Copia os valores de energia de uma estrutura `TomographicSingleEvent` de origem para uma estrutura de destino.

**Parâmetros:**
- `TomographicSingleEvent &destination`: A estrutura de destino.
- `const TomographicSingleEvent &source`: A estrutura de origem.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
TomographicSingleEvent event1, event2;
fill(event1, 0, 100.0, 0.0);
copyEnergyTo(event2, event1);
// event2.energyChannels[0] será 100.0
```

##### `void copyTimeTo(TomographicSingleEvent &destination, const TomographicSingleEvent &source)`

Copia os valores de tempo de uma estrutura `TomographicSingleEvent` de origem para uma estrutura de destino.

**Parâmetros:**
- `TomographicSingleEvent &destination`: A estrutura de destino.
- `const TomographicSingleEvent &source`: A estrutura de origem.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
TomographicSingleEvent event1, event2;
fill(event1, 0, 0.0, 50.0);
copyTimeTo(event2, event1);
// event2.timeChannels[0] será 50.0
```

##### `void copyEnergyFromVector(TomographicSingleEvent &event, const std::vector<double> &energyVector)`

Copia os valores de um vetor de `double` para os canais de energia de um `TomographicSingleEvent`.

**Parâmetros:**
- `TomographicSingleEvent &event`: A estrutura de evento a ser modificada.
- `const std::vector<double> &energyVector`: O vetor de energia de origem.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
TomographicSingleEvent event;
std::vector<double> energies = {10.0, 20.0, 30.0};
copyEnergyFromVector(event, energies);
// event.energyChannels terá os valores de energies
```

##### `void copyTimeFromVector(TomographicSingleEvent &event, const std::vector<double> &timeVector)`

Copia os valores de um vetor de `double` para os canais de tempo de um `TomographicSingleEvent`.

**Parâmetros:**
- `TomographicSingleEvent &event`: A estrutura de evento a ser modificada.
- `const std::vector<double> &timeVector`: O vetor de tempo de origem.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
TomographicSingleEvent event;
std::vector<double> times = {1.0, 2.0, 3.0};
copyTimeFromVector(event, times);
// event.timeChannels terá os valores de times
```

##### `void fillEnergyFromTmatrixT(TomographicSingleEvent &event, const TMatrixT<double> &matrix)`

Preenche os canais de energia de um `TomographicSingleEvent` a partir de uma matriz `TMatrixT<double>`. A matriz é linearizada para preencher o vetor de canais.

**Parâmetros:**
- `TomographicSingleEvent &event`: A estrutura de evento a ser modificada.
- `const TMatrixT<double> &matrix`: A matriz de energia de origem.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
TomographicSingleEvent event;
TMatrixT<double> energyMatrix(8, 8);
// ... preencher energyMatrix ...
fillEnergyFromTmatrixT(event, energyMatrix);
```

##### `void fillTimeFromTmatrixT(TomographicSingleEvent &event, const TMatrixT<double> &matrix)`

Preenche os canais de tempo de um `TomographicSingleEvent` a partir de uma matriz `TMatrixT<double>`. A matriz é linearizada para preencher o vetor de canais.

**Parâmetros:**
- `TomographicSingleEvent &event`: A estrutura de evento a ser modificada.
- `const TMatrixT<double> &matrix`: A matriz de tempo de origem.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
TomographicSingleEvent event;
TMatrixT<double> timeMatrix(8, 8);
// ... preencher timeMatrix ...
fillTimeFromTmatrixT(event, timeMatrix);
```




### 3.8. `source/2-histograms/HistoMaps.C`

Este arquivo contém funções relacionadas à geração de histogramas e mapas de resposta, que são essenciais para a visualização e análise dos dados de aquisição PET.

#### 3.8.1. Funções

##### `void generateHistos(TString inputFileName, TString outputFileName)`

Gera um conjunto de histogramas a partir de um arquivo de entrada ROOT contendo dados de eventos PET e salva esses histogramas em um novo arquivo ROOT de saída. Esta função é tipicamente usada para criar mapas de energia, tempo e posicionamento para análise posterior.

**Parâmetros:**
- `TString inputFileName`: O caminho completo para o arquivo ROOT de entrada contendo os dados brutos ou processados.
- `TString outputFileName`: O caminho completo para o arquivo ROOT de saída onde os histogramas gerados serão salvos.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
generateHistos("raw_data.root", "histograms_output.root");
```




### 3.9. `source/2-histograms/SiPM.C`

Este arquivo define a classe `SiPM`, que modela um SiPM (Silicon Photomultiplier) e suas propriedades, incluindo dimensões, microcélulas, e métodos para carregar fatores de calibração e calcular a Largura Total a Meio Máximo (FWHM) de picos de energia. É fundamental para o processamento de dados de detectores.

#### 3.9.1. Classes

##### `class SiPM`

Representa um SiPM, encapsulando suas características físicas e dados de calibração. A classe gerencia matrizes de energia, tempo e canais, e fornece métodos para inicialização, calibração e análise de dados.

**Atributos:**
- `Int_t dimensions[2]`: Dimensões do SiPM (X, Y).
- `Int_t rows`: Número de linhas de microcélulas.
- `Int_t cols`: Número de colunas de microcélulas.
- `Int_t microcellNum`: Número total de microcélulas.
- `TMatrixT<double> energyMatrix`: Matriz de energia.
- `TMatrixT<double> timeMatrix`: Matriz de tempo.
- `TMatrixT<double> channelMatrix`: Matriz de canais.
- `TMatrixT<double> correctedEnergyMatrix`: Matriz de energia corrigida.
- `TMatrixT<double> positionsX`: Matriz de posições X.
- `TMatrixT<double> positionsY`: Matriz de posições Y.
- `TMatrixT<double> hEnergy`: Histograma de energia.
- `TMatrixT<double> hTime`: Histograma de tempo.
- `TMatrixT<double> hChannel`: Histograma de canais.

**Construtor:**
##### `SiPM()`

Construtor padrão da classe `SiPM`. Inicializa as dimensões e o número de microcélulas.

**Exemplo de Uso:**
```cpp
SiPM mySiPM;
```

**Métodos:**

##### `void setDimensions(Int_t x, Int_t y)`

Define as dimensões X e Y do SiPM.

**Parâmetros:**
- `Int_t x`: Dimensão X.
- `Int_t y`: Dimensão Y.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
SiPM mySiPM;
mySiPM.setDimensions(8, 8);
```

##### `void setRows(Int_t r)`

Define o número de linhas de microcélulas.

**Parâmetros:**
- `Int_t r`: Número de linhas.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
SiPM mySiPM;
mySiPM.setRows(8);
```

##### `void setCols(Int_t c)`

Define o número de colunas de microcélulas.

**Parâmetros:**
- `Int_t c`: Número de colunas.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
SiPM mySiPM;
mySiPM.setCols(8);
```

##### `void setMicrocellNum(Int_t num)`

Define o número total de microcélulas.

**Parâmetros:**
- `Int_t num`: Número de microcélulas.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
SiPM mySiPM;
mySiPM.setMicrocellNum(64);
```

##### `void setChannelNumber(Int_t channel, Int_t x, Int_t y)`

Define o número do canal para uma posição (x, y) específica na matriz de canais.

**Parâmetros:**
- `Int_t channel`: O número do canal.
- `Int_t x`: Coordenada X.
- `Int_t y`: Coordenada Y.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
SiPM mySiPM;
mySiPM.setChannelNumber(0, 0, 0);
```

##### `void setHistos(TH1F* energyHist, TH1F* timeHist, TH1F* channelHist)`

Define os histogramas de energia, tempo e canais. Esta função provavelmente associa os ponteiros dos histogramas passados como argumento aos membros da classe `SiPM`.

**Parâmetros:**
- `TH1F* energyHist`: Ponteiro para o histograma de energia.
- `TH1F* timeHist`: Ponteiro para o histograma de tempo.
- `TH1F* channelHist`: Ponteiro para o histograma de canais.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
TH1F* hE = new TH1F("hEnergy", "Energy", 100, 0, 1024);
TH1F* hT = new TH1F("hTime", "Time", 100, 0, 100);
TH1F* hC = new TH1F("hChannel", "Channel", 64, 0, 64);
SiPM mySiPM;
mySiPM.setHistos(hE, hT, hC);
```

##### `void InitializePixels()`

Inicializa as matrizes de energia, tempo, canais e posições com base nas dimensões do SiPM. Esta função deve ser chamada antes de preencher as matrizes com dados.

**Parâmetros:**
- Nenhum.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
SiPM mySiPM;
mySiPM.setDimensions(8, 8);
mySiPM.InitializePixels();
```

##### `void LoadCalibrationFactors(TString fileName)`

Carrega os fatores de calibração de um arquivo especificado para as matrizes de energia e tempo. Esta função é crucial para corrigir os dados brutos do SiPM.

**Parâmetros:**
- `TString fileName`: O caminho para o arquivo de calibração.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
SiPM mySiPM;
mySiPM.LoadCalibrationFactors("calibration_factors.root");
```

##### `double FindFWHM(TH1F* hist)`

Calcula a Largura Total a Meio Máximo (FWHM) de um pico em um histograma 1D (`TH1F`). Esta função é útil para determinar a resolução energética ou temporal.

**Parâmetros:**
- `TH1F* hist`: Ponteiro para o histograma de entrada.

**Retorno:**
- `double`: O valor do FWHM.

**Exemplo de Uso:**
```cpp
TH1F* myHist = new TH1F("h", "", 100, 0, 100);
// ... preencher myHist com dados ...
double fwhm = mySiPM.FindFWHM(myHist);
```

##### `double FindGauss(TH1F* hist)`

Realiza um ajuste gaussiano em um histograma 1D (`TH1F`) e retorna o FWHM do pico ajustado. Similar a `FindFWHM`, mas explicitamente usa um ajuste gaussiano.

**Parâmetros:**
- `TH1F* hist`: Ponteiro para o histograma de entrada.

**Retorno:**
- `double`: O valor do FWHM do ajuste gaussiano.

**Exemplo de Uso:**
```cpp
TH1F* myHist = new TH1F("h", "", 100, 0, 100);
// ... preencher myHist com dados ...
double fwhm_gauss = mySiPM.FindGauss(myHist);
```

- **Outros Getters:** A classe também fornece métodos `get` para seus atributos, como `getDimensions()`, `getRows()`, `getCols()`, `getMicrocellNum()`, `getEnergyMatrix()`, `getTimeMatrix()`, `getChannelMatrix()`, `getCorrectedEnergyMatrix()`, `getPositionsX()`, `getPositionsY()`, `getXposition(Int_t channel)`, `getYposition(Int_t channel)`.




### 3.10. `source/3-positioning/positionings.C`

Este arquivo contém funções que implementam algoritmos de posicionamento de eventos, cruciais para determinar a localização espacial de um evento de detecção dentro de um detector PET. Os algoritmos de Anger são comumente usados para este propósito.

#### 3.10.1. Funções

##### `double anger(double E1, double E2, double E3, double E4)`

Implementa o algoritmo de Anger clássico para determinar uma coordenada de posição (geralmente X ou Y) a partir de quatro sinais de energia (`E1`, `E2`, `E3`, `E4`) de um conjunto de fotomultiplicadores ou canais de detector. Este algoritmo é fundamental para a reconstrução da posição de interação de um fóton em um cristal.

**Parâmetros:**
- `double E1`: Sinal de energia do primeiro canal.
- `double E2`: Sinal de energia do segundo canal.
- `double E3`: Sinal de energia do terceiro canal.
- `double E4`: Sinal de energia do quarto canal.

**Retorno:**
- `double`: A coordenada de posição calculada. Retorna 0 se a soma total das energias for zero para evitar divisão por zero.

**Exemplo de Uso:**
```cpp
double posX = anger(100.0, 50.0, 20.0, 30.0);
```

##### `double angerNum(double E1, double E2, double E3, double E4)`

Calcula o numerador do algoritmo de Anger. Esta função é uma parte interna do cálculo de Anger e pode ser usada para depuração ou para implementações mais complexas do algoritmo.

**Parâmetros:**
- `double E1`: Sinal de energia do primeiro canal.
- `double E2`: Sinal de energia do segundo canal.
- `double E3`: Sinal de energia do terceiro canal.
- `double E4`: Sinal de energia do quarto canal.

**Retorno:**
- `double`: O valor do numerador do algoritmo de Anger.

**Exemplo de Uso:**
```cpp
double numerator = angerNum(100.0, 50.0, 20.0, 30.0);
```

##### `double angerModified(double E1, double E2, double E3, double E4)`

Implementa uma versão modificada do algoritmo de Anger. Esta modificação pode ser usada para otimizar o cálculo de posição ou para lidar com características específicas do detector que o algoritmo de Anger clássico não aborda adequadamente.

**Parâmetros:**
- `double E1`: Sinal de energia do primeiro canal.
- `double E2`: Sinal de energia do segundo canal.
- `double E3`: Sinal de energia do terceiro canal.
- `double E4`: Sinal de energia do quarto canal.

**Retorno:**
- `double`: A coordenada de posição calculada pela versão modificada do algoritmo de Anger. Retorna 0 se a soma total das energias for zero.

**Exemplo de Uso:**
```cpp
double posY = angerModified(100.0, 50.0, 20.0, 30.0);
```

##### `double applyAlpha(double x, double alpha)`

Aplica um fator de correção `alpha` a uma coordenada de posição `x`. Esta função pode ser usada para linearizar ou ajustar as coordenadas de posição obtidas pelos algoritmos de Anger, corrigindo distorções inerentes ao detector.

**Parâmetros:**
- `double x`: A coordenada de posição a ser corrigida.
- `double alpha`: O fator de correção a ser aplicado.

**Retorno:**
- `double`: A coordenada de posição corrigida.

**Exemplo de Uso:**
```cpp
double correctedX = applyAlpha(posX, 0.95);
```




### 3.11. `source/4-Writer/GimnWrite.C`

Este arquivo define a classe `GimnWriteClass`, que é responsável por escrever dados de eventos PET (pulsos, singles, coincidências e AQD - Acquisition Data) em arquivos ROOT. Ela permite a criação de árvores (`TTree`) e a definição de branches para armazenar diferentes tipos de dados, facilitando a persistência e a análise posterior.

#### 3.11.1. Classes

##### `class GimnWriteClass`

Gerencia a escrita de dados de eventos PET em um arquivo ROOT. A classe permite a criação de um `TFile` e `TTree`, e a configuração de branches para diferentes tipos de dados (pulsos, singles, coincidências, AQD).

**Atributos:**
- `TFile *f`: Ponteiro para o arquivo ROOT de saída.
- `TTree *fTree`: Ponteiro para a árvore ROOT onde os dados serão escritos.
- `TString fileName`: Nome do arquivo de saída.
- `TString treeName`: Nome da árvore a ser criada.

**Construtor:**
##### `GimnWriteClass(TString fName, TString tName)`

Construtor da classe `GimnWriteClass`. Abre um novo arquivo ROOT com o nome `fName` e cria uma nova árvore `TTree` com o nome `tName` dentro desse arquivo.

**Parâmetros:**
- `TString fName`: O nome do arquivo ROOT de saída.
- `TString tName`: O nome da árvore a ser criada.

**Exemplo de Uso:**
```cpp
GimnWriteClass writer("output_data.root", "events");
```

**Métodos:**

##### `void SetBranchesPulses()`

Configura os branches da árvore `fTree` para armazenar dados de pulsos. Isso inclui informações como energia, tempo, canal, etc. Os detalhes exatos dos branches dependem da estrutura de dados de pulsos do sistema.

**Parâmetros:**
- Nenhum.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
GimnWriteClass writer("output_data.root", "pulses");
writer.SetBranchesPulses();
```

##### `void SetBranchesSingles()`

Configura os branches da árvore `fTree` para armazenar dados de eventos singles (eventos detectados individualmente, não em coincidência). Isso inclui informações como energia, tempo, posição, etc.

**Parâmetros:**
- Nenhum.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
GimnWriteClass writer("output_data.root", "singles");
writer.SetBranchesSingles();
```

##### `void SetBranchesCoincidences()`

Configura os branches da árvore `fTree` para armazenar dados de eventos de coincidência. Isso inclui informações sobre os dois eventos que formam a coincidência, como seus canais, energias, tempos e posições.

**Parâmetros:**
- Nenhum.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
GimnWriteClass writer("output_data.root", "coincidences");
writer.SetBranchesCoincidences();
```

##### `void SetBranchesAQD()`

Configura os branches da árvore `fTree` para armazenar dados de Aquisição (AQD). Estes dados podem incluir informações de calibração, configurações do detector, ou outros metadados da aquisição.

**Parâmetros:**
- Nenhum.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
GimnWriteClass writer("output_data.root", "aqd_data");
writer.SetBranchesAQD();
```

##### `void putData()`

Preenche uma nova entrada na árvore `fTree` com os dados atuais dos buffers associados aos branches. Esta função deve ser chamada após os dados de um evento terem sido preparados nos buffers.

**Parâmetros:**
- Nenhum.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
GimnWriteClass writer("output_data.root", "events");
writer.SetBranchesPulses();
// ... preencher buffers de dados de pulsos ...
writer.putData(); // Escreve o evento na árvore
```

##### `void putDataAQD()`

Preenche uma nova entrada na árvore `fTree` com os dados atuais dos buffers associados aos branches de AQD. Esta função é específica para dados de aquisição.

**Parâmetros:**
- Nenhum.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
GimnWriteClass writer("output_data.root", "aqd_data");
writer.SetBranchesAQD();
// ... preencher buffers de dados AQD ...
writer.putDataAQD();
```

##### `void writeObject(TObject *obj)`

Escreve um objeto ROOT genérico (`TObject`) no arquivo de saída. Isso pode ser usado para salvar histogramas, gráficos, ou outros objetos ROOT que não são parte da `TTree`.

**Parâmetros:**
- `TObject *obj`: Ponteiro para o objeto ROOT a ser escrito.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
GimnWriteClass writer("output_data.root", "events");
TH1F *myHist = new TH1F("hist", "My Histogram", 100, 0, 100);
// ... preencher myHist ...
writer.writeObject(myHist);
```

##### `void writeTree()`

Escreve a árvore `fTree` no arquivo ROOT. Esta função deve ser chamada antes de fechar o arquivo para garantir que todos os dados na árvore sejam salvos.

**Parâmetros:**
- Nenhum.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
GimnWriteClass writer("output_data.root", "events");
// ... adicionar dados à árvore ...
writer.writeTree();
writer.close();
```

##### `void close()`

Fecha o arquivo ROOT de saída. É crucial chamar este método para garantir que todos os dados sejam gravados no disco e os recursos sejam liberados.

**Parâmetros:**
- Nenhum.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
GimnWriteClass writer("output_data.root", "events");
// ... escrever dados ...
writer.close();
```




### 3.12. `source/4-Writer/GimnWriteConf.C`

Este arquivo define classes para a escrita de arquivos de configuração, permitindo a persistência de parâmetros e configurações do sistema em um formato estruturado. Isso é útil para salvar calibrações, configurações de processamento ou outros metadados.

#### 3.12.1. Classes

##### `class WriteGenericConf`

Uma classe base ou auxiliar para a escrita de configurações genéricas. Ela provavelmente encapsula a lógica comum para abrir e escrever em arquivos de configuração.

**Atributos:**
- `std::ofstream file`: Stream de saída para o arquivo de configuração.
- `TString fileName`: Nome do arquivo de configuração.

**Construtor:**
##### `WriteGenericConf(TString fName)`

Construtor da classe `WriteGenericConf`. Abre um arquivo com o nome `fName` para escrita. Se o arquivo não puder ser aberto, uma mensagem de erro é exibida.

**Parâmetros:**
- `TString fName`: O nome do arquivo de configuração a ser criado ou sobrescrito.

**Exemplo de Uso:**
```cpp
WriteGenericConf confWriter("my_config.txt");
// confWriter.file agora está aberto para escrita
```

##### `class GimnWriteConf`

Especializa a escrita de configurações para o sistema GimnPET. Esta classe pode conter métodos específicos para formatar e escrever diferentes tipos de parâmetros de configuração.

**Atributos:**
- `std::ofstream file`: Stream de saída para o arquivo de configuração.
- `TString fileName`: Nome do arquivo de configuração.

**Construtor:**
##### `GimnWriteConf(TString fName)`

Construtor da classe `GimnWriteConf`. Abre um arquivo com o nome `fName` para escrita. Internamente, ele chama o construtor de `WriteGenericConf`.

**Parâmetros:**
- `TString fName`: O nome do arquivo de configuração a ser criado ou sobrescrito.

**Exemplo de Uso:**
```cpp
GimnWriteConf gimnConfWriter("gimn_parameters.conf");
```

**Métodos:**

##### `void setFileName(TString fName)`

Define o nome do arquivo de configuração. Esta função pode ser usada para alterar o arquivo de destino antes de iniciar a escrita.

**Parâmetros:**
- `TString fName`: O novo nome do arquivo.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
GimnWriteConf conf;
conf.setFileName("new_config.txt");
```

##### `void WriteConf(TString fName)`

Este método é um alias para o construtor, ou uma forma de reabrir/redefinir o arquivo de configuração. Ele abre um arquivo com o nome `fName` para escrita.

**Parâmetros:**
- `TString fName`: O nome do arquivo de configuração a ser criado ou sobrescrito.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
GimnWriteConf conf;
conf.WriteConf("another_config.txt");
```




### 3.13. `source/4-Writer/GimnWriteSimple.C`

Este arquivo define a classe `GimnWriteSimple`, uma versão simplificada da classe `GimnWriteClass` para escrita de dados de eventos PET em arquivos ROOT. Ela foca na escrita de dados de pulsos e AQD (Acquisition Data), sendo útil para cenários onde a complexidade de singles e coincidências não é necessária.

#### 3.13.1. Classes

##### `class GimnWriteSimple`

Gerencia a escrita de dados de pulsos e AQD em um arquivo ROOT de forma simplificada. Permite a criação de um `TFile` e `TTree`, e a configuração de branches para os tipos de dados suportados.

**Atributos:**
- `TFile *f`: Ponteiro para o arquivo ROOT de saída.
- `TTree *fTree`: Ponteiro para a árvore ROOT onde os dados serão escritos.
- `TString fileName`: Nome do arquivo de saída.
- `TString treeName`: Nome da árvore a ser criada.

**Construtor:**
##### `GimnWriteSimple(TString fName, TString tName)`

Construtor da classe `GimnWriteSimple`. Abre um novo arquivo ROOT com o nome `fName` e cria uma nova árvore `TTree` com o nome `tName` dentro desse arquivo.

**Parâmetros:**
- `TString fName`: O nome do arquivo ROOT de saída.
- `TString tName`: O nome da árvore a ser criada.

**Exemplo de Uso:**
```cpp
GimnWriteSimple simpleWriter("simple_output.root", "simple_events");
```

**Métodos:**

##### `void SetBranchesPulses()`

Configura os branches da árvore `fTree` para armazenar dados de pulsos. Similar à `GimnWriteClass::SetBranchesPulses()`, mas pode ter uma estrutura de branches mais simplificada.

**Parâmetros:**
- Nenhum.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
GimnWriteSimple simpleWriter("simple_output.root", "pulses");
simpleWriter.SetBranchesPulses();
```

##### `void SetBranchesAQD()`

Configura os branches da árvore `fTree` para armazenar dados de Aquisição (AQD). Similar à `GimnWriteClass::SetBranchesAQD()`, para metadados da aquisição.

**Parâmetros:**
- Nenhum.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
GimnWriteSimple simpleWriter("simple_output.root", "aqd_data");
simpleWriter.SetBranchesAQD();
```

##### `void putData()`

Preenche uma nova entrada na árvore `fTree` com os dados atuais dos buffers associados aos branches de pulsos. Esta função é usada para adicionar um evento de pulso à árvore.

**Parâmetros:**
- Nenhum.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
GimnWriteSimple simpleWriter("simple_output.root", "pulses");
simpleWriter.SetBranchesPulses();
// ... preencher buffers de dados de pulsos ...
simpleWriter.putData(); // Escreve o evento de pulso na árvore
```

##### `void putDataAQD()`

Preenche uma nova entrada na árvore `fTree` com os dados atuais dos buffers associados aos branches de AQD. Esta função é específica para dados de aquisição.

**Parâmetros:**
- Nenhum.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
GimnWriteSimple simpleWriter("simple_output.root", "aqd_data");
simpleWriter.SetBranchesAQD();
// ... preencher buffers de dados AQD ...
simpleWriter.putDataAQD();
```

##### `void writeObject(TObject *obj)`

Escreve um objeto ROOT genérico (`TObject`) no arquivo de saída. Permite salvar objetos como histogramas ou gráficos que não fazem parte da `TTree`.

**Parâmetros:**
- `TObject *obj`: Ponteiro para o objeto ROOT a ser escrito.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
GimnWriteSimple simpleWriter("simple_output.root", "events");
TH1F *myHist = new TH1F("hist", "My Simple Histogram", 50, 0, 50);
// ... preencher myHist ...
simpleWriter.writeObject(myHist);
```

##### `void close()`

Fecha o arquivo ROOT de saída. Essencial para garantir que todos os dados sejam gravados e os recursos liberados.

**Parâmetros:**
- Nenhum.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
GimnWriteSimple simpleWriter("simple_output.root", "pulses");
// ... escrever dados ...
simpleWriter.close();
```




### 3.14. `source/4-Writer/GimnWriteTomo.C`

Este arquivo define a classe `GimnWriteTomo`, especializada na escrita de dados de eventos tomográficos (singles e coincidências) em arquivos ROOT. Ela é projetada para lidar com a estrutura de dados específica necessária para a reconstrução tomográfica.

#### 3.14.1. Classes

##### `class GimnWriteTomo`

Gerencia a escrita de dados de eventos singles e de coincidência tomográficos em um arquivo ROOT. A classe permite a criação de um `TFile` e `TTree`, e a configuração de branches para os tipos de dados tomográficos.

**Atributos:**
- `TFile *f`: Ponteiro para o arquivo ROOT de saída.
- `TTree *fTree`: Ponteiro para a árvore ROOT onde os dados serão escritos.
- `TString fileName`: Nome do arquivo de saída.
- `TString treeName`: Nome da árvore a ser criada.

**Construtor:**
##### `GimnWriteTomo(TString fName, TString tName)`

Construtor da classe `GimnWriteTomo`. Abre um novo arquivo ROOT com o nome `fName` e cria uma nova árvore `TTree` com o nome `tName` dentro desse arquivo.

**Parâmetros:**
- `TString fName`: O nome do arquivo ROOT de saída.
- `TString tName`: O nome da árvore a ser criada.

**Exemplo de Uso:**
```cpp
GimnWriteTomo tomoWriter("tomo_data.root", "tomo_events");
```

**Métodos:**

##### `void SetBranchesSinglesTomo()`

Configura os branches da árvore `fTree` para armazenar dados de eventos singles tomográficos. Isso inclui informações como energia, tempo, e posições 3D dos eventos.

**Parâmetros:**
- Nenhum.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
GimnWriteTomo tomoWriter("tomo_data.root", "tomo_singles");
tomoWriter.SetBranchesSinglesTomo();
```

##### `void SetBranchesCoincidencesTomo()`

Configura os branches da árvore `fTree` para armazenar dados de eventos de coincidência tomográficos. Isso inclui informações sobre os dois eventos que formam a coincidência, suas posições 3D, e outros parâmetros relevantes para a reconstrução tomográfica.

**Parâmetros:**
- Nenhum.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
GimnWriteTomo tomoWriter("tomo_data.root", "tomo_coincidences");
tomoWriter.SetBranchesCoincidencesTomo();
```

##### `void putData()`

Preenche uma nova entrada na árvore `fTree` com os dados atuais dos buffers associados aos branches (sejam singles ou coincidências tomográficas). Esta função é usada para adicionar um evento tomográfico à árvore.

**Parâmetros:**
- Nenhum.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
GimnWriteTomo tomoWriter("tomo_data.root", "tomo_events");
tomoWriter.SetBranchesSinglesTomo();
// ... preencher buffers de dados de singles tomográficos ...
tomoWriter.putData(); // Escreve o evento na árvore
```

##### `void writeObject(TObject *obj)`

Escreve um objeto ROOT genérico (`TObject`) no arquivo de saída. Permite salvar objetos como histogramas ou gráficos que não fazem parte da `TTree`.

**Parâmetros:**
- `TObject *obj`: Ponteiro para o objeto ROOT a ser escrito.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
GimnWriteTomo tomoWriter("tomo_data.root", "tomo_events");
TH2F *myTomoHist = new TH2F("tomo_hist", "Tomographic Histogram", 100, 0, 100, 100, 0, 100);
// ... preencher myTomoHist ...
tomoWriter.writeObject(myTomoHist);
```

##### `void close()`

Fecha o arquivo ROOT de saída. Essencial para garantir que todos os dados sejam gravados e os recursos liberados.

**Parâmetros:**
- Nenhum.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
GimnWriteTomo tomoWriter("tomo_data.root", "tomo_events");
// ... escrever dados ...
tomoWriter.close();
```




### 3.15. `source/5-timing/timing.C`

Este arquivo contém funções relacionadas ao processamento de informações temporais de eventos PET. O tempo de chegada dos fótons é crucial para a determinação da Linha de Resposta (LOR) em coincidências e para a correção de tempo de voo (TOF).

#### 3.15.1. Funções

##### `double tea(double t1, double t2)`

Calcula a diferença de tempo entre dois eventos (`t1` e `t2`). Esta função é fundamental para a análise de coincidências e para a determinação do tempo de voo.

**Parâmetros:**
- `double t1`: Tempo do primeiro evento.
- `double t2`: Tempo do segundo evento.

**Retorno:**
- `double`: A diferença de tempo (`t1 - t2`).

**Exemplo de Uso:**
```cpp
double time_diff = tea(100.5, 90.2); // time_diff será 10.3
```

##### `double tsa(double t1, double t2)`

Calcula a soma de tempo entre dois eventos (`t1` e `t2`). Embora menos comum que a diferença de tempo em PET, a soma de tempo pode ser relevante em certas análises ou para fins de sincronização.

**Parâmetros:**
- `double t1`: Tempo do primeiro evento.
- `double t2`: Tempo do segundo evento.

**Retorno:**
- `double`: A soma dos tempos (`t1 + t2`).

**Exemplo de Uso:**
```cpp
double time_sum = tsa(100.5, 90.2); // time_sum será 190.7
```




### 3.16. `source/6-Calibration/calibrate.C`

Este arquivo contém funções essenciais para a calibração espacial e energética dos detectores PET. A calibração é um passo crítico para garantir a precisão das medições de energia e posição, corrigindo não-linearidades e variações entre os canais do detector.

#### 3.16.1. Funções

##### `std::map<std::string, double> calibrate(TString inputFileName, TString outputFileName)`

Realiza o processo de calibração completo para os dados de um arquivo de entrada. Esta função orquestra diversas etapas de calibração, como correção de saturação, linearização e ajuste de picos, salvando os resultados em um arquivo de saída.

**Parâmetros:**
- `TString inputFileName`: O caminho para o arquivo ROOT de entrada contendo os dados brutos a serem calibrados.
- `TString outputFileName`: O caminho para o arquivo ROOT de saída onde os dados calibrados e os parâmetros de calibração serão salvos.

**Retorno:**
- `std::map<std::string, double>`: Um mapa contendo métricas ou resultados sumarizados da calibração (ex: FWHM, qualidade do flood).

**Exemplo de Uso:**
```cpp
std::map<std::string, double> cal_results = calibrate("raw_data.root", "calibrated_data.root");
```

##### `std::vector<double> FindAndMarkPeaks(TH1F* hist, int nPeaks, double sigma, double threshold)`

Detecta e marca picos em um histograma 1D (`TH1F`) usando o algoritmo `TSpectrum` do ROOT. Esta função é comumente usada para identificar os picos de energia (ex: 511 keV) ou picos de posicionamento em histogramas.

**Parâmetros:**
- `TH1F* hist`: Ponteiro para o histograma de entrada.
- `int nPeaks`: O número máximo de picos a serem procurados.
- `double sigma`: A largura esperada dos picos (em unidades de bins do histograma).
- `double threshold`: Limiar para a detecção de picos (fração da altura máxima do histograma).

**Retorno:**
- `std::vector<double>`: Um vetor contendo as posições (valores do eixo X) dos picos detectados.

**Exemplo de Uso:**
```cpp
TH1F* energyHist = new TH1F("hEnergy", "Energy Spectrum", 1000, 0, 2000);
// ... preencher energyHist ...
std::vector<double> peakPositions = FindAndMarkPeaks(energyHist, 2, 2.0, 0.05);
```

##### `std::vector<double> FindAndMarkPeaksTot(TH1F* hist, int nPeaks, double sigma, double threshold)`

Similar a `FindAndMarkPeaks`, mas especificamente para histogramas de TOT (Time Over Threshold). Pode ter parâmetros ou lógica ligeiramente diferentes para otimização com dados de TOT.

**Parâmetros:**
- `TH1F* hist`: Ponteiro para o histograma de entrada (TOT).
- `int nPeaks`: O número máximo de picos a serem procurados.
- `double sigma`: A largura esperada dos picos.
- `double threshold`: Limiar para a detecção de picos.

**Retorno:**
- `std::vector<double>`: Um vetor contendo as posições (valores do eixo X) dos picos detectados.

**Exemplo de Uso:**
```cpp
TH1F* totHist = new TH1F("hTot", "TOT Spectrum", 500, 0, 1000);
// ... preencher totHist ...
std::vector<double> totPeakPositions = FindAndMarkPeaksTot(totHist, 1, 1.5, 0.1);
```

##### `void SaturationCorrectionNadig(TH1F* hist)`

Aplica uma correção de saturação ao histograma de energia usando o modelo de Nadig. A saturação ocorre em detectores quando a taxa de contagem é muito alta, levando a uma subestimação da energia. Esta função tenta corrigir esse efeito.

**Parâmetros:**
- `TH1F* hist`: Ponteiro para o histograma de energia a ser corrigido.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
TH1F* rawEnergyHist = new TH1F("hRawEnergy", "Raw Energy", 1000, 0, 2000);
// ... preencher rawEnergyHist ...
SaturationCorrectionNadig(rawEnergyHist);
```

##### `void FindLinearizationPETSYS(TH2F* hist, TString outputFileName)`

Encontra os parâmetros de linearização para um detector PETsys a partir de um histograma 2D (`TH2F`) de energia vs. canal. A linearização corrige a resposta não-linear dos canais do detector, garantindo que a energia medida seja proporcional à energia real.

**Parâmetros:**
- `TH2F* hist`: Ponteiro para o histograma 2D de energia vs. canal.
- `TString outputFileName`: O nome do arquivo onde os parâmetros de linearização serão salvos.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
TH2F* energyChannelHist = new TH2F("hEnergyChannel", "Energy vs Channel", 64, 0, 64, 1000, 0, 2000);
// ... preencher energyChannelHist ...
FindLinearizationPETSYS(energyChannelHist, "linearization_params.root");
```

##### `void polinomialCorrection(TH1F* hist, double p0, double p1, double p2)`

Aplica uma correção polinomial de segundo grau a um histograma 1D (`TH1F`). Esta função pode ser usada para corrigir não-linearidades na resposta de energia ou tempo.

**Parâmetros:**
- `TH1F* hist`: Ponteiro para o histograma a ser corrigido.
- `double p0`: Coeficiente constante do polinômio.
- `double p1`: Coeficiente linear do polinômio.
- `double p2`: Coeficiente quadrático do polinômio.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
TH1F* inputHist = new TH1F("hInput", "Input Data", 100, 0, 100);
// ... preencher inputHist ...
polinomialCorrection(inputHist, 0.0, 1.0, -0.001);
```

##### `std::vector<double> getKeysFromResult(const std::map<double, double>& resultMap)`

Extrai as chaves (keys) de um `std::map<double, double>` e as retorna como um `std::vector<double>`. Útil para processar os resultados de funções que retornam mapas.

**Parâmetros:**
- `const std::map<double, double>& resultMap`: O mapa de onde as chaves serão extraídas.

**Retorno:**
- `std::vector<double>`: Um vetor contendo todas as chaves do mapa.

**Exemplo de Uso:**
```cpp
std::map<double, double> myMap = {{1.0, 10.0}, {2.0, 20.0}};
std::vector<double> keys = getKeysFromResult(myMap);
// keys será {1.0, 2.0}
```

##### `std::vector<double> getValuesFromResult(const std::map<double, double>& resultMap)`

Extrai os valores (values) de um `std::map<double, double>` e os retorna como um `std::vector<double>`. Útil para processar os resultados de funções que retornam mapas.

**Parâmetros:**
- `const std::map<double, double>& resultMap`: O mapa de onde os valores serão extraídos.

**Retorno:**
- `std::vector<double>`: Um vetor contendo todos os valores do mapa.

**Exemplo de Uso:**
```cpp
std::map<double, double> myMap = {{1.0, 10.0}, {2.0, 20.0}};
std::vector<double> values = getValuesFromResult(myMap);
// values será {10.0, 20.0}
```

##### `TGraph* GetMapHistosRoot(TString fileName, TString histoName)`

Carrega um histograma 2D (`TH2F`) de um arquivo ROOT e o converte em um `TGraph` (gráfico de pontos). Esta função é útil para visualizar dados de mapas de resposta ou calibração como pontos em um gráfico.

**Parâmetros:**
- `TString fileName`: O caminho para o arquivo ROOT contendo o histograma.
- `TString histoName`: O nome do histograma 2D dentro do arquivo.

**Retorno:**
- `TGraph*`: Um ponteiro para o `TGraph` criado, ou `nullptr` se o arquivo ou histograma não puder ser encontrado.

**Exemplo de Uso:**
```cpp
TGraph* myGraph = GetMapHistosRoot("calibration_maps.root", "hEnergyMap");
if (myGraph) {
    myGraph->Draw("AP");
}
```




### 3.17. `source/6-Calibration/findXY.C`

Este arquivo contém a função `FindXY`, que é utilizada para determinar as coordenadas X e Y de um evento em um detector, geralmente a partir de dados de energia de múltiplos canais. Esta função é crucial para o mapeamento da resposta do detector e para a calibração espacial.

#### 3.17.1. Funções

##### `void FindXY(TH2F* hist, TH1F* hEnergy, TH1F* hTime, TH1F* hChannel, TH2F* hPosition, TH2F* hPositionEnergy, TH2F* hTimePosition, TH2F* hTimeEnergy)`

Processa os dados de eventos para determinar as coordenadas X e Y de cada evento e preenche os histogramas de saída correspondentes. Esta função integra informações de energia, tempo e canal para gerar mapas de posição e suas correlações com energia e tempo.

**Parâmetros:**
- `TH2F* hist`: Histograma 2D geral (pode ser um histograma de flood ou similar).
- `TH1F* hEnergy`: Histograma 1D de energia.
- `TH1F* hTime`: Histograma 1D de tempo.
- `TH1F* hChannel`: Histograma 1D de canais.
- `TH2F* hPosition`: Histograma 2D de posição (X vs Y).
- `TH2F* hPositionEnergy`: Histograma 2D de posição vs. energia.
- `TH2F* hTimePosition`: Histograma 2D de tempo vs. posição.
- `TH2F* hTimeEnergy`: Histograma 2D de tempo vs. energia.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
// Supondo que todos os ponteiros de histogramas foram inicializados
// e que os dados brutos foram carregados.
FindXY(hFlood, hEnergy, hTime, hChannel, hPosition, hPositionEnergy, hTimePosition, hTimeEnergy);
```




### 3.18. `source/7-process/groupPreCal.C`

Este arquivo contém a função `groupPreCal`, que é parte do pipeline de pré-calibração e processamento de dados de grupo. Esta função provavelmente agrupa eventos brutos e aplica algumas etapas iniciais de processamento antes da calibração completa.

#### 3.18.1. Funções

##### `void groupPreCal(TString inputFileName, TString outputFileName)`

Processa um arquivo de entrada contendo dados brutos de eventos e gera um arquivo de saída com eventos agrupados e pré-calibrados. Esta função é um passo intermediário no fluxo de trabalho de processamento de dados, preparando os dados para calibrações mais avançadas.

**Parâmetros:**
- `TString inputFileName`: O caminho para o arquivo ROOT de entrada contendo os dados brutos.
- `TString outputFileName`: O caminho para o arquivo ROOT de saída onde os dados agrupados e pré-calibrados serão salvos.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
groupPreCal("raw_events.root", "precal_groups.root");
```




### 3.19. `source/7-process/HistoFinder.C`

Este arquivo contém a função `GetHistosFromCalibrated`, que é responsável por extrair e gerar histogramas a partir de dados já calibrados. Isso permite a análise e visualização dos dados após as correções de calibração terem sido aplicadas.

#### 3.19.1. Funções

##### `void GetHistosFromCalibrated(TString inputFileName, TString outputFileName)`

Lê um arquivo ROOT contendo dados de eventos PET já calibrados e gera um conjunto de histogramas (energia, tempo, posição, etc.) a partir desses dados. Os histogramas resultantes são salvos em um novo arquivo ROOT de saída.

**Parâmetros:**
- `TString inputFileName`: O caminho para o arquivo ROOT de entrada contendo os dados calibrados.
- `TString outputFileName`: O caminho para o arquivo ROOT de saída onde os histogramas gerados serão salvos.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
GetHistosFromCalibrated("calibrated_data.root", "calibrated_histograms.root");
```




### 3.20. `source/7-process/multiThreader.C`

Este arquivo define a classe `MultiThreaderGimn`, que é projetada para gerenciar a execução de tarefas em múltiplos threads. Isso é crucial para otimizar o desempenho de operações intensivas em termos de computação, como o processamento massivo de dados PET, aproveitando os recursos de processadores multi-core.

#### 3.20.1. Classes

##### `class MultiThreaderGimn`

Gerencia um pool de threads para executar tarefas em paralelo. A classe pode ser configurada com um número máximo de threads e fornece mecanismos para aguardar a conclusão das tarefas.

**Atributos:**
- `std::vector<std::thread> threads`: Um vetor para armazenar os objetos `std::thread`.
- `int maxThreads_`: O número máximo de threads que podem ser executados simultaneamente.

**Construtor:**
##### `MultiThreaderGimn(int maxThreads)`

Construtor da classe `MultiThreaderGimn`. Inicializa o `MultiThreaderGimn` com o número máximo de threads permitido.

**Parâmetros:**
- `int maxThreads`: O número máximo de threads que esta instância do `MultiThreaderGimn` pode gerenciar.

**Exemplo de Uso:**
```cpp
MultiThreaderGimn threadPool(4); // Cria um pool de threads com no máximo 4 threads
```

**Métodos:**

##### `void waitForCompletion()`

Aguarda a conclusão de todas as threads que foram iniciadas e estão atualmente ativas no pool. Esta função é tipicamente chamada após o lançamento de várias tarefas para garantir que todas tenham terminado antes de prosseguir.

**Parâmetros:**
- Nenhum.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
MultiThreaderGimn threadPool(4);
// ... adicionar tarefas aos threads ...
threadPool.waitForCompletion(); // Espera todas as tarefas terminarem
```

##### `int maxThreads()`

Retorna o número máximo de threads que esta instância do `MultiThreaderGimn` pode gerenciar.

**Parâmetros:**
- Nenhum.

**Retorno:**
- `int`: O número máximo de threads configurado.

**Exemplo de Uso:**
```cpp
MultiThreaderGimn threadPool(8);
int numThreads = threadPool.maxThreads(); // numThreads será 8
```




### 3.21. `source/7-process/ProcessCoincidencePreCal.C`

Este arquivo contém a função `processCoincidencePreCal`, que é responsável pelo pré-processamento de dados de coincidência. Esta etapa é crucial para filtrar e preparar os eventos de coincidência antes de calibrações mais complexas ou reconstrução tomográfica.

#### 3.21.1. Funções

##### `void processCoincidencePreCal(TString inputFileName, TString outputFileName)`

Processa um arquivo de entrada contendo dados brutos de coincidência e gera um arquivo de saída com os eventos de coincidência pré-calibrados. Esta função pode incluir etapas como filtragem de ruído, seleção de eventos válidos e organização dos dados para as próximas fases do pipeline de processamento.

**Parâmetros:**
- `TString inputFileName`: O caminho para o arquivo ROOT de entrada contendo os dados brutos de coincidência.
- `TString outputFileName`: O caminho para o arquivo ROOT de saída onde os dados de coincidência pré-calibrados serão salvos.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
processCoincidencePreCal("raw_coincidences.root", "precal_coincidences.root");
```




### 3.22. `source/7-process/processGroupPre.C`

Este arquivo contém a função `processGroupPre`, que é responsável pelo pré-processamento de dados de grupo. Similar ao processamento de coincidências, esta função prepara os eventos agrupados para etapas subsequentes de calibração e análise.

#### 3.22.1. Funções

##### `void processGroupPre(TString inputFileName, TString outputFileName)`

Processa um arquivo de entrada contendo dados brutos de grupo e gera um arquivo de saída com os eventos de grupo pré-processados. Esta função pode envolver a aplicação de filtros básicos, a organização dos dados e a preparação para calibrações mais detalhadas.

**Parâmetros:**
- `TString inputFileName`: O caminho para o arquivo ROOT de entrada contendo os dados brutos de grupo.
- `TString outputFileName`: O caminho para o arquivo ROOT de saída onde os dados de grupo pré-processados serão salvos.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
processGroupPre("raw_groups.root", "preprocessed_groups.root");
```




### 3.23. `source/7-process/ProcessPulsesCoincidence.C`

Este arquivo contém a função `ProcessPulsesCoincidences`, que é responsável por processar pulsos individuais e identificar eventos de coincidência. Esta é uma etapa fundamental na pipeline de processamento de dados PET, onde os pulsos brutos são correlacionados no tempo para formar pares de coincidência.

#### 3.23.1. Funções

##### `void ProcessPulsesCoincidences(TString inputFileName, TString outputFileName)`

Lê um arquivo de entrada contendo dados de pulsos brutos, processa esses pulsos para identificar coincidências e salva os eventos de coincidência resultantes em um arquivo de saída. Esta função aplica critérios de tempo e, possivelmente, de energia para determinar se dois pulsos constituem uma coincidência válida.

**Parâmetros:**
- `TString inputFileName`: O caminho para o arquivo ROOT de entrada contendo os dados de pulsos brutos.
- `TString outputFileName`: O caminho para o arquivo ROOT de saída onde os eventos de coincidência serão salvos.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
ProcessPulsesCoincidences("raw_pulses.root", "coincidences.root");
```




### 3.24. `source/7-process/ProcessPulsesGroup.C`

Este arquivo contém a função `ProcessPulses`, que é responsável por processar pulsos individuais e agrupá-los. O agrupamento de pulsos é uma etapa importante para consolidar eventos que ocorrem em detectores adjacentes ou em um curto período de tempo, formando um único evento de grupo.

#### 3.24.1. Funções

##### `void ProcessPulses(TString inputFileName, TString outputFileName)`

Lê um arquivo de entrada contendo dados de pulsos brutos, processa esses pulsos para formar grupos e salva os eventos de grupo resultantes em um arquivo de saída. Esta função aplica critérios espaciais e/ou temporais para agrupar pulsos relacionados.

**Parâmetros:**
- `TString inputFileName`: O caminho para o arquivo ROOT de entrada contendo os dados de pulsos brutos.
- `TString outputFileName`: O caminho para o arquivo ROOT de saída onde os eventos de grupo serão salvos.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
ProcessPulses("raw_pulses.root", "grouped_events.root");
```




### 3.25. `source/7-process/ProssesSingleGroup.C`

Este arquivo contém a função `ProcessPulsesSingle`, que é responsável por processar pulsos individuais e classificá-los como eventos 


singles (eventos que não formam coincidências ou grupos). Esta função é importante para a análise de eventos que não se enquadram nos critérios de coincidência ou agrupamento.

#### 3.25.1. Funções

##### `void ProcessPulsesSingle(TString inputFileName, TString outputFileName)`

Lê um arquivo de entrada contendo dados de pulsos brutos, processa esses pulsos e salva os eventos singles resultantes em um arquivo de saída. Esta função pode ser usada para isolar eventos que não foram correlacionados com outros pulsos, ou para analisar a taxa de eventos singles.

**Parâmetros:**
- `TString inputFileName`: O caminho para o arquivo ROOT de entrada contendo os dados de pulsos brutos.
- `TString outputFileName`: O caminho para o arquivo ROOT de saída onde os eventos singles serão salvos.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
ProcessPulsesSingle("raw_pulses.root", "singles_events.root");
```




### 3.26. `source/8-analyze/analyze.C`

Este arquivo contém a função `analyze`, que é utilizada para realizar análises de desempenho e qualidade dos dados processados. Esta função pode gerar gráficos, calcular métricas e fornecer insights sobre a qualidade da aquisição e do processamento.

#### 3.26.1. Funções

##### `void analyze(TString inputFileName, TString outputFileName)`

Realiza uma análise abrangente dos dados contidos no `inputFileName` e salva os resultados da análise (como histogramas, gráficos ou relatórios) no `outputFileName`. Esta função é um ponto de entrada para a avaliação da qualidade dos dados PET.

**Parâmetros:**
- `TString inputFileName`: O caminho para o arquivo ROOT de entrada contendo os dados a serem analisados.
- `TString outputFileName`: O caminho para o arquivo ROOT de saída onde os resultados da análise serão salvos.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
analyze("processed_data.root", "analysis_results.root");
```




### 3.27. `source/8-analyze/analyzetime.C`

Este arquivo contém a função `analyzeTime`, que é especializada na análise de informações temporais de eventos PET. A análise de tempo é crucial para avaliar a resolução temporal do sistema e para a implementação de técnicas como o Time-of-Flight (TOF).

#### 3.27.1. Funções

##### `void analyzeTime(TString inputFileName, TString outputFileName)`

Realiza uma análise detalhada dos dados temporais contidos no `inputFileName` e salva os resultados da análise (como histogramas de diferença de tempo, FWHM temporal, etc.) no `outputFileName`. Esta função é essencial para caracterizar o desempenho temporal do sistema PET.

**Parâmetros:**
- `TString inputFileName`: O caminho para o arquivo ROOT de entrada contendo os dados a serem analisados temporalmente.
- `TString outputFileName`: O caminho para o arquivo ROOT de saída onde os resultados da análise temporal serão salvos.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
analyzeTime("coincidences.root", "time_analysis_results.root");
```




### 3.28. `source/9-massiveProcessing/massiveAnalyze.C`

Este arquivo contém a função `massiveAnalyze`, que é projetada para realizar análises em larga escala de múltiplos arquivos de dados. Esta função é útil para processar grandes volumes de dados de forma automatizada, como em estudos de desempenho de longo prazo ou em análises de grandes campanhas de aquisição.

#### 3.28.1. Funções

##### `void massiveAnalyze(TString inputFolder, TString outputFolder)`

Percorre um diretório de entrada (`inputFolder`), identifica arquivos de dados e aplica a função de análise (`analyze`) a cada um deles. Os resultados de cada análise são salvos em um diretório de saída (`outputFolder`), mantendo a organização dos dados.

**Parâmetros:**
- `TString inputFolder`: O caminho para o diretório contendo os arquivos de dados a serem analisados.
- `TString outputFolder`: O caminho para o diretório onde os resultados da análise massiva serão salvos.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
massiveAnalyze("/data/runs_to_analyze", "/results/massive_analysis");
```




### 3.29. `source/9-massiveProcessing/massiveAnalyzeTime.C`

Este arquivo contém a função `massiveTimeAnalyze`, que é uma extensão da análise massiva, focada especificamente na análise temporal de grandes conjuntos de dados. Ela permite a avaliação da resolução temporal e de outros parâmetros de tempo em múltiplos arquivos de forma automatizada.

#### 3.29.1. Funções

##### `void massiveTimeAnalyze(TString inputFolder, TString outputFolder)`

Percorre um diretório de entrada (`inputFolder`), identifica arquivos de dados e aplica a função de análise temporal (`analyzeTime`) a cada um deles. Os resultados de cada análise temporal são salvos em um diretório de saída (`outputFolder`), mantendo a organização dos dados.

**Parâmetros:**
- `TString inputFolder`: O caminho para o diretório contendo os arquivos de dados a serem analisados temporalmente.
- `TString outputFolder`: O caminho para o diretório onde os resultados da análise temporal massiva serão salvos.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
massiveTimeAnalyze("/data/time_data_runs", "/results/massive_time_analysis");
```




### 3.30. `source/9-massiveProcessing/massiveCalibration.C`

Este arquivo contém a função `massiveCalibration`, que é projetada para realizar a calibração de múltiplos arquivos de dados de forma massiva. Esta função é essencial para processar grandes volumes de dados de aquisição, aplicando as correções de calibração necessárias de maneira automatizada.

#### 3.30.1. Funções

##### `void massiveCalibration(TString inputFolder, TString outputFolder)`

Percorre um diretório de entrada (`inputFolder`), identifica arquivos de dados brutos e aplica o processo de calibração (`calibrate`) a cada um deles. Os arquivos de dados calibrados são salvos em um diretório de saída (`outputFolder`), garantindo que todos os dados sejam corrigidos de forma consistente.

**Parâmetros:**
- `TString inputFolder`: O caminho para o diretório contendo os arquivos de dados brutos a serem calibrados.
- `TString outputFolder`: O caminho para o diretório onde os arquivos de dados calibrados serão salvos.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
massiveCalibration("/data/raw_acquisitions", "/data/calibrated_acquisitions");
```




### 3.31. `source/9-massiveProcessing/massiveCalibrationPositioning.C`

Este arquivo contém a função `massiveCalibrationPositioning`, que é uma especialização da calibração massiva, focada na calibração de posicionamento. Ela é utilizada para aplicar correções de posicionamento a grandes conjuntos de dados, garantindo a precisão espacial dos eventos detectados.

#### 3.31.1. Funções

##### `void massiveCalibrationPositioning(TString inputFolder, TString outputFolder)`

Percorre um diretório de entrada (`inputFolder`), identifica arquivos de dados e aplica o processo de calibração de posicionamento a cada um deles. Os arquivos de dados com posicionamento calibrado são salvos em um diretório de saída (`outputFolder`).

**Parâmetros:**
- `TString inputFolder`: O caminho para o diretório contendo os arquivos de dados a serem calibrados em posicionamento.
- `TString outputFolder`: O caminho para o diretório onde os arquivos de dados com posicionamento calibrado serão salvos.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
massiveCalibrationPositioning("/data/raw_positions", "/data/calibrated_positions");
```




### 3.32. `source/9-massiveProcessing/massivePreCalibration.C`

Este arquivo contém a função `massivePreCalibration`, que é responsável por realizar o pré-calibração em massa de dados. Esta etapa é um passo inicial no pipeline de calibração, aplicando correções básicas antes de calibrações mais complexas ou refinadas.

#### 3.32.1. Funções

##### `void massivePreCalibration(TString inputFolder, TString outputFolder)`

Percorre um diretório de entrada (`inputFolder`), identifica arquivos de dados brutos e aplica o processo de pré-calibração a cada um deles. Os arquivos de dados pré-calibrados são salvos em um diretório de saída (`outputFolder`).

**Parâmetros:**
- `TString inputFolder`: O caminho para o diretório contendo os arquivos de dados brutos a serem pré-calibrados.
- `TString outputFolder`: O caminho para o diretório onde os arquivos de dados pré-calibrados serão salvos.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
massivePreCalibration("/data/raw_data", "/data/pre_calibrated_data");
```




### 3.33. `source/9-massiveProcessing/massivePreProcessing.C`

Este arquivo contém a função `massivePreProcess`, que é responsável pelo pré-processamento massivo de dados. Esta função é fundamental para preparar grandes volumes de dados brutos para as etapas subsequentes de calibração e análise, realizando operações como filtragem, organização e formatação.

#### 3.33.1. Funções

##### `void massivePreProcess(TString inputFolder, TString outputFolder)`

Percorre um diretório de entrada (`inputFolder`), identifica arquivos de dados brutos e aplica o processo de pré-processamento a cada um deles. Os arquivos de dados pré-processados são salvos em um diretório de saída (`outputFolder`).

**Parâmetros:**
- `TString inputFolder`: O caminho para o diretório contendo os arquivos de dados brutos a serem pré-processados.
- `TString outputFolder`: O caminho para o diretório onde os arquivos de dados pré-processados serão salvos.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
massivePreProcess("/data/raw_acquisitions", "/data/pre_processed_acquisitions");
```




### 3.34. `source/9-massiveProcessing/massiveTimeCoincidences.C`

Este arquivo contém a função `massiveTimeCoincidences`, que é responsável por processar dados de coincidência em massa, com foco em informações temporais. Esta função é crucial para a análise de grandes volumes de dados de coincidência, permitindo a avaliação da resolução temporal e a aplicação de correções de tempo de voo (TOF).

#### 3.34.1. Funções

##### `void massiveTimeCoincidences(TString inputFolder, TString outputFolder)`

Percorre um diretório de entrada (`inputFolder`), identifica arquivos de dados de coincidência e aplica o processamento temporal a cada um deles. Os arquivos de dados de coincidência processados temporalmente são salvos em um diretório de saída (`outputFolder`).

**Parâmetros:**
- `TString inputFolder`: O caminho para o diretório contendo os arquivos de dados de coincidência a serem processados temporalmente.
- `TString outputFolder`: O caminho para o diretório onde os arquivos de dados de coincidência processados temporalmente serão salvos.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
massiveTimeCoincidences("/data/coincidence_runs", "/data/time_corrected_coincidences");
```




### 3.35. `source/91-sinogram/GantryPositions.C`

Este arquivo define a classe `GimnPET`, que é fundamental para o gerenciamento das posições do gantry e dos detectores em um sistema PET. Ela lida com a geometria do scanner, incluindo rotações, translações e o mapeamento de posições globais para as coordenadas do detector, sendo crucial para a geração de sinogramas.

#### 3.35.1. Classes

##### `class GimnPET`

Representa a geometria de um scanner PET, incluindo a configuração do gantry, a posição dos cristais e a capacidade de calcular posições globais de eventos. É essencial para a reconstrução tomográfica e a geração de sinogramas.

**Atributos:**
- `TString identificatorFileName`: Nome do arquivo de identificadores de detectores.
- `double crystalDimension`: Dimensão do cristal.
- `double gantryRadius`: Raio do gantry.
- `double plaThickness`: Espessura do PLA (Position-Sensitive Light Absorber).
- `double globalCenterX`: Coordenada X do centro global do gantry.
- `double globalCenterY`: Coordenada Y do centro global do gantry.
- `double globalCenterZ`: Coordenada Z do centro global do gantry.
- `std::vector<Identificator> identificators`: Vetor de objetos `Identificator` para todos os detectores.

**Construtor:**
##### `GimnPET()`

Construtor padrão da classe `GimnPET`. Inicializa os atributos com valores padrão ou nulos.

**Exemplo de Uso:**
```cpp
GimnPET petScanner;
```

**Métodos:**

##### `void setCrystalDimension(double dim)`

Define a dimensão do cristal do detector.

**Parâmetros:**
- `double dim`: A dimensão do cristal.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
GimnPET petScanner;
petScanner.setCrystalDimension(4.0); // 4mm
```

##### `void setGantryRadius(double radius)`

Define o raio do gantry do scanner PET.

**Parâmetros:**
- `double radius`: O raio do gantry.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
GimnPET petScanner;
petScanner.setGantryRadius(400.0); // 400mm
```

##### `void setPLAThickness(double thickness)`

Define a espessura do PLA (Position-Sensitive Light Absorber).

**Parâmetros:**
- `double thickness`: A espessura do PLA.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
GimnPET petScanner;
petScanner.setPLAThickness(1.0); // 1mm
```

##### `void setGlobalCenter(double x, double y, double z)`

Define as coordenadas do centro global do gantry.

**Parâmetros:**
- `double x`: Coordenada X do centro.
- `double y`: Coordenada Y do centro.
- `double z`: Coordenada Z do centro.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
GimnPET petScanner;
petScanner.setGlobalCenter(0.0, 0.0, 0.0);
```

##### `void loadIdentificators(TString fileName)`

Carrega as informações de identificação dos detectores a partir de um arquivo de configuração (geralmente `identificators.conf`). Esta função preenche o vetor `identificators` da classe.

**Parâmetros:**
- `TString fileName`: O nome do arquivo de identificadores.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
GimnPET petScanner;
petScanner.loadIdentificators("identificators.conf");
```

##### `TVector3 getGlobalPositionPETSYS(Int_t detNum, Int_t channel, double xPos, double yPos)`

Calcula a posição global (X, Y, Z) de um evento no sistema PET, dado o número do detector, canal e as posições X e Y relativas ao detector. Esta função utiliza as informações de geometria do gantry e dos detectores para mapear as coordenadas locais para o espaço global.

**Parâmetros:**
- `Int_t detNum`: Número de identificação do detector.
- `Int_t channel`: Número do canal dentro do detector.
- `double xPos`: Posição X relativa ao detector.
- `double yPos`: Posição Y relativa ao detector.

**Retorno:**
- `TVector3`: Um vetor 3D (`TVector3`) contendo as coordenadas globais (X, Y, Z) do evento.

**Exemplo de Uso:**
```cpp
GimnPET petScanner;
petScanner.loadIdentificators("identificators.conf");
TVector3 globalPos = petScanner.getGlobalPositionPETSYS(0, 10, 5.0, 5.0);
std::cout << "Posição Global: (" << globalPos.X() << ", " << globalPos.Y() << ", " << globalPos.Z() << ")" << std::endl;
```

##### `void initializeRotators()`

Inicializa os rotadores internos ou transformações geométricas necessárias para calcular as posições globais. Esta função deve ser chamada após a configuração das dimensões e centro do gantry.

**Parâmetros:**
- Nenhum.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
GimnPET petScanner;
petScanner.setGantryRadius(400.0);
petScanner.initializeRotators();
```

##### `void setCrystalGantryRotation(Int_t detNum, double angle)`

Define a rotação do cristal do gantry para um detector específico. Isso é usado para ajustar a orientação dos detectores no espaço.

**Parâmetros:**
- `Int_t detNum`: Número de identificação do detector.
- `double angle`: O ângulo de rotação.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
GimnPET petScanner;
petScanner.setCrystalGantryRotation(0, 45.0);
```

##### `double getZRotation(Int_t detNum)`

Retorna a rotação Z de um detector específico.

**Parâmetros:**
- `Int_t detNum`: Número de identificação do detector.

**Retorno:**
- `double`: A rotação Z do detector.

**Exemplo de Uso:**
```cpp
GimnPET petScanner;
double zRot = petScanner.getZRotation(0);
```

##### `std::vector<TString> getListOfFiles(TString folder)`

Retorna uma lista de arquivos ROOT (`.root`) dentro de uma pasta especificada. Esta função é útil para iterar sobre os arquivos de dados de aquisição.

**Parâmetros:**
- `TString folder`: O caminho para a pasta.

**Retorno:**
- `std::vector<TString>`: Um vetor de `TString` contendo os nomes dos arquivos ROOT.

**Exemplo de Uso:**
```cpp
GimnPET petScanner;
std::vector<TString> files = petScanner.getListOfFiles("/data/raw_acquisitions");
```

##### `double gantryAngleStep(TString folder)`

Calcula o passo angular do gantry a partir dos nomes dos arquivos em uma pasta. Esta função assume que os nomes dos arquivos contêm informações sobre o ângulo de aquisição.

**Parâmetros:**
- `TString folder`: O caminho para a pasta contendo os arquivos de aquisição.

**Retorno:**
- `double`: O passo angular do gantry.

**Exemplo de Uso:**
```cpp
GimnPET petScanner;
double angleStep = petScanner.gantryAngleStep("/data/acquisition_angles");
```

##### `std::vector<double> get_angle(TString folder)`

Extrai os ângulos de aquisição dos nomes dos arquivos em uma pasta. Esta função é útil para determinar a sequência de ângulos em uma aquisição rotacional.

**Parâmetros:**
- `TString folder`: O caminho para a pasta contendo os arquivos de aquisição.

**Retorno:**
- `std::vector<double>`: Um vetor de `double` contendo os ângulos de aquisição.

**Exemplo de Uso:**
```cpp
GimnPET petScanner;
std::vector<double> angles = petScanner.get_angle("/data/acquisition_angles");
```

##### `std::vector<double> getDistanceAndAngle(TString folder)`

Extrai informações de distância e ângulo dos nomes dos arquivos em uma pasta. Esta função pode ser usada para obter parâmetros de aquisição de dados.

**Parâmetros:**
- `TString folder`: O caminho para a pasta contendo os arquivos de aquisição.

**Retorno:**
- `std::vector<double>`: Um vetor de `double` contendo os valores de distância e ângulo.

**Exemplo de Uso:**
```cpp
GimnPET petScanner;
std::vector<double> params = petScanner.getDistanceAndAngle("/data/acquisition_params");
```

##### `bool checkIfTomographyc(TString folder)`

Verifica se uma pasta contém dados que indicam uma aquisição tomográfica. Isso pode ser feito verificando a presença de arquivos com padrões de nome específicos ou a estrutura de subdiretórios.

**Parâmetros:**
- `TString folder`: O caminho para a pasta a ser verificada.

**Retorno:**
- `bool`: `true` se a pasta contiver dados tomográficos, `false` caso contrário.

**Exemplo de Uso:**
```cpp
GimnPET petScanner;
if (petScanner.checkIfTomographyc("/data/patient_scan")) {
    std::cout << "Dados tomográficos encontrados." << std::endl;
}
```

##### `void setRotateAlong(TString axis)`

Define o eixo de rotação do gantry (X, Y ou Z). Esta função é importante para configurar a geometria de rotação do scanner.

**Parâmetros:**
- `TString axis`: O eixo de rotação ("X", "Y" ou "Z").

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
GimnPET petScanner;
petScanner.setRotateAlong("Y");
```




### 3.36. `source/92-TomoProcessing/ProcessAcquisition.C`

Este arquivo define a classe `ProcessAcquisition`, que é central para o processamento de aquisições tomográficas. Ela gerencia os caminhos de arquivos, parâmetros de calibração e linearização, e orquestra as etapas de processamento para transformar dados brutos em dados prontos para a reconstrução de imagens.

#### 3.36.1. Classes

##### `class ProcessAcquisition`

Gerencia o pipeline de processamento de uma aquisição PET, desde a leitura de dados brutos até a aplicação de calibrações e linearizações. A classe armazena e gerencia os caminhos para diversos arquivos e diretórios de entrada e saída, bem como os fatores de calibração e linearização.

**Atributos:**
- `TString rawFileName`: Nome do arquivo de dados brutos de entrada.
- `TString outFileName`: Nome do arquivo de saída para os dados processados.
- `TString calibrationFileName`: Nome do arquivo de calibração.
- `TString linearizationFileName`: Nome do arquivo de linearização.
- `TString outLinearizationFileName`: Nome do arquivo de saída para os parâmetros de linearização.
- `TString processedFolder`: Caminho para a pasta de dados processados.
- `TString alignmentFolder`: Caminho para a pasta de dados de alinhamento.
- `TString calibrationFolder`: Caminho para a pasta de dados de calibração.
- `TString plotsFolder`: Caminho para a pasta de plots/gráficos.
- `TString identificatorPath`: Caminho para o arquivo de identificadores.
- `TString alignmentName`: Nome do arquivo de alinhamento.
- `TString calibrationName`: Nome do arquivo de calibração.
- `std::map<int, double> calibrationFactors`: Mapa de fatores de calibração.
- `std::map<int, double> linearizationFactors`: Mapa de fatores de linearização.
- `bool verbose`: Flag para ativar/desativar o modo verboso.
- `bool obtainAlignmentData`: Flag para indicar se dados de alinhamento devem ser obtidos.
- `int method`: Método de processamento a ser utilizado.
- `int classCounter_`: Contador interno da classe.

**Construtor:**
##### `ProcessAcquisition()`

Construtor padrão da classe `ProcessAcquisition`. Inicializa os atributos com valores padrão ou nulos.

**Exemplo de Uso:**
```cpp
ProcessAcquisition acquisitionProcessor;
```

**Métodos:**

##### `void setRawFileName(TString name)`

Define o nome do arquivo de dados brutos de entrada.

**Parâmetros:**
- `TString name`: O nome do arquivo.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
ProcessAcquisition acquisitionProcessor;
acquisitionProcessor.setRawFileName("raw_scan_001.root");
```

##### `void setOutFileName(TString name)`

Define o nome do arquivo de saída para os dados processados.

**Parâmetros:**
- `TString name`: O nome do arquivo.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
ProcessAcquisition acquisitionProcessor;
acquisitionProcessor.setOutFileName("processed_scan_001.root");
```

##### `void setCalibrationFileName(TString name)`

Define o nome do arquivo de calibração.

**Parâmetros:**
- `TString name`: O nome do arquivo.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
ProcessAcquisition acquisitionProcessor;
acquisitionProcessor.setCalibrationFileName("calibration_params.root");
```

##### `void setLinearizationFileName(TString name)`

Define o nome do arquivo de linearização.

**Parâmetros:**
- `TString name`: O nome do arquivo.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
ProcessAcquisition acquisitionProcessor;
acquisitionProcessor.setLinearizationFileName("linearization_params.root");
```

##### `void setOutLinearizationFileName(TString name)`

Define o nome do arquivo de saída para os parâmetros de linearização.

**Parâmetros:**
- `TString name`: O nome do arquivo.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
ProcessAcquisition acquisitionProcessor;
acquisitionProcessor.setOutLinearizationFileName("output_linearization.root");
```

##### `void setProcessedFolder(TString path)`

Define o caminho para a pasta de dados processados.

**Parâmetros:**
- `TString path`: O caminho da pasta.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
ProcessAcquisition acquisitionProcessor;
acquisitionProcessor.setProcessedFolder("/data/processed");
```

##### `void setAlignmentFolder(TString path)`

Define o caminho para a pasta de dados de alinhamento.

**Parâmetros:**
- `TString path`: O caminho da pasta.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
ProcessAcquisition acquisitionProcessor;
acquisitionProcessor.setAlignmentFolder("/data/alignment");
```

##### `void setCalibrationFolder(TString path)`

Define o caminho para a pasta de dados de calibração.

**Parâmetros:**
- `TString path`: O caminho da pasta.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
ProcessAcquisition acquisitionProcessor;
acquisitionProcessor.setCalibrationFolder("/data/calibration");
```

##### `void setPlotsFolder(TString path)`

Define o caminho para a pasta de plots/gráficos.

**Parâmetros:**
- `TString path`: O caminho da pasta.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
ProcessAcquisition acquisitionProcessor;
acquisitionProcessor.setPlotsFolder("/data/plots");
```

##### `void setIdentificatorPath(TString path)`

Define o caminho para o arquivo de identificadores.

**Parâmetros:**
- `TString path`: O caminho do arquivo.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
ProcessAcquisition acquisitionProcessor;
acquisitionProcessor.setIdentificatorPath("identificators.conf");
```

##### `void setAlignmentName(TString name)`

Define o nome do arquivo de alinhamento.

**Parâmetros:**
- `TString name`: O nome do arquivo.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
ProcessAcquisition acquisitionProcessor;
acquisitionProcessor.setAlignmentName("alignment_data.root");
```

##### `void setCalibrationName(TString name)`

Define o nome do arquivo de calibração.

**Parâmetros:**
- `TString name`: O nome do arquivo.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
ProcessAcquisition acquisitionProcessor;
acquisitionProcessor.setCalibrationName("calibration_data.root");
```

##### `void setCalibrationFactors(std::map<int, double> factors)`

Define os fatores de calibração a serem aplicados.

**Parâmetros:**
- `std::map<int, double> factors`: Um mapa de inteiros para doubles representando os fatores de calibração.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
ProcessAcquisition acquisitionProcessor;
std::map<int, double> calFactors = {{0, 1.0}, {1, 1.05}};
acquisitionProcessor.setCalibrationFactors(calFactors);
```

##### `void setLinearizationFactors(std::map<int, double> factors)`

Define os fatores de linearização a serem aplicados.

**Parâmetros:**
- `std::map<int, double> factors`: Um mapa de inteiros para doubles representando os fatores de linearização.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
ProcessAcquisition acquisitionProcessor;
std::map<int, double> linFactors = {{0, 0.98}, {1, 1.02}};
acquisitionProcessor.setLinearizationFactors(linFactors);
```

##### `void setVerbose(bool v)`

Define o modo verboso (ativa/desativa mensagens de log detalhadas).

**Parâmetros:**
- `bool v`: `true` para ativar, `false` para desativar.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
ProcessAcquisition acquisitionProcessor;
acquisitionProcessor.setVerbose(true);
```

##### `void setObtainAlignmentData(bool o)`

Define se os dados de alinhamento devem ser obtidos durante o processamento.

**Parâmetros:**
- `bool o`: `true` para obter, `false` para não obter.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
ProcessAcquisition acquisitionProcessor;
acquisitionProcessor.setObtainAlignmentData(true);
```

##### `void setMethod(int m)`

Define o método de processamento a ser utilizado.

**Parâmetros:**
- `int m`: O código do método.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
ProcessAcquisition acquisitionProcessor;
acquisitionProcessor.setMethod(1); // Exemplo: método 1
```

##### `void initializeEmpty()`

Inicializa a classe com caminhos e parâmetros vazios ou padrão. Útil para redefinir o estado da classe.

**Parâmetros:**
- Nenhum.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
ProcessAcquisition acquisitionProcessor;
acquisitionProcessor.initializeEmpty();
```

##### `void setClassCounter(int counter)`

Define o valor do contador interno da classe.

**Parâmetros:**
- `int counter`: O valor do contador.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
ProcessAcquisition acquisitionProcessor;
acquisitionProcessor.setClassCounter(10);
```

##### `int classCounter()`

Retorna o valor atual do contador interno da classe.

**Parâmetros:**
- Nenhum.

**Retorno:**
- `int`: O valor do contador.

**Exemplo de Uso:**
```cpp
ProcessAcquisition acquisitionProcessor;
int currentCounter = acquisitionProcessor.classCounter();
```

##### `void clear()`

Limpa todos os caminhos de arquivos e diretórios, bem como os fatores de calibração e linearização, redefinindo-os para um estado vazio ou padrão.

**Parâmetros:**
- Nenhum.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
ProcessAcquisition acquisitionProcessor;
acquisitionProcessor.clear();
```

- **Outros Getters:** A classe também fornece métodos `get` para seus atributos, como `getRawFileName()`, `getOutFileName()`, `getCalibrationFileName()`, `getLinearizationFileName()`, `getOutLinearizationFileName()`, `getProcessedFolder()`, `getAlignmentFolder()`, `getCalibrationFolder()`, `getPlotsFolder()`, `getIdentificatorPath()`, `getAlignmentName()`, `getCalibrationName()`, `getCalibrationFactors()`, `getLinearizationFactors()`, `getVerbose()`, `getObtainAlignmentData()`, `getMethod()`.




### 3.37. `source/93- Menu/Tomographyc.C`

Este arquivo contém funções que servem como interface de menu para a execução de tarefas de processamento tomográfico. Ele orquestra a chamada de outras funções e classes para realizar calibração, processamento de coincidências e grupos, e análise tomográfica.

#### 3.37.1. Funções

##### `void TomographyProcessingMenu()`

Exibe um menu interativo no console que permite ao usuário selecionar diferentes operações de processamento tomográfico, como calibração, processamento de coincidências, processamento de grupos e alinhamento. Esta função atua como o ponto de entrada principal para a interação do usuário com o pipeline de processamento tomográfico.

**Parâmetros:**
- Nenhum.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
TomographyProcessingMenu(); // Inicia o menu de processamento tomográfico
```

##### `void runCalibration(TString inputFolder, TString outputFolder)`

Executa o processo de calibração para os dados localizados em `inputFolder` e salva os resultados em `outputFolder`. Esta função pode chamar `massiveCalibration` ou `calibrate` dependendo da estrutura dos dados e da necessidade de processamento em massa.

**Parâmetros:**
- `TString inputFolder`: O caminho para o diretório contendo os dados brutos a serem calibrados.
- `TString outputFolder`: O caminho para o diretório onde os dados calibrados serão salvos.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
runCalibration("/data/raw_scans", "/data/calibrated_scans");
```

##### `void ProcessCoincidenceFile(TString inputFileName, TString outputFileName)`

Processa um arquivo de dados de coincidência específico, aplicando as etapas necessárias para prepará-lo para a reconstrução tomográfica. Esta função pode chamar `ProcessPulsesCoincidences` ou `processCoincidencePreCal`.

**Parâmetros:**
- `TString inputFileName`: O caminho para o arquivo ROOT de entrada contendo os dados de coincidência.
- `TString outputFileName`: O caminho para o arquivo ROOT de saída onde os dados processados serão salvos.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
ProcessCoincidenceFile("raw_coincidences.root", "processed_coincidences.root");
```

##### `void processGroupFile(TString inputFileName, TString outputFileName)`

Processa um arquivo de dados de grupo específico, aplicando as etapas necessárias para prepará-lo para a reconstrução tomográfica. Esta função pode chamar `ProcessPulses` ou `processGroupPre`.

**Parâmetros:**
- `TString inputFileName`: O caminho para o arquivo ROOT de entrada contendo os dados de grupo.
- `TString outputFileName`: O caminho para o arquivo ROOT de saída onde os dados processados serão salvos.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
processGroupFile("raw_groups.root", "processed_groups.root");
```

##### `void runAlignment(TString inputFolder, TString outputFolder)`

Executa o processo de alinhamento para os dados localizados em `inputFolder` e salva os resultados em `outputFolder`. O alinhamento é crucial para corrigir desvios geométricos e garantir a precisão da reconstrução de imagens.

**Parâmetros:**
- `TString inputFolder`: O caminho para o diretório contendo os dados a serem alinhados.
- `TString outputFolder`: O caminho para o diretório onde os dados alinhados serão salvos.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
runAlignment("/data/unaligned_data", "/data/aligned_data");
```

##### `void ProcessTomography(TString inputFolder, TString outputFolder)`

Orquestra o pipeline completo de processamento tomográfico para uma pasta de entrada, incluindo calibração, processamento de coincidências/grupos e alinhamento, salvando os resultados em uma pasta de saída. Esta é uma função de alto nível que integra várias etapas do processamento.

**Parâmetros:**
- `TString inputFolder`: O caminho para o diretório contendo os dados brutos da aquisição tomográfica.
- `TString outputFolder`: O caminho para o diretório onde os resultados do processamento tomográfico serão salvos.

**Retorno:**
- Nenhum.

**Exemplo de Uso:**
```cpp
ProcessTomography("/data/raw_tomo_scan", "/data/final_tomo_results");
```


## 4. Funcionamento do Programa

O sistema PET descrito nesta documentação é projetado para ser executado dentro do framework ROOT C++. O ROOT é um framework de análise de dados e visualização de dados desenvolvido no CERN, amplamente utilizado em física de partículas e nuclear. Ele fornece as ferramentas necessárias para manipulação de dados, histogramas, árvores e gráficos, que são extensivamente utilizados neste projeto.

### 4.1. Execução no ROOT C++ Framework

Para executar o programa, o usuário deve iniciar uma sessão ROOT e carregar o script principal, que é tipicamente um arquivo com a extensão `.C` (por exemplo, `Run.C` ou `Tomographyc.C` conforme mencionado no `README.md` e na estrutura de diretórios). A execução é realizada utilizando o comando `.X` seguido do nome do arquivo.

**Exemplo de Execução:**

```bash
root
```

Dentro do prompt do ROOT:

```cpp
.X Run.C
```

Ao executar o script principal, ele irá carregar as classes e funções definidas nos diversos arquivos `.C` (que são compilados e carregados dinamicamente pelo ROOT, se necessário) e iniciar o fluxo de trabalho definido. Por exemplo, o `Tomographyc.C` inicia um menu interativo que guia o usuário através das diferentes etapas de processamento (calibração, processamento de coincidências, etc.).


## 5. Conclusão

Esta documentação detalhada visa fornecer uma compreensão clara e abrangente do código-fonte do sistema PET. Ao descrever cada classe e função, seus propósitos, atributos, métodos e exemplos de uso, esperamos facilitar a manutenção, o desenvolvimento e a colaboração neste projeto.

O sistema, construído sobre o robusto framework ROOT C++, oferece uma solução modular e escalável para o processamento de dados de tomografia por emissão de pósitrons, desde a aquisição bruta até a análise final e a preparação para a reconstrução de imagens. A modularidade do design permite que componentes individuais sejam aprimorados ou substituídos sem afetar drasticamente o sistema como um todo.

Para futuras contribuições, é recomendável seguir as convenções de codificação e documentação estabelecidas, garantindo a consistência e a legibilidade do código. A compreensão aprofundada das classes e funções aqui detalhadas será um recurso inestimável para qualquer desenvolvedor ou pesquisador que trabalhe com este sistema.

>>>>>>> c5a7e97 (Upload)
