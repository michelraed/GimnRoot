TVector3 FindXY(HistogramCoincidences &Histos, int chipID, std::string plotsFolder)
{

    //std::lock_guard<std::mutex> lock(mtx); // Lock mutex for thread safety

    // Obter histograma bruto (já em shared_ptr)
    std::shared_ptr<TH2F> floodRaw = Histos.floodRaw[chipID];
    TH2F* h2 = floodRaw.get(); // necessário temporariamente para Clone()

    // Clonar o histograma como TH2D e empacotar em smart pointer
    std::shared_ptr<TH2D> h2Clean(static_cast<TH2D*>(h2->Clone("h2Clean")));

    h2Clean->Smooth();


    //h2Clean->Smooth();
    // Determinar valor máximo
    double max = h2Clean->GetMaximum();

    // Limpeza de picos locais (> 30% do máximo)
    for (int ix = 1; ix <= h2Clean->GetNbinsX(); ++ix) {
        for (int iy = 1; iy <= h2Clean->GetNbinsY(); ++iy) {

            if (h2Clean->GetBinContent(ix, iy) > 0.05 * max)
                h2Clean->SetBinContent(ix, iy, 0);
        }
    }

    // Criar canvas usando smart pointer
    std::shared_ptr<TCanvas> canvas = std::make_shared<TCanvas>("C1", "Fit Gaussiano", 800, 700);

    // Limites do histograma
    double xmin = h2Clean->GetXaxis()->GetXmin();
    double xmax = h2Clean->GetXaxis()->GetXmax();
    double ymin = h2Clean->GetYaxis()->GetXmin();
    double ymax = h2Clean->GetYaxis()->GetXmax();


    // Criar TF2 com smart pointer
    std::shared_ptr<TF2> gaus2D = std::make_shared<TF2>(
        "gaus2D",
        "[0]*exp(-0.5*((x-[1])**2/[2]**2 + (y-[3])**2/[4]**2))",
        xmin, xmax, ymin, ymax
    );

    // Parâmetros iniciais
    double muX = 0;//h2Clean->GetMean(1);
    double muY = 0;//h2Clean->GetMean(2);
    double sigmaX = 5;//h2Clean->GetRMS(1);
    double sigmaY = 5;//h2Clean->GetRMS(2);

    gaus2D->SetParameters(max, muX, sigmaX, muY, sigmaY);
    gaus2D->SetParNames("Amplitude", "muX", "sigmaX", "muY", "sigmaY");

    
    // Fit 2D
    h2Clean->Fit(gaus2D.get(), "R");


    // Plot do resultado
    h2Clean->Draw("COLZ");
    gaus2D->Draw("same CONT3");

    std::string outName =  h2->GetName();
    // Salvar imagem
    outName = plotsFolder+outName +".png";
    canvas->SaveAs(outName.c_str(), "png");

    // Impressão
    std::cout << "\n--- Parâmetros estimados ---\n";
    std::cout << "μx = " << gaus2D->GetParameter(1) << std::endl;
    std::cout << "σx = " << gaus2D->GetParameter(2) << std::endl;
    std::cout << "μy = " << gaus2D->GetParameter(3) << std::endl;
    std::cout << "σy = " << gaus2D->GetParameter(4) << std::endl;

    // Retorna posição estimada (z = 0)
    return TVector3(gaus2D->GetParameter(1), gaus2D->GetParameter(3), 0.0);
}