

HistosMaps generateHistos(std::string file_name)
{
    
    std::cout << "Generating Histograms" << std::endl;
    std::cout << "File Name : " << file_name << std::endl;

    
    std::cout << "Gets here" << std::endl;
    ROOT::RDataFrame configurations("Configurations", file_name.c_str());
    HistosMaps Histograms;

    auto nEntries = configurations.Count().GetValue();
    std::cout << "Number of entries : " << nEntries << std::endl;
    
    std::cout << "Gets here - 1" << std::endl;

    // Lamda declaration
    auto readConfigurations = [&Histograms](int chipID)
    {   
        // Creates the Histograms
        std::shared_ptr<TH2F> floodRaw(new TH2F( Form("Flood Hist - Raw - %d-%d", chipID,DIFERENTIATOR), Form("Flood Hist - Raw ChipID-%d", chipID),128, -13.44, 13.44, 256, -13.44, 13.44));
        std::shared_ptr<TH2F> floodNadig(new TH2F( Form("Flood Hist - Nadig - %d-%d", chipID,DIFERENTIATOR), Form("Flood Hist - Nadig ChipID-%d", chipID),128, -13.44, 13.44, 256, -13.44, 13.44));
        std::shared_ptr<TH2F> floodWang(new TH2F(Form("Flood Hist - Wang - %d-%d", chipID,DIFERENTIATOR), Form("Flood Hist - Wang  ChipID-%d", chipID), 128, -13.44, 13.44, 256, -13.44, 13.44));
        std::shared_ptr<TH2F> floodPoli(new TH2F(Form("Flood Hist - Poli - %d-%d", chipID,DIFERENTIATOR), Form("Flood Hist - Poli  ChipID-%d", chipID), 128, -13.44, 13.44, 256, -13.44, 13.44));
        std::shared_ptr<TH2F> floodPetSys(new TH2F(Form("Flood Hist - PetSys - %d-%d", chipID,DIFERENTIATOR), Form("Flood Hist - PetSys  ChipID-%d", chipID), 128, -13.44, 13.44, 256, -13.44, 13.44));
        
        
        
        std::shared_ptr<TH2F> floodNadig_window(new TH2F( Form("Flood Hist - Nadig - %d-%d- window", chipID,DIFERENTIATOR), Form("Flood Hist - Nadig ChipID-%d- window", chipID),128, -13.44, 13.44, 256, -13.44, 13.44));
        std::shared_ptr<TH2F> floodWang_window(new TH2F(Form("Flood Hist - Wang - %d-%d- window", chipID,DIFERENTIATOR), Form("Flood Hist - Wang  ChipID-%d- window", chipID), 128, -13.44, 13.44, 256, -13.44, 13.44));
        std::shared_ptr<TH2F> floodPoli_window(new TH2F(Form("Flood Hist - Poli - %d-%d- window", chipID,DIFERENTIATOR), Form("Flood Hist - Poli  ChipID-%d- window", chipID), 128, -13.44, 13.44, 256, -13.44, 13.44));
        std::shared_ptr<TH2F> floodPetSys_window(new TH2F(Form("Flood Hist - PetSys - %d-%d- window", chipID,DIFERENTIATOR), Form("Flood Hist - PetSys  ChipID-%d- window", chipID), 128, -13.44, 13.44, 256, -13.44, 13.44));
        
        std::shared_ptr<TH1F> globalHistoNadig(new TH1F(Form("global corrected Nadig ChipID-%d-%d", chipID,DIFERENTIATOR),Form("global corrected Nadig ChipID-%d", chipID),  200, 0, 1400));
        std::shared_ptr<TH1F> globalHistoWang(new TH1F( Form("global corrected Wang ChipID-%d-%d", chipID,DIFERENTIATOR),Form("global corrected Wang ChipID-%d", chipID), 200, 0, 1400));
        std::shared_ptr<TH1F> globalHistoRaw(new TH1F(Form("global Cru  ChipID-%d-%d", chipID,DIFERENTIATOR), Form("global raw  ChipID-%d", chipID),  200, 0, 0));
        std::shared_ptr<TH1F> globalHistoPoli(new TH1F( Form("global corrected Poli ChipID-%d-%d", chipID,DIFERENTIATOR),Form("global corrected Poli ChipID-%d", chipID), 200, 0, 1400));
        std::shared_ptr<TH1F> globalHistoPetSys(new TH1F( Form("global corrected PetSys ChipID-%d-%d", chipID,DIFERENTIATOR),Form("global corrected PetSys ChipID-%d", chipID), 200, 0, 1400));


        // Fill The Maps
        Histograms.chipIDs.push_back(chipID);
        Histograms.GlobalHistsNadig[chipID] = globalHistoNadig;
        Histograms.GlobalHistsWang[chipID] = globalHistoWang;
        Histograms.GlobalHistsRaw[chipID] = globalHistoRaw;
        Histograms.GlobalHistsPoli[chipID] = globalHistoPoli;
        Histograms.GlobalHistsPetSys[chipID] = globalHistoPetSys;

        Histograms.FloodsRaw[chipID] = floodRaw;
        Histograms.FloodsWang_window[chipID] = floodWang_window;
        Histograms.FloodsNadig_window[chipID] = floodNadig_window;
        Histograms.FloodsPoli_window[chipID] = floodPoli_window;
        Histograms.FloodsPetSys_window[chipID] = floodPetSys_window;


        Histograms.FloodsWang[chipID] = floodWang;
        Histograms.FloodsNadig[chipID] = floodNadig;
        Histograms.FloodsPoli[chipID] = floodPoli;
        Histograms.FloodsPetSys[chipID] = floodPetSys;

    };

    configurations.Foreach(readConfigurations, {"chipID"});
    std::cout << "Gets here 2" << std::endl;
    return Histograms;
}
