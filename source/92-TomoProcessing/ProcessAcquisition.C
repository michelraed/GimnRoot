class ProcessAcquisition
{
private:
    int classCounter{0};
    //  Internal variables for reading and correcting the data from petsys

    Identificator identificator;
    correctionParameters cPar;
    correctionParameters lPar;
    std::map<int, std::map<std::string, float>> alignmentCorrection;

    SiPM sipm;
    HistogramCoincidences histograms;

    bool useEnergyCorrection = false;
    bool useLinearization = false;
    bool withPlots = false;
    bool useAlignment = false;
    bool verbose = true;
    bool obtainAlignmentData = false;

    std::map<int, std::map<int, double>> linearComponent;

    void initializeHistograms();

    // Functions to Loading The Correction Parameters;
    correctionParameters loadCalibrationFactors(std::string calibrationParameters);
    correctionParameters loadLinearizationFactors(std::string linearizationParameters);

    // Calibration Functions Factors;
    std::map<TString, Float_t> SaturationCorrectionNadig(std::map<int, double> peaks);
    std::map<TString, Float_t> correctionWang(std::map<int, double> peaks);
    std::map<TString, Float_t> polinomialCorrection(std::map<int, double> peaks);
    std::map<TString, Float_t> FindLinearizationPETSYS(std::map<int, double> peaks);

    // Internal Paths
    std::string rawFileName = "/home/gimn6/Documentos/Michel/produtos_moles/gimnpet/Simulations/time_360_t1_20_t2_20_te_15_ov_4_0_HW_OFF_qdcOVERVOLTAGE_VARIATION_coinc.root";
    std::string calibrationFileName = "/home/gimn6/Documentos/Michel/DOUTORADO_CARACTERIZACAO/RootOutputs/PreProcessed/Caracterization/OVERVOLTAGE_HIGH_COUNT/CalibrationFactors/time_360_t1_20_t2_20_te_15_ov_4_0_HW_OFF_qdcOVERVOLTAGE_VARIATION_group_pre_CalFactorsChannels.root";
    std::string calibrationName = "";
    std::string linearizationFileName = "";
    std::string outFileName = "";
    std::string identificatorPath = "/home/gimn6/Documentos/Michel/produtos_moles/gimnpet/GimnROOT/petsys_conf/identificators.conf";
    std::string outLinearizationName = "Linearization.root";
    std::string method = "PetSys";
    std::string fileName = "";
    std::string calibrationFolder = "";
    std::string plotsFolder = "";
    std::string processedFolder = "";
    std::string alignmentName = "";
    std::string alignmentFolder = "";
    std::string alignmentFileName = "";

public:
    // Configuration Parameteres

    inline void setPlots(bool plot) { withPlots = plot; }
    inline void initializeEmpty()
    {
        initializeHistograms();
        sipm = SiPM(64, 8, 8, 3.36, this->identificator);
    }
    inline void clear()
    {
        histograms.clear();
        initializeHistograms();
    }
    void ExtractNames(std::string inputFileName = "");

    // Setters

    inline void setRawFileName(const std::string &rawFileName) { this->rawFileName = rawFileName; }
    inline void setClassCounter(int classCounter) { this->classCounter = classCounter; }
    inline void setCalibrationName(const std::string &calibrationName) { this->calibrationName = calibrationName; }
    inline void setCalibrationFileName(const std::string &calibrationFileName) { this->calibrationFileName = calibrationFileName; }
    inline void setLinearizationFileName(const std::string &linearizationFileName) { this->linearizationFileName = linearizationFileName; }
    inline void setOutFileName(const std::string &outFileName) { this->outFileName = outFileName; }
    inline void setIdentificatorPath(const std::string &identificatorPath) { this->identificatorPath = identificatorPath; }
    inline void setOutLinearizationFileName(const std::string &outLinearizationName) { this->outLinearizationName = outLinearizationName; }
    inline void setMethod(const std::string &method) { this->method = method; }
    inline void setAlignmentName(std::string name) { this->alignmentName = name; }
    inline void setVerbose(bool v) { verbose = v; }

    inline void setObtainAlignmentData(bool b) { this->obtainAlignmentData = b; }

    inline void setAligmentFileName(std::string fileName)
    {
        this->alignmentFileName = fileName;
        auto splited = splitString(fileName, '/');
        this->alignmentName = splited[splited.size() - 1];
        splited.pop_back();
        this->alignmentFolder = joinString(splited, '/');
        this->readAligment();

        if (verbose)
        {
            std::cout << "#-------------------------------------------------------------------#" << std::endl;
            std::cout << "#                                        Alignment read              " << std::endl;
            std::cout << "# File path :     " << alignmentFileName << std::endl;
            std::cout << "# Folder path :   " << alignmentFolder << std::endl;
            std::cout << "# File Name :     " << alignmentName << std::endl;
            std::cout << "#-------------------------------------------------------------------#" << std::endl;

            std::cout << "Read Values : " << std::endl;
            for (auto &chd : identificator.chipID)
            {
                std::cout << "chip ID : " << chd << " X alignment " << alignmentCorrection[chd]["x"] << std::endl;
                std::cout << "chip ID : " << chd << " Y alignment " << alignmentCorrection[chd]["y"] << std::endl;
            }
        }
    }

    inline void setAlignmentFolder(std::string folderName)
    {
        this->alignmentFolder = folderName;
        if (this->alignmentFolder[int(this->calibrationFolder.size() - 1)] != '/')
            this->alignmentFolder = this->alignmentFolder + "/";
    }

    inline void setCalibrationFolder(std::string calibrationFolder)
    {
        this->calibrationFolder = calibrationFolder;
        if (this->calibrationFolder[int(this->calibrationFolder.size() - 1)] != '/')
            this->calibrationFolder = this->calibrationFolder + "/";
    }

    inline void setPlotsFolder(std::string plotsFolder)
    {
        this->plotsFolder = plotsFolder;
        if (this->plotsFolder[int(this->plotsFolder.size() - 1)] != '/')
            this->plotsFolder = this->plotsFolder + "/";

        withPlots = true;
    }
    inline void setProcessedFolder(std::string processedFolder)
    {
        this->processedFolder = processedFolder;
        if (this->processedFolder[int(this->processedFolder.size() - 1) != '/'])
            this->processedFolder = this->processedFolder + "/";
    }
    void setIdentificator(std::string identificatorPath)
    {
        this->identificatorPath = identificatorPath;
        identificator.LoadIdentificators(identificatorPath);
    }


    void setCalibrationFactors(std::string CalibrationFactorsPath)
    {
        useEnergyCorrection = true;
        setCalibrationFileName(CalibrationFactorsPath);
        this->cPar = loadCalibrationFactors(CalibrationFactorsPath);
    }

    void setLinearizationFactors(std::string LinearizationFactorsPath)
    {
        setLinearizationFileName(LinearizationFactorsPath);
        this->lPar = loadLinearizationFactors(LinearizationFactorsPath);
    }

    // Getters
    inline std::string getRawFileName() const { return rawFileName; }
    inline std::string getCalibrationFileName() { return this->calibrationFileName; }
    inline std::string getCalibrationName() { return this->calibrationName; }
    inline std::string getLinearizationFileName() { return this->linearizationFileName; }
    inline std::string getOutFileName() { return this->outFileName; }
    inline std::string getIdentificatorPath() const { return identificatorPath; }
    inline std::string getOutLinearizationFileName() { return this->outLinearizationName; }
    inline std::string getMethod() const { return method; }
    inline std::string getCalibrationFolder() { return this->calibrationFolder; }
    inline std::string getPlotsFolder() { return this->plotsFolder; }
    inline std::string getProcessedFolder() { return this->processedFolder; }
    std::string getAlignmentName() { return this->alignmentName; }
    std::string getAlignmentFolder() { return this->alignmentFolder; }
    std::string getAligmentFileName() { return this->alignmentFileName; }
    // Configuration Files

    Identificator getIdentificator() const { return identificator; }
    correctionParameters getCorrectionParameters() const { return cPar; }
    HistogramCoincidences getHistograms() const { return histograms; }
    SiPM getSiPM() const { return sipm; }

    // Calibration

    void saveAligment(HistogramCoincidences &Histos);
    void checkNeedLinearization();
    void Calibrate();
    void LinearizeEnergies();  // will perform the linearization step of the histograms
    void FillVoidParameters(); // will fill cpar and lpar with 0s
    void writeCalibration();
    void writeLinearization();
    double correctEnergy(int chipID, int channel, double energy, std::string method);
    void readAligment();
    // Processing
    std::map<int, double> FindAndMarkPeaks(TH1F *histo, Float_t sigma, Float_t Threshold);
    void processCoincidenceFromRaw();
    void ProcessGroupFromRaw();
    void ProcessGroupFromRaw(std::string aqdName);
    void ProcessGroupTomographic(std::string aqdName);
    void processCoincidenceTomographic(std::string fileName, std::string rotateAlong = "y", std::string gantryCrystalRotation = "y");

    // Constructors
    ProcessAcquisition(const ProcessAcquisition &other);
    ProcessAcquisition &operator=(const ProcessAcquisition &other);
    ProcessAcquisition(int counter);
    ProcessAcquisition();
    ProcessAcquisition(std::string rawFileName);
    ProcessAcquisition(std::string rawFileName, std::string identificatorPath);
    ProcessAcquisition(std::string rawFileName, std::string calibrationFileName, std::string identificatorPath, std::string method);
    ProcessAcquisition(std::string rawFileName, std::string calibrationFileName, std::string linearizationFileName, std::string identificatorPath, std::string method);

    void writeMatrixOfDetectors(HistogramCoincidences histograms);
    void writeMatrixOfDetectorsRaw(HistogramCoincidences histograms);
    void plotChannelHistogramRaw(int ch, int chipID);
    void writeMatrixOfDetectorsWhithHeatMap(HistogramCoincidences histograms);
    void plotGlobalHistogramRaw(int chipID);

};

/*

Method Implementations

*/

// CONSTRUCTOR METHODS
//  Default constructor

ProcessAcquisition::ProcessAcquisition(int counter)
{
    // ROOT::EnableImplicitMT();
    ROOT::EnableThreadSafety();
    this->classCounter = counter;
    std::cout << "Please: set the configurations Path" << std::endl;
    std::cout << "Class Counter << " << classCounter << std::endl;
}

ProcessAcquisition::ProcessAcquisition()
{
    //ROOT::EnableImplicitMT();
    ROOT::EnableThreadSafety();
    std::cout << "Please: set the configurations Path" << std::endl;
    std::cout << "Class Counter << " << classCounter << std::endl;
}
// Constructor with rawFileName and calibrationFileName

// Constructor with rawFileName only
ProcessAcquisition::ProcessAcquisition(std::string rawFileName)
{
    //ROOT::EnableImplicitMT();
    ROOT::EnableThreadSafety();
    this->rawFileName = rawFileName;
    ExtractNames();
    FillVoidParameters();
}

ProcessAcquisition::ProcessAcquisition(std::string rawFileName, std::string identificatorPath)
{
    // gROOT->SetBatch(kTRUE);
    // ROOT::EnableImplicitMT();
    ROOT::EnableThreadSafety();

    this->rawFileName = rawFileName;
    this->setIdentificator(identificatorPath);
    sipm = SiPM(64, 8, 8, 3.36, this->identificator);
    ExtractNames();
    initializeHistograms();
    FillVoidParameters();
}

// Constructor with all parameters
ProcessAcquisition::ProcessAcquisition(std::string rawFileName, std::string calibrationFileName, std::string identificatorPath, std::string method)
{
    // ROOT::EnableImplicitMT();
    ROOT::EnableThreadSafety();
    this->rawFileName = rawFileName;
    this->calibrationFileName = calibrationFileName;
    this->identificatorPath = identificatorPath;
    this->method = method;
    sipm = SiPM(64, 8, 8, 3.36, this->identificator);
    this->cPar = loadCalibrationFactors(calibrationFileName);
    this->lPar = this->cPar;
    ExtractNames();
    initializeHistograms();
}

ProcessAcquisition::ProcessAcquisition(std::string rawFileName, std::string calibrationFileName, std::string linearizationFileName, std::string identificatorPath, std::string method)
{

    // ROOT::EnableImplicitMT();
    ROOT::EnableThreadSafety();
    this->rawFileName = rawFileName;
    this->calibrationFileName = calibrationFileName;
    this->identificatorPath = identificatorPath;
    this->method = method;
    this->setIdentificator(identificatorPath);
    ExtractNames();
    sipm = SiPM(64, 8, 8, 3.36, this->identificator);
    this->cPar = loadCalibrationFactors(calibrationFileName);
    this->lPar = loadLinearizationFactors(linearizationFileName);
    initializeHistograms();
}

// Copy constructor
ProcessAcquisition::ProcessAcquisition(const ProcessAcquisition &other)
    : identificator(other.identificator),
      // coinc(std::make_shared<PetsysCoincidences>(*other.coinc)),
      // group(std::make_shared<PetsysGroup>(*other.group)),
      cPar(other.cPar),
      histograms(other.histograms),
      sipm(other.sipm),
      lPar(other.lPar),
      rawFileName(other.rawFileName),
      calibrationFileName(other.calibrationFileName),
      outFileName(other.outFileName),
      identificatorPath(other.identificatorPath),
      method(other.method),
      classCounter(other.classCounter)
{

    initializeHistograms();
    ExtractNames();
}

// OPERATORS
// Copy assignment operator
ProcessAcquisition &ProcessAcquisition::operator=(const ProcessAcquisition &other)
{
    if (this == &other)
        return *this;
    classCounter = other.classCounter;
    identificator = other.identificator;
    // coinc = std::make_shared<PetsysCoincidences>(*other.coinc);
    // group = std::make_shared<PetsysGroup>(*other.group);

    cPar = other.cPar;
    histograms = other.histograms;
    sipm = other.sipm;
    rawFileName = other.rawFileName;
    calibrationFileName = other.calibrationFileName;
    outFileName = other.outFileName;
    identificatorPath = other.identificatorPath;
    method = other.method;
    lPar = other.lPar;

    initializeHistograms();

    return *this;
}

void ProcessAcquisition::initializeHistograms()
{
    for (auto chipID : identificator.getChipIDs())
    {
        // std::shared_ptr<TH2F> RawFlood(new TH2F(Form("Flood Hist - Raw - %d-%d", chipID, classCounter), Form("Flood Hist - Raw  ChipID-%d", chipID), 128, -13.44, 13.44, 256, -13.44, 13.44));
        std::shared_ptr<TH2F> RawFlood(new TH2F(Form("Flood Hist - Raw - %d-%d", chipID, classCounter), Form("Flood Hist - Raw  ChipID-%d", chipID), 128, 0, 0, 128, 0, 0));
        std::shared_ptr<TH2F> CorrectedFlood(new TH2F(Form("Flood Hist - Corrected %s- %d-%d", method.c_str(), chipID, classCounter), Form("Flood Hist -  Corrected %s ChipID-%d", method.c_str(), chipID), 128, 0, 0, 128, 0, 0));
        std::shared_ptr<TH1F> globalHistoRaw(new TH1F(Form("global corrected Raw ChipID-%d-%d", chipID, classCounter), Form("global corrected Nadig ChipID-%d", chipID), 200, 0, 0));
        std::shared_ptr<TH1F> globalHistoCorrected(new TH1F(Form("global corrected %s ChipID-%d-%d", method.c_str(), chipID, classCounter), Form("global corrected corrected %s ChipID-%d", method.c_str(), chipID), 200, 0, 1400));

        histograms.chipIDs.push_back(chipID);
        histograms.floodRaw[chipID] = RawFlood;
        histograms.floodCorrected[chipID] = CorrectedFlood;
        histograms.globalEnergyHistogramRaw[chipID] = globalHistoRaw;
        histograms.globalEnergyHistogramCorrected[chipID] = globalHistoCorrected;

        std::vector<std::shared_ptr<TH1F>> channelRaw;
        std::vector<std::shared_ptr<TH1F>> channelCor;
        std::vector<std::shared_ptr<TH1F>> timeSkew;
        std::vector<std::shared_ptr<TH2F>> timeWalk;

        int nchan = sipm.getRows() * sipm.getCols();
        for (int ch = 0; ch < nchan; ch++)
        {
            linearComponent[chipID][ch] = 0.0;
            std::string cname = Form("canal-%d", ch);
            std::string gname1 = cname + "-" + std::to_string(chipID) + "Raw";
            std::string gname2 = cname + "-" + std::to_string(chipID) + "Corrected " + method;
            std::string gname3 = cname + "-" + std::to_string(chipID) + "TimeSkew";
            std::string gname4 = cname + "-" + std::to_string(chipID) + "timeWalk";

            std::string gname1D = gname1 + "-" + std::to_string(classCounter);
            std::string gname2D = gname2 + "-" + std::to_string(classCounter);
            std::string gname3D = gname3 + "-" + std::to_string(classCounter);
            std::string gname4D = gname4 + "-" + std::to_string(classCounter);

            // std::cout<<"gname1D : "<<gname1D<<std::endl;
            // std::cout<<"gname2D : "<<gname2D<<std::endl;
            // std::cout<<"gname3D : "<<gname3D<<std::endl;
            // std::cout<<"gname4D : "<<gname4D<<std::endl;

            std::shared_ptr<TH1F> aux1(new TH1F(gname1D.c_str(), gname1D.c_str(), 200, 0, 0));
            std::shared_ptr<TH1F> aux2(new TH1F(gname2D.c_str(), gname2D.c_str(), 200, 0, 1500));
            std::shared_ptr<TH1F> aux3(new TH1F(gname3D.c_str(), gname3D.c_str(), 200, 0, 0));
            std::shared_ptr<TH2F> aux4(new TH2F(gname4D.c_str(), gname4.c_str(), 200, 0, 0, 200, 0, 0));

            channelRaw.push_back(aux1);
            channelCor.push_back(aux2);
            timeSkew.push_back(aux3);
            timeWalk.push_back(aux4);
        }

        histograms.rawEnergyChannel[chipID] = channelRaw;
        histograms.correctedEnergyChannel[chipID] = channelCor;
        histograms.timeSkew[chipID] = timeSkew;
        histograms.timeWalk[chipID] = timeWalk;
    }
}

double ProcessAcquisition::correctEnergy(int chipID, int channel, double energy, std::string method)
{
    double correctedEnergy = 0;
    if (method == "Nadig")
    {
        double c = cPar.fatoresDeCalibracaoNadig[chipID][channel]["c"];
        double s = cPar.fatoresDeCalibracaoNadig[chipID][channel]["s"];
        correctedEnergy = c * s * log(1 / (1 - energy / s));
    }
    else if (method == "Wang")
    {
        double k1 = cPar.fatoresDeCalibracaoWang[chipID][channel]["k1"];
        double k2 = cPar.fatoresDeCalibracaoWang[chipID][channel]["k2"];
        double b = cPar.fatoresDeCalibracaoWang[chipID][channel]["b"];
        correctedEnergy = log(k1 / (k1 + b - energy)) * (1 / k2);
    }
    else if (method == "Poli")
    {
        double pa = cPar.fatoresDeCalibracaoPoli[chipID][channel]["pa"];
        double pb = cPar.fatoresDeCalibracaoPoli[chipID][channel]["pb"];
        double pc = cPar.fatoresDeCalibracaoPoli[chipID][channel]["pc"];
        correctedEnergy = pa * pow(energy, 2) + pb * energy + pc;
    }
    else if (method == "PetSys")
    {
        double P0 = cPar.fatoresDeCalibracaoPetSys[chipID][channel]["P0"];
        double P1 = cPar.fatoresDeCalibracaoPetSys[chipID][channel]["P1"];
        double P2 = cPar.fatoresDeCalibracaoPetSys[chipID][channel]["P2"];
        double P3 = cPar.fatoresDeCalibracaoPetSys[chipID][channel]["P3"];
        correctedEnergy = P0 * pow(P1, pow(energy, P2)) + P3 * energy + P0;
    }
    return correctedEnergy;
}

void ProcessAcquisition::processCoincidenceFromRaw()
{
    // #COINCIDENCE

    // Variables that are going to be needed
    int coincidenceID{0};                                                  // id from the coincidence
    double esum1{0}, esum2{0};                                             // stores energy sum
    int firstChannel1{0}, firstChannel2{0};                                // first events to get winer take all
    int initial_event{0}, ini_n1{0}, ini_n2{0};                            // first events to get winer take all
    double firstEnergy1{0}, firstEnergy2{0}, firstTime1{0}, firstTime2{0}; // first events to get winer take all
    double firstPosition1x{0}, firstPosition2x{0};                         // first events to get winer take all
    double firstPosition1y{0}, firstPosition2y{0};                         // first events to get winer take all
    int groupSize = 1;

    // cria as variaveis nescessárias à leitura do arquivo root
    UShort_t mh_n1{0}, mh_j1{0}, mh_n2{0}, mh_j2{0};
    UInt_t channelID1{0}, channelID2{0};
    Float_t energy1{0.0}, energy2{0};
    Long64_t time1{0}, time2{0};

    std::vector<Long64_t> timeChanels1(64, 0);
    std::vector<Float_t> energyChannels1(64, 0);
    std::vector<Long64_t> timeChanels2(64, 0);
    std::vector<Float_t> energyChannels2(64, 0);

    // abre o arquivo root
    std::shared_ptr<TFile> file(new TFile(rawFileName.c_str(), "read"));
    std::shared_ptr<TTree> tree((TTree *)file->Get("data"));

    // set branches
    // Group 1
    tree->SetBranchAddress("mh_n1", &mh_n1);
    tree->SetBranchAddress("mh_j1", &mh_j1);
    tree->SetBranchAddress("channelID1", &channelID1);
    tree->SetBranchAddress("energy1", &energy1);
    tree->SetBranchAddress("time1", &time1);
    // Group 2
    tree->SetBranchAddress("mh_n2", &mh_n2);
    tree->SetBranchAddress("mh_j2", &mh_j2);
    tree->SetBranchAddress("channelID2", &channelID2);
    tree->SetBranchAddress("energy2", &energy2);
    tree->SetBranchAddress("time2", &time2);

    int entries = tree->GetEntries();

    std::shared_ptr<GimnWriteClass> Output;
    if (outFileName.empty())
    {
        std::string outName = removePart(fileName, ".root");
        outName = processedFolder + outName + "_processed.root";
        Output = std::make_shared<GimnWriteClass>(outName.c_str());
    }
    else
    {
        std::string outName = processedFolder + outFileName;
        Output = std::make_shared<GimnWriteClass>(outName.c_str());
        // GimnWriteClass Output(outName.c_str());
    }

    for (int entry = 0; entry < entries; entry++)
    {
        tree->GetEntry(entry);
        int pos1 = -1;
        int pos2 = -1;
        bool isValid = true;

        // Collects the chipID and channel information from global channel
        // for both detectors of a given coincidence
        Int_t chipID1 = (int)channelID1 / 64;
        Int_t channel1 = channelID1 - 64 * chipID1;

        Int_t chipID2 = (int)channelID2 / 64;
        Int_t channel2 = channelID2 - 64 * chipID2;

        // check if the number of events in the group is greater than the minimum size
        if ((mh_n1 >= groupSize) && (mh_n2 >= groupSize))
        {

            // It will check on loaded data from identificators where this chip ID should Fit
            auto it = std::find(identificator.chipID.begin(), identificator.chipID.end(), chipID1);
            if (it != identificator.chipID.end())
                pos1 = std::distance(identificator.chipID.begin(), it);
            else
                isValid = false;

            // gets the position of chipID2 on the identificator data.
            it = std::find(identificator.chipID.begin(), identificator.chipID.end(), chipID2);
            if (it != identificator.chipID.end())
                pos2 = std::distance(identificator.chipID.begin(), it);
            else
                isValid = false;

            // Check if both detectors are in an aceptable coincidenece,
            // in case of using several coincidence options
            if (!identificator.checkInCoincidence(pos1, chipID2))
            {
                isValid = false;
            }

            if (isValid)
            {
                auto pos_xy1 = sipm.CorrectChannelToPixelPs(channel1);
                auto pos_xy2 = sipm.CorrectChannelToPixelPs(channel2);

                // Correct the energy and put on the histograms
                auto correctedEnergy1 = correctEnergy(chipID1, channel1, energy1, method) + linearComponent[chipID1][channel1];
                auto correctedEnergy2 = correctEnergy(chipID2, channel2, energy2, method) + linearComponent[chipID2][channel2];

                // Fill times
                Float_t timeInSeconds1 = (float)time1 / 1.0e12;
                Float_t timeInSeconds2 = (float)time2 / 1.0e12;

                Long64_t dt = time1 - time2;
                Float_t timeDiference = (Float_t)dt / 1.0e12;

                if (mh_j1 == 0 && mh_j2 == 0) // First Event
                {
                    coincidenceID++;
                    initial_event = entry;
                    ini_n1 = mh_n1;
                    ini_n2 = mh_n2;

                    histograms.rawEnergyChannel[chipID1][channel1]->Fill(energy1);
                    histograms.rawEnergyChannel[chipID2][channel2]->Fill(energy2);
                    histograms.correctedEnergyChannel[chipID1][channel1]->Fill(correctedEnergy1);
                    histograms.correctedEnergyChannel[chipID2][channel2]->Fill(correctedEnergy2);
                    histograms.globalEnergyHistogramRaw[chipID1]->Fill(energy1);
                    histograms.globalEnergyHistogramRaw[chipID2]->Fill(energy2);
                    histograms.globalEnergyHistogramCorrected[chipID1]->Fill(correctedEnergy1);
                    histograms.globalEnergyHistogramCorrected[chipID2]->Fill(correctedEnergy2);

                    sipm.Sipm_e[pos1](pos_xy1[0], pos_xy1[1]) = (float)energy1;
                    sipm.Sipm_e[pos2](pos_xy2[0], pos_xy2[1]) = (float)energy2;
                    sipm.Sipm_ecor[pos1](pos_xy1[0], pos_xy1[1]) = correctedEnergy1;
                    sipm.Sipm_ecor[pos2](pos_xy2[0], pos_xy2[1]) = correctedEnergy2;

                    firstPosition1x = sipm.getPositionsX()[pos_xy1[0]];
                    firstPosition1y = sipm.getPositionsY()[pos_xy1[1]];
                    firstPosition2x = sipm.getPositionsX()[pos_xy2[0]];
                    firstPosition2y = sipm.getPositionsX()[pos_xy2[1]];

                    firstEnergy1 = correctedEnergy1;
                    firstEnergy2 = correctedEnergy2;
                    firstChannel1 = channel1;
                    firstChannel2 = channel2;
                    firstTime1 = timeInSeconds1;
                    firstTime2 = timeInSeconds2;

                    sipm.Sipm_t[pos1](pos_xy1[0], pos_xy1[1]) = timeInSeconds1;
                    sipm.Sipm_t[pos2](pos_xy2[0], pos_xy2[1]) = timeInSeconds2;

                    // FIll TimeWalk and Time Skew
                    histograms.timeSkew[chipID1][channel1]->Fill(timeDiference);
                    histograms.timeSkew[chipID2][channel2]->Fill(timeDiference);
                    histograms.timeWalk[chipID1][channel1]->Fill(energy1 * timeDiference, timeDiference);
                    histograms.timeWalk[chipID2][channel2]->Fill(energy2 * timeDiference, timeDiference);
                }
                else if (mh_j1 != 0 && mh_j2 == 0) // Intermediary event group1 goes until last event
                {
                    sipm.Sipm_e[pos1](pos_xy1[0], pos_xy1[1]) = +energy1;
                    sipm.Sipm_ecor[pos1](pos_xy1[0], pos_xy1[1]) = +correctedEnergy1;
                    sipm.Sipm_t[pos1](pos_xy1[0], pos_xy1[1]) = +timeInSeconds1;
                }
                else if (mh_j1 == 0 && mh_j2 != 0) // Intermediary event group1 goes until last event
                {
                    sipm.Sipm_e[pos2](pos_xy2[0], pos_xy2[1]) = +energy2;
                    sipm.Sipm_ecor[pos2](pos_xy2[0], pos_xy2[1]) = +correctedEnergy2;
                    sipm.Sipm_t[pos2](pos_xy2[0], pos_xy2[1]) = +timeInSeconds2;
                }
                if (entry == (initial_event + (ini_n1 + ini_n2) - 2)) // last event itself
                {
                    GimnEvent ev;
                    ev.Energy_1 = firstEnergy1;
                    ev.time_1 = firstTime1;
                    ev.channel_1 = firstChannel1;

                    // Calculate Position for corrected energy
                    auto pos3D1 = anger(sipm.Sipm_ecor[pos1]);
                    auto pos3D2 = anger(sipm.Sipm_ecor[pos2]);

                    // Calculate Position for raw energy
                    auto pos3Dnc1 = anger(sipm.Sipm_e[pos1]);
                    auto pos3Dnc2 = anger(sipm.Sipm_e[pos2]);

                    // using first energy for winner take all
                    ev.xmm_1 = pos3D1[0];
                    ev.ymm_1 = pos3D1[1];
                    ev.doi_1 = pos3D1[2];
                    ev.Port_1 = chipID1 / 2;
                    ev.sipmX_1 = firstPosition1x;
                    ev.sipmY_1 = firstPosition1y;
                    ev.Energy_2 = firstEnergy2;
                    ev.time_2 = firstTime2;
                    ev.channel_2 = firstChannel2;
                    ev.xmm_2 = pos3D2[0];
                    ev.ymm_2 = pos3D2[1];
                    ev.doi_2 = pos3D2[2];
                    ev.Port_2 = chipID2 / 2;
                    ev.sipmX_2 = firstPosition2x;
                    ev.sipmY_2 = firstPosition2y;
                    ev.coincidenceID = coincidenceID;

                    histograms.floodRaw[chipID1]->Fill(pos3Dnc1[0], pos3Dnc1[1]);
                    histograms.floodRaw[chipID2]->Fill(pos3Dnc2[0], pos3Dnc2[1]);
                    histograms.floodCorrected[chipID1]->Fill(pos3D1[0], pos3D1[1]);
                    histograms.floodCorrected[chipID2]->Fill(pos3D2[0], pos3D2[1]);

                    Output->putData(ev);
                    sipm.Sipm_e[pos1].Zero();
                    sipm.Sipm_e[pos2].Zero();
                    sipm.Sipm_ecor[pos1].Zero();
                    sipm.Sipm_ecor[pos2].Zero();
                    sipm.Sipm_t[pos1].Zero();
                    sipm.Sipm_t[pos2].Zero();
                }
            }
            else
            {
            }
        }
    }
    writeMatrixOfDetectors(histograms);
    Output->writeObject(histograms, identificator, method);
    Output->close();
}

void ProcessAcquisition::FillVoidParameters()
{

    int nchans = 64;
    std::vector<int> chanEquivalent(nchans, 0);

    std::map<int, std::vector<std::map<std::string, double>>> fatoresDeCalibracaoNadig;
    std::map<int, std::vector<std::map<std::string, double>>> fatoresDeCalibracaoWang;
    std::map<int, std::vector<std::map<std::string, double>>> fatoresDeCalibracaoPoli;
    std::map<int, std::vector<std::map<std::string, double>>> fatoresDeCalibracaoPetSys;

    for (auto &val : identificator.chipID)
    {

        for (int ch = 0; ch < nchans; ch++)
        {

            std::map<std::string, Double_t> parametrosNadig;
            parametrosNadig["c"] = -1.0;
            parametrosNadig["s"] = -1.0;
            fatoresDeCalibracaoNadig[val].push_back(parametrosNadig);

            std::map<std::string, Double_t> parametrosWang;
            parametrosWang["k1"] = -1.0;
            parametrosWang["k2"] = -1.0;
            parametrosWang["b"] = -1.0;
            fatoresDeCalibracaoWang[val].push_back(parametrosWang);

            std::map<std::string, Double_t> parametrosPoli;
            parametrosPoli["pa"] = -1.0;
            parametrosPoli["pb"] = -1.0;
            parametrosPoli["pc"] = -1.0;
            fatoresDeCalibracaoPoli[val].push_back(parametrosPoli);

            std::map<std::string, Double_t> parametrosPetSys;
            parametrosPetSys["P0"] = -1.0;
            parametrosPetSys["P1"] = -1.0;
            parametrosPetSys["P2"] = -1.0;
            parametrosPetSys["P3"] = -1.0;
            fatoresDeCalibracaoPetSys[val].push_back(parametrosPetSys);

            auto pos_xy = sipm.CorrectChannelToPixelPs(ch);
            int equivalent = 8 * pos_xy[1] + pos_xy[0];
            chanEquivalent[ch] = equivalent;
        }
    }
    correctionParameters cParam;
    cParam.fatoresDeCalibracaoNadig = fatoresDeCalibracaoNadig;
    cParam.fatoresDeCalibracaoWang = fatoresDeCalibracaoWang;
    cParam.fatoresDeCalibracaoPoli = fatoresDeCalibracaoPoli;
    cParam.fatoresDeCalibracaoPetSys = fatoresDeCalibracaoPetSys;
    cParam.chanEquivalent = chanEquivalent;

    this->cPar = cParam;
    this->lPar = cParam;
}

correctionParameters ProcessAcquisition::loadCalibrationFactors(std::string calibrationParameters)
{

    this->useEnergyCorrection = true;
    // declares root Dataframe for retrieving calibration parameters
    ROOT::RDataFrame CalibrationFactors("CalibrationFactors", calibrationParameters.c_str());

    // Utils
    std::vector<Float_t> xPos = this->sipm.getPositionsX();
    std::vector<Float_t> yPos = this->sipm.getPositionsY();

    int nchans = 64;
    std::vector<int> chanEquivalent(nchans, 0);

    std::map<int, std::vector<std::map<std::string, double>>> fatoresDeCalibracaoNadig;
    std::map<int, std::vector<std::map<std::string, double>>> fatoresDeCalibracaoWang;
    std::map<int, std::vector<std::map<std::string, double>>> fatoresDeCalibracaoPoli;
    std::map<int, std::vector<std::map<std::string, double>>> fatoresDeCalibracaoPetSys;

    std::vector<int> chipIDs;

    auto chipIDCounter = [&chipIDs](int chipID)
    {
        static int lastChipID = -1;
        if (chipID != lastChipID)
        {
            lastChipID = chipID;
            chipIDs.push_back(chipID);
        }
    };

    CalibrationFactors.Foreach(chipIDCounter, {"chipID"});

    for (auto &val : chipIDs)
    {

        for (int ch = 0; ch < nchans; ch++)
        {

            std::map<std::string, Double_t> parametrosNadig;
            parametrosNadig["c"] = -1.0;
            parametrosNadig["s"] = -1.0;
            fatoresDeCalibracaoNadig[val].push_back(parametrosNadig);

            std::map<std::string, Double_t> parametrosWang;
            parametrosWang["k1"] = -1.0;
            parametrosWang["k2"] = -1.0;
            parametrosWang["b"] = -1.0;
            fatoresDeCalibracaoWang[val].push_back(parametrosWang);

            std::map<std::string, Double_t> parametrosPoli;
            parametrosPoli["pa"] = -1.0;
            parametrosPoli["pb"] = -1.0;
            parametrosPoli["pc"] = -1.0;
            fatoresDeCalibracaoPoli[val].push_back(parametrosPoli);

            std::map<std::string, Double_t> parametrosPetSys;
            parametrosPetSys["P0"] = -1.0;
            parametrosPetSys["P1"] = -1.0;
            parametrosPetSys["P2"] = -1.0;
            parametrosPetSys["P3"] = -1.0;
            fatoresDeCalibracaoPetSys[val].push_back(parametrosPetSys);

            auto pos_xy = sipm.CorrectChannelToPixelPs(ch);
            int equivalent = 8 * pos_xy[1] + pos_xy[0];
            chanEquivalent[ch] = equivalent;
        }
    }

    auto ParameterReader = [&fatoresDeCalibracaoNadig, &fatoresDeCalibracaoWang, &fatoresDeCalibracaoPoli, &fatoresDeCalibracaoPetSys](int chipID, int channel, double c, double s, double k1, double k2, double b, double pa, double pb, double pc, double P0, double P1, double P2, double P3)
    {
        // std::cout << "chipID " << chipID << " channel " << channel << " c " << c << " s " << s << " k1 " << k1 << " k2 " << k2 << " b " << b << " pa " << pa << " pb " << pb << " pc " << pc << " P0 " << P0 << " P1 " << P1 << " P2 " << P2 << " P3 " << P3 << std::endl;
        fatoresDeCalibracaoNadig[chipID][channel]["c"] = c;
        fatoresDeCalibracaoNadig[chipID][channel]["s"] = s;

        fatoresDeCalibracaoWang[chipID][channel]["k1"] = k1;
        fatoresDeCalibracaoWang[chipID][channel]["k2"] = k2;
        fatoresDeCalibracaoWang[chipID][channel]["b"] = b;

        fatoresDeCalibracaoPoli[chipID][channel]["pa"] = pa;
        fatoresDeCalibracaoPoli[chipID][channel]["pb"] = pb;
        fatoresDeCalibracaoPoli[chipID][channel]["pc"] = pc;

        fatoresDeCalibracaoPetSys[chipID][channel]["P0"] = P0;
        fatoresDeCalibracaoPetSys[chipID][channel]["P1"] = P1;
        fatoresDeCalibracaoPetSys[chipID][channel]["P2"] = P2;
        fatoresDeCalibracaoPetSys[chipID][channel]["P3"] = P3;
    };

    CalibrationFactors.Foreach(ParameterReader, {"chipID", "channel", "c", "s", "k1", "k2", "b", "pa", "pb", "pc", "P0", "P1", "P2", "P3"});

    correctionParameters cParam;
    cParam.fatoresDeCalibracaoNadig = fatoresDeCalibracaoNadig;
    cParam.fatoresDeCalibracaoWang = fatoresDeCalibracaoWang;
    cParam.fatoresDeCalibracaoPoli = fatoresDeCalibracaoPoli;
    cParam.fatoresDeCalibracaoPetSys = fatoresDeCalibracaoPetSys;
    cParam.chanEquivalent = chanEquivalent;

    std::cout << "Calibration Parameters Read" << std::endl;
    return cParam;
}

void ProcessAcquisition::LinearizeEnergies()
{
    std::map<int, std::vector<std::shared_ptr<TH1F>>> channelEnergy = histograms.rawEnergyChannel;
    for (auto &chipID : identificator.chipID)
    {
        for (int ch = 0; ch < 64; ch++)
        {
            std::map<int, double> peak = this->FindAndMarkPeaks(channelEnergy[chipID][ch].get(), 2.0, 0.01);
            peak.erase(1274);

            std::cout << "channel " << ch << " chipID " << chipID << std::endl;

            auto cal = SaturationCorrectionNadig(peak);
            lPar.fatoresDeCalibracaoNadig[chipID][ch]["c"] = cal["c"];
            lPar.fatoresDeCalibracaoNadig[chipID][ch]["s"] = cal["s"];
            cal = correctionWang(peak);
            lPar.fatoresDeCalibracaoWang[chipID][ch]["k1"] = cal["k1"];
            lPar.fatoresDeCalibracaoWang[chipID][ch]["k2"] = cal["k2"];
            lPar.fatoresDeCalibracaoWang[chipID][ch]["b"] = cal["b"];
            cal = polinomialCorrection(peak);
            lPar.fatoresDeCalibracaoPoli[chipID][ch]["a"] = cal["a"];
            lPar.fatoresDeCalibracaoPoli[chipID][ch]["b"] = cal["b"];
            lPar.fatoresDeCalibracaoPoli[chipID][ch]["c"] = cal["c"];
            cal = FindLinearizationPETSYS(peak);
            lPar.fatoresDeCalibracaoPetSys[chipID][ch]["P0"] = cal["P0"];
            lPar.fatoresDeCalibracaoPetSys[chipID][ch]["P1"] = cal["P1"];
            lPar.fatoresDeCalibracaoPetSys[chipID][ch]["P2"] = cal["P2"];
            lPar.fatoresDeCalibracaoPetSys[chipID][ch]["P3"] = cal["P3"];
        }
    }
}

void ProcessAcquisition::Calibrate()
{
    std::map<int, std::vector<std::shared_ptr<TH1F>>> channelEnergy = histograms.rawEnergyChannel;

    for (auto &chipID : identificator.chipID)
    {
        for (int ch = 0; ch < 64; ch++)
        {

            std::map<int, double> peak = this->FindAndMarkPeaks(channelEnergy[chipID][ch].get(), 2.0, 0.01);
            std::cout << "channel " << ch << " chipID " << chipID << std::endl;

            auto cal = SaturationCorrectionNadig(peak);
            cPar.fatoresDeCalibracaoNadig[chipID][ch]["c"] = cal["c"];
            cPar.fatoresDeCalibracaoNadig[chipID][ch]["s"] = cal["s"];
            cal = correctionWang(peak);
            cPar.fatoresDeCalibracaoWang[chipID][ch]["k1"] = cal["k1"];
            cPar.fatoresDeCalibracaoWang[chipID][ch]["k2"] = cal["k2"];
            cPar.fatoresDeCalibracaoWang[chipID][ch]["b"] = cal["b"];
            cal = polinomialCorrection(peak);
            cPar.fatoresDeCalibracaoPoli[chipID][ch]["a"] = cal["a"];
            cPar.fatoresDeCalibracaoPoli[chipID][ch]["b"] = cal["b"];
            cPar.fatoresDeCalibracaoPoli[chipID][ch]["c"] = cal["c"];
            cal = FindLinearizationPETSYS(peak);
            cPar.fatoresDeCalibracaoPetSys[chipID][ch]["P0"] = cal["P0"];
            cPar.fatoresDeCalibracaoPetSys[chipID][ch]["P1"] = cal["P1"];
            cPar.fatoresDeCalibracaoPetSys[chipID][ch]["P2"] = cal["P2"];
            cPar.fatoresDeCalibracaoPetSys[chipID][ch]["P3"] = cal["P3"];
        }
    }
}

void ProcessAcquisition::checkNeedLinearization()
{

    std::map<int, std::vector<std::shared_ptr<TH1F>>> channelEnergy = histograms.correctedEnergyChannel;

    int countOff = 0;
    for (auto &chipID : identificator.chipID)
    {
        for (int ch = 0; ch < 64; ch++)
        {

            std::map<int, double> peaks = this->FindAndMarkPeaks(channelEnergy[chipID][ch].get(), 2.0, 0.01);

            std::cout << "peaks for chipID" << chipID << " channel " << ch << " 511 : " << peaks[511] << " 1274 :" << peaks[1274] << std::endl;

            linearComponent[chipID][ch] = 511.0 - peaks[511];
            double percentageOff = abs(511 - peaks[511]) / 511;

            if (percentageOff > 0.05)
            {
                countOff++;
            }
        }
    }

    if (countOff > 3)
    {

        std::cout << "You need to relinearize the acquisitions" << std::endl;
        LinearizeEnergies();
        // clear previous histograms
        initializeHistograms();
        std::cout << "checando novamente o arquivo : " << std::endl;
        useLinearization = true;
        processCoincidenceFromRaw();
    }
}

void ProcessAcquisition::ProcessGroupFromRaw()
{
    // #GroupFromFile;

    if (rawFileName.find("group") != -1)
    {
        std::cout << " Processing Group file " << std::endl;

        Float_t cut = 1.0; // energy cut
        int event_counter = 0;

        int MinimumGroupSize = 1;
        double first_e{0}, first_t{0}, firstXmm{0}, firstYmm{0};
        int first_c{0}, chan{0};
        float esum;

        // cria as variaveis nescessárias à leitura do arquivo root
        UShort_t mh_n{0}, mh_j{0};
        UInt_t channelID{0};
        Float_t energy{0.0};
        Long64_t time{0};
        std::vector<TString> ports;

        // abre o arquivo root
        std::unique_ptr<TFile> file(new TFile(rawFileName.c_str(), "read"));
        std::unique_ptr<TTree> tree((TTree *)file->Get("data"));

        // cria os vetores que armazenarão as informações de tempo e energia dos eventos no grupo para calculo do timing
        std::vector<Long64_t> timeChanels(64, 0);
        std::vector<Float_t> energyChanels(64, 0);

        // set branches
        tree->SetBranchAddress("mh_n", &mh_n);
        tree->SetBranchAddress("mh_j", &mh_j);
        tree->SetBranchAddress("channelID", &channelID);
        tree->SetBranchAddress("energy", &energy);
        tree->SetBranchAddress("time", &time);

        auto inCoincidenceWith = identificator.getInCoincidenceWith();
        auto reference = identificator.getRef();

        std::shared_ptr<GimnWriteClass> Output;
        if (outFileName.empty())
        {
            std::string outName = removePart(fileName, ".root");
            outName = processedFolder + outName + "_processed.root";
            Output = std::make_shared<GimnWriteClass>(outName.c_str());
        }
        else
        {
            std::string outName = processedFolder + outFileName;
            Output = std::make_shared<GimnWriteClass>(outName.c_str());
            // GimnWriteClass Output(outName.c_str());
        }

        const int entries = tree->GetEntries();
        int pos;
        // Begins the iteration through the events
        for (Int_t event = 0; event <= entries; event++)
        {
            tree->GetEntry(event);
            int pos;
            Int_t chipID = (int)channelID / 64;
            Int_t channel = channelID - 64 * chipID;

            // =============================================================================================

            // aqui o programa busca qual a posição do chipID no vetor chpid de configuração
            // Primeiramente o programa ira buscar pelo chip ID no vetor de chipID's, se nao encontrar, irá criar um novo;

            auto it = std::find(identificator.chipID.begin(), identificator.chipID.end(), chipID);

            // Se ele nao entrar aqui ele vai perder a conta colocar o else depois
            if (it != identificator.chipID.end())
                pos = std::distance(identificator.chipID.begin(), it);

            // std::cout<<"Pos "<<pos<<std::endl;
            //  se numero de eventos no grupo maior do que um numero minimo

            if ((mh_n >= MinimumGroupSize) && (!(chipID == reference[pos]))) // se ele for maior do que o numero minimo, e nao for o detector de referencia
            {

                // APENAS PARA O PRIMEIRO EVENTO
                auto pos_xy = sipm.CorrectChannelToPixelPs(channel);
                // x esta para colunas [0]
                // y para linhas [1]

                double correctedEnergy;
                if (useEnergyCorrection)
                {
                    if (useLinearization)
                    {
                        correctedEnergy = correctEnergy(chipID, channel, energy, method) + linearComponent[chipID][channel];
                    }
                    else
                    {
                        correctedEnergy = correctEnergy(chipID, channel, energy, method);
                    }
                }
                else
                {
                    correctedEnergy = energy;
                }

                Float_t timeInSeconds = (float)time / 1.0e12;

                // eventos do grupo // Roda também pelo primeiro evento
                if (mh_j < (mh_n - 1))
                {
                    if (mh_j == 0) // Pega o primeiro evento dentro do grupo
                    {
                        event_counter++;
                        // std::cout << "------------------------" << std::endl;
                        // std::cout << "first event" << std::endl;
                        // std::cout << "------------------------" << std::endl;
                        if (energy > cut)
                        {
                            histograms.rawEnergyChannel[chipID][channel]->Fill(energy);
                            histograms.globalEnergyHistogramRaw[chipID]->Fill(energy);
                            histograms.correctedEnergyChannel[chipID][channel]->Fill(correctedEnergy);
                            histograms.globalEnergyHistogramCorrected[chipID]->Fill(correctedEnergy);
                        }
                        // Fills first Event;

                        first_e = energy;
                        first_t = time;
                        firstXmm = sipm.getPositionsX()[pos_xy[0]];
                        firstYmm = sipm.getPositionsY()[pos_xy[1]];
                        first_c = 8 * pos_xy[1] + pos_xy[0];
                        chan = channel;
                    }
                    sipm.Sipm_ecor[pos](pos_xy[0], pos_xy[1]) = +correctedEnergy;
                    sipm.Sipm_e[pos](pos_xy[0], pos_xy[1]) = +energy;
                    sipm.Sipm_t[pos](pos_xy[0], pos_xy[1]) = +timeInSeconds;
                    esum += energy;
                }
                // ultimo evento
                else if (mh_j == mh_n - 1)
                {
                    if (mh_j == 0) // Pega o primeiro evento dentro do grupo
                    {
                        event_counter++;
                        // std::cout << "------------------------" << std::endl;
                        // std::cout << "first event" << std::endl;
                        // std::cout << "------------------------" << std::endl;
                        if (energy > cut)
                        {

                            histograms.rawEnergyChannel[chipID][channel]->Fill(energy);
                            histograms.globalEnergyHistogramRaw[chipID]->Fill(energy);
                            if (useEnergyCorrection)
                            {
                                histograms.correctedEnergyChannel[chipID][channel]->Fill(correctedEnergy);
                                histograms.globalEnergyHistogramCorrected[chipID]->Fill(correctedEnergy);
                            }
                        }
                        // Fills first Event;
                        first_e = energy;
                        first_t = timeInSeconds;
                        firstXmm = sipm.getPositionsX()[pos_xy[0]];
                        firstYmm = sipm.getPositionsY()[pos_xy[1]];
                        first_c = 8 * pos_xy[1] + pos_xy[0];
                        chan = channel;
                    }

                    if (first_e > cut)
                    {
                        // std::cout << "------------------------" << std::endl;
                        // std::cout << "Last event" << std::endl;
                        // std::cout << "------------------------" << std::endl;

                        sipm.Sipm_ecor[pos](pos_xy[0], pos_xy[1]) = +correctedEnergy;
                        sipm.Sipm_e[pos](pos_xy[0], pos_xy[1]) = +energy;
                        sipm.Sipm_t[pos](pos_xy[0], pos_xy[1]) = +timeInSeconds;

                        auto Pos3D = anger(sipm.Sipm_ecor[pos]);
                        auto pos3Dnc = anger(sipm.Sipm_e[pos]);

                        GimnEvent ev;
                        ev.channel = chan;
                        ev.xmm = Pos3D[0];
                        ev.ymm = Pos3D[1];
                        ev.doi = Pos3D[2];
                        ev.sipmX = firstXmm;
                        ev.sipmY = firstYmm;
                        if (useEnergyCorrection)
                        {
                            ev.Energy = correctedEnergy;
                        }
                        else
                        {
                            ev.Energy = energy;
                        }
                        ev.time = time;

                        // std::cout<<" xmm "<<ev.xmm<<" y mm : "<<ev.ymm<<"energy : "<<ev.Energy<<std::endl;

                        histograms.floodRaw[chipID]->Fill(pos3Dnc[0], pos3Dnc[1]);
                        histograms.floodCorrected[chipID]->Fill(Pos3D[0], Pos3D[1]);

                        Output->putData(ev);
                    }
                    sipm.Sipm_ecor[pos].Zero();
                    sipm.Sipm_e[pos].Zero();
                    sipm.Sipm_t[pos].Zero();
                }
            }
        }

        int cnt = 0;
        writeMatrixOfDetectors(histograms);
        writeMatrixOfDetectorsRaw(histograms);
        Output->writeObject(histograms, identificator, method);
        Output->close();

        if (VERBOSE)
            std::cout << "finish" << std::endl;
        std::cout << "o programa detectou " << event_counter << std::endl;
    }
    else
    {

        std::cout << "The given file is not a group file" << std::endl;
    }
}

void ProcessAcquisition::ExtractNames(std::string inputFileName = "")
{
    if (inputFileName.empty())
    {
        auto NameVector = splitString(this->rawFileName, '/');
        int pos = NameVector.size() - 1;

        fileName = NameVector[pos];
    }
    else
    {
        auto NameVector = splitString(inputFileName, '/');
        int pos = NameVector.size() - 1;
        fileName = NameVector[pos];
    }
    std::cout << "fileName is : " << fileName << std::endl;
}

#include <stdexcept>
#include <iostream>

void ProcessAcquisition::plotChannelHistogramRaw(int chipID, int ch)
{
    try
    {
        // Validate indices first
        if (chipID < 0 || chipID >= histograms.rawEnergyChannel.size())
        {
            throw std::out_of_range("Invalid chipID index");
        }

        if (ch < 0 || ch >= histograms.rawEnergyChannel[chipID].size())
        {
            throw std::out_of_range("Invalid channel index");
        }

        // Get histogram pointer and validate
        std::shared_ptr<TH1F> hist = histograms.rawEnergyChannel[chipID][ch];
        if (!hist)
        {
            throw std::runtime_error("Histogram pointer is null");
        }

        // --- ROOT Style Config ---
        gStyle->SetOptStat(0);
        gStyle->SetTitleFont(42, "XYZ");
        gStyle->SetLabelFont(42, "XYZ");

        // --- Create Canvas ---
        TCanvas c1("c1", "Raw Energy Histogram", 1200, 800);
        c1.SetMargin(0.10, 0.05, 0.12, 0.08);

        // --- Histogram Styling ---
        hist->SetLineColor(kBlue + 2);
        hist->SetLineWidth(2);
        hist->SetFillColorAlpha(kBlue - 9, 0.6);

        // --- Axis Config ---
        hist->GetXaxis()->SetTitle("Energy [ADC Counts]");
        hist->GetYaxis()->SetTitle("Counts");
        hist->GetXaxis()->SetTitleSize(0.045);
        hist->GetYaxis()->SetTitleSize(0.045);
        hist->GetXaxis()->SetLabelSize(0.04);
        hist->GetYaxis()->SetLabelSize(0.04);
        hist->GetYaxis()->SetTitleOffset(1.2);

        // --- Draw ---
        hist->Draw("HIST E");

        // --- Add Grid ---
        c1.SetGridx();
        c1.SetGridy();
        c1.Update();

        // --- Add Annotation ---
        TLatex latex;
        latex.SetNDC();
        latex.SetTextFont(62);
        latex.SetTextSize(0.045);
        latex.DrawLatex(0.15, 0.91, Form("ChipID: %d  Channel: %d", chipID, ch));

        // --- Save Output ---
        std::string outName = removePart(fileName, ".root");
        outName += Form("-RawHistogram-chipID-%d-ch-%d", chipID, ch);

        outName = plotsFolder + outName;
        c1.SaveAs((outName + ".png").c_str());
        c1.SaveAs((outName + ".pdf").c_str());
        c1.SaveAs((outName + ".root").c_str());
    }
    catch (const std::out_of_range &e)
    {
        std::cerr << "Error: " << e.what()
                  << " | chipID: " << chipID
                  << " | ch: " << ch
                  << std::endl;
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << "Error: " << e.what()
                  << " | chipID: " << chipID
                  << " | ch: " << ch
                  << std::endl;
    }
    catch (...)
    {
        std::cerr << "Unknown error occurred during histogram plotting"
                  << std::endl;
    }
}


void ProcessAcquisition::plotGlobalHistogramRaw( int chipID)
{
    try
    {

        // Get histogram pointer and validate
        std::shared_ptr<TH1F> hist = histograms.globalEnergyHistogramCorrected[chipID];
        if (!hist)
        {
            throw std::runtime_error("Histogram pointer is null");
        }

        // --- ROOT Style Config ---
        gStyle->SetOptStat(0);
        gStyle->SetTitleFont(42, "XYZ");
        gStyle->SetLabelFont(42, "XYZ");

        // --- Create Canvas ---
        TCanvas c1("c1", "Raw Energy Histogram", 1200, 800);
        c1.SetMargin(0.10, 0.05, 0.12, 0.08);

        // --- Histogram Styling ---
        hist->SetLineColor(kBlue + 2);
        hist->SetLineWidth(2);
        hist->SetFillColorAlpha(kBlue - 9, 0.6);

        // --- Axis Config ---
        hist->GetXaxis()->SetTitle("Energy [keV]");
        hist->GetYaxis()->SetTitle("Counts");
        hist->GetXaxis()->SetTitleSize(0.045);
        hist->GetYaxis()->SetTitleSize(0.045);
        hist->GetXaxis()->SetLabelSize(0.04);
        hist->GetYaxis()->SetLabelSize(0.04);
        hist->GetYaxis()->SetTitleOffset(1.2);

        // --- Draw ---
        hist->Draw("HIST E");

        // --- Add Grid ---
        c1.SetGridx();
        c1.SetGridy();
        c1.Update();

        // --- Add Annotation ---
        TLatex latex;
        latex.SetNDC();
        latex.SetTextFont(62);
        latex.SetTextSize(0.045);
        latex.DrawLatex(0.15, 0.91, Form("ChipID: %d", chipID));

        // --- Save Output ---
        std::string outName = removePart(fileName, ".root");
        outName += Form("-GlobalHistogram-chipID-%d", chipID);

        outName = plotsFolder + outName;
        c1.SaveAs((outName + ".png").c_str());
    }
    catch (const std::out_of_range &e)
    {
        std::cerr << "Error: " << e.what()
                  << " | chipID: " << chipID
                  << std::endl;
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << "Error: " << e.what()
                  << " | chipID: " << chipID
                  << std::endl;
    }
    catch (...)
    {
        std::cerr << "Unknown error occurred during histogram plotting"
                  << std::endl;
    }
}

void ProcessAcquisition::writeMatrixOfDetectors(HistogramCoincidences histograms)
{
    std::lock_guard<std::mutex> lock(mtx);
    // Configurações de estilo global
    gStyle->SetOptStat(0);
    gStyle->SetTextFont(42);
    gStyle->SetLabelSize(0.075, "x");
    gStyle->SetLabelSize(0.075, "y");
    gStyle->SetTitleSize(0.08, "x");
    gStyle->SetTitleSize(0.08, "y");
    gStyle->SetPadTickX(1);
    gStyle->SetPadTickY(1);

    for (auto &chipID : histograms.chipIDs)
    {
        TCanvas c1("janela", Form("janela - %i", chipID), 3840, 2160);
        c1.Divide(8, 8);

        for (Int_t channels = 0; channels < 64; channels++)
        {
            auto pos_yx = sipm.CorrectChannelToPixelPs(channels);
            int padNumber = (pos_yx[0] * 8 + pos_yx[1]) + 1;

            //             Pad Index:
            //   1   2   3   4   5   6   7   8
            //   9  10  11  12  13  14  15  16
            //  17  18  19  20  21  22  23  24
            //  25  26  27  28  29  30  31  32
            //  33  34  35  36  37  38  39  40
            //  41  42  43  44  45  46  47  48
            //  49  50  51  52  53  54  55  56
            //  57  58  59  60  61  62  63  64

            c1.cd(padNumber);
            // Aumentar margem superior (último parâmetro de 0.05 para 0.12)
            gPad->SetMargin(0.10, 0.05, 0.12, 0.12); // left, right, bottom, top
            gPad->SetFrameLineWidth(2);

            // Configurações do histograma
            // auto hist = histograms.correctedEnergyChannel[chipID][channels];

            std::string name = histograms.correctedEnergyChannel[chipID][channels]->GetName();
            name = removePart(name, "Corrected Nadig");
            histograms.correctedEnergyChannel[chipID][channels]->SetName(name.c_str());

            histograms.correctedEnergyChannel[chipID][channels]->SetLineWidth(2);
            histograms.correctedEnergyChannel[chipID][channels]->SetLineColor(kBlue + 2);
            histograms.correctedEnergyChannel[chipID][channels]->SetFillColorAlpha(kAzure + 2, 0.5);

            // Configurar título principal
            histograms.correctedEnergyChannel[chipID][channels]->SetTitleSize(0.6, "T");   // Tamanho do título principal
            histograms.correctedEnergyChannel[chipID][channels]->SetTitleOffset(1.2, "T"); // Ajuste o offset conforme necessário

            // Configurações dos eixos
            histograms.correctedEnergyChannel[chipID][channels]->GetXaxis()->CenterTitle(true);
            histograms.correctedEnergyChannel[chipID][channels]->GetYaxis()->CenterTitle(true);
            histograms.correctedEnergyChannel[chipID][channels]->GetXaxis()->SetNdivisions(505);
            // hist->GetXaxis()->SetTitle("Energy [keV]");
            // hist->GetYaxis()->SetTitle("Counts");

            histograms.correctedEnergyChannel[chipID][channels]->Draw("HIST");

            // Adicionar texto de total manualmente
            TLatex latex;
            latex.SetNDC(true);
            latex.SetTextSize(0.11);
            latex.SetTextAlign(31);
            latex.SetTextFont(42);
            latex.DrawLatex(0.75, 0.78, Form("#scale[0.85]{#bf{Total: %.1e}}", histograms.correctedEnergyChannel[chipID][channels]->GetEntries()));
        }

        std::string outName = fileName + Form("-Detector-%i.png", chipID);
        outName = plotsFolder + outName;
        c1.SaveAs(outName.c_str(), "png -dpi 300 -compress 9");
    }
}

void ProcessAcquisition::writeMatrixOfDetectorsWhithHeatMap(HistogramCoincidences histograms)
{
    std::lock_guard<std::mutex> lock(mtx);
    // Configurações de estilo global
    gStyle->SetOptStat(0);
    gStyle->SetTextFont(42);
    gStyle->SetLabelSize(0.075, "x");
    gStyle->SetLabelSize(0.075, "y");
    gStyle->SetTitleSize(0.08, "x");
    gStyle->SetTitleSize(0.08, "y");
    gStyle->SetPadTickX(1);
    gStyle->SetPadTickY(1);

    for (auto &chipID : histograms.chipIDs)
    {
        // -------------------- Canvas com todos os histogramas --------------------
        TCanvas c1("janela", Form("janela - %i", chipID), 3840, 2160);
        c1.Divide(8, 8);

        // Criar matriz para o mapa de calor
        TH2F *heatmap = new TH2F(Form("heatmap_%d", chipID), Form("Total Counts - Detector %d", chipID),
                                 8, 0, 8, 8, 0, 8);

        for (Int_t channels = 0; channels < 64; channels++)
        {
            auto pos_yx = sipm.CorrectChannelToPixelPs(channels);
            int row = pos_yx[0];
            int col = pos_yx[1];
            int padNumber = (row * 8 + col) + 1;

            // Inserir contagens no mapa de calor
            double entries = histograms.correctedEnergyChannel[chipID][channels]->GetEntries();
            heatmap->SetBinContent(col + 1, 8 - row, entries); // Inverte Y para topo ser 8

            // Plotar o histograma individual
            c1.cd(padNumber);
            gPad->SetMargin(0.10, 0.05, 0.12, 0.12); // left, right, bottom, top
            gPad->SetFrameLineWidth(2);

            std::string name = histograms.correctedEnergyChannel[chipID][channels]->GetName();
            name = removePart(name, "Corrected Nadig");
            histograms.correctedEnergyChannel[chipID][channels]->SetName(name.c_str());

            auto hist = histograms.correctedEnergyChannel[chipID][channels];
            hist->SetLineWidth(2);
            hist->SetLineColor(kBlue + 2);
            hist->SetFillColorAlpha(kAzure + 2, 0.5);

            hist->SetTitleSize(0.6, "T");
            hist->SetTitleOffset(1.2, "T");

            hist->GetXaxis()->CenterTitle(true);
            hist->GetYaxis()->CenterTitle(true);
            hist->GetXaxis()->SetNdivisions(505);

            hist->Draw("HIST");

            // Texto com total de contagens
            TLatex latex;
            latex.SetNDC(true);
            latex.SetTextSize(0.11);
            latex.SetTextAlign(31);
            latex.SetTextFont(42);
            latex.DrawLatex(0.75, 0.78, Form("#scale[0.85]{#bf{Total: %.1e}}", entries));
        }

        std::string outName = fileName + Form("-Detector-%i.png", chipID);
        outName = plotsFolder + outName;
        c1.SaveAs(outName.c_str(), "png -dpi 300 -compress 9");

        // -------------------- Canvas com mapa de calor --------------------
        TCanvas c2(Form("heatmap_canvas_%d", chipID), Form("Heatmap - Detector %d", chipID), 1200, 1000);
        // gPad->SetRightMargin(0.15);
        gStyle->SetPalette(kRainBow); // Ou outro palette: kBird, kCool, etc.
        heatmap->GetXaxis()->SetTitle("X (coluna)");
        heatmap->GetYaxis()->SetTitle("Y (linha)");
        heatmap->GetZaxis()->SetTitle("Total Counts");
        heatmap->GetXaxis()->CenterTitle();
        heatmap->GetYaxis()->CenterTitle();
        heatmap->GetZaxis()->CenterTitle();
        heatmap->Draw("COLZ TEXT");

        std::string heatmapName = fileName + Form("-Heatmap-Detector-%i.png", chipID);
        heatmapName = plotsFolder + heatmapName;
        c2.SaveAs(heatmapName.c_str(), "png");

        TCanvas c3(Form("Flood-%d", chipID), Form("Flood - Detector %d", chipID), 1200, 1000);
        histograms.floodCorrected[chipID]->Draw("COLZ");
        std::string floodName = fileName + Form("Flood-Detector-%i.png", chipID);
        floodName = plotsFolder + floodName;
        c3.SaveAs(floodName.c_str(), "png");

        TCanvas c4(Form("Flood-Raw-%d", chipID), Form("Flood-Raw - Detector %d", chipID), 1200, 1000);
        histograms.floodRaw[chipID]->Draw("COLZ");
        std::string floodNameRaw = fileName + Form("Flood-Raw-Detector-%i.png", chipID);
        floodNameRaw = plotsFolder + floodNameRaw;
        c4.SaveAs(floodNameRaw.c_str(), "png");

        delete heatmap;
    }
}

void ProcessAcquisition::writeMatrixOfDetectorsRaw(HistogramCoincidences histograms)
{
    std::lock_guard<std::mutex> lock(mtx);

    // Configurações de estilo global
    gStyle->SetOptStat(0);
    gStyle->SetTextFont(42);
    gStyle->SetLabelSize(0.075, "x");
    gStyle->SetLabelSize(0.075, "y");
    gStyle->SetTitleSize(0.08, "x");
    gStyle->SetTitleSize(0.08, "y");
    gStyle->SetPadTickX(1);
    gStyle->SetPadTickY(1);

    for (auto &chipID : histograms.chipIDs)
    {
        TCanvas c1("janela", Form("janela - %i", chipID), 3840, 2160);
        c1.Divide(8, 8);

        for (Int_t channels = 0; channels < 64; channels++)
        {
            auto pos_yx = sipm.CorrectChannelToPixelPs(channels);
            int padNumber = (pos_yx[0] * 8 + pos_yx[1]) + 1;

            c1.cd(padNumber);
            // Aumentar margem superior (último parâmetro de 0.05 para 0.12)
            gPad->SetMargin(0.10, 0.05, 0.12, 0.12); // left, right, bottom, top
            gPad->SetFrameLineWidth(2);

            // Configurações do histograma
            // auto hist = histograms.correctedEnergyChannel[chipID][channels];

            std::string name = histograms.rawEnergyChannel[chipID][channels]->GetName();
            name = removePart(name, "Corrected Nadig");
            histograms.correctedEnergyChannel[chipID][channels]->SetName(name.c_str());

            histograms.rawEnergyChannel[chipID][channels]->SetLineWidth(2);
            histograms.rawEnergyChannel[chipID][channels]->SetLineColor(kBlue + 2);
            histograms.rawEnergyChannel[chipID][channels]->SetFillColorAlpha(kAzure + 2, 0.5);

            // Configurar título principal
            histograms.rawEnergyChannel[chipID][channels]->SetTitleSize(0.6, "T");   // Tamanho do título principal
            histograms.rawEnergyChannel[chipID][channels]->SetTitleOffset(1.2, "T"); // Ajuste o offset conforme necessário

            // Configurações dos eixos
            histograms.rawEnergyChannel[chipID][channels]->GetXaxis()->CenterTitle(true);
            histograms.rawEnergyChannel[chipID][channels]->GetYaxis()->CenterTitle(true);
            histograms.rawEnergyChannel[chipID][channels]->GetXaxis()->SetNdivisions(505);
            // hist->GetXaxis()->SetTitle("Energy [keV]");
            // hist->GetYaxis()->SetTitle("Counts");

            histograms.rawEnergyChannel[chipID][channels]->Draw("HIST");

            // Adicionar texto de total manualmente
            TLatex latex;
            latex.SetNDC(true);
            latex.SetTextSize(0.11);
            latex.SetTextAlign(31);
            latex.SetTextFont(42);
            latex.DrawLatex(0.75, 0.78, Form("#scale[0.85]{#bf{Total: %.1e}}", histograms.correctedEnergyChannel[chipID][channels]->GetEntries()));
        }

        std::string outName = fileName + Form("-Detector-%i-RAW.png", chipID);
        outName = plotsFolder + outName;
        c1.SaveAs(outName.c_str(), "png -dpi 300 -compress 9");
    }
}
// void ProcessAcquisition::writeMatrixOfDetectors(HistogramCoincidences histograms)
// {
//     // Configurações de estilo global
//     gStyle->SetOptStat(0);   // Remove a caixa de estatísticas padrão
//     gStyle->SetTextFont(42); // Usa fonte Helvetica
//     gStyle->SetLabelSize(0.075, "x");//0.045
//     gStyle->SetLabelSize(0.075, "y");//0.045
//     gStyle->SetTitleSize(0.08, "x");//0.05
//     gStyle->SetTitleSize(0.08, "y");//0.05
//     gStyle->SetPadTickX(1); // Marcadores no eixo X
//     gStyle->SetPadTickY(1); // Marcadores no eixo Y

//     for (auto &chipID : histograms.chipIDs)
//     {
//         TCanvas c1("janela", Form("janela - %i", chipID), 3840, 2160); // Resolução 4K
//         c1.Divide(8, 8);

//         for (Int_t channels = 0; channels < 64; channels++)
//         {
//             auto pos_yx = sipm.CorrectChannelToPixelPs(channels);
//             int padNumber = (pos_yx[0] * 8 + pos_yx[1]) + 1;

//             c1.cd(padNumber);
//             gPad->SetMargin(0.10, 0.05, 0.12, 0.05); // Margens: left, right, bottom, top
//             gPad->SetFrameLineWidth(2);

//             // Configurações do histograma
//             histograms.correctedEnergyChannel[chipID][channels]->SetLineWidth(2);
//             histograms.correctedEnergyChannel[chipID][channels]->SetLineColor(kBlue + 2);
//             histograms.correctedEnergyChannel[chipID][channels]->SetFillColorAlpha(kAzure + 2, 0.5);

//             // Configurações dos eixos
//             histograms.correctedEnergyChannel[chipID][channels]->GetXaxis()->CenterTitle(true);
//             histograms.correctedEnergyChannel[chipID][channels]->GetYaxis()->CenterTitle(true);
//             histograms.correctedEnergyChannel[chipID][channels]->GetXaxis()->SetNdivisions(505);
//             histograms.correctedEnergyChannel[chipID][channels]->GetXaxis()->SetTitle("Energy [kV]");
//             histograms.correctedEnergyChannel[chipID][channels]->GetYaxis()->SetTitle("Counts");
//             // Desenha o histograma
//             histograms.correctedEnergyChannel[chipID][channels]->Draw("HIST");

//             // Adiciona o texto com LaTeX
//             TLatex latex;
//             latex.SetNDC(true);
//             latex.SetTextSize(0.11);
//             latex.SetTextAlign(31); // Alinhamento top-right
//             latex.SetTextFont(42);
//             latex.DrawLatex(0.75, 0.78, Form("#scale[0.85]{#bf{Total: %.1e}}", histograms.correctedEnergyChannel[chipID][channels]->GetEntries()));
//         }

//         std::string outName = Form("Detector-%i_HQ.png", chipID);
//         c1.SaveAs(outName.c_str(), "png -dpi 300 -compress 9"); // Alta qualidade
//     }
// }

void ProcessAcquisition::writeCalibration()
{

    this->useEnergyCorrection = true;
    // Creates the file
    std::shared_ptr<TFile> calOut;

    std::string calNamOut = "";
    if (calibrationName.empty())
    {
        std::cout << "Calibration Name empty setting name as :" << std::endl;
        // std::shared_ptr<GimnWriteClass> Output;
        // if (outFileName.empty())
        // {
        //     std::string outName = removePart(fileName, ".root");
        //     outName = processedFolder + outName + "_processed.root";
        //     Output = std::make_shared<GimnWriteClass>(outName.c_str());
        // }
        // else
        // {
        //     std::string outName = processedFolder + outFileName;
        //     Output = std::make_shared<GimnWriteClass>(outName.c_str());
        //     // GimnWriteClass Output(outName.c_str());
        // }
        calNamOut = removePart(fileName, ".root");
        calNamOut = calNamOut + "-CalibrationFactors.root";
        calNamOut = calibrationFolder + calNamOut;
        std::cout << calNamOut << std::endl;
        calOut = std::make_shared<TFile>(calNamOut.c_str(), "RECREATE");
    }
    else
    {
        std::cout << "Calibration Name " << std::endl;
        std::string calNamOut = calibrationFolder + calibrationName.c_str();
        std::cout << calNamOut << std::endl;
        calOut = std::make_shared<TFile>(calNamOut.c_str(), "RECREATE");
    }

    // Creates the tree to store the calibration Factors
    std::shared_ptr<TTree> calFactors;
    calFactors = std::make_shared<TTree>("calFactorChannel", "calFactorChannel");

    Int_t chan{-1};
    Double_t C, S, Pos511, Pos1274;
    Int_t cpN;
    Double_t k1, k2, b;
    Double_t pa, pb, pc;
    Double_t P0, P1, P2, P3;

    calFactors->Branch("channel", &chan);
    calFactors->Branch("c", &C);
    calFactors->Branch("s", &S);
    calFactors->Branch("511Pos", &Pos511);
    calFactors->Branch("1274Pos", &Pos1274);
    calFactors->Branch("chipID", &cpN);
    calFactors->Branch("k1", &k1);
    calFactors->Branch("k2", &k2);
    calFactors->Branch("b", &b);
    calFactors->Branch("pa", &pa);
    calFactors->Branch("pb", &pb);
    calFactors->Branch("pc", &pc);
    calFactors->Branch("P0", &P0);
    calFactors->Branch("P1", &P1);
    calFactors->Branch("P2", &P2);
    calFactors->Branch("P3", &P3);

    Calibrate();
    for (auto &chipID : identificator.chipID)
    {
        for (int ch = 0; ch < 64; ch++)
        {
            cpN = chipID;
            chan = ch;
            // Simple Saturation Model
            C = cPar.fatoresDeCalibracaoNadig[chipID][ch]["c"];
            S = cPar.fatoresDeCalibracaoNadig[chipID][ch]["s"];

            // Wang Model
            k1 = cPar.fatoresDeCalibracaoWang[chipID][ch]["k1"];
            k2 = cPar.fatoresDeCalibracaoWang[chipID][ch]["k2"];
            b = cPar.fatoresDeCalibracaoWang[chipID][ch]["b"];

            // Polinomial Correction
            pa = cPar.fatoresDeCalibracaoPoli[chipID][ch]["pa"];
            pb = cPar.fatoresDeCalibracaoPoli[chipID][ch]["pb"];
            pc = cPar.fatoresDeCalibracaoPoli[chipID][ch]["pc"];

            // Petsts saturation model
            P0 = cPar.fatoresDeCalibracaoPetSys[chipID][ch]["P0"];
            P1 = cPar.fatoresDeCalibracaoPetSys[chipID][ch]["P1"];
            P2 = cPar.fatoresDeCalibracaoPetSys[chipID][ch]["P2"];
            P3 = cPar.fatoresDeCalibracaoPetSys[chipID][ch]["P3"];
            calFactors->Fill();
        }
    }
    calOut->WriteObject(calFactors.get(), "CalibrationFactors");
    calOut->Close();
}

void ProcessAcquisition::writeLinearization()
{
    this->useLinearization = true;
    // Creates the file
    std::shared_ptr<TFile> calOut;

    std::string calNamOut = "";
    if (calibrationName.empty())
    {
        calNamOut = removePart(fileName, ".root");
        calNamOut = calNamOut + "-CalibrationFactors.root";
        calNamOut = calibrationFolder + fileName;
        calOut = std::make_shared<TFile>(calNamOut.c_str(), "RECREATE");
    }
    else
    {
        std::string calNamOut = calibrationFolder + calibrationName.c_str();
        calOut = std::make_shared<TFile>(calNamOut.c_str(), "RECREATE");
    }

    // Creates the tree to store the calibration Factors
    std::shared_ptr<TTree> calFactors;
    calFactors = std::make_shared<TTree>("calFactorChannel", "calFactorChannel");

    Int_t chan{-1};
    Double_t C, S, Pos511, Pos1274;
    Int_t cpN;
    Double_t k1, k2, b;
    Double_t pa, pb, pc;
    Double_t P0, P1, P2, P3;

    calFactors->Branch("channel", &chan);
    calFactors->Branch("c", &C);
    calFactors->Branch("s", &S);
    calFactors->Branch("511Pos", &Pos511);
    calFactors->Branch("1274Pos", &Pos1274);
    calFactors->Branch("chipID", &cpN);
    calFactors->Branch("k1", &k1);
    calFactors->Branch("k2", &k2);
    calFactors->Branch("b", &b);
    calFactors->Branch("pa", &pa);
    calFactors->Branch("pb", &pb);
    calFactors->Branch("pc", &pc);
    calFactors->Branch("P0", &P0);
    calFactors->Branch("P1", &P1);
    calFactors->Branch("P2", &P2);
    calFactors->Branch("P3", &P3);

    for (auto &chipID : identificator.chipID)
    {
        for (int ch = 0; ch < 64; ch++)
        {
            cpN = chipID;
            chan = ch;
            LinearizeEnergies();
            C = lPar.fatoresDeCalibracaoNadig[chipID][ch]["c"];
            S = lPar.fatoresDeCalibracaoNadig[chipID][ch]["s"];

            k1 = lPar.fatoresDeCalibracaoWang[chipID][ch]["k1"];
            k2 = lPar.fatoresDeCalibracaoWang[chipID][ch]["k2"];
            b = lPar.fatoresDeCalibracaoWang[chipID][ch]["b"];

            pa = lPar.fatoresDeCalibracaoPoli[chipID][ch]["pa"];
            pb = lPar.fatoresDeCalibracaoPoli[chipID][ch]["pb"];
            pc = lPar.fatoresDeCalibracaoPoli[chipID][ch]["pc"];

            P0 = lPar.fatoresDeCalibracaoPetSys[chipID][ch]["P0"];
            P1 = lPar.fatoresDeCalibracaoPetSys[chipID][ch]["P1"];
            P2 = lPar.fatoresDeCalibracaoPetSys[chipID][ch]["P2"];
            P3 = lPar.fatoresDeCalibracaoPetSys[chipID][ch]["P3"];
            calFactors->Fill();
        }
    }
    calOut->WriteObject(calFactors.get(), "CalibrationFactors");
    calOut->Close();
}

correctionParameters ProcessAcquisition::loadLinearizationFactors(std::string linearizationParameters)
{
    this->useLinearization = true;
    // declares root Dataframe for retrieving calibration parameters
    ROOT::RDataFrame CalibrationFactors("CalibrationFactors", linearizationParameters.c_str());

    // Utils
    std::vector<Float_t> xPos = this->sipm.getPositionsX();
    std::vector<Float_t> yPos = this->sipm.getPositionsY();

    int nchans = 64;
    std::vector<int> chanEquivalent(nchans, 0);

    std::map<int, std::vector<std::map<std::string, double>>> fatoresDeCalibracaoNadig;
    std::map<int, std::vector<std::map<std::string, double>>> fatoresDeCalibracaoWang;
    std::map<int, std::vector<std::map<std::string, double>>> fatoresDeCalibracaoPoli;
    std::map<int, std::vector<std::map<std::string, double>>> fatoresDeCalibracaoPetSys;

    std::vector<int> chipIDs;

    auto chipIDCounter = [&chipIDs](int chipID)
    {
        static int lastChipID = -1;
        if (chipID != lastChipID)
        {
            lastChipID = chipID;
            chipIDs.push_back(chipID);
        }
    };

    CalibrationFactors.Foreach(chipIDCounter, {"chipID"});

    for (auto &val : chipIDs)
    {

        for (int ch = 0; ch < nchans; ch++)
        {

            std::map<std::string, Double_t> parametrosNadig;
            parametrosNadig["c"] = -1.0;
            parametrosNadig["s"] = -1.0;
            fatoresDeCalibracaoNadig[val].push_back(parametrosNadig);

            std::map<std::string, Double_t> parametrosWang;
            parametrosWang["k1"] = -1.0;
            parametrosWang["k2"] = -1.0;
            parametrosWang["b"] = -1.0;
            fatoresDeCalibracaoWang[val].push_back(parametrosWang);

            std::map<std::string, Double_t> parametrosPoli;
            parametrosPoli["pa"] = -1.0;
            parametrosPoli["pb"] = -1.0;
            parametrosPoli["pc"] = -1.0;
            fatoresDeCalibracaoPoli[val].push_back(parametrosPoli);

            std::map<std::string, Double_t> parametrosPetSys;
            parametrosPetSys["P0"] = -1.0;
            parametrosPetSys["P1"] = -1.0;
            parametrosPetSys["P2"] = -1.0;
            parametrosPetSys["P3"] = -1.0;
            fatoresDeCalibracaoPetSys[val].push_back(parametrosPetSys);

            auto pos_xy = sipm.CorrectChannelToPixelPs(ch);
            int equivalent = 8 * pos_xy[1] + pos_xy[0];
            chanEquivalent[ch] = equivalent;
        }
    }

    auto ParameterReader = [&fatoresDeCalibracaoNadig, &fatoresDeCalibracaoWang, &fatoresDeCalibracaoPoli, &fatoresDeCalibracaoPetSys](int chipID, int channel, double c, double s, double k1, double k2, double b, double pa, double pb, double pc, double P0, double P1, double P2, double P3)
    {
        std::cout << "chipID " << chipID << " channel " << channel << " c " << c << " s " << s << " k1 " << k1 << " k2 " << k2 << " b " << b << " pa " << pa << " pb " << pb << " pc " << pc << " P0 " << P0 << " P1 " << P1 << " P2 " << P2 << " P3 " << P3 << std::endl;
        fatoresDeCalibracaoNadig[chipID][channel]["c"] = c;
        fatoresDeCalibracaoNadig[chipID][channel]["s"] = s;

        fatoresDeCalibracaoWang[chipID][channel]["k1"] = k1;
        fatoresDeCalibracaoWang[chipID][channel]["k2"] = k2;
        fatoresDeCalibracaoWang[chipID][channel]["b"] = b;

        fatoresDeCalibracaoPoli[chipID][channel]["pa"] = pa;
        fatoresDeCalibracaoPoli[chipID][channel]["pb"] = pb;
        fatoresDeCalibracaoPoli[chipID][channel]["pc"] = pc;

        fatoresDeCalibracaoPetSys[chipID][channel]["P0"] = P0;
        fatoresDeCalibracaoPetSys[chipID][channel]["P1"] = P1;
        fatoresDeCalibracaoPetSys[chipID][channel]["P2"] = P2;
        fatoresDeCalibracaoPetSys[chipID][channel]["P3"] = P3;
    };

    CalibrationFactors.Foreach(ParameterReader, {"chipID", "channel", "c", "s", "k1", "k2", "b", "pa", "pb", "pc", "P0", "P1", "P2", "P3"});

    correctionParameters lParam;
    lParam.fatoresDeCalibracaoNadig = fatoresDeCalibracaoNadig;
    lParam.fatoresDeCalibracaoWang = fatoresDeCalibracaoWang;
    lParam.fatoresDeCalibracaoPoli = fatoresDeCalibracaoPoli;
    lParam.fatoresDeCalibracaoPetSys = fatoresDeCalibracaoPetSys;
    lParam.chanEquivalent = chanEquivalent;

    std::cout << "Linearization Parameters Read" << std::endl;
    return lParam;
}

std::map<int, double> ProcessAcquisition::FindAndMarkPeaks(TH1F *histo, Float_t sigma, Float_t Threshold)
{
    // Float_t sigma {5} , Threshold {0.001};
    std::vector<Float_t> pks;
    TSpectrum *s = new TSpectrum;
    int npeaks = s->Search(histo, sigma, "", Threshold);
    double *peaks = s->GetPositionX();

    // double maxPeak = histo->GetXaxis()->GetXmax();

    std::map<int, double> sodiumPeaks;
    sodiumPeaks[511] = 0.0;
    sodiumPeaks[1274] = 0.0;
    sodiumPeaks[0] = npeaks;
    bool first = true;
    if (VERBOSE)
        std::cout << "nPeaks " << npeaks << std::endl;

    double maior = 0;
    // double menor = 99999;
    for (int i = 0; i < npeaks; i++)
    {
        // std::cout<<menor<<std::endl;
        // if (peaks[i]>maxPeak/10){
        //     if (peaks[i]< menor)
        //         menor = peaks[i];
        if (i == 0)
        {
            sodiumPeaks[511] = peaks[i];
        }
        if (peaks[i] > maior)
            maior = peaks[i];
        if (VERBOSE)
            std::cout << "pico encontrado em " << peaks[i] << std::endl;
    }
    // sodiumPeaks[511] = menor;
    sodiumPeaks[1274] = maior;

    delete s;
    return sodiumPeaks;
}

std::map<TString, Float_t> ProcessAcquisition::polinomialCorrection(std::map<int, double> peaks)
{
    std::cout << " on Poli " << std::endl;

    std::shared_ptr<TGraph> graph(nullptr);

    int out_type = {-1};

    if (peaks.size() == 3)
    {
        Double_t E[3]{0, 511, 1274}; // 1274};
        Double_t Q[3]{0, peaks[511], peaks[1274]};
        graph = std::make_shared<TGraph>(3, Q, E);
        out_type = 1;
    }
    else if (peaks.size() == 2)
    {
        Double_t E[2]{0, 511};
        Double_t Q[2]{0, peaks[511]};
        graph = std::make_shared<TGraph>(3, Q, E);
        out_type = 2;
    }

    std::map<TString, Float_t> out;
    TF1 fitFunc("polinomial", "[0]*pow(x,2)+[1]*x+[2]");

    for (int pa = -10; pa < 10; pa++)
    {
        for (int pb = -10; pb < 10; pb++)
        {
            for (int pc = -10; pc < 100; pc++)
            {
                std::map<TString, Float_t> partial1;
                fitFunc.SetParameter(0, pa); // Valor inicial para c
                fitFunc.SetParameter(1, pb); // Valor inicial para s
                fitFunc.SetParameter(2, pc); // Valor inicial para s

                graph->Fit(&fitFunc, "Q");

                float a = fitFunc.GetParameter(0);
                float b = fitFunc.GetParameter(1);
                float c = fitFunc.GetParameter(2);

                // float T = fitFunc.GetParameter(2);

                partial1.insert(std::make_pair("a", a));
                partial1.insert(std::make_pair("b", b));
                partial1.insert(std::make_pair("c", c));

                float calculation511 = a * pow(peaks[511], 2) + b * peaks[511] + c;
                // std::cout << " the value is " << calculation511 << "  and " << calculation1274 << std::endl;

                if (out_type == 1)
                {
                    float calculation1274 = a * pow(peaks[1274], 2) + b * peaks[1274] + c;

                    if ((calculation511 > 510.999 && calculation511 < 511.001) && ((calculation1274 > 1273.999 && calculation1274 < 1274.001)))
                    {
                        // std::cout << pa << " " << pb << " " << pc << std::endl;

                        partial1["out"] = true;
                        return partial1;
                    }
                }
                else if (out_type == 2)
                {
                    if (calculation511 > 510.9 && calculation511 < 511.01)
                    {

                        std::cout << pa << " " << pb << " " << pc << std::endl;
                        return partial1;
                    }
                }
            }
        }
    }
    return out;
}

std::map<TString, Float_t> ProcessAcquisition::SaturationCorrectionNadig(std::map<int, double> peaks)
{
    std::cout << " on Nadig " << std::endl;

    std::shared_ptr<TGraph> graph(nullptr);

    int out_type = {-1};

    if (peaks.size() == 3)
    {
        Double_t E[3]{0, 511, 1274}; // 1274};
        Double_t Q[3]{0, peaks[511], peaks[1274]};
        graph = std::make_shared<TGraph>(3, Q, E);
        out_type = 1;
    }
    else if (peaks.size() == 2)
    {
        Double_t E[2]{0, 511};
        Double_t Q[2]{0, peaks[511]};
        graph = std::make_shared<TGraph>(3, Q, E);
        out_type = 2;
    }

    std::map<TString, Float_t> out;
    TF1 fitFunc("nadigShug", "[0]*[1]*log(1/(1-x/[1]))");

    bool outBool = false;
    float parameter_a = -1000;
    float parameter_b = -1000;
    int contador{0};
    while (!outBool)
    {
        std::map<TString, Float_t> partial;
        // std::cout<<"contador "<<contador <<std::endl;

        fitFunc.SetParameter(0, parameter_a); // Valor inicial para c
        fitFunc.SetParameter(1, parameter_b); // Valor inicial para s
        // fitFunc.SetParameter(2, 100); // Valor inicial para s

        graph->Fit(&fitFunc, "Q");

        float C = fitFunc.GetParameter(0);
        float S = fitFunc.GetParameter(1);
        // float T = fitFunc.GetParameter(2);

        partial.insert(std::make_pair("c", C));
        partial.insert(std::make_pair("s", S));
        // out.insert(std::make_pair("t", T));

        parameter_a += 0.1;
        parameter_b += 0.1;
        float calculation511 = partial["c"] * partial["s"] * log(1 / (1 - peaks[511] / partial["s"]));
        // std::cout<<"peak 511 ->"<<peaks[511]<<std::endl;
        // std::cout<<"peak 1274 ->"<<peaks[1274]<<std::endl;
        // std::cout<<calculation511<<std::endl;
        // std::cout<<calculation1274<<std::endl;
        // std::cout<<"PARAMETRO a->"<<parameter_a<<std::endl;
        // std::cout<<"PARAMETRO b->"<<parameter_b<<std::endl;
        // std::cout<<"PARAMETRO C->"<<C<<std::endl;
        // std::cout<<"PARAMETRO S->"<<S<<std::endl;
        if (contador > 1000000)
        {
            std::cout << "got stuck Nadig" << std::endl;
            gROOT->ProcessLine(".q");
        }

        if (out_type == 1)
        {
            float calculation1274 = partial["c"] * partial["s"] * log(1 / (1 - peaks[1274] / partial["s"]));
            if ((calculation511 > 510.999 && calculation511 < 511.001) && ((calculation1274 > 1273.999 && calculation1274 < 1274.001)))
            {
                // std::cout<<" the value is "<<calulation511<<std::endl;
                outBool = true;
                return partial;
            }
        }

        else if (out_type == 2)
        {
            if (calculation511 > 510.9 && calculation511 < 511.01)
            {
                std::cout << " a" << parameter_a << " b " << parameter_b << calculation511 << std::endl;
                outBool = true;
                return partial;
            }
        }
        contador++;
    }

    return out;
}

std::map<TString, Float_t> ProcessAcquisition::correctionWang(std::map<int, double> peaks)
{
    std::cout << " on wang " << std::endl;
    std::shared_ptr<TGraph> graph(nullptr);

    int out_type = {-1};

    if (peaks.size() == 3)
    {
        Double_t E[3]{0, 511, 1274}; // 1274};
        Double_t Q[3]{0, peaks[511], peaks[1274]};
        graph = std::make_shared<TGraph>(3, Q, E);
        out_type = 1;
    }
    else if (peaks.size() == 2)
    {
        Double_t E[2]{0, 511};
        Double_t Q[2]{0, peaks[511]};
        graph = std::make_shared<TGraph>(3, Q, E);
        out_type = 2;
    }

    std::map<TString, Float_t> out;
    // TF1 fitFunc("wang", "[0]*(1-exp(-[1]*x)+[2])");

    // TF1 fitFunc("wang", "log((x-[2])/[0])*(1/[1])");

    TF1 fitFunc("wang", "log([0]/([0]+[2]-x))*(1/[1])");

    bool outBool = false;

    for (int pa = -1000; pa < 1000; pa += 20)
    {
        for (int pb = -2000; pb < 10000; pb += 100)
        {
            for (int pc = -2000; pc < 1000; pc += 100)
            {
                std::map<TString, Float_t> partial1;
                fitFunc.SetParameter(0, pa); // Valor inicial para c
                fitFunc.SetParameter(1, pb); // Valor inicial para s
                fitFunc.SetParameter(2, pc); // Valor inicial para s

                graph->Fit(&fitFunc, "Q");

                float k1 = fitFunc.GetParameter(0);
                float k2 = fitFunc.GetParameter(1);
                float b = fitFunc.GetParameter(2);

                // float T = fitFunc.GetParameter(2);

                partial1.insert(std::make_pair("k1", k1));
                partial1.insert(std::make_pair("k2", k2));
                partial1.insert(std::make_pair("b", b));

                // float calculation511 = k1 * (1 - exp(-k2 * peaks[511])) + b;
                // float calculation1274 = k1 * (1 - exp(-k2 * peaks[1274])) + b;

                float calculation511 = log(k1 / (k1 + b - peaks[511])) * (1 / k2);
                float calculation1274 = log(k1 / (k1 + b - peaks[1274])) * (1 / k2);
                if (out_type == 1)
                {

                    if ((calculation511 > 510.999 && calculation511 < 511.001) && ((calculation1274 > 1273.999 && calculation1274 < 1274.001)))
                    {
                        std::cout << "calculated 511  " << calculation511 << std::endl;
                        // std::cout<<"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"<< std::endl;
                        std::cout << "k1 :" << k1 << "k2 " << k2 << " b " << b << std::endl;
                        // std::cout << " the value of 511 peak is " << calculation511 << "  and  1274 is" << calculation1274 << std::endl;
                        partial1["out"] = true;
                        outBool = true;
                        return partial1;
                    }
                }

                else if (out_type == 2)
                {
                    if (calculation511 > 510.9 && calculation511 < 511.01)
                    {
                        outBool = true;
                        std::cout << "k1 :" << k1 << "k2 " << k2 << " b " << b << std::endl;
                        break;
                        return partial1;
                    }
                }
            }
        }
    }
    return out;
}

// Funcao petsys;
std::map<TString, Float_t> ProcessAcquisition::FindLinearizationPETSYS(std::map<int, double> peaks)
{
    std::cout << " on Petsys " << std::endl;

    std::map<TString, Float_t> out;

    std::shared_ptr<TGraph> graph(nullptr);

    int out_type = {-1};

    if (peaks.size() == 3)
    {
        Double_t E[3]{0, 511, 1274}; // 1274};
        Double_t Q[3]{0, peaks[511], peaks[1274]};
        graph = std::make_shared<TGraph>(3, Q, E);
        out_type = 1;
    }
    else if (peaks.size() == 2)
    {
        Double_t E[2]{0, 511};
        Double_t Q[2]{0, peaks[511]};
        graph = std::make_shared<TGraph>(3, Q, E);
        out_type = 2;
    }

    TF1 fitFunc("fitFunc", " [0] * pow([1], pow(x, [2])) + [3] * x - [0]");

    for (int pa = -10; pa < 10; pa++)
    {
        for (int pb = 20; pb < 109; pb++)
        {
            for (int pc = 10; pc < 100; pc++)
            {
                for (int pd = -10; pd < 1; pd++)
                {
                    std::map<TString, Float_t> partial1;

                    fitFunc.SetParameter(0, pa / 10); // Valor inicial para P0
                    fitFunc.SetParameter(1, pb / 10); // Valor inicial para P1
                    fitFunc.SetParameter(2, pc / 10); // Valor inicial para P2
                    fitFunc.SetParameter(3, pd / 10); // Valor inicial para P3

                    graph->Fit(&fitFunc, "Q");

                    float P0 = fitFunc.GetParameter(0);
                    float P1 = fitFunc.GetParameter(1);
                    float P2 = fitFunc.GetParameter(2);
                    float P3 = fitFunc.GetParameter(3);

                    // float T = fitFunc.GetParameter(2);

                    partial1.insert(std::make_pair("P0", fitFunc.GetParameter(0)));
                    partial1.insert(std::make_pair("P1", fitFunc.GetParameter(1)));
                    partial1.insert(std::make_pair("P2", fitFunc.GetParameter(2)));
                    partial1.insert(std::make_pair("P3", fitFunc.GetParameter(3)));

                    float calculation511 = P0 * pow(P1, pow(peaks[511], P2)) + P3 * peaks[511] + P0;

                    if (out_type == 1)
                    {
                        float calculation1274 = P0 * pow(P1, pow(peaks[1274], P2)) + P3 * peaks[1274] + P0;
                        if ((calculation511 > 510.999 && calculation511 < 511.001) && ((calculation1274 > 1273.99 && calculation1274 < 1274.001)))
                        {
                            // std::cout << "PETSYS" << std::endl;
                            // std::cout << " Pa : " << pa << " Pb : " << pb << " Pc : " << pc << " Pd : " << pd << std::endl;
                            // std::cout << pa << " " << pb << " " << pc << std::endl;
                            // std::cout << " P0 " << P0 << " P1 " << P1 << " P2 " << P2 << " P3 " << P3;
                            // std::cout << " the value is " << calculation511 << "  and " << calculation1274 << std::endl;
                            partial1["out"] = true;
                            return partial1;
                        }
                    }

                    else if (out_type == 2)
                    {
                        if (calculation511 > 510.9 && calculation511 < 511.01)
                            std::cout << " Pa : " << pa << " Pb : " << pb << " Pc : " << pc << " Pd : " << pd << std::endl;
                        return partial1;
                    }
                }
            }
        }
    }

    return out;
}

void ProcessAcquisition::ProcessGroupTomographic(std::string aqdName)
{

    GimnPET gantry;
    gantry.loadIdentificators(this->identificatorPath);
    std::string crystalGantryRotation = "y";
    gantry.setCrystalGantryRotation(crystalGantryRotation);
    std::string rotateAlong = "y";
    gantry.setRotateAlong(rotateAlong);
    gantry.setPLAThickness(2.0);   // cm
    gantry.setGantryRadius(35.00); // mm
    gantry.setCrystalDimension(TVector3(20.00, 26.88, 26.88));
    gantry.setGlobalCenter(TVector3(0, 0, 0));

    ExtractNames(aqdName);

    if (aqdName.find("group") != -1)
    {
        std::cout << " Processing Group file " << std::endl;

        Float_t cut = 1.0; // energy cut
        int event_counter = 0;
        Long64_t first_time{-1};
        int MinimumGroupSize = 1;
        float first_e{0}, first_tsec{0}, firstXmm{0}, firstYmm{0};
        int first_c{0}, chan{0}, first_chipID;
        float esum;
        int first_i{-1}, first_j{-1};

        // cria as variaveis nescessárias à leitura do arquivo root
        UShort_t mh_n{0}, mh_j{0};
        UInt_t channelID{0};
        Float_t energy{0.0};
        Long64_t time{0};
        std::vector<TString> ports;

        // abre o arquivo root
        std::unique_ptr<TFile> file(new TFile(aqdName.c_str(), "read"));
        std::unique_ptr<TTree> tree((TTree *)file->Get("data"));

        // cria os vetores que armazenarão as informações de tempo e energia dos eventos no grupo para calculo do timing
        std::vector<Long64_t> timeChanels(64, 0);
        std::vector<Float_t> energyChanels(64, 0);

        // set branches
        tree->SetBranchAddress("mh_n", &mh_n);
        tree->SetBranchAddress("mh_j", &mh_j);
        tree->SetBranchAddress("channelID", &channelID);
        tree->SetBranchAddress("energy", &energy);
        tree->SetBranchAddress("time", &time);

        auto inCoincidenceWith = identificator.getInCoincidenceWith();
        auto reference = identificator.getRef();

        std::shared_ptr<GimnWriteTomo> Output;
        // std::shared_ptr<GimnWriteClass> Output;
        if (outFileName.empty())
        {
            std::string outName = removePart(fileName, ".root");
            outName = processedFolder + outName + "_processed.root";
            Output = std::make_shared<GimnWriteTomo>(outName.c_str());
        }
        else
        {
            std::string outName = processedFolder + outFileName;
            Output = std::make_shared<GimnWriteTomo>(outName.c_str());
        }

        const int entries = tree->GetEntries();
        int pos;
        // Begins the iteration through the events
        for (Int_t event = 0; event <= entries; event++)
        {
            tree->GetEntry(event);
            int pos;
            Int_t chipID = (int)channelID / 64;
            Int_t channel = channelID - 64 * chipID;

            // =============================================================================================

            // aqui o programa busca qual a posição do chipID no vetor chpid de configuração
            // Primeiramente o programa ira buscar pelo chip ID no vetor de chipID's, se nao encontrar, irá criar um novo;

            auto it = std::find(identificator.chipID.begin(), identificator.chipID.end(), chipID);

            // Se ele nao entrar aqui ele vai perder a conta colocar o else depois
            if (it != identificator.chipID.end())
                pos = std::distance(identificator.chipID.begin(), it);

            // std::cout<<"Pos "<<pos<<std::endl;
            //  se numero de eventos no grupo maior do que um numero minimo

            if ((mh_n >= MinimumGroupSize) && (!(chipID == reference[pos]))) // se ele for maior do que o numero minimo, e nao for o detector de referencia
            {

                // APENAS PARA O PRIMEIRO EVENTO
                auto pos_xy = sipm.CorrectChannelToPixelPs(channel);
                // x esta para colunas [0]
                // y para linhas [1]

                double correctedEnergy;
                if (useEnergyCorrection)
                {
                    if (useLinearization)
                    {
                        correctedEnergy = correctEnergy(chipID, channel, energy, method) + linearComponent[chipID][channel];
                    }
                    else
                    {
                        correctedEnergy = correctEnergy(chipID, channel, energy, method);
                    }
                }
                else
                {
                    correctedEnergy = energy;
                }

                Float_t timeInSeconds = (float)time / 1.0e12;

                // eventos do grupo // Roda também pelo primeiro evento
                if (mh_j < (mh_n - 1))
                {
                    if (mh_j == 0) // Pega o primeiro evento dentro do grupo
                    {
                        event_counter++;
                        // std::cout << "------------------------" << std::endl;
                        // std::cout << "first event" << std::endl;
                        // std::cout << "------------------------" << std::endl;
                        if (energy > cut)
                        {
                            histograms.rawEnergyChannel[chipID][channel]->Fill(energy);
                            histograms.globalEnergyHistogramRaw[chipID]->Fill(energy);
                            histograms.correctedEnergyChannel[chipID][channel]->Fill(correctedEnergy);
                            histograms.globalEnergyHistogramCorrected[chipID]->Fill(correctedEnergy);
                        }
                        // Fills first Event;

                        first_e = energy;
                        first_time = time;
                        first_tsec = timeInSeconds;
                        first_i = pos_xy[0];
                        first_j = pos_xy[1];
                        firstXmm = sipm.getPositionsX()[pos_xy[1]];
                        firstYmm = sipm.getPositionsY()[pos_xy[0]];
                        first_c = 8 * pos_xy[1] + pos_xy[0];
                        chan = channel;
                        first_chipID = chipID;
                    }
                    sipm.Sipm_ecor[pos](pos_xy[0], pos_xy[1]) = +correctedEnergy;
                    sipm.Sipm_e[pos](pos_xy[0], pos_xy[1]) = +energy;
                    sipm.Sipm_t[pos](pos_xy[0], pos_xy[1]) = +timeInSeconds;
                    esum += energy;
                }
                // ultimo evento
                else if (mh_j == mh_n - 1)
                {
                    if (mh_j == 0) // Pega o primeiro evento dentro do grupo
                    {
                        event_counter++;
                        if (energy > cut)
                        {

                            histograms.rawEnergyChannel[chipID][channel]->Fill(energy);
                            histograms.globalEnergyHistogramRaw[chipID]->Fill(energy);
                            if (useEnergyCorrection)
                            {
                                histograms.correctedEnergyChannel[chipID][channel]->Fill(correctedEnergy);
                                histograms.globalEnergyHistogramCorrected[chipID]->Fill(correctedEnergy);
                            }
                        }
                        // Fills first Event;
                        first_e = energy;
                        first_i = pos_xy[0];
                        first_j = pos_xy[1];
                        first_time = time;
                        first_tsec = timeInSeconds;
                        firstXmm = sipm.getPositionsX()[pos_xy[1]];
                        firstYmm = sipm.getPositionsY()[pos_xy[0]];
                        first_c = 8 * pos_xy[1] + pos_xy[0];
                        chan = channel;
                    }
                    sipm.Sipm_ecor[pos](pos_xy[0], pos_xy[1]) = +correctedEnergy;
                    sipm.Sipm_e[pos](pos_xy[0], pos_xy[1]) = +energy;
                    sipm.Sipm_t[pos](pos_xy[0], pos_xy[1]) = +timeInSeconds;

                    if (first_e > cut)
                    {
                        // std::cout << "------------------------" << std::endl;
                        // std::cout << "Last event" << std::endl;
                        // std::cout << "------------------------" << std::endl;

                        // Fills the last event

                        // Uses the SiPM event buffer to calculate the event position
                        // using anger method
                        auto Pos3D = anger(sipm.Sipm_ecor[pos]);
                        auto Pos3DNum = angerNum(sipm.Sipm_ecor[pos]);

                        // Fill the event Filler
                        TomographicSingleEvent tse;
                        tse.mh_n = mh_n;
                        tse.chipID = first_chipID;
                        tse.channelID = chan;
                        tse.posI = first_i;
                        tse.posJ = first_j;
                        tse.SiPMPosX = firstXmm;
                        tse.SiPMPosY = firstYmm;
                        // Global and globalPrePosition
                        TVector3 rawPosition(firstXmm, firstYmm, 0);
                        TVector3 prePosition = gantry.getZRotation(rawPosition, first_chipID);
                        tse.globalPosXpre = prePosition[0];
                        tse.globalPosYpre = prePosition[1];
                        tse.globalPosZpre = prePosition[2];
                        TVector3 globalPos = gantry.getGlobalPositionPETSYS(rawPosition, first_chipID);
                        tse.globalPosX = globalPos[0];
                        tse.globalPosY = globalPos[1];
                        tse.globalPosZ = globalPos[2];
                        // Global and globalPrePosition for anger
                        TVector3 rawPositionAnger(Pos3D[0], Pos3D[1], Pos3D[2]);
                        TVector3 prePositionAnger = gantry.getZRotation(rawPositionAnger, first_chipID);
                        tse.globalPosXpreAnger = prePositionAnger[0];
                        tse.globalPosYpreAnger = prePositionAnger[1];
                        tse.globalPosZpreAnger = prePositionAnger[2];
                        TVector3 globalPosAnger = gantry.getGlobalPositionPETSYS(rawPositionAnger, first_chipID);
                        tse.globalPosXAnger = globalPosAnger[0];
                        tse.globalPosYAnger = globalPosAnger[1];
                        tse.globalPosZAnger = globalPosAnger[2];
                        // 2D Anger
                        tse.angerPosX = Pos3D[0];
                        tse.angerPosY = Pos3D[1];

                        // Sinogram Filling Not used in single Events;
                        tse.angleSino = -1;
                        tse.angleGantry = -1;
                        tse.rSino = -1;
                        tse.timeI = first_time;
                        tse.timeSec = first_tsec;
                        tse.coincidence_ID = 0;
                        tse.esum = sipm.Sipm_ecor[pos].Sum();
                        tse.energyCorrected = correctedEnergy;
                        tse.energy = energy;

                        TVector3 posDet(Pos3D[0], Pos3D[1], 0);
                        TVector3 autoFlood1 = gantry.getGlobalPositionPETSYS(posDet, first_chipID);

                        histograms.floodRaw[chipID]->Fill(autoFlood1[2], autoFlood1[1]);
                        histograms.floodCorrected[chipID]->Fill(Pos3D[0], Pos3D[1]);

                        Output->putData(tse);
                    }
                    sipm.Sipm_ecor[pos].Zero();
                    sipm.Sipm_e[pos].Zero();
                    sipm.Sipm_t[pos].Zero();
                }
            }
        }

        int cnt = 0;
        if (withPlots)
        {
            // writeMatrixOfDetectors(histograms);
            writeMatrixOfDetectorsRaw(histograms);
            writeMatrixOfDetectorsWhithHeatMap(histograms);
        }
        Output->writeObject(histograms, identificator, method);
        Output->close();

        if (VERBOSE)
            std::cout << "finish" << std::endl;
        std::cout << "o programa detectou " << event_counter << std::endl;
    }
    else
    {

        std::cout << "The given file is not a group file" << std::endl;
    }
}

void ProcessAcquisition::processCoincidenceTomographic(std::string aqdName, std::string rotateAlong = "y", std::string gantryCrystalRotation = "y")
{

    GimnPET gantry;
    gantry.loadIdentificators(this->identificatorPath);
    gantry.setCrystalGantryRotation(gantryCrystalRotation);
    gantry.setRotateAlong(rotateAlong);
    gantry.setPLAThickness(2.0);   // cm
    gantry.setGantryRadius(35.00); // mm
    gantry.setCrystalDimension(TVector3(20.00, 26.88, 26.88));
    gantry.setGlobalCenter(TVector3(0, 0, 0));

    ExtractNames(aqdName);
    float aqdAngle{0};

    if (fileName.find("Angle") == std::string::npos)
    {
        std::cout << "The file name does not contain 'Angle', so is not a Tomographic acquisition." << std::endl;
    }
    else
    {
        aqdAngle = get_angle(fileName);
    }

    gantry.gantryAngleStep(aqdAngle);

    // #COINCIDENCE tomo
    int coincidenceID{0};
    int firstChannel1{0}, firstChannel2{0};
    int initial_event{0}, ini_n1{0}, ini_n2{0};
    double firstEnergy1{0}, firstEnergy2{0}, timeSec1{0}, timeSec2{0};
    Long64_t first_time1{0}, first_time2{0};

    double first_e1{0}, first_e2{0};

    double firstXmm1{0}, firstYmm1{0}, firstXmm2{0}, firstYmm2{0};
    double first_i1{0}, first_j1{0}, first_i2{0}, first_j2{0};
    int groupSize = 1;

    Int_t first_chipID1{-1}, first_chipID2{-1};

    // Leitura do ROOT
    UShort_t mh_n1{0}, mh_j1{0}, mh_n2{0}, mh_j2{0};
    UInt_t channelID1{0}, channelID2{0};
    Float_t energy1{0.0}, energy2{0};
    Long64_t time1{0}, time2{0};

    std::shared_ptr<TFile> file(new TFile(aqdName.c_str(), "read"));
    std::shared_ptr<TTree> tree((TTree *)file->Get("data"));

    tree->SetBranchAddress("mh_n1", &mh_n1);
    tree->SetBranchAddress("mh_j1", &mh_j1);
    tree->SetBranchAddress("channelID1", &channelID1);
    tree->SetBranchAddress("energy1", &energy1);
    tree->SetBranchAddress("time1", &time1);
    tree->SetBranchAddress("mh_n2", &mh_n2);
    tree->SetBranchAddress("mh_j2", &mh_j2);
    tree->SetBranchAddress("channelID2", &channelID2);
    tree->SetBranchAddress("energy2", &energy2);
    tree->SetBranchAddress("time2", &time2);

    int entries = tree->GetEntries();

    std::shared_ptr<GimnWriteTomo> Output;
    if (outFileName.empty())
    {
        std::string outName = removePart(fileName, ".root");
        outName = processedFolder + outName + "_tomographic.root";
        Output = std::make_shared<GimnWriteTomo>(outName.c_str());
    }
    else
    {
        std::string outName = processedFolder + outFileName;
        Output = std::make_shared<GimnWriteTomo>(outName.c_str());
    }

    for (int entry = 0; entry < entries; entry++)
    {
        tree->GetEntry(entry);
        int pos1 = -1, pos2 = -1;

        Int_t chipID1 = channelID1 / 64;
        Int_t channel1 = channelID1 % 64;
        Int_t chipID2 = channelID2 / 64;
        Int_t channel2 = channelID2 % 64;

        if ((mh_n1 >= groupSize) && (mh_n2 >= groupSize))
        {
            auto it1 = std::find(identificator.chipID.begin(), identificator.chipID.end(), chipID1);
            auto it2 = std::find(identificator.chipID.begin(), identificator.chipID.end(), chipID2);

            if (it1 == identificator.chipID.end() || it2 == identificator.chipID.end())
                continue;

            pos1 = std::distance(identificator.chipID.begin(), it1);
            pos2 = std::distance(identificator.chipID.begin(), it2);

            if (!identificator.checkInCoincidence(pos1, chipID2))
                continue;

            auto pos_xy1 = sipm.CorrectChannelToPixelPs(channel1);
            auto pos_xy2 = sipm.CorrectChannelToPixelPs(channel2);

            double correctedEnergy1 = energy1, correctedEnergy2 = energy2;
            if (useEnergyCorrection)
            {
                if (useLinearization)
                {
                    correctedEnergy1 = correctEnergy(chipID1, channel1, energy1, method) + linearComponent[chipID1][channel1];
                    correctedEnergy2 = correctEnergy(chipID2, channel2, energy2, method) + linearComponent[chipID2][channel2];
                }
                else
                {
                    correctedEnergy1 = correctEnergy(chipID1, channel1, energy1, method);
                    correctedEnergy2 = correctEnergy(chipID2, channel2, energy2, method);
                }
            }

            if (std::isnan(correctedEnergy1))
            {
                correctedEnergy1 = 0;
            }
            if (std::isnan(correctedEnergy2))
            {
                correctedEnergy2 = 0;
            }

            Float_t timeInSeconds1 = (float)time1 / 1.0e12;
            Float_t timeInSeconds2 = (float)time2 / 1.0e12;
            Float_t timeDiference = (Float_t)(time1 - time2) / 1.0e12;

            if (mh_j1 == 0 && mh_j2 == 0)
            {
                coincidenceID++;
                initial_event = entry;
                ini_n1 = mh_n1;
                ini_n2 = mh_n2;

                sipm.Sipm_e[pos1](pos_xy1[0], pos_xy1[1]) = energy1;
                sipm.Sipm_e[pos2](pos_xy2[0], pos_xy2[1]) = energy2;
                sipm.Sipm_ecor[pos1](pos_xy1[0], pos_xy1[1]) = correctedEnergy1;
                sipm.Sipm_ecor[pos2](pos_xy2[0], pos_xy2[1]) = correctedEnergy2;
                sipm.Sipm_t[pos1](pos_xy1[0], pos_xy1[1]) = timeInSeconds1;
                sipm.Sipm_t[pos2](pos_xy2[0], pos_xy2[1]) = timeInSeconds2;

                firstXmm1 = sipm.getPositionsX()[pos_xy1[1]];
                firstYmm1 = sipm.getPositionsY()[pos_xy1[0]];
                firstXmm2 = sipm.getPositionsX()[pos_xy2[1]];
                firstYmm2 = sipm.getPositionsY()[pos_xy2[0]];

                first_i1 = pos_xy1[0];
                first_j1 = pos_xy1[1];
                first_i2 = pos_xy2[0];
                first_j2 = pos_xy2[1];
                first_e1 = energy1;
                first_e2 = energy2;
                first_chipID1 = chipID1;
                first_chipID2 = chipID2;
                firstEnergy1 = correctedEnergy1;
                firstEnergy2 = correctedEnergy2;
                firstChannel1 = channel1;
                firstChannel2 = channel2;
                timeSec1 = timeInSeconds1;
                timeSec2 = timeInSeconds2;
                first_time1 = time1;
                first_time2 = time2;

                histograms.rawEnergyChannel[chipID1][channel1]->Fill(energy1);
                histograms.rawEnergyChannel[chipID2][channel2]->Fill(energy2);
                histograms.correctedEnergyChannel[chipID1][channel1]->Fill(correctedEnergy1);
                histograms.correctedEnergyChannel[chipID2][channel2]->Fill(correctedEnergy2);
                histograms.globalEnergyHistogramRaw[chipID1]->Fill(energy1);
                histograms.globalEnergyHistogramRaw[chipID2]->Fill(energy2);
                histograms.globalEnergyHistogramCorrected[chipID1]->Fill(correctedEnergy1);
                histograms.globalEnergyHistogramCorrected[chipID2]->Fill(correctedEnergy2);
            }
            else if (mh_j1 != 0 && mh_j2 == 0)
            {
                sipm.Sipm_e[pos1](pos_xy1[0], pos_xy1[1]) += energy1;
                sipm.Sipm_ecor[pos1](pos_xy1[0], pos_xy1[1]) += correctedEnergy1;
                sipm.Sipm_t[pos1](pos_xy1[0], pos_xy1[1]) += timeInSeconds1;
            }
            else if (mh_j1 == 0 && mh_j2 != 0)
            {
                sipm.Sipm_e[pos2](pos_xy2[0], pos_xy2[1]) += energy2;
                sipm.Sipm_ecor[pos2](pos_xy2[0], pos_xy2[1]) += correctedEnergy2;
                sipm.Sipm_t[pos2](pos_xy2[0], pos_xy2[1]) += timeInSeconds2;
            }

            // if (mh_j1 == ini_n1 - 1 && mh_j2 == ini_n2 - 1)
            if (entry == (initial_event + (ini_n1 + ini_n2) - 2))
            {
                auto Pos3D1 = anger(sipm.Sipm_ecor[pos1]);
                auto Pos3D2 = anger(sipm.Sipm_ecor[pos2]);

                if (useAlignment)
                {

                    firstXmm1 = firstXmm1 - alignmentCorrection[first_chipID1]["x"];
                    firstYmm1 = firstYmm1 - alignmentCorrection[first_chipID1]["y"];
                    Pos3D1[0] = Pos3D1[0] - alignmentCorrection[first_chipID1]["x"];
                    Pos3D1[1] = Pos3D1[1] - alignmentCorrection[first_chipID1]["y"];

                    firstXmm2 = firstXmm2 - alignmentCorrection[first_chipID2]["x"];
                    firstYmm2 = firstYmm2 - alignmentCorrection[first_chipID2]["y"];
                    Pos3D2[0] = Pos3D2[0] - alignmentCorrection[first_chipID2]["x"];
                    Pos3D2[1] = Pos3D2[1] - alignmentCorrection[first_chipID2]["y"];

                    // std::cout<<"Pos3D1 : "<<Pos3D1[0]<<Pos3D1[0]<<Pos3D1[0]<<std::endl;
                    // std::cout<<"Pos3D2 : "<<Pos3D2[0]<<Pos3D2[0]<<Pos3D2[0]<<std::endl;
                    // std::cout<<"first_chipID1 "<<first_chipID1<<std::endl;
                    // std::cout<<"first_chipID2 "<<first_chipID2<<std::endl;
                    // std::cout<<"firstXmm1 : "<<firstXmm1<<std::endl;
                    // std::cout<<"firstYmm1 : "<<firstYmm1<<std::endl;
                    // std::cout<<"firstXmm2 : "<<firstXmm2<<std::endl;
                    // std::cout<<"firstYmm2 : "<<firstYmm2<<std::endl;
                    // std::cout<<"Pos3D1[0] : "<<Pos3D1[0]<<std::endl;
                    // std::cout<<"Pos3D1[1] : "<<Pos3D1[1]<<std::endl;
                    // std::cout<<"Pos3D2[0] : "<<Pos3D2[0]<<std::endl;
                    // std::cout<<"Pos3D2[1] : "<<Pos3D2[1]<<std::endl;
                }

                // Fill the event Filler
                TomographicSingleEvent tse1;
                tse1.mh_n = mh_n1;
                tse1.chipID = first_chipID1;
                tse1.channelID = firstChannel1;
                tse1.posI = first_i1;
                tse1.posJ = first_j2;
                tse1.SiPMPosX = firstXmm1;
                tse1.SiPMPosY = firstYmm1;

                // Global and globalPrePosition
                TVector3 rawPosition1(firstXmm1, firstYmm1, 0);
                TVector3 prePosition1 = gantry.getZRotation(rawPosition1, first_chipID1);
                tse1.globalPosXpre = prePosition1[0];
                tse1.globalPosYpre = prePosition1[1];
                tse1.globalPosZpre = prePosition1[2];
                TVector3 globalPos1 = gantry.getGlobalPositionPETSYS(rawPosition1, first_chipID1);
                tse1.globalPosX = globalPos1[0];
                tse1.globalPosY = globalPos1[1];
                tse1.globalPosZ = globalPos1[2];

                // Global and globalPrePosition for anger
                TVector3 rawPositionAnger1(Pos3D1[0], Pos3D1[1], 0);
                TVector3 prePositionAnger1 = gantry.getZRotation(rawPositionAnger1, first_chipID1);
                tse1.globalPosXpreAnger = prePositionAnger1[0];
                tse1.globalPosYpreAnger = prePositionAnger1[1];
                tse1.globalPosZpreAnger = prePositionAnger1[2];
                TVector3 globalPosAnger1 = gantry.getGlobalPositionPETSYS(rawPositionAnger1, first_chipID1);
                tse1.globalPosXAnger = globalPosAnger1[0];
                tse1.globalPosYAnger = globalPosAnger1[1];
                tse1.globalPosZAnger = globalPosAnger1[2];
                // 2D Anger
                tse1.angerPosX = Pos3D1[0];
                tse1.angerPosY = Pos3D1[1];
                tse1.timeI = first_time1;
                tse1.timeSec = timeSec1;

                // Event 2
                TomographicSingleEvent tse2;
                tse2.mh_n = mh_n2;
                tse2.chipID = first_chipID2;
                tse2.channelID = firstChannel2;
                tse2.posI = first_i2;
                tse2.posJ = first_j2;
                tse2.SiPMPosX = firstXmm2;
                tse2.SiPMPosY = firstYmm2;

                // Global and globalPrePosition
                TVector3 rawPosition2(firstXmm2, firstYmm2, 0);
                TVector3 prePosition2 = gantry.getZRotation(rawPosition2, first_chipID2);
                tse2.globalPosXpre = prePosition2[0];
                tse2.globalPosYpre = prePosition2[1];
                tse2.globalPosZpre = prePosition2[2];
                TVector3 globalPos2 = gantry.getGlobalPositionPETSYS(rawPosition2, first_chipID2);
                tse2.globalPosX = globalPos2[0];
                tse2.globalPosY = globalPos2[1];
                tse2.globalPosZ = globalPos2[2];

                // Global and globalPrePosition for anger
                TVector3 rawPositionAnger2(Pos3D2[0], Pos3D2[1], 0);
                TVector3 prePositionAnger2 = gantry.getZRotation(rawPositionAnger2, first_chipID2);
                tse2.globalPosXpreAnger = prePositionAnger2[0];
                tse2.globalPosYpreAnger = prePositionAnger2[1];
                tse2.globalPosZpreAnger = prePositionAnger2[2];
                TVector3 globalPosAnger2 = gantry.getGlobalPositionPETSYS(rawPositionAnger2, first_chipID2);
                tse2.globalPosXAnger = globalPosAnger2[0];
                tse2.globalPosYAnger = globalPosAnger2[1];
                tse2.globalPosZAnger = globalPosAnger2[2];
                // 2D Anger
                tse2.angerPosX = Pos3D2[0];
                tse2.angerPosY = Pos3D2[1];
                tse2.timeI = first_time2;
                tse2.timeSec = timeSec2;

                std::vector<double> DistanceAndAngle = gantry.getDistanceAndAngle(globalPos1, globalPos2, "y");
                std::vector<double> DistanceAndAngleAnger = gantry.getDistanceAndAngle(globalPosAnger1, globalPosAnger2, "y");

                tse1.angleSino = DistanceAndAngle[1];
                tse1.rSino = DistanceAndAngle[0];
                tse2.angleSino = DistanceAndAngle[1];
                tse2.rSino = DistanceAndAngle[0];
                tse1.angleSinoAnger = DistanceAndAngleAnger[1];
                tse1.rSinoAnger = DistanceAndAngleAnger[0];
                tse2.angleSinoAnger = DistanceAndAngleAnger[1];
                tse2.rSinoAnger = DistanceAndAngleAnger[0];

                // valido para segmentados
                auto slice = pos_xy1[1] + pos_xy2[1];
                tse1.slice = slice;
                tse2.slice = slice;

                tse1.angleGantry = aqdAngle;
                tse2.angleGantry = aqdAngle;
                tse1.coincidence_ID = coincidenceID;
                tse2.coincidence_ID = coincidenceID;

                tse1.energy = first_e1;
                tse1.energyCorrected = firstEnergy1;
                tse1.esum = sipm.Sipm_ecor[pos1].Sum();
                tse2.energy = first_e2;
                tse2.energyCorrected = firstEnergy2;
                tse2.esum = sipm.Sipm_ecor[pos2].Sum();

                TomographicCoincidenceEvent tce;
                tce.fill(tse1, tse2);

                Output->putData(tse1);
                Output->putData(tse2);
                Output->putData(tce);

                if (obtainAlignmentData)
                {

                    if (firstEnergy1 < 400.0)
                        histograms.floodRaw[first_chipID1]->Fill(Pos3D1[0], Pos3D1[1]);

                    if (firstEnergy2 < 400.0)
                        histograms.floodRaw[first_chipID2]->Fill(Pos3D2[0], Pos3D2[1]);
                }
                else
                {
                    if (firstEnergy1 > 420.0 && firstEnergy1 < 590)
                        histograms.floodRaw[first_chipID1]->Fill(Pos3D1[0], Pos3D1[1]);

                    if (firstEnergy2 > 420.0 && firstEnergy2 < 590)
                        histograms.floodRaw[first_chipID2]->Fill(Pos3D2[0], Pos3D2[1]);
                }
                
                if (firstEnergy1 > 420.0 && firstEnergy1 < 590)
                    histograms.floodCorrected[first_chipID2]->Fill(globalPosAnger2[2], globalPosAnger2[1]);
                if (firstEnergy2 > 420.0 && firstEnergy2 < 590)

                    histograms.floodCorrected[first_chipID1]->Fill(globalPosAnger1[2], globalPosAnger1[1]);

                sipm.Sipm_e[pos1].Zero();
                sipm.Sipm_e[pos2].Zero();
                sipm.Sipm_ecor[pos1].Zero();
                sipm.Sipm_ecor[pos2].Zero();
                sipm.Sipm_t[pos1].Zero();
                sipm.Sipm_t[pos2].Zero();
            }
        }
    }
    Output->writeObject(histograms, identificator, method);
    Output->close();
    if (withPlots)
    {
        // writeMatrixOfDetectors(histograms);
        writeMatrixOfDetectorsRaw(histograms);
        writeMatrixOfDetectorsWhithHeatMap(histograms);
        
        for (auto & cid : identificator.chipID )
            plotGlobalHistogramRaw(cid);
            //FindXY(histograms, 0, plotsFolder);
            //FindXY(histograms, 15, plotsFolder);
    }
    if (obtainAlignmentData)
    {
        saveAligment(histograms);
    }
    std::cout<<"Getting Out"<<std::endl;
}

void ProcessAcquisition::saveAligment(HistogramCoincidences &Histos)
{
    std::lock_guard<std::mutex> lock(mtx);

    std::string outputName = "";
    if (alignmentName.empty())
    {
        if (!fileName.empty())
        {
            std::cout << "filename is not empty the name is : " << fileName << std::endl;
            if (fileName.find(".root") != -1)
                outputName = removePart(fileName, ".root");
            else
                outputName = fileName;

            outputName = outputName + "-AlignmentCorrection.root";
        }
        else
            outputName = "AlignmentCorrection.root";
    }
    else if (alignmentName.find(".root") != -1)
    {
        outputName = removePart(alignmentName, ".root");
        outputName = outputName + "-AlignmentCorrection.root";
    }
    else
    {
        outputName = alignmentName;
    }

    outputName = alignmentFolder + outputName;



    std::cout << outputName << std::endl;
    std::shared_ptr<TFile> alignmentCorrectionFile(new TFile(outputName.c_str(), "recreate"));
    std::shared_ptr<TTree> alignment;
    alignment = std::make_shared<TTree>("AligmentCorrection", Form("AligmentCorrection-%d", DIFERENTIATOR));
    Float_t xCorrection;
    Float_t yCorrection;
    Int_t chipID;
    alignment->Branch("chipID", &chipID);
    alignment->Branch("xCorrection", &xCorrection);
    alignment->Branch("yCorrection", &yCorrection);

    for (auto &cid : identificator.chipID)
    {
        std::cout<<"ChipID"<<cid<<std::endl;

        TVector3 XY = FindXY(histograms, cid, plotsFolder);
        alignmentCorrection[cid]["x"] = XY[0];
        alignmentCorrection[cid]["y"] = XY[1];
        chipID = cid;
        xCorrection = XY[0];
        yCorrection = XY[1];
        alignment->Fill();
        std::cout<<"Process Filled"<<std::endl;
    }

    alignmentCorrectionFile->cd();
    alignment->Write();
    alignmentCorrectionFile->Close();

    useAlignment = true;
}

void ProcessAcquisition::readAligment()
{
    std::lock_guard<std::mutex> lock(mtx);
    if (!alignmentFileName.empty())
    {
        std::unique_ptr<TFile> file(new TFile(alignmentFileName.c_str(), "read"));
        std::unique_ptr<TTree> aligment((TTree *)file->Get("AligmentCorrection"));
        Float_t xCorrection;
        Float_t yCorrection;
        Int_t chipID;
        aligment->SetBranchAddress("chipID", &chipID);
        aligment->SetBranchAddress("xCorrection", &xCorrection);
        aligment->SetBranchAddress("yCorrection", &yCorrection);

        int entries = aligment->GetEntries();
        std::map<int, std::map<std::string, float>> alignment;
        for (int entry = 0; entry < entries; entry++)
        {
            aligment->GetEntry(entry);
            alignmentCorrection[chipID]["x"] = xCorrection;
            alignmentCorrection[chipID]["y"] = yCorrection;
        }
        useAlignment = true;
    }
    else
    {
        std::cout << "Please set the AligmentFile" << std::endl;
    }
}
