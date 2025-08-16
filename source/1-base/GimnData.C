
class GimnData
{

public:
    std::vector<std::vector<std::shared_ptr<TH1F>>> channelTimeSkew;
    std::vector<std::vector<std::shared_ptr<TH2F>>> channelTimeWalk;
    std::vector<std::vector<std::shared_ptr<TH2F>>> channelTimeWalk1;
    std::vector<std::vector<std::shared_ptr<TH2F>>> channelTimeWalk2;
    std::vector<std::vector<std::shared_ptr<TH2F>>> channelTimeWalk3;
    std::vector<std::vector<std::shared_ptr<TH1F>>> channelEnergyHistogram;
    std::vector<std::shared_ptr<TH1F>> GlobalEnergy;
    std::vector<std::shared_ptr<TH1F>> GlobalTime;
    std::vector<std::shared_ptr<TH1F>> depthOfInteraction;
    std::vector<std::shared_ptr<TH2F>> FloodHist;
    vector<TMatrixT<double>> Sipm;
    Identificator Id;
    

protected:
    int counter{0};
    int channelsPerArray{64};
    // Energy Histogram
    int H1ECBins{250};
    int H1ECMin{0};
    int H1ECMax{0};

    int H1EGBins{250};
    int H1EGMin{0};
    int H1EGMax{0};

    // Time Histogram
    int H1TCBins{200};
    int H1TCMax{0};
    int H1TCMin{0};

    // Time 2D
    int H2TBinsX{128};
    int H2TMinX{0};
    int H2TMaxX{0};
    int H2TBinsY{128};
    int H2TMinY{0};
    int H2TMaxY{0};

    // Flood Histogram
    int H2FBinsX{500};
    int H2FMaxX{0};
    int H2FMinX{0};

    int H2FBinsY{500};
    int H2FMaxY{0};
    int H2FMinY{0};

public:
    GimnData() {}
    GimnData(int counter): counter(counter){}
    
    void setCounter (int counter){this->counter = counter;}
    
    GimnData(Identificator const &Id,int counter)
    {
        this->counter = counter;
        this->Id = Id;
        this->initializeHistograms();
    }
    GimnData(Identificator const &Id)
    {
        this->Id = Id;
        this->initializeHistograms();
    }
    GimnData(GimnData const &gd) : channelTimeSkew(gd.channelTimeSkew),
                                   channelTimeWalk(gd.channelTimeWalk),
                                   channelTimeWalk1(gd.channelTimeWalk1),
                                   channelTimeWalk2(gd.channelTimeWalk2),
                                   channelTimeWalk3(gd.channelTimeWalk3),
                                   channelEnergyHistogram(gd.channelEnergyHistogram),
                                   GlobalEnergy(gd.GlobalEnergy),
                                   GlobalTime(gd.GlobalTime),
                                   depthOfInteraction(gd.depthOfInteraction),
                                   FloodHist(gd.FloodHist),
                                   Sipm(gd.Sipm),
                                   Id(gd.Id),
                                   counter(gd.counter) {}

    GimnData &operator=(const GimnData &other)
    {
        if (this != &other)
        {
            this->channelTimeSkew = other.channelTimeSkew;
            this->channelTimeWalk = other.channelTimeWalk;
            this->channelTimeWalk1 = other.channelTimeWalk1;
            this->channelTimeWalk2 = other.channelTimeWalk2;
            this->channelTimeWalk3 = other.channelTimeWalk3;
            this->channelEnergyHistogram = other.channelEnergyHistogram;
            this->GlobalEnergy = other.GlobalEnergy;
            this->GlobalTime = other.GlobalTime;
            this->depthOfInteraction = other.depthOfInteraction;
            this->FloodHist = other.FloodHist;
            this->Sipm = other.Sipm;
            this->Id = other.Id;
        }
        return *this;
    }

    void initializeHistograms()
    {
        std::string diff =std::to_string(DIFERENTIATOR);
        if (VERBOSE){
            std::cout << "initializing histograms" << std::endl;
        std::cout << Id.getDetNum() << std::endl;
        }
        for (int detector = 0; detector < Id.getDetNum(); detector++)
        {
            Sipm.push_back(TMatrixT<double>(8, 8));
            std::vector<std::shared_ptr<TH1F>> auxHistChan;
            std::vector<std::shared_ptr<TH1F>> auxHistChanTimeSkew;
            std::vector<std::shared_ptr<TH2F>> auxHistChanTimeWalk;
            std::vector<std::shared_ptr<TH2F>> auxHistChanTimeWalk1;
            std::vector<std::shared_ptr<TH2F>> auxHistChanTimeWalk2;
            std::vector<std::shared_ptr<TH2F>> auxHistChanTimeWalk3;

            for (int chan = 0; chan < channelsPerArray; chan++)
            {
                string channelName{"channel-"};
                channelName = channelName + std::to_string(chan) + std::string(Id.getDetectorName(detector).c_str())+diff;
                if (VERBOSE)
                    std::cout << "initializing Energy histogram : " << channelName << std::endl;
                std::string energyHistoName = channelName + "-EnergyHisto"+diff;
                std::shared_ptr<TH1F> myHisto = std::make_shared<TH1F>(channelName.c_str(), (channelName+std::to_string(counter)).c_str(), H1ECBins, H1ECMin, H1ECMax);
                
                
                auxHistChan.push_back(myHisto);
                if (VERBOSE)
                    std::cout << "initializing Time Skew histogram : " << channelName << std::endl;
                std::string timeSkewHistoName = channelName + "-TimeSkew"+diff;
                std::shared_ptr<TH1F> myHisto2 = std::make_shared<TH1F>(timeSkewHistoName.c_str(), (timeSkewHistoName+std::to_string(counter)).c_str(), H1TCBins, H1TCMin, H1TCMax);
                auxHistChanTimeSkew.push_back(myHisto2);
                if (VERBOSE)
                    std::cout << "initializing Time Walk histogram : " << channelName << std::endl;
                std::string timeWalkHistoName = channelName + "-TimeWalk"+diff;
                std::shared_ptr<TH2F> myHisto3 = std::make_shared<TH2F>(timeWalkHistoName.c_str(), (timeWalkHistoName+std::to_string(counter)).c_str(), H2TBinsX, H2TMinX, H2TMaxX, H2TBinsY, H2TMinY, H2TMaxY);
                auxHistChanTimeWalk.push_back(myHisto3);
                if (VERBOSE)
                    std::cout << "initializing Time Walk histogram1 : " << channelName << std::endl;
                std::string timeWalkHistoName1 = channelName + "-TimeWalk1"+diff;
                std::shared_ptr<TH2F> myHisto4 = std::make_shared<TH2F>(timeWalkHistoName1.c_str(), (timeWalkHistoName1+std::to_string(counter)).c_str(), H2TBinsX, H2TMinX, H2TMaxX, H2TBinsY, H2TMinY, H2TMaxY);
                auxHistChanTimeWalk1.push_back(myHisto4);
                if (VERBOSE)
                    std::cout << "initializing Time Walk histogram2 : " << channelName << std::endl;
                std::string timeWalkHistoName2 = channelName + "-TimeWalk2"+diff;
                std::shared_ptr<TH2F> myHisto5 = std::make_shared<TH2F>(timeWalkHistoName2.c_str(), (timeWalkHistoName2+std::to_string(counter)).c_str(), H2TBinsX, H2TMinX, H2TMaxX, H2TBinsY, H2TMinY, H2TMaxY);
                auxHistChanTimeWalk2.push_back(myHisto5);
                if (VERBOSE)
                    std::cout << "initializing Time Walk histogram1 : " << channelName << std::endl;
                std::string timeWalkHistoName3 = channelName + "-TimeWalk3"+diff;
                std::shared_ptr<TH2F> myHisto6 = std::make_shared<TH2F>(timeWalkHistoName3.c_str(), (timeWalkHistoName3+std::to_string(counter)).c_str(), H2TBinsX, H2TMinX, H2TMaxX, H2TBinsY, H2TMinY, H2TMaxY);
                auxHistChanTimeWalk3.push_back(myHisto6);
            }

            string globalName = std::string(Id.getDetectorName(detector).c_str())+diff;
            std::shared_ptr<TH1F> myglobalHisto = std::make_shared<TH1F>(globalName.c_str(), (globalName+std::to_string(counter)).c_str(), H1EGBins, H1EGMin, H1EGMax);
            std::string floodName = globalName + "flood"+diff;
            std::shared_ptr<TH2F> flood = std::make_shared<TH2F>(floodName.c_str(), (floodName+std::to_string(counter)).c_str(), 256, -13.44, 13.44, 256, -13.44, 13.44);
            std::string DoiName = globalName + "- Doi"+diff;
            std::shared_ptr<TH1F> myDoi = std::make_shared<TH1F>(DoiName.c_str(), (DoiName+std::to_string(counter)).c_str(), 256, 0, 3);

            depthOfInteraction.push_back(myDoi);
            GlobalEnergy.push_back(myglobalHisto);
            FloodHist.push_back(flood);

            channelEnergyHistogram.push_back(auxHistChan);
            channelTimeSkew.push_back(auxHistChanTimeSkew);
            channelTimeWalk.push_back(auxHistChanTimeWalk);
            channelTimeWalk1.push_back(auxHistChanTimeWalk1);
            channelTimeWalk2.push_back(auxHistChanTimeWalk2);
            channelTimeWalk3.push_back(auxHistChanTimeWalk3);
        }
    }
};