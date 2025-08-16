/**
 * @brief This class stores information from the SiPM used while processing acquired data.
 * It have some important features such as convert channelID into an index for rows and columns, or into the position of the event
 * in mm, in this case the zero is located in the center of the crystal array.
 * \n In the case of segmented crystals the position represents the center of the crystal element.
 * \n Other options such as microCells have been added for future works.
 * \n This class have hard coded the positions of the channels in a matrix  using as reference the Ketek Pa 3325 wb 0808  channels disposition
 * \n It also have implemented a method for converting the crystalID from a gate acquisition into a i,j index that represents the position in a mxn matrix.
 */
class SiPM
{
    // informações da SiPM
private:
    Float_t PichX{0}, PichY{0};
    Int_t channelNum{0}, microCells{0}, rows{0}, cols{0};
    Float_t dimensionXmm{0.0}, dimensionYmm{0.0};
    std::vector<Float_t> positionsX;
    std::vector<Float_t> positionsY;
    int counter = 0;

    // Initialize the vectors containing the position in mm relative to the center of each SiPM into an array
    void Initialize();

public:
    Identificator Id;
    GimnData Histos;
    HistogramCoincidences HistoCoinc;



    std::vector<TMatrixT<Float_t>> Sipm_e;
    std::vector<TMatrixT<Float_t>> Sipm_ecor;
    std::vector<TMatrixT<Float_t>> Sipm_t;
    std::vector<TMatrixT<Float_t>> Sipm_c;


    // costructors
    SiPM();
    SiPM(SiPM const &s);
    SiPM(Int_t ChanNum, Int_t Rows, Int_t Columns, Float_t Pitch, Identificator Id);

    SiPM(Int_t ChanNum, Int_t Rows, Int_t Columns, Float_t Pitch);

    
    SiPM(Int_t ChanNum, Int_t Rows, Int_t Columns, Float_t Pitch, Identificator Id, int counter);
    //~SiPM(){Histos.flush();}

    // Getters

    /// @brief Builded to give access to the vector containing the positions of the SiPM centers in X direction;
    /// @return returns the vector containing the centers of the SiPM's in mm in X direction
    inline std::vector<Float_t> getPositionsX() { return positionsX; }
    /// @brief Builded to give access to the vector containing the positions of the SiPM centers in Y direction;
    /// @return returns the vector containing the centers of the SiPM's in mm in Y direction
    inline std::vector<Float_t> getPositionsY() { return positionsY; }
    /// @brief Give acces to protected data containing number of channels;
    /// @return The number of channels in the SiPM
    inline Int_t getChannelNumber() { return channelNum; }
    /// @brief Give acces to protected data containing the number of channels;
    /// @return The number of channels that the SiPM have;
    inline Int_t getMicrocellNum() { return microCells; }
    /// @brief Give acces to protected data containing the number of rows
    /// @return The number of rows
    inline Int_t getRows() { return rows; }
    /// @brief Give acces to protected data containing the number of columns
    /// @return The number of columns
    inline Int_t getCols() { return cols; }
    /// @brief  this method is designed for returning the dimensions of the SiPM used;
    /// @return a vector containing in the index 0 the dimensions in mm of the X axis and in the index 1 the dimensions in mm of Y axis
    inline std::vector<Float_t> getDimensions()
    {
        std::vector<Float_t> a{dimensionXmm, dimensionYmm};
        return a;
    }
    /// @brief This metod is designed to return the position in mm from a given index in the X direction
    /// @param pos refers to the index of the SiPM in a given axis
    /// @return The position in mm from the index "pos" in the X axis;
    inline Float_t getXposition(Int_t pos)
    {
        if ((pos >= cols) && (pos < 0))
            return 666;
        else
            return positionsX[pos];
    }
    /// @brief This metod is designed to return the position in mm from a given index in the Y direction
    /// @param pos refers to the index of the SiPM in a given axis
    /// @return The position in mm from the index "pos" in the Y axis;
    inline Float_t getYposition(Int_t pos)
    {
        if ((pos >= rows) && (pos < 0))
            return 666;
        else
            return positionsY[pos];
    }

    inline GimnData &getHistos() { return this->Histos; }

    // setters
    inline void setChannelNumber(Int_t chn) { channelNum = chn; }
    inline void setRows(Int_t row) { rows = row; }
    inline void setCols(Int_t col) { cols = col; }
    inline void setDimensionX(Float_t x) { dimensionXmm = x; }
    inline void setDimensionY(Float_t y) { dimensionYmm = y; }
    inline void setMicrocellNum(Int_t mcn) { microCells = mcn; }
    inline void setDimensions(Float_t x, Float_t y)
    {
        dimensionXmm = x;
        dimensionYmm = y;
    }
    inline void setHistos(GimnData &Histos) { this->Histos = Histos; }
    inline TMatrixT<Float_t> getEnergyMatrix(int det) { return Sipm_e[det]; }
    inline TMatrixT<Float_t> getCorrectedEnergyMatrix(int det) { return Sipm_ecor[det]; }
    inline TMatrixT<Float_t> getTimeMatrix(int det) { return Sipm_t[det]; }
    inline TMatrixT<Int_t> getChannelMatrix(int det) { return Sipm_c[det]; }


    void InitializePixels()
    {

        if (VERBOSE)
            std::cout << Id.getDetNum() << std::endl;

        for (int detector = 0; detector < Id.getDetNum(); detector++)
        {
            Sipm_e.push_back(TMatrixT<double>(rows, cols));
            Sipm_ecor.push_back(TMatrixT<double>(rows, cols));
            Sipm_t.push_back(TMatrixT<double>(rows, cols));
            Sipm_c.push_back(TMatrixT<double>(rows, cols));

        }
    }

    // Positioners :
    /// @brief This method is designed for returning the index j,i relative to the position of the SiPM inside the array. Is designed for a Ketek 3325 Wb0808 PA
    /// @param Channel Representes the SiPM channel to be converted into a pair <y,x>
    /// @return the indexes of the position of the SiPM inside the array as a pair <y,x>
    std::vector<Int_t> CorrectChannelToPixelPs(Int_t Channel); // Petsys

    std::vector<Int_t> CorrectChannelToPixelGate(Int_t Channel); // Petsys

    // Printers
    /// @brief Prints the position of the center of each pixel in X and Y directions
    void showPositions(); // Prints the center position of each SiPM into an Array.

    // Operators
    SiPM &operator=(SiPM const &);

    // implementar metodo da parabola

    std::map<std::string, Float_t> FindFWHM(TH1F *histo, Double_t sigma, Double_t Threshold)
    {
        std::map<std::string, Float_t> out;

        TSpectrum *s = new TSpectrum();
        int npeaks = s->Search(histo, sigma, "", Threshold);
        double *peaks = s->GetPositionX();

        // Find the peak with the highest amplitude
        int maxPeakIndex = 0;
        double maxPeakHeight = 0;
        for (int i = 0; i < npeaks; i++)
        {
            double height = histo->GetBinContent(histo->FindBin(peaks[i]));
            if (height > maxPeakHeight)
            {
                maxPeakHeight = height;
                maxPeakIndex = i;
            }
        }

        // Fit a Gaussian to the peak with the highest amplitude
        double mean = peaks[maxPeakIndex];
        Double_t amplitude = histo->GetBinContent(histo->FindBin(mean));
        double sigma2 = 1; // Valor inicial para sigma
        TF1 *fitFunc = new TF1("fitFunc", "[0] * TMath::Gaus(x, [1], [2])", mean - 1 * sigma2, mean + 1 * sigma2);
        fitFunc->SetParameters(amplitude, mean, sigma2);
        histo->Fit(fitFunc, "Q", "", mean - 1, mean + 1);
        // Calculate FWHM
        double fwhm = 2.35482 * fitFunc->GetParameter(2); // FWHM = 2.35482 * sigma

        // Return a map with the FWHM value
        out.insert(std::make_pair("FWHM", fwhm));

        return out;
    }


    std::map<std::string, Float_t> FindGauss(TH1F *histo, Double_t sigma, Double_t Threshold)
    {
        std::map<std::string, Float_t> out;

        TSpectrum s;
        int npeaks = s.Search(histo, sigma, "", Threshold);
        double *peaks = s.GetPositionX();

        // Encontra o pico com maior amplitude
        int maxPeakIndex = 0;
        double maxPeakHeight = 0;
        for (int i = 0; i < npeaks; i++)
        {
            double height = histo->GetBinContent(histo->FindBin(peaks[i]));
            if (height > maxPeakHeight)
            {
                maxPeakHeight = height;
                maxPeakIndex = i;
            }
        }

        // Ajusta uma gaussiana no pico de maior amplitude
        double mean = peaks[maxPeakIndex];
        double sigma2 = 1; // Valor inicial para sigma
        TF1 *fitFunc = new TF1("fitFunc", "gaus", mean - 3 * sigma2, mean + 3 * sigma2);
        fitFunc->SetParameters(maxPeakHeight, mean, sigma2);
        histo->Fit(fitFunc, "Q");

        // Retorna um mapa com os parâmetros da gaussiana ajustada
        out.insert(std::make_pair("Mean", fitFunc->GetParameter(1)));
        out.insert(std::make_pair("Sigma", fitFunc->GetParameter(2)));
        out.insert(std::make_pair("Amplitude", fitFunc->GetParameter(0)));

        return out;
    }
};

/// @brief  Default constructor of the SiPM. In this case the SiPM will be builded using the parameters for a Ketek 3325WB0808
SiPM::SiPM()
{
    // default start;

    rows = 8;
    cols = 8;
    channelNum = 64;
    PichX = 3.36;
    PichY = 3.36;
    Initialize();
    InitializePixels();
}
/**
 * @brief Construct a new Si P M:: Si P M object
 *
 * @param s
 */
SiPM::SiPM(SiPM const &s) : PichX{s.PichX}, PichY{s.PichY},
                            positionsX{s.positionsX}, positionsY{s.positionsY},
                            channelNum{s.channelNum}, microCells{s.microCells},
                            rows{s.rows}, cols{s.cols},
                            dimensionXmm{s.dimensionXmm}, dimensionYmm{s.dimensionYmm}
{
}

SiPM::SiPM(Int_t ChanNum, Int_t Rows, Int_t Columns, Float_t Pitch, Identificator Id)
{

    this->channelNum = ChanNum;
    this->rows = Rows;
    this->cols = Columns;
    this->PichX = this->PichY = Pitch;
    this->Id = Id;
    Histos = GimnData(Id);
    Initialize();
    InitializePixels();
}

SiPM::SiPM(Int_t ChanNum, Int_t Rows, Int_t Columns, Float_t Pitch)
{

    this->channelNum = ChanNum;
    this->rows = Rows;
    this->cols = Columns;
    this->PichX = this->PichY = Pitch;
    Histos = GimnData(Id);
    Initialize();
    InitializePixels();
}


SiPM::SiPM(Int_t ChanNum, Int_t Rows, Int_t Columns, Float_t Pitch, Identificator Id, int counter)
{
    this->counter = counter;
    this->channelNum = ChanNum;
    this->rows = Rows;
    this->cols = Columns;
    this->PichX = this->PichY = Pitch;
    this->Id = Id;
    Histos = GimnData(Id);
    Initialize();
    InitializePixels();
}

SiPM &SiPM::operator=(SiPM const &s)
{

    PichX = s.PichX;
    PichY = s.PichY;
    channelNum = s.channelNum;
    microCells = s.microCells;
    rows = s.rows;
    cols = s.cols;
    dimensionXmm = s.dimensionXmm;
    dimensionYmm = s.dimensionYmm;
    positionsX = s.positionsX;
    positionsY = s.positionsY;
    Id = s.Id;
    Histos = s.Histos;
    InitializePixels();
    return *this;
}

std::vector<Int_t> SiPM::CorrectChannelToPixelGate(Int_t crystalID){
    std::vector<Int_t> out;

    //preencher

    

    return out;
}


std::vector<Int_t> SiPM::CorrectChannelToPixelPs(Int_t channel)
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
    // retorna linha/coluna
    std::vector<Int_t> aux{CORRECTION_MAP[channel][1], CORRECTION_MAP[channel][2]};
    return aux;
}

void SiPM::Initialize()
{
    positionsX.assign(cols, 0.0);
    positionsY.assign(rows, 0.0);

    Float_t x{0}, y{0};

    for (Int_t i = 0; i < cols; i++)
    {

        x = (i + 4) - 8;
        if (x < 0)
            x = x + 1;

        if (i < 4)
            x = x * PichX - PichX / 2.0;
        else
            x = x * PichX + PichX / 2.0;
        positionsX[i] = x;
    }

    for (Int_t j = 0; j < rows; j++)
    {

        y = (j + 4) - 8;
        if (y < 0)
            y = y + 1;

        if (j < 4)
            y = -(y * PichY - PichY / 2.0);
        else
            y = -(y * PichY + PichY / 2.0);

        positionsY[j] = y;
    }


}

void SiPM::showPositions()
{
    
    std::cout << "positions for X" << std::endl;
    for (int i = 0; i < positionsX.size(); i++)
    {
        std::cout << positionsX[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "positions for y" << std::endl;
    for (int i = 0; i < positionsY.size(); i++)
    {
        std::cout << positionsY[i] << " ";
    }
    std::cout << std::endl;
}



correctionParameters LoadCalibrationFactors(std::string calibrationParameters, HistosMaps &Histograms, SiPM &sipm)
{
    // declares root Dataframe for retrieving calibration parameters
    ROOT::RDataFrame CalibrationFactors("CalibrationFactors", calibrationParameters.c_str());

    // Utils
    std::vector<Float_t> xPos = sipm.getPositionsX();
    std::vector<Float_t> yPos = sipm.getPositionsY();

    int nchans = 64;
    std::vector<int> chanEquivalent(nchans, 0);

    std::map<int, std::vector<std::map<std::string, double>>> fatoresDeCalibracaoNadig;
    std::map<int, std::vector<std::map<std::string, double>>> fatoresDeCalibracaoWang;
    std::map<int, std::vector<std::map<std::string, double>>> fatoresDeCalibracaoPoli;
    std::map<int, std::vector<std::map<std::string, double>>> fatoresDeCalibracaoPetSys;



    std::map<int, std::vector<std::shared_ptr<TH1F>>> channelRaw;
    std::map<int, std::vector<std::shared_ptr<TH1F>>> canaisCorNadig;
    std::map<int, std::vector<std::shared_ptr<TH1F>>> canaisCorWang;
    std::map<int, std::vector<std::shared_ptr<TH1F>>> canaisCorPoli;
    std::map<int, std::vector<std::shared_ptr<TH1F>>> canaisCorPetSys;


    for (auto &val : Histograms.chipIDs)
    {
        std::vector<std::shared_ptr<TH1F>> chanRaw;
        std::vector<std::shared_ptr<TH1F>> canCorNadig;
        std::vector<std::shared_ptr<TH1F>> canCorWang;
        std::vector<std::shared_ptr<TH1F>> canCorPoli;
        std::vector<std::shared_ptr<TH1F>> canCorPetSys;

        for (int ch = 0; ch < nchans; ch++)
        {

            std::string cname = Form("canal-%d", ch);
            std::string gname1 = cname + "-" + std::to_string(val) + "cru";
            std::string gname2 = cname + "-" + std::to_string(val) + "corWang";
            std::string gname3 = cname + "-" + std::to_string(val) + "corNadig";
            std::string gname4 = cname + "-" + std::to_string(val) + "corPoli";
            std::string gname5 = cname + "-" + std::to_string(val) + "corPetSys";

            
            std::string gname1D = gname1 + "-" + std::to_string(DIFERENTIATOR);
            std::string gname2D = gname2 + "-" + std::to_string(DIFERENTIATOR);
            std::string gname3D = gname3 + "-" + std::to_string(DIFERENTIATOR);
            std::string gname4D = gname4 + "-" + std::to_string(DIFERENTIATOR);
            std::string gname5D = gname5 + "-" + std::to_string(DIFERENTIATOR);


            std::shared_ptr<TH1F> aux1(new TH1F(gname1D.c_str(), gname1.c_str(), 200, 0, 0));
            std::shared_ptr<TH1F> aux2(new TH1F(gname2D.c_str(), gname2.c_str(), 200, 0, 1500));
            std::shared_ptr<TH1F> aux3(new TH1F(gname3D.c_str(), gname3.c_str(), 200, 0, 1500));
            std::shared_ptr<TH1F> aux4(new TH1F(gname4D.c_str(), gname4.c_str(), 200, 0, 1500));
            std::shared_ptr<TH1F> aux5(new TH1F(gname5D.c_str(), gname5.c_str(), 200, 0, 1500));


            chanRaw.push_back(aux1);
            canCorWang.push_back(aux2);
            canCorNadig.push_back(aux3);
            canCorPoli.push_back(aux4);
            canCorPetSys.push_back(aux5);


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
            chanEquivalent[ch] =equivalent;
        }

        Histograms.channelRaw[val] = chanRaw; //
        Histograms.canaisCorNadig[val] = canCorNadig;
        Histograms.canaisCorWang[val] = canCorWang;

        Histograms.canaisCorPoli[val] = canCorPoli;
        Histograms.canaisCorPetSys[val] = canCorPetSys;
    }

    auto ParameterReader = [&fatoresDeCalibracaoNadig, &fatoresDeCalibracaoWang, &fatoresDeCalibracaoPoli,&fatoresDeCalibracaoPetSys](int chipID, int channel, double c, double s, double k1, double k2, double b,  double pa, double pb, double pc, double P0, double P1,double P2,double P3 )
    {
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

    CalibrationFactors.Foreach(ParameterReader, {"chipID", "channel", "c", "s", "k1", "k2", "b" ,"pa","pb","pc","P0","P1","P2","P3"});

    correctionParameters cParam;
    cParam.fatoresDeCalibracaoNadig = fatoresDeCalibracaoNadig;
    cParam.fatoresDeCalibracaoWang = fatoresDeCalibracaoWang;
    cParam.fatoresDeCalibracaoPoli = fatoresDeCalibracaoPoli;
    cParam.fatoresDeCalibracaoPetSys = fatoresDeCalibracaoPetSys;
    cParam.chanEquivalent = chanEquivalent;

    return cParam;
}