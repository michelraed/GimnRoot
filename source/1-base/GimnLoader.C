void GimnLoader() {}

/**
 * @brief Loader será a classe base para ler os arquivos de configuração do projeto
 * Uma vez que varios dos processamentos necessitam de informação extra, os arquivos de configuração
 * servirão para fornecer ao script de processamento os dados necessários para que este seja efetuado;
 * Por exemplo, antes de ler o arquivo root durante o processamento não há como saber quantos detectores
 * havia conectados sem antes ler todos os eventos no interior do arquivo root. Além disso, informações como
 * tipo de algoritmo a ser utilizado para o posicionamento, bem como qual o tipo de cristal utilizado, são
 * lidos nestes arquivos de configuração.
 * De modo a facilitar a leitura dos arquivos será criada uma classe base responsável pelo tratamento do arquivo
 * e desta serão derivadas classes para a leitura dos identificadores, e parametros de calibração para cada uma das
 * etapas necessárias.
 */
class Loader
{

protected:
    std::fstream reader;  // leitor de arquivos, baseado em fstream
    std::string fileName; // caminho apra o arquivo que se deseja ler;

public:
    // Construtores
    Loader() {}

    /// @brief Construtor da Classe Loader, esta irá receber como parametro fileToOpen
    /// @param fileToOpen string que representa o caminho onde o arquivo que se deseja abrir estaá salvo;
    Loader(std::string fileToOpen)
    {
        fileName = fileToOpen;
        reader.open(fileToOpen.c_str(), std::ios::in);
    }

    /// @brief Construtor de copia da classe Loader
    /// @param l classe a ser copiada
    Loader(Loader const &l) : fileName{l.fileName}
    {
        reader.open(fileName, std::ios::in);
    }

    // Destrutor :
    ~Loader()
    {
        if (reader.is_open())
        {
            reader.close();
        }
    }

    /// @brief Irá fazer com que a variavel fileName, interna da propria classe, seja ajustada ao valor passado
    /// @param filePath string contendo o o caminho para o arquivo que se deseja abrir
    inline void setFileName(std::string filePath) { this->fileName = filePath; }

    /// @brief Irá imprimir na tela o nome do arquivo que consta como fileName
    inline void printFileName() { std::cout << this->fileName << std::endl; }

    /// @brief Ira Abrir diretamente o arquivo fornecido;
    /// @param filePath indica o caminho para o arquivo que se deseja abrir
    inline void loadFile(std::string filePath)
    {
        this->fileName = filePath;
        reader.open(this->fileName.c_str(), std::ios::in);
    }

    /// @brief  Irá abrir o arquivo carregado
    inline void Load() { reader.open(this->fileName.c_str(), std::ios::in); }

    /// @brief  Irá checat se o arquivo está aberto e caso esteja irá fechar o arquivo;
    inline void closeFile() { reader.is_open() ? reader.close() : (std::cout << "Arquivo nao está aberto" << std::endl, void()); }

    /// @brief Metodo que retorna a string guardada como fileName
    /// @return string fileName que representa o endereço para o arquivo que está sendo tratado
    inline std::string getFileName() { return this->fileName; }

    /// @brief Imprime na tela o conteudo do arquivo sendo lido
    /// @param fileToOpen
    virtual void print()
    {
        /**
         * @brief Este metodo irá imprimir na tela todas as linhas do arquivo Fornecido;
         *
         */
        if (this->fileName.empty())
            std::cout << "nenhum arquivo carregado" << std::endl;

        else
        {
            reader.open(fileName.c_str(), std::ios::in);
            std::string linha;
            int counter = 0;
            // itera sobre as linhas
            while (std::getline(reader, linha))
            {
                std::istringstream iss(linha);
                if (VERBOSE)
                    std::cout << linha << std::endl;
            }
            reader.close();
        }
    }

    /// @brief Ajusta o valor do ponteiro de leitura para o valor dado em relação ao inicio do corpo do texto
    /// @param pos posição relativa ao inicio
    void setPontLeitura(int pos) { reader.seekg(pos, std::ios::beg); }

    /// @brief retorna a posição atual do ponteiro de leitura do arquivo
    void getPontLeitura() { std::cout << reader.tellg() << std::endl; }
};

/**
 * @brief Esta classe irá gerar o carregador para o identificador do sistema.
 * O identificador irá fornecer aos arquivos de processamento informações sobre o sistema petsys
 * tais como o tipo de cristal utilizado, quantos cristais foram utilizados para o processamento,
 * tipos de algoritmos de posicionamento e timing a serem utilizados, nome do detector e  detector de referencia.
 *
 */
class Identificator : public Loader
{
public:
    // Atributos

    int detNum{0};
    std::vector<int> port;
    std::vector<int> chipID;
    std::vector<std::vector<int>> inCoincidenceWith;
    std::vector<int> ref;
    std::vector<std::string> detectorName;
    std::vector<std::string> positioning;
    std::vector<std::string> timing;
    std::vector<std::string> crystalType;
    std::vector<double> detectorAngle;
    std::vector<double> detectorZRotation;

    /**
     * @brief Função interna da classe Identificator que servirá para imprimir os valores de cada linha lida na tabela de identificação
     * do sistema petsys
     *
     * @param counter  indica a linha
     * @param port     indica a porta na qual o detector está conectado
     * @param chipID   indica o chipID no qual o detector está sendo utilizado
     * @param inCoincidence     indica com quais detectores este detector está em coincidencia
     * @param ref       indica o detector de referencia para os calculos do tempo
     * @param name      indica o nome do detector
     * @param positioning   indica o algoritmo de posicionamento a ser utilizado para este detector
     * @param timing        indica o algoritmo de timing a ser utilizado por este detector
     * @param crystalType   indica o tipo de cristal cintilador utilizado neste detector
     * @param detectorAngle indica o angulo de rotação do detector
     * @param detectorZRotation indica a rotação em Z do detector (XY no plano da face do detector), para rotacionar o detector em Z caso nao esteja orientado
     
     
     */
    void printLine(int counter, std::string port, std::string chipID, std::string inCoincidence, std::string ref,
                   std::string name, std::string positioning, std::string timing, std::string crystalType, 
                   std::string detectorAngle, std::string detectorZRotation)
    {
        std::cout << "--------------------------------------" << std::endl;
        std::cout << " linha " << counter << std::endl;
        std::cout << " port  read " << port << std::endl;
        std::cout << " chipID read " << chipID << std::endl;
        std::cout << " inCoincidence read " << inCoincidence << std::endl;
        std::cout << " ref read " << ref << std::endl;
        std::cout << " name read " << name << std::endl;
        std::cout << " positioning read " << positioning << std::endl;
        std::cout << " timing read " << timing << std::endl;
        std::cout << " crystal read " << crystalType << std::endl;
        std::cout << " detector angle read " << detectorAngle << std::endl;
        std::cout << " detector Z rotation read " << detectorZRotation<< std::endl;
        std::cout << "--------------------------------------" << std::endl;
    }

    // constructors

    /**
     * @brief Cria um objeto dentificador porém nao inicializa neste nenhum valor. Estes deverão ser manipulados utilizando os getters e setters;
     *
     */
    Identificator() : Loader() {}

    /**
     * @brief Cria um objeto de leitura dos arquivos de identificação. Este irá ler a informação contida no arquovo "identificators.conf" fornecido
     *
     * @param identFileName  é o caminho para o arquivo *./identificators.conf
     */
    Identificator(std::string identFileName) : Loader(identFileName)
    {
    }

    Identificator(Identificator const &id) : detNum{id.detNum},
                                             port{id.port},
                                             chipID{id.chipID},
                                             inCoincidenceWith{id.inCoincidenceWith},
                                             ref{id.ref},
                                             detectorName{id.detectorName},
                                             positioning{id.positioning},
                                             timing{id.timing},
                                             crystalType{id.crystalType},
                                             detectorAngle{id.detectorAngle},
                                             detectorZRotation{id.detectorZRotation}{}; // copy constructor


    Identificator &operator=(const Identificator &other)
    {
        if (this != &other)
        {
            this->detNum = other.detNum;
            this->port = other.port;
            this->chipID = other.chipID;
            this->inCoincidenceWith = other.inCoincidenceWith;
            this->ref = other.ref;
            this->detectorName = other.detectorName;
            this->positioning = other.positioning;
            this->timing = other.timing;
            this->crystalType = other.crystalType;
            this->detectorAngle = other.detectorAngle;
            this->detectorZRotation = other.detectorZRotation;

        }
        return *this;
    }

    // Getters

    /// @brief Metodo getter para obter o valor do numero total de detectores conectados
    /// @return inteiro representativo da quantiade de detecotores conectados
    inline int getDetNum() { return this->detNum; }
    /// @brief Método getter para obter um vetor contendo as portas sendo utilizadas
    /// @return vetor de inteiros onde cada posição do vetor corresponde ao numero de porta na qual o detector encontra-se conectado
    inline std::vector<int> getPorts() { return this->port; }

    /// @brief Método Getter para obter um vetor contendo os chipIDs sendo utilizados
    /// @return vetor de inteiros onde cada posição do vetor corresponde ao chipI.qD no qual o detector encontra-se conectado
    inline std::vector<int> getChipIDs() { return this->chipID; }

    /// @brief Método Getter para obter um vetor contendo o chip ID dos detectores que encontram-se em coincidencia com o detector sendo utilizado
    /// @return vetor de vetores onde cada posição do vetor contem um vetor com todos os detectores que estão em coincidencia com esse detector.
    inline std::vector<std::vector<int>> getInCoincidenceWith() { return this->inCoincidenceWith; }

    /// @brief Método Getter para obter um vetor contendo para cada detecor o seu detector de referencia para calculo de tempo
    /// @return  vetor de inteiros contendo o detector de referencia para cada detector
    inline std::vector<int> getRef() { return this->ref; }

    /// @brief Método Getter para obter um vetor contendo os nomes dos detectores
    /// @return vetor de strings contendo os nomes dos detectores
    inline std::vector<std::string> getDetectorName() { return this->detectorName; }

    /// @brief Método Getter para obter um vetor contendo os algorimos de posicionamento a serem utilizados no processamentod e cada detector
    /// @return vetor de strings contendo os algoritmos de posicionamento a serem utilizados
    inline std::vector<std::string> getPositioning() { return this->positioning; }

    /// @brief Método Getter para obter um vetor contendo os algoritmos de timing a serem utilizados no processamento de cada detector
    /// @return vetor de strings contendo os algoritmos de timing a serem utilizados
    inline std::vector<std::string> getTiming() { return this->timing; }

    /// @brief Método Getter para obter um vetor contendo os tipos de cristais sendo utilizados, podendo ser mono para monoliticos e seg para segmentados
    /// @return vetor de strings contendo os tipos de cristais a serem utilizados
    inline std::vector<std::string> getCrystalType() { return this->crystalType; }

    /// @brief Método Getter para obter um vetor contendo os angulos de rotação dos detectores
    /// @return vetor de doubles contendo os angulos de rotação dos detectores
    inline std::vector<double> getDetectorAngle() { return this->detectorAngle; }

    /// @brief Método Getter para obter um vetor contendo as rotações em Z dos detectores
    /// @return vetor de doubles contendo as rotações em Z dos detectores
    inline std::vector<double> getDetectorZRotation() { return this->detectorZRotation; }

    // Setters

    /// @brief Método setter para ajustar o valor do numero total de detectores conectados
    /// @param detNum inteiro representativo da quantiade de detecotores conectados
    inline void setDetNum(int detNum) { this->detNum = detNum; }

    /// @brief Método setter para ajustar o valor das portas sendo utilizadas
    /// @param port vetor de inteiros onde cada posição do vetor corresponde ao numero de porta na qual o detector encontra-se conectado
    inline void setPorts(std::vector<int> port) { this->port = port; }

    /// @brief Método setter para ajustar o valor dos chipIDs sendo utilizados
    /// @param chipID vetor de inteiros onde cada posição do vetor corresponde ao chipI.qD no qual o detector encontra-se conectado
    inline void setChipIDs(std::vector<int> chipID) { this->chipID = chipID; }

    /// @brief Método setter para ajustar o valor do chip ID dos detectores que encontram-se em coincidencia com o detector sendo utilizado
    /// @param inCoincidenceWith vetor de vetores onde cada posição do vetor contem um vetor com todos os detectores que estão em coincidencia com esse detector.
    inline void setInCoincidenceWith(std::vector<std::vector<int>> inCoincidenceWith) { this->inCoincidenceWith = inCoincidenceWith; }






    inline std::string getDetectorName(int det) { return this->detectorName[det]; }
    inline std::string getTiming(int det) { return this->timing[det]; }
    inline std::string getPositioning(int det) { return this->positioning[det]; }
    inline std::string getCrysalType(int det) { return this->crystalType[det]; }
    inline int getRef(int det) { return this->ref[det]; }

    /// @brief Irá ler o arquivo identificators.conf e guardar os valores lidos em vetores que poderão ser acessados via os metodos getter
    /// @param identificatorPath caminho para o arquivo identificators.conf
    void LoadIdentificators(std::string identificatorPath)
    {
        loadFile(identificatorPath);
        std::string linha;
        int counter = 0;
        std::vector<std::string> separatedCoincidences;
        if (VERBOSE){
            std::cout << " Reading Configurations" << std::endl;
            std::cout << identificatorPath << std::endl;
        }
        this->detNum = 0;
        while (std::getline(this->reader, linha))
        {
            if (!(counter == 0)) // 0 is header
            {
                std::istringstream iss(linha);
                std::string port, chipID, inCoincidence, ref, name, positioning, timing, crystalType, detectorAngle, detectorZRotation;
                iss >> port >> chipID >> inCoincidence >> ref >> name >> positioning >> timing >> crystalType >> detectorAngle >> detectorZRotation;

                // função para debug
                if (VERBOSE)
                    printLine(counter, port, chipID, inCoincidence, ref, name, positioning, timing, crystalType, detectorAngle, detectorZRotation);
                std::vector<int> aux;

                if (!port.empty())
                    this->port.push_back(stoi(port));

                if (!chipID.empty())
                    this->chipID.push_back(stoi(chipID));
                // se tiver mais de um sepaara por virgula
                if (!inCoincidence.empty())
                {
                    if (inCoincidence.find(',') == -1)
                    {
                        aux.push_back(stoi(inCoincidence));
                    }
                    else
                    {
                        separatedCoincidences = splitString(inCoincidence, ',');
                        if (VERBOSE)
                            std::cout << inCoincidence << std::endl;
                        for (auto &inCoin : separatedCoincidences)
                        {
                            if (VERBOSE)
                                std::cout << "in coin " << inCoin << std::endl;
                            aux.push_back(stoi(inCoin));
                        }
                    }
                    this->inCoincidenceWith.push_back(aux);
                    aux.clear();
                }
                if (!ref.empty())
                    this->ref.push_back(stoi(ref));

                if (!name.empty())
                    this->detectorName.push_back(name);
                if (!positioning.empty())
                    this->positioning.push_back(positioning);
                if (!timing.empty())
                    this->timing.push_back(timing);

                if (!crystalType.empty())
                    this->crystalType.push_back(crystalType);
                
                if(!detectorAngle.empty())
                    this->detectorAngle.push_back(stod(detectorAngle));

                if(!detectorZRotation.empty())
                    this->detectorZRotation.push_back(stod(detectorZRotation));
                    this->detNum++;
            }
            counter++;
        }
    }

    bool checkInCoincidence(int position, int chipID)
    {
        bool found = false;
        for (auto &eval : inCoincidenceWith[position])
        {
            if (eval == chipID)
                return true;
        }
        return found;
    }

    /// @brief Irá imprimir os valores armazenados nos vetores de portas, chipID, coincidencia entre outros
    void print() override
    {
        std::cout << "________________________________________" << std::endl;
        std::cout << " Data read : " << std::endl;
        std::cout << " Ports " << std::endl;
        int mini_counter{0};
        for (auto &element : this->port)
        {
            std::cout << this->detectorName[mini_counter] << " port : " << element << std::endl;
            mini_counter++;
        }
        std::cout << "________________________________________" << std::endl;
        std::cout << " chipIDs " << std::endl;
        mini_counter = 0;
        for (auto &element : this->chipID)
        {
            std::cout << this->detectorName[mini_counter] << " chipID : " << element << std::endl;
            mini_counter++;
        }
        std::cout << "________________________________________" << std::endl;
        std::cout << " inCoincidenceWith :" << std::endl;
        mini_counter = 0;
        for (auto &element : this->inCoincidenceWith)
        {
            for (auto &element2 : element)
            {
                std::cout << "port[" << this->port[mini_counter] << "] - " << element2 << std::endl;
            }
            mini_counter++;
        }

        std::cout << "________________________________________" << std::endl;
        std::cout << " refs " << std::endl;
        mini_counter = 0;
        for (auto &element : this->ref)
        {
            std::cout << this->detectorName[mini_counter] << "  ref : " << element << std::endl;
            mini_counter++;
        }
        std::cout << "________________________________________" << std::endl;
        std::cout << " names " << std::endl;

        mini_counter = 0;
        for (auto &element : this->detectorName)
        {
            std::cout << mini_counter << " : " << element << std::endl;
            mini_counter++;
        }
        std::cout << "________________________________________" << std::endl;
        std::cout << " positionings " << std::endl;
        mini_counter = 0;
        for (auto &element : this->positioning)
        {
            std::cout << this->detectorName[mini_counter] << " positioning : " << element << std::endl;
            mini_counter++;
        }
        std::cout << "________________________________________" << std::endl;
        std::cout << " timings " << std::endl;
        mini_counter = 0;
        for (auto &element : this->timing)
        {
            std::cout << this->detectorName[mini_counter] << " timing : " << element << std::endl;
            mini_counter++;
        }
        std::cout << "________________________________________" << std::endl;
        std::cout << " Crystal type " << std::endl;
        mini_counter = 0;
        for (auto &element : this->crystalType)
        {
            std::cout << this->detectorName[mini_counter] << " crystla type : " << element << std::endl;
            mini_counter++;
        }
        std::cout << "________________________________________" << std::endl;
        std::cout << " Detector Angle " << std::endl;
        mini_counter = 0;
        for (auto &element : this->detectorAngle)
        {
            std::cout << this->detectorName[mini_counter] << " detector angle : " << element << std::endl;
            mini_counter++;
        }
        std::cout << "________________________________________" << std::endl;
        std::cout << " Detector Z Rotation " << std::endl;
        mini_counter = 0;
        for (auto &element : this->detectorZRotation)
        {
            std::cout << this->detectorName[mini_counter] << " detector Z rotation : " << element << std::endl;
            mini_counter++;
        }

        std::cout << "________________________________________" << std::endl;


    }
};

class histoConf : public Loader
{
};