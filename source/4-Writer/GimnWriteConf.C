#include <fstream>
#include <string>

class GimnWriteConf
{

protected:
    std::fstream writer;
    std::string fileName;

public:
    GimnWriteConf() {}
    GimnWriteConf(std::string fileName)
    {
        writer.open(fileName.c_str(), std::ios::out);
    }
    /// @brief Construtor de copia da classe GimnWriteConf
    /// @param gwc classe a ser copiada
    GimnWriteConf(GimnWriteConf const &gwc) : fileName{gwc.fileName}
    {
        writer.open(fileName, std::ios::in);
    }

    void setFileName(std::string fileName)
    {
        if (writer.is_open())
        {
            writer.close();
        }
        this->fileName = fileName;
        writer.open(fileName, std::ios::out);
    }

    // Destrutor :
    ~GimnWriteConf()
    {
        if (writer.is_open())
        {
            writer.close();
        }
    }
};

class WriteGenericConf : public GimnWriteConf
{

    std::vector<std::vector<std::map<std::string, Float_t>>> conf;
    Identificator Id;

    public:
    WriteGenericConf() {}
    WriteGenericConf(std::vector<std::vector<std::map<std::string, Float_t>>> conf, Identificator Id, std::string fileName) : conf{conf}, Id{Id}{
        std::cout<<fileName<<std::endl;
        std::string saveOn = "./"+fileName +".cal";
        setFileName(saveOn);
    }

    void WriteConf()
    {
        for (int port = 0; port < conf.size(); port++)
        {   
            std::string porta = "Porta " + std::to_string(Id.port[port]);
            std::cout << porta << " " << port << std::endl;
            std::cout<<fileName<<std::endl;
            for (int ch = 0; ch < conf[port].size(); ch++)
            {
                writer << porta << " channel " << ch << " Mean " << conf[port][ch]["Mean"] << "\n";
            }
        }
        writer.close();
    }
};