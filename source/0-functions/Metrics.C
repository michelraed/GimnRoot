
struct Point2D
{
    double x, y;
};

double distance(const Point2D &a, const Point2D &b)
{
    double dx = b.x - a.x;
    double dy = b.y - a.y;

    std::cout << "Distance " << std::sqrt(dx * dx + dy * dy) << std::endl;
    return std::sqrt(dx * dx + dy * dy);
}

double CalculteFWHMPixel(std::shared_ptr<TH2F> hist, char direction, double xPosition, double yPosition)
{
    // This function will calculate the FWHM for each dot corresponding to a crystal in the Flood Image

    std::cout << "xPosition " << xPosition << std::endl;
    std::cout << "yPosition " << yPosition << std::endl;

    std::shared_ptr<TH1F> projection;
    
    // Gets the projections in order to count the Total of Pixels.
    Int_t binPeak{0};
    if (direction == 'x')
    {
        binPeak = hist->GetXaxis()->FindBin(xPosition);
        int ybn = hist->GetYaxis()->FindBin(yPosition);
        std::shared_ptr<TH1F> pj((TH1F *)hist->ProjectionX("px", ybn - 5, ybn + 5));
        projection = pj;
    }

    else if (direction == 'y')
    {
        int xbn = hist->GetXaxis()->FindBin(xPosition);
        binPeak = hist->GetYaxis()->FindBin(yPosition);
        std::shared_ptr<TH1F> pj((TH1F *)hist->ProjectionY("py", xbn - 5, xbn + 5));
        projection = pj;
    }

    std::cout << "Bin Peak " << binPeak << std::endl;

    int peakBinIndex = binPeak;

    double countsAtPeak{0};
    double peakPositonmm{0};
    double peakPos{0};
    double yVal = 0;

    // Search for the peak 
    for (int count = peakBinIndex - 5; count <= peakBinIndex + 5; count++)
    {
        yVal = projection->GetBinContent(count);
        // std::cout<<"YVal "<<yVal<<std::endl;
        // std::cout<<"position "<<count<<std::endl;
        if (countsAtPeak < yVal)
        {
            countsAtPeak = yVal;
            peakPositonmm = projection->GetBinCenter(count);
            peakPos = count;

            // std::cout << " counts ate peak " << countsAtPeak << std::endl;
            // std::cout << " PeakPosition in mm " << peakPositonmm << std::endl;
            // std::cout << " PeakPosition " << count << std::endl;
        }
    }

    // Generates a left index and a right index to get the lateral values of the FWHM
    int leftIndex = peakPos;
    int rightIndex = leftIndex;

    std::cout << "Left Index " << leftIndex << std::endl;
    std::cout << "right Index " << rightIndex << std::endl;

    while (leftIndex > 1 && projection->GetBinContent(leftIndex) > (countsAtPeak / 2))
    {
        --leftIndex;
    }

    while (rightIndex < projection->GetNbinsX() && projection->GetBinContent(rightIndex) > (countsAtPeak / 2))
    {
        ++rightIndex;
    }

    std::vector<double> xValues;


    // Calculates the interpolation of the point befor and after the peak/2 to get the fwhm
    if (leftIndex > 1)
    {
        double left_x_value = ((countsAtPeak / 2) - projection->GetBinContent(leftIndex)) *
                                  (projection->GetBinCenter(leftIndex + 1) - projection->GetBinCenter(leftIndex)) /
                                  (projection->GetBinContent(leftIndex + 1) - projection->GetBinContent(leftIndex)) +
                              projection->GetBinCenter(leftIndex);
        xValues.push_back(left_x_value);
        std::cout << left_x_value << std::endl;
    }

    if (rightIndex < projection->GetNbinsX())
    {
        double right_x_value = ((countsAtPeak / 2) - projection->GetBinContent(rightIndex - 1)) *
                                   (projection->GetBinCenter(rightIndex) - projection->GetBinCenter(rightIndex - 1)) /
                                   (projection->GetBinContent(rightIndex) - projection->GetBinContent(rightIndex - 1)) +
                               projection->GetBinCenter(rightIndex - 1);
        xValues.push_back(right_x_value);
        std::cout << right_x_value << std::endl;
    }

    // Calculates the FWHM in terms of the distance between the left and right indexes found;
    double FWHM = fabs(xValues[1] - xValues[0]);

    std::cout << "FWHM: " << FWHM << std::endl;
    return FWHM;
}

std::map<std::string, double> CalculateFloodQuality(std::shared_ptr<TH2F> histo)
{

    // Find the positions
    auto projectionX = histo->ProjectionX();
    auto projectionY = histo->ProjectionY();
    TSpectrum s1;
    TSpectrum s2;

    int nPeaksX = s1.Search(projectionX, 0.1, "", 0.1);
    int nPeaksY = s2.Search(projectionY, 0.1, "", 0.1);
    float meanx{0.1}, meany{0.1};
    float threshX{0.1}, threshY{0.1};

    bool out = false;
    while (!out)
    {
        if (meanx >9){
            threshX+=0.01;
            meanx=0;
        }

        if (meany >9){
            threshY+=0.01;
            meany=0;
        }
        if (nPeaksX != 8)
        {
            meanx += 0.1;
            nPeaksX = s1.Search(projectionX, meanx, "", threshX);
        }
        if (nPeaksY != 8)
        {
            meany += 0.1;
            nPeaksY = s2.Search(projectionY, meany, "", threshY);
        }

        if ((nPeaksX == 8) && (nPeaksY == 8))
        {
            out = true;
        }
    }

    double *xPeaks = s1.GetPositionX();
    double *yPeaks = s2.GetPositionX();

    std::vector<double> xVec(nPeaksX, 0);
    std::vector<double> yVec(nPeaksY, 0);

    if (nPeaksX == nPeaksY)
    {
        for (int i = 0; i < nPeaksX; i++)
        {
            xVec[i] = xPeaks[i];
            yVec[i] = yPeaks[i];
        }

        std::sort(xVec.begin(), xVec.end());
        std::sort(yVec.begin(), yVec.end());

        double neighboor1_fwhm;
        double neighboor2_fwhm;
        double neighboor3_fwhm;
        double neighboor4_fwhm;
        double d1, d2, d3, d4;
        double self_x;
        double self_y;
        double W;
        double D;
        std::vector<double> k(64, 0);

        int pixel{0};

        for (int x = 0; x < nPeaksX; x++)
        {
            for (int y = 0; y < nPeaksY; y++)
            {

                if ((x == 0 && y == 0) || ((x == (nPeaksX - 1)) && (y == (nPeaksY - 1))) || ((x == (nPeaksX - 1)) && (y == 0)) || ((x == 0) && (y == (nPeaksY - 1))))
                {
                    if ((x == 0 && y == 0))
                    {
                        self_x = CalculteFWHMPixel(histo, 'x', xVec[x], yVec[y]);
                        self_y = CalculteFWHMPixel(histo, 'y', xVec[x], yVec[y]);
                        neighboor1_fwhm = CalculteFWHMPixel(histo, 'x', xVec[x + 1], yVec[y]);
                        neighboor2_fwhm = CalculteFWHMPixel(histo, 'y', xVec[x], yVec[y + 1]);

                        Point2D ref{xVec[x], yVec[y]};
                        Point2D n1{xVec[x + 1], yVec[y]};
                        Point2D n2{xVec[x], yVec[y + 1]};

                        double d1 = distance(ref, n1);
                        double d2 = distance(ref, n2);
                        D = sqrt(d1 * d2);
                        W = (self_x + self_x + neighboor1_fwhm + neighboor2_fwhm) / 4.0;
                    }
                    else if ((x == (nPeaksX - 1)) && (y == (nPeaksY - 1)))
                    {
                        self_x = CalculteFWHMPixel(histo, 'x', xVec[x], yVec[y]);
                        self_y = CalculteFWHMPixel(histo, 'y', xVec[x], yVec[y]);
                        neighboor1_fwhm = CalculteFWHMPixel(histo, 'x', xVec[x - 1], yVec[y]);
                        neighboor2_fwhm = CalculteFWHMPixel(histo, 'y', xVec[x], yVec[y - 1]);
                        W = (self_x + self_x + neighboor1_fwhm + neighboor2_fwhm) / 4.0;

                        Point2D ref{xVec[x], yVec[y]};
                        Point2D n1{xVec[x - 1], yVec[y]};
                        Point2D n2{xVec[x], yVec[y - 1]};

                        double d1 = distance(ref, n1);
                        double d2 = distance(ref, n2);
                        D = sqrt(d1 * d2);
                    }
                    else if ((x == (nPeaksX - 1)) && (y == 0))
                    {
                        self_x = CalculteFWHMPixel(histo, 'x', xVec[x], yVec[y]);
                        self_y = CalculteFWHMPixel(histo, 'y', xVec[x], yVec[y]);
                        neighboor1_fwhm = CalculteFWHMPixel(histo, 'x', xVec[x - 1], yVec[y]);
                        neighboor2_fwhm = CalculteFWHMPixel(histo, 'y', xVec[x], yVec[y + 1]);

                        Point2D ref{xVec[x], yVec[y]};
                        Point2D n1{xVec[x - 1], yVec[y]};
                        Point2D n2{xVec[x], yVec[y + 1]};

                        double d1 = distance(ref, n1);
                        double d2 = distance(ref, n2);
                        D = sqrt(d1 * d2);
                        W = (self_x + self_x + neighboor1_fwhm + neighboor2_fwhm) / 4.0;
                    }
                    else if ((x == 0) && (y == (nPeaksY - 1)))
                    {
                        self_x = CalculteFWHMPixel(histo, 'x', xVec[x], yVec[y]);
                        self_y = CalculteFWHMPixel(histo, 'y', xVec[x], yVec[y]);
                        neighboor1_fwhm = CalculteFWHMPixel(histo, 'x', xVec[x + 1], yVec[y]);
                        neighboor2_fwhm = CalculteFWHMPixel(histo, 'y', xVec[x], yVec[y - 1]);

                        Point2D ref{xVec[x], yVec[y]};
                        Point2D n1{xVec[x + 1], yVec[y]};
                        Point2D n2{xVec[x], yVec[y - 1]};

                        double d1 = distance(ref, n1);
                        double d2 = distance(ref, n2);
                        D = sqrt(d1 * d2);
                        W = (self_x + self_x + neighboor1_fwhm + neighboor2_fwhm) / 4.0;
                    }

                    std::cout << "corner : x" << x << " y " << y << std::endl;
                }

                else if ((x == 0) || (x == (nPeaksX - 1)) || (y == 0) || (y == (nPeaksY - 1)))
                {
                    if (x == 0)
                    {
                        self_x = CalculteFWHMPixel(histo, 'x', xVec[x], yVec[y]);
                        self_y = CalculteFWHMPixel(histo, 'y', xVec[x], yVec[y]);
                        neighboor1_fwhm = CalculteFWHMPixel(histo, 'x', xVec[x + 1], yVec[y]);
                        neighboor2_fwhm = CalculteFWHMPixel(histo, 'y', xVec[x], yVec[y + 1]);
                        neighboor3_fwhm = CalculteFWHMPixel(histo, 'y', xVec[x], yVec[y - 1]);

                        Point2D ref{xVec[x], yVec[y]};
                        Point2D n1{xVec[x + 1], yVec[y]};
                        Point2D n2{xVec[x], yVec[y + 1]};
                        Point2D n3{xVec[x], yVec[y - 1]};

                        double d1 = distance(ref, n1);
                        double d2 = distance(ref, n2);
                        double d3 = distance(ref, n3);

                        D = pow((d1 * d2 * d3), (1.0 / 3.0));
                        W = (self_x + self_x + neighboor1_fwhm + neighboor2_fwhm + neighboor3_fwhm) / 5.0;
                    }
                    else if (x == (nPeaksX - 1))
                    {
                        self_x = CalculteFWHMPixel(histo, 'x', xVec[x], yVec[y]);
                        self_y = CalculteFWHMPixel(histo, 'y', xVec[x], yVec[y]);
                        neighboor1_fwhm = CalculteFWHMPixel(histo, 'x', xVec[x - 1], yVec[y]);
                        neighboor2_fwhm = CalculteFWHMPixel(histo, 'y', xVec[x], yVec[y + 1]);
                        neighboor3_fwhm = CalculteFWHMPixel(histo, 'y', xVec[x], yVec[y - 1]);

                        Point2D ref{xVec[x], yVec[y]};
                        Point2D n1{xVec[x - 1], yVec[y]};
                        Point2D n2{xVec[x], yVec[y + 1]};
                        Point2D n3{xVec[x], yVec[y - 1]};

                        double d1 = distance(ref, n1);
                        double d2 = distance(ref, n2);
                        double d3 = distance(ref, n3);
                        D = pow((d1 * d2 * d3), (1.0 / 3.0));
                        W = (self_x + self_x + neighboor1_fwhm + neighboor2_fwhm + neighboor3_fwhm) / 5.0;
                    }
                    else if (y == 0)
                    {
                        self_x = CalculteFWHMPixel(histo, 'x', xVec[x], yVec[y]);
                        self_y = CalculteFWHMPixel(histo, 'y', xVec[x], yVec[y]);
                        neighboor1_fwhm = CalculteFWHMPixel(histo, 'x', xVec[x - 1], yVec[y]);
                        neighboor2_fwhm = CalculteFWHMPixel(histo, 'x', xVec[x + 1], yVec[y]);
                        neighboor3_fwhm = CalculteFWHMPixel(histo, 'y', xVec[x], yVec[y + 1]);
                        W = (self_x + self_x + neighboor1_fwhm + neighboor2_fwhm + neighboor3_fwhm) / 5.0;

                        Point2D ref{xVec[x], yVec[y]};
                        Point2D n1{xVec[x - 1], yVec[y]};
                        Point2D n2{xVec[x + 1], yVec[y]};
                        Point2D n3{xVec[x], yVec[y + 1]};

                        double d1 = distance(ref, n1);
                        double d2 = distance(ref, n2);
                        double d3 = distance(ref, n3);
                        D = pow((d1 * d2 * d3), (1.0 / 3.0));
                    }
                    else if (y == (nPeaksY - 1))
                    {
                        self_x = CalculteFWHMPixel(histo, 'x', xVec[x], yVec[y]);
                        self_y = CalculteFWHMPixel(histo, 'y', xVec[x], yVec[y]);
                        neighboor1_fwhm = CalculteFWHMPixel(histo, 'x', xVec[x - 1], yVec[y]);
                        neighboor2_fwhm = CalculteFWHMPixel(histo, 'x', xVec[x + 1], yVec[y]);
                        neighboor3_fwhm = CalculteFWHMPixel(histo, 'y', xVec[x], yVec[y - 1]);

                        Point2D ref{xVec[x], yVec[y]};
                        Point2D n1{xVec[x - 1], yVec[y]};
                        Point2D n2{xVec[x + 1], yVec[y]};
                        Point2D n3{xVec[x], yVec[y - 1]};

                        double d1 = distance(ref, n1);
                        double d2 = distance(ref, n2);
                        double d3 = distance(ref, n3);
                        D = pow((d1 * d2 * d3), (1.0 / 3.0));
                        W = (self_x + self_x + neighboor1_fwhm + neighboor2_fwhm + neighboor3_fwhm) / 5.0;
                    }

                    std::cout << " edge " << x << " y " << y << std::endl;
                }

                else
                {
                    self_x = CalculteFWHMPixel(histo, 'x', xVec[x], yVec[y]);
                    self_y = CalculteFWHMPixel(histo, 'y', xVec[x], yVec[y]);
                    neighboor1_fwhm = CalculteFWHMPixel(histo, 'x', xVec[x - 1], yVec[y]);
                    neighboor2_fwhm = CalculteFWHMPixel(histo, 'x', xVec[x + 1], yVec[y]);
                    neighboor3_fwhm = CalculteFWHMPixel(histo, 'y', xVec[x], yVec[y + 1]);
                    neighboor4_fwhm = CalculteFWHMPixel(histo, 'y', xVec[x], yVec[y - 1]);

                    Point2D ref{xVec[x], yVec[y]};
                    Point2D n1{xVec[x - 1], yVec[y]};
                    Point2D n2{xVec[x + 1], yVec[y]};
                    Point2D n3{xVec[x], yVec[y + 1]};
                    Point2D n4{xVec[x], yVec[y - 1]};

                    double d1 = distance(ref, n1);
                    double d2 = distance(ref, n2);
                    double d3 = distance(ref, n3);
                    double d4 = distance(ref, n4);

                    D = pow((d1 * d2 * d3 * d4), (1.0 / 4.0));
                    W = (self_x + self_x + neighboor1_fwhm + neighboor2_fwhm + neighboor3_fwhm + neighboor4_fwhm) / 6.0;

                    std::cout << " inner  " << x << " y " << y << std::endl;
                }

                std::cout << "Pixel " << pixel << " D " << D << " W " << W << std::endl;
                k[pixel] = D / W;
                pixel++;
            }
        }
        double Ksum{0};
        double N{1};

        // estimates K
        for (auto &ki : k)
        {
            Ksum += ki;
            N += 1;
        }
        double K = Ksum / N;

        Ksum = 0;

        std::cout << "printink" << std::endl;
        // Estimates K stdev
        for (auto &ki : k)
        {
            std::cout << ki << std::endl;
            Ksum += pow((ki - K), 2);
        }
        double stdDevK = sqrt((1 / (N - 1)) * Ksum);

        std::map<std::string, double> output;
        output["k"] = K;
        output["stdev"] = stdDevK;

        std::cout << "K value" << K << " error " << stdDevK << std::endl;
        return output;
    }
    else
    {
        std::map<std::string, double> output;
        std::cout << "The number of peaks detected in each is not equal in x and y directions" << std::endl;
        return output;
    }
}


double Calculate511keVFWHM(std::shared_ptr<TH1F> hist) {
    TF1 gauss_fit("gauss", "gaus(0)", 400, 600);
    TH1* raw_hist = hist.get();
    
    // Configuração inicial do ajuste
    gauss_fit.SetParameters(
        raw_hist->GetBinContent(raw_hist->GetMaximumBin()),
        511.0,
        raw_hist->GetRMS()
    );

    // Execução do ajuste
    auto fit_result = raw_hist->Fit(&gauss_fit, "SQRN", "", 490, 530);
    
    if(fit_result->Status() != 0) {
        throw std::runtime_error("Erro no ajuste: Código " + 
                                std::to_string(fit_result->Status()));
    }

    // Cálculo e impressão dos parâmetros
    const double sigma = gauss_fit.GetParameter(2);
    const double fwhm = 2.355 * sigma;
    const double fwhm_err = 2.355 * gauss_fit.GetParError(2);
    const double fwhm_prctg = 100*fwhm /gauss_fit.GetParameter(1);
    const double fwhm_err_prctg =  fwhm_err/gauss_fit.GetParameter(1);

    
    // std::string outfname = hist->GetName();
    // outfname = outfname+"gauss.png";

    // canvas.SaveAs(outfname.c_str());

    return fwhm_prctg;
}




double ajustParable(std::shared_ptr<TH1F> hist)
{
    // TCanvas *canvas = new TCanvas(hist->GetName(), hist->GetTitle(), 800, 600);

    // hist->Draw();

    TSpectrum spectrum;
    int nPeaks = spectrum.Search(hist.get(), 2, "", 0.1); // 2 é o número mínimo de bins entre picos e 0.1 é o threshold
    double *xPeaks = spectrum.GetPositionX();

    int maxPeakIndex = 0;
    double maxPeakHeight = 0;


    double peak;
    for(int idx= 0 ; idx<nPeaks; idx++){
        std::cout<<"peak "<<xPeaks[idx]<<std::endl;
        if ( xPeaks[idx]>500 && xPeaks[idx]<560 ){
            peak = xPeaks[idx];
        }
    }
    std::cout<<peak<<std::endl;
    //int peakBinIndex =  hist->GetMaximumBin();       
    int peakBinIndex =  hist->FindBin(peak);       
    
    //double peakBinIndex = ;
    double yVal = 0 ;//hist->GetBinContent(peakBinIndex);
    double x1Counts = 0; //hist->GetBinCenter(peakBinIndex - 1);
    double x2Counts = 0; //hist->GetBinCenter(peakBinIndex + 1);
    double x1Energy = 0; //hist->GetBinCenter(peakBinIndex - 1);
    double x2Energy = 0; //hist->GetBinCenter(peakBinIndex + 1);
    double peakEnergy = 0;
    double peakPos {0};
    double countsAtPeak {0};

    std::cout<<"Maximum is at "<<peakBinIndex<<std::endl;
//    yVal = hist->GetBinContent(peakBinIndex);
    yVal = hist->GetBinContent(peakBinIndex);


    for (int count = peakBinIndex -5 ; count<= peakBinIndex+5; count++ ){
            yVal = hist->GetBinContent(count);
            std::cout<<"Peak counts"<<yVal<<std::endl;
            if (countsAtPeak <yVal){
                countsAtPeak = yVal;
                peakPos = count;
                peakEnergy = hist->GetBinCenter(count);
                x1Counts = hist->GetBinContent(count-2);
                x1Energy = hist->GetBinCenter(count -2);
                x2Counts = hist->GetBinContent(count+2);
                x2Energy = hist->GetBinCenter(count +2);
            }
    }



    Double_t E[3]{x1Energy,peakEnergy,x2Energy}; // 1274};
    Double_t C[3]{x1Counts,countsAtPeak,x2Counts};

    TGraph graph(3, E,C);

    std::cout<<"fileName "<<hist->GetName()<<std::endl;
    std::cout<<"Peak Energy "<<peakEnergy<<" peakCounts "<<countsAtPeak<<std::endl;
    std::cout<<"x1 Energy "<<x1Energy<<" peakCounts "<<x1Counts<<std::endl;
    std::cout<<"x2 Energy "<<x2Energy<<" peakCounts "<<x2Counts<<std::endl;



    // Criando função de ajuste
    //TF1 *fitParable = new TF1("fitParable", "pol2");

    TF1 fitFunc("parable", "[0]*pow(x,2)+[1]*x+[2]");

    graph.Fit(&fitFunc, "Q");

    // TCanvas c1;
    // graph.Draw();
    // c1.SaveAs("graph.png");
    // Ajustando ao histograma


    //hist->Fit(fitParable, "R");
    // fitParable->Draw("same");

    // Recuperando os coeficientes: y = ax^2 + bx + c
    double p0 = fitFunc.GetParameter(0);
    double p1 = fitFunc.GetParameter(1);
    double p2 = fitFunc.GetParameter(2);

    std::cout<<"finding vertex"<<std::endl;
    // Encontrando o vértice
    // double xVert = ((-1) * p1) / (2 * p2);
    // double yVert = p2 * (xVert * xVert) + p1 * xVert + p0;
    // std::cout<<" p0  "<< p0 <<std::endl;
    // std::cout<<" p1  "<< p1 <<std::endl; 
    // std::cout<<" p2  "<< p2 <<std::endl; 

    double xVert = -((p1)/(2*p0));
    double yVert = ((-pow(p1,2))+(4*p0*p2))/(4*p0);

    int leftIndex = peakPos;
    int rightIndex = leftIndex;



    std::cout<<"vertex "<<yVert <<std::endl;
    while (leftIndex > 1 && hist->GetBinContent(leftIndex) > (yVert / 2))
    {
        --leftIndex;
    }

    while (rightIndex < hist->GetNbinsX() && hist->GetBinContent(rightIndex) > (yVert / 2))
    {
        ++rightIndex;
    }

    std::vector<double> xValues;

    // std::cout<<"right index "<<rightIndex<<" left index "<<leftIndex<<std::endl;
    // std::cout<<"interpolating"<<std::endl;

    // Encontrando os valores de x por interpolação
    if (leftIndex > 1)
    {
        double left_x_value = ((yVert / 2) - hist->GetBinContent(leftIndex)) *
                                  (hist->GetBinCenter(leftIndex + 1) - hist->GetBinCenter(leftIndex)) /
                                  (hist->GetBinContent(leftIndex + 1) - hist->GetBinContent(leftIndex)) +
                              hist->GetBinCenter(leftIndex);
        xValues.push_back(left_x_value);
        std::cout<<left_x_value<<std::endl;
    }

    if (rightIndex < hist->GetNbinsX())
    {
        double right_x_value = ((yVert / 2) - hist->GetBinContent(rightIndex - 1)) *
                                   (hist->GetBinCenter(rightIndex) - hist->GetBinCenter(rightIndex - 1)) /
                                   (hist->GetBinContent(rightIndex) - hist->GetBinContent(rightIndex - 1)) +
                               hist->GetBinCenter(rightIndex - 1);
        xValues.push_back(right_x_value);
        std::cout<<right_x_value<<std::endl;

    }

    std::cout<<" calculating fhwm"<<std::endl;
    double xPeak = hist->GetBinCenter(peakPos);

    std::cout<<" xpeak "<<xPeak<<std::endl;
    std::cout<<" xvalues  "<<xValues[1]<<" "<<xValues[0]<<std::endl;

    double FWHM = fabs(xValues[1] - xValues[0]) / xPeak;

    std::cout << "FWHM: " << FWHM << std::endl;

    // TCanvas *c1 = new TCanvas("Janela1", "Janela Histograma Energia");
    // hist->Draw();


    // std::string outfname = hist->GetName();
    // outfname = outfname+".png";

    // c1->SaveAs(outfname.c_str());

    return FWHM;

    // Esperar pela interação do usuário antes de fechar o canvas
    // canvas->Update();
    // canvas->WaitPrimitive();
}


Double_t CalculateCRT_TSpectrum(std::shared_ptr<TH1F> hist, Bool_t verbose = kTRUE) {
    if (!hist || hist->GetEntries() < 1000) {
        std::cerr << "Histograma inválido ou estatística insuficiente!" << std::endl;
        return -1;
    }

    std::cout<<"GET EHRE"<<std::endl;
    // Configuração do TSpectrum
    TSpectrum spec(1); // Máximo 1 pico
    const Double_t search_sigma = 7.0;   // Sigma alto para suavização agressiva
    const Double_t threshold = 0.05;     // 5% do pico máximo

    // Executa a busca de picos
    Int_t npeaks = spec.Search(hist.get(), search_sigma, "", threshold);
    

    std::cout<<"Agter spectrum"<<std::endl;

    if(npeaks != 1) {
        std::cerr << "Número de picos detectados inválido: " << npeaks 
                 << ". Ajuste os parâmetros do TSpectrum!" << std::endl;
        return -1;
    }

    // Obtém parâmetros do pico
    Double_t *xpeaks = spec.GetPositionX();
    Double_t peak_mean = xpeaks[0];
    Double_t peak_amp = spec.GetPositionY()[0];


    std::cout<<"Rebining"<<std::endl;
    
    // Pré-processamento do histograma
    hist->Rebin(2);
    Double_t sigma_estimate = hist->GetStdDev();
    
    // Cria função de ajuste Gaussiana centrada no pico
    TF1 gaussFit("gaussFit", "gaus", 
                           peak_mean - 3*sigma_estimate, 
                           peak_mean + 3*sigma_estimate);
    
    
                           std::cout<<"Fitting"<<std::endl;
                           
                           gaussFit.SetParameters(peak_amp, peak_mean, sigma_estimate);
                           
                           // Executa o ajuste
                           hist->Fit(&gaussFit, "RQ0"); 

    std::cout<<"Calculating "<<std::endl;

    // Cálculo final do CRT
    Double_t sigma = gaussFit.GetParameter(2);
    Double_t fwhm = 2.355 * sigma;

    // Cálculo direto para verificação
    Double_t half_max = gaussFit.GetParameter(0)/2.0;
    Double_t left_edge = gaussFit.GetX(half_max, peak_mean - 5*sigma, peak_mean);
    Double_t right_edge = gaussFit.GetX(half_max, peak_mean, peak_mean + 5*sigma);
    Double_t direct_fwhm = right_edge - left_edge;

    if(verbose) {
        std::cout << "\n======== Análise CRT com TSpectrum ========" << endl;
        std::cout << "Pico detectado em: " << peak_mean << " ns" << endl;
        std::cout << "Amplitude do pico: " << peak_amp << " counts" << endl;
        std::cout << "Sigma do ajuste: " << sigma << " ns" << endl;
        std::cout << "FWHM calculado: " << direct_fwhm << " ns" << endl;
        std::cout << "FWHM calculado fit: " << fwhm << " ns" << endl;

    }

    return direct_fwhm;
}