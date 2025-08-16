#include <vector>
#include <TROOT.h>
std::vector<double> anger(TMatrixT<double> energy);
std::vector<double> angerModified(double alpha, TMatrixT<double> energy);
void applyAlpha(double alpha, std::vector<double>& vec);




void applyAlpha(double alpha, std::vector<double> &vec)
{
    for (auto &v : vec)
    {
        v = pow(v, alpha);
    }
}

std::vector<double> anger(TMatrixT<double> energy)
{

    SiPM aux;
    std::vector<Float_t> xPos = aux.getPositionsX();
    std::vector<Float_t> yPos = aux.getPositionsY();

    auto sum = energy.Sum();
    std::vector<double> rowSums(energy.GetNrows(), 0);
    std::vector<double> colSums(energy.GetNcols(), 0);


    const auto I = energy.Max();

    getRowAndColumnSums(energy, rowSums, colSums);

    // Scrimger and Baker formula
    // N = 2*PI * I * t²
    // N is the integral of scintilation light  (esum)
    // I is the maximum intnsity
    // t is the distance between photon scintilation and the detection plane "(doi)"
    //
    // t  =  sqrt(N/(2*PI*I))

    //const double DOI = sqrt(sum / (2 * TMath::Pi() * I));
     const double DOI = I/sum;

    float u{0}, v{0};
    for (int i = 0; i < 8; i++)
    {
        v += rowSums[i] * yPos[i];
        u += colSums[i] * xPos[i];
    }
 
    u = u / sum;
    v = v / sum;

    std::vector<double> output (3,0);

    output[0]=u;
    output[1]=v;
    output[2]=DOI;

    return output;
}

std::vector<double> angerNum(TMatrixT<double> energy)
{

    SiPM aux;
    std::vector<Float_t> xPos = aux.getPositionsX();
    std::vector<Float_t> yPos = aux.getPositionsY();

    auto sum = energy.Sum();
    std::vector<double> rowSums(energy.GetNrows(), 0);
    std::vector<double> colSums(energy.GetNcols(), 0);


    const auto I = energy.Max();

    getRowAndColumnSums(energy, rowSums, colSums);

    // Scrimger and Baker formula
    // N = 2*PI * I * t²
    // N is the integral of scintilation light  (esum)
    // I is the maximum intnsity
    // t is the distance between photon scintilation and the detection plane "(doi)"
    //
    // t  =  sqrt(N/(2*PI*I))

    //const double DOI = sqrt(sum / (2 * TMath::Pi() * I));
     const double DOI = I/sum;

    float u{0}, v{0};
    for (int i = 0; i < 8; i++)
    for(int j = 0 ; j< 8 ; j++)
    {
        v += energy[i][j] * i;
        u += energy[i][j] * j;
    }
 
    u = u / sum;
    v = v / sum;

    std::vector<double> output (3,0);

    output[0]=u;
    output[1]=v;
    output[2]=DOI;

    return output;
}

std::vector<double> angerModified(double alpha, TMatrixT<double> energy){

    std::vector<double> rowSums(energy.GetNrows(), 0);
    std::vector<double> colSums(energy.GetNcols(), 0);

    const auto I = energy.Max();
    const auto sum = energy.Sum();
    getRowAndColumnSums(energy, rowSums, colSums);

    applyAlpha(alpha, rowSums);
    applyAlpha(alpha, colSums);

    // Scrimger and Baker formula
    // N = 2*PI * I * t²
    // N is the integral of scintilation light  (esum)
    // I is the maximum intnsity
    // t is the distance between photon scintilation and the detection plane "(doi)"
    //
    // t  =  sqrt(N/(2*PI*I))

    const double DOI = sqrt(sum / (2 * TMath::Pi() * I));

    float u{0}, v{0};
    for (int i = 0; i < 8; i++)
    {
        v += rowSums[i] * (double)(i + 1);
        u += colSums[i] * (double)(i + 1);
    }
 
    
    u = u / sum;
    v = v / sum;

    std::vector<double> output (3,0);

    output[0]=u;
    output[1]=v;
    output[2]=DOI;
    return output;
}