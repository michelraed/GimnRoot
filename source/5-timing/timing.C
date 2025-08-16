
//
//Original paper
//Exploring TOF capabilities of PET detector blocks based on large monolithic
//crystals and analog SiPMs
//Efthymios Lamprou⁎, Antonio J. Gonzalez, Filomeno Sanchez, Jose M. Benlloch
//


double tsa ( vector <Long64_t> data ){

    int n = 0;
    
    Long64_t tsa =0;
    for ( int i= 0 ; i < data.size();i++){
        tsa+= data[i];
        n++;
    }

    return (double) tsa/n;
}


double tea ( vector<Long64_t> time , vector<Float_t> energy ){

    double tea{0};
    float esum {0};
    for (int i =0; i <64 ; i++){
        tea += time[i]*energy[i];
        esum += energy[i];
    }
    return tea/esum;

}