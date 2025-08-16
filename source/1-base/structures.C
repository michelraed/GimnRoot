using ROOT::VecOps::RVec;

struct HistogramCoincidences
{
    // This structure will store all the histograms from a coincidence Acquisition
    std::vector<int> chipIDs;
    std::map<int, std::vector<std::shared_ptr<TH1F>>> rawEnergyChannel;
    std::map<int, std::vector<std::shared_ptr<TH1F>>> correctedEnergyChannel;
    std::map<int, std::vector<std::shared_ptr<TH1F>>> timeSkew;
    std::map<int, std::vector<std::shared_ptr<TH2F>>> timeWalk;

    std::map<int, std::shared_ptr<TH2F>> floodRaw;
    std::map<int, std::shared_ptr<TH2F>> floodCorrected;
    std::map<int, std::shared_ptr<TH1F>> globalEnergyHistogramRaw;
    std::map<int, std::shared_ptr<TH1F>> globalEnergyHistogramCorrected;

    void clear()
    {
        rawEnergyChannel.clear();
        correctedEnergyChannel.clear();
        timeSkew.clear();
        timeWalk.clear();
        floodRaw.clear();
        floodCorrected.clear();
        globalEnergyHistogramRaw.clear();
        globalEnergyHistogramCorrected.clear();
    }
};

struct HistosMaps
{
    std::vector<int> chipIDs;
    std::map<int, std::vector<std::shared_ptr<TH1F>>> channelRaw;
    std::map<int, std::vector<std::shared_ptr<TH1F>>> canaisCorNadig;
    std::map<int, std::vector<std::shared_ptr<TH1F>>> canaisCorWang;
    std::map<int, std::vector<std::shared_ptr<TH1F>>> canaisCorPoli;
    std::map<int, std::vector<std::shared_ptr<TH1F>>> canaisCorPetSys;

    std::map<int, std::shared_ptr<TH2F>> FloodsWang;
    std::map<int, std::shared_ptr<TH2F>> FloodsNadig;
    std::map<int, std::shared_ptr<TH2F>> FloodsPoli;
    std::map<int, std::shared_ptr<TH2F>> FloodsPetSys;

    std::map<int, std::shared_ptr<TH2F>> FloodsRaw;
    std::map<int, std::shared_ptr<TH2F>> FloodsWang_window;
    std::map<int, std::shared_ptr<TH2F>> FloodsNadig_window;
    std::map<int, std::shared_ptr<TH2F>> FloodsPoli_window;
    std::map<int, std::shared_ptr<TH2F>> FloodsPetSys_window;

    std::map<int, std::shared_ptr<TH1F>> GlobalHistsWang;
    std::map<int, std::shared_ptr<TH1F>> GlobalHistsNadig;
    std::map<int, std::shared_ptr<TH1F>> GlobalHistsRaw;
    std::map<int, std::shared_ptr<TH1F>> GlobalHistsPoli;
    std::map<int, std::shared_ptr<TH1F>> GlobalHistsPetSys;
};

struct correctionParameters
{
    std::vector<int> chanEquivalent;
    std::map<int, std::vector<std::map<std::string, double>>> fatoresDeCalibracaoNadig;
    std::map<int, std::vector<std::map<std::string, double>>> fatoresDeCalibracaoWang;
    std::map<int, std::vector<std::map<std::string, double>>> fatoresDeCalibracaoPoli;
    std::map<int, std::vector<std::map<std::string, double>>> fatoresDeCalibracaoPetSys;
};

struct GimnAQD
{

    int chipID;
    int port;
};

struct GimnEvent
{
    Long64_t CTR;
    Int_t channel_1{0};
    Float_t Energy_1{0};
    Float_t time_1{0};
    Float_t xmm_1{0};
    Float_t ymm_1{0};
    Float_t doi_1{0};
    Int_t Port_1{0};
    Int_t sipmX_1{0};
    Int_t sipmY_1{0};
    Int_t channel_2{0};
    Float_t Energy_2{0};
    Float_t time_2{0};
    Float_t xmm_2{0};
    Float_t ymm_2{0};
    Float_t doi_2{0};
    Int_t Port_2{0};
    Int_t sipmX_2{0};
    Int_t sipmY_2{0};
    Int_t coincidenceID{0};
    Int_t channel{0};
    Float_t Energy{0};
    Float_t time{0};
    Float_t xmm{0};
    Float_t ymm{0};
    Float_t doi{0};
    Int_t Port{0};
    Int_t sipmX{0};
    Int_t sipmY{0};
    Double_t angle{0};
    Double_t distance{0};
    Int_t slice{0};
};

struct GimnPulse
{

    RVec<Float_t> EnergyVec;
    RVec<Float_t> TimeVec;
    // RVec<Float_t> channelVec;

    Float_t *timeChanels = new Float_t[64];
    Float_t *energyChanels = new Float_t[64];
    // Float_t * channelNum =  new Float_t[64];
    int chipID = -1;
    int Port = -1;
    int chan = -1;
    Float_t first_e{0.0};
    Float_t first_t{0.0};
    int first_c{-1};
    Long64_t coincidenceID = -1;
    Float_t sipm_xmm{-1.0};
    Float_t sipm_ymm{-1.0};
    Float_t esum{-1};

    ~GimnPulse()
    {
        delete[] timeChanels;
        delete[] energyChanels;
    }

    void fillEnergyFromTmatrixT(TMatrixT<Float_t> e)
    {
        int counter = 0;
        for (int col = 0; col < e.GetNcols(); col++)
        {
            for (int row = 0; row < e.GetNrows(); row++)
            {
                if (counter < 64)
                    energyChanels[counter] = e(row, col);
                counter++;
            }
        }
    }
    void fillTimeFromTmatrixT(TMatrixT<Float_t> t)
    {
        int counter = 0;

        for (int col = 0; col < t.GetNcols(); col++)
        {

            for (int row = 0; row < t.GetNrows(); row++)

            {
                if (counter < 64)
                    timeChanels[counter] = t(row, col);

                counter++;
            }
        }
    }

    void FillEnergyChanelsWith(Float_t val)
    {
        for (int i = 0; i < 64; i++)
        {
            energyChanels[i] = val;
        }
    }
    void FillTimeChanelsWith(Float_t val)
    {
        for (int i = 0; i < 64; i++)
        {
            energyChanels[i] = val;
        }
    }

    void copyEnergyFromVector(std::vector<Float_t> energies)
    {
        int counter = 0;
        for (auto &element : energies)
        {
            if (counter < 64)
                energyChanels[counter] = element;
            counter++;
        }
    }
    void copyTimeFromVector(std::vector<Float_t> times)
    {
        int counter = 0;
        for (auto &element : times)
        {
            if (counter < 64)
                timeChanels[counter] = element;

            counter++;
        }
    }
    void copyEnergyTo(Float_t *en)
    {
        for (int i = 0; i < 64; i++)
        {
            en[i] = energyChanels[i];
        }
    }
    void copyTimeTo(Float_t *t)
    {

        for (int i = 0; i < 64; i++)
        {
            t[i] = timeChanels[i];
        }
    }
};

struct TomographicSingleEvent
{
    //
    // @brief This structure will contain all data for a given  event in a tomographic aquisition
    //
    Float_t globalPosXpre{0};
    Float_t globalPosYpre{0};
    Float_t globalPosZpre{0};

    Float_t globalPosX{0};
    Float_t globalPosY{0};
    Float_t globalPosZ{0};

    Float_t globalPosXpreAnger{0};
    Float_t globalPosYpreAnger{0};
    Float_t globalPosZpreAnger{0};

    Float_t globalPosXAnger{0};
    Float_t globalPosYAnger{0};
    Float_t globalPosZAnger{0};

    Int_t mh_n{-1};
    Int_t chipID{-1}, channelID{-1};
    Int_t posI{-1}, posJ{-1}, slice{-1};
    Float_t energy{-1};
    Float_t esum{-1};
    Float_t SiPMPosX{0}, SiPMPosY{0};
    Float_t angerPosX{0}, angerPosY{0};
    Float_t angleSino{-1};
    Float_t rSino{0};
    Float_t angleGantry{-1};
    Long64_t timeI{-1};
    Float_t timeSec{-1};
    Int_t coincidence_ID{-1};
    Float_t energyCorrected{-1};

    Float_t angleSinoAnger{-1};
    Float_t rSinoAnger{-1};

    TomographicSingleEvent() {}
    TomographicSingleEvent(const TomographicSingleEvent &other)
    {
        globalPosXpre = other.globalPosXpre;
        globalPosYpre = other.globalPosYpre;
        globalPosZpre = other.globalPosZpre;

        globalPosX = other.globalPosX;
        globalPosY = other.globalPosY;
        globalPosZ = other.globalPosZ;

        globalPosXpreAnger = other.globalPosXpreAnger;
        globalPosYpreAnger = other.globalPosYpreAnger;
        globalPosZpreAnger = other.globalPosZpreAnger;

        globalPosXAnger = other.globalPosXAnger;
        globalPosYAnger = other.globalPosYAnger;
        globalPosZAnger = other.globalPosZAnger;

        mh_n = other.mh_n;
        chipID = other.chipID;
        channelID = other.channelID;
        posI = other.posI;
        posJ = other.posJ;
        slice = other.slice;
        energy = other.energy;
        esum = other.esum;

        SiPMPosX = other.SiPMPosX;
        SiPMPosY = other.SiPMPosY;

        angerPosX = other.angerPosX;
        angerPosY = other.angerPosY;

        angleSino = other.angleSino;
        rSino = other.rSino;
        angleGantry = other.angleGantry;

        timeI = other.timeI;
        timeSec = other.timeSec;
        coincidence_ID = other.coincidence_ID;
        energyCorrected = other.energyCorrected;

        angleSinoAnger = other.angleSinoAnger;
        rSinoAnger = other.rSinoAnger;
    }
};

struct TomographicCoincidenceEvent
{
    //
    // @brief This structure will contain all data for a given  event in a tomographic aquisition
    //
    Int_t mh_n1{-1};
    Int_t chipID1{-1}, channelID1{-1};
    Int_t posI1{-1}, posJ1{-1}, slice1{-1};
    Float_t energy1{-1};
    Float_t esum1{-1};
    Float_t SiPMPosX1{0}, SiPMPosY1{0};
    Float_t angerPosX1{0}, angerPosY1{0};
    Float_t energyCorrected1{-1};

    Float_t angleSino1{-1};
    Float_t rSino1{0};
    Float_t angleGantry1{-1};
    Long64_t timeI1{-1};
    Float_t timeSec1{-1};
    Float_t globalPosXpre1{0};
    Float_t globalPosYpre1{0};
    Float_t globalPosZpre1{0};

    Float_t globalPosX1{0};
    Float_t globalPosY1{0};
    Float_t globalPosZ1{0};

    Int_t mh_n2{-1};
    Int_t chipID2{-1}, channelID2{-1};
    Int_t posI2{-1}, posJ2{-1}, slice2{-1};
    Float_t energy2{-1};
    Float_t esum2{-1};
    Float_t SiPMPosX2{0}, SiPMPosY2{0};
    Float_t angerPosX2{0}, angerPosY2{0};
    Float_t energyCorrected2{-1};

    Float_t angleSino2{-1};
    Float_t rSino2{0};
    Float_t angleGantry2{-1};
    Long64_t timeI2{-1};
    Float_t timeSec2{-1};

    Float_t globalPosXpre2{0};
    Float_t globalPosYpre2{0};
    Float_t globalPosZpre2{0};

    Float_t globalPosX2{0};
    Float_t globalPosY2{0};
    Float_t globalPosZ2{0};

    Float_t globalPosXpreAnger1{0};
    Float_t globalPosYpreAnger1{0};
    Float_t globalPosZpreAnger1{0};

    Float_t globalPosXAnger1{0};
    Float_t globalPosYAnger1{0};
    Float_t globalPosZAnger1{0};

    Float_t globalPosXpreAnger2{0};
    Float_t globalPosYpreAnger2{0};
    Float_t globalPosZpreAnger2{0};

    Float_t globalPosXAnger2{0};
    Float_t globalPosYAnger2{0};
    Float_t globalPosZAnger2{0};

    Float_t angleSinoAnger1{-1};
    Float_t angleSinoAnger2{-1};
    Float_t rSinoAnger1{-1};
    Float_t rSinoAnger2{-1};

    void fill(const TomographicSingleEvent &event1, const TomographicSingleEvent &event2)
    {
        // Evento 1
        mh_n1 = event1.mh_n;
        chipID1 = event1.chipID;
        channelID1 = event1.channelID;
        posI1 = event1.posI;
        posJ1 = event1.posJ;
        slice1 = event1.slice;
        energy1 = event1.energy;
        esum1 = event1.esum;
        SiPMPosX1 = event1.SiPMPosX;
        SiPMPosY1 = event1.SiPMPosY;
        angerPosX1 = event1.angerPosX;
        angerPosY1 = event1.angerPosY;
        angleSino1 = event1.angleSino;
        rSino1 = event1.rSino;
        angleGantry1 = event1.angleGantry;
        timeI1 = event1.timeI;
        timeSec1 = event1.timeSec;
        energyCorrected1 = event1.energyCorrected;

        globalPosX1 = event1.globalPosX;
        globalPosY1 = event1.globalPosY;
        globalPosZ1 = event1.globalPosZ;

        globalPosXpre1 = event1.globalPosXpre;
        globalPosYpre1 = event1.globalPosYpre;
        globalPosZpre1 = event1.globalPosZpre;

        globalPosXpreAnger1 = event1.globalPosXpreAnger;
        globalPosYpreAnger1 = event1.globalPosYpreAnger;
        globalPosZpreAnger1 = event1.globalPosZpreAnger;

        globalPosXAnger1 = event1.globalPosXAnger;
        globalPosYAnger1 = event1.globalPosYAnger;
        globalPosZAnger1 = event1.globalPosZAnger;

        angleSinoAnger1 = event1.angleSinoAnger;
        angleSinoAnger2 = event2.angleSinoAnger;
        rSinoAnger1 = event1.rSinoAnger;
        rSinoAnger2 = event2.rSinoAnger;

        // Evento 2
        mh_n2 = event2.mh_n;
        chipID2 = event2.chipID;
        channelID2 = event2.channelID;
        posI2 = event2.posI;
        posJ2 = event2.posJ;
        slice2 = event2.slice;
        energy2 = event2.energy;
        esum2 = event2.esum;
        SiPMPosX2 = event2.SiPMPosX;
        SiPMPosY2 = event2.SiPMPosY;
        angerPosX2 = event2.angerPosX;
        angerPosY2 = event2.angerPosY;
        angleSino2 = event2.angleSino;
        rSino2 = event2.rSino;
        angleGantry2 = event2.angleGantry;
        timeI2 = event2.timeI;
        timeSec2 = event2.timeSec;
        energyCorrected2 = event2.energyCorrected;

        globalPosX2 = event2.globalPosX;
        globalPosY2 = event2.globalPosY;
        globalPosZ2 = event2.globalPosZ;

        globalPosXpre2 = event2.globalPosXpre;
        globalPosYpre2 = event2.globalPosYpre;
        globalPosZpre2 = event2.globalPosZpre;

        globalPosXpreAnger2 = event2.globalPosXpreAnger;
        globalPosYpreAnger2 = event2.globalPosYpreAnger;
        globalPosZpreAnger2 = event2.globalPosZpreAnger;

        globalPosXAnger2 = event2.globalPosXAnger;
        globalPosYAnger2 = event2.globalPosYAnger;
        globalPosZAnger2 = event2.globalPosZAnger;
    }
};