
class PetsysCoincidences {
    protected:
    
    
    
        //virtual ~PetsysCoincidences(){delete file;}
        Int_t _entries;
        Float_t _step1,_step2;
        UShort_t _mh_n1, _mh_j1;
        UShort_t _mh_n2, _mh_j2;
        UInt_t _channelID1;
        UInt_t _channelID2;
        Int_t _xi1, _yi1, _chipID1;
        Int_t _xi2, _yi2, _chipID2;
        Float_t _energy1;
        Float_t _energy2;
        Long64_t _time1;
        Long64_t _time2;
        Float_t _x, _y, _z;
        Float_t _x2, _y2, _z2;
        std::shared_ptr<TFile> file;
        std::shared_ptr<TTree> Leaves;
        bool state = false;
        inline void switchState(){state = !state;}
        void PetsysCoincidenceBranchAddress();
    
    public:
        PetsysCoincidences();
        PetsysCoincidences(const PetsysCoincidences &pc );
        PetsysCoincidences(std::string filename);
    
        inline void getEntry(Int_t entry){Leaves->GetEntry(entry);}
    
    
    
        inline Int_t Entries(){return _entries;}
        inline std::shared_ptr<const TTree> GetCoincidencesTrees() const { return Leaves; }
        inline Float_t step1 () const{return _step1 ;}
        inline Float_t step2 () const{return _step2;}
        inline UShort_t mh_n1() const{return _mh_n1;}
        inline UShort_t mh_j1() const{return _mh_j1;}
        inline UShort_t mh_n2() const{return _mh_n2;}
        inline UShort_t mh_j2() const{return _mh_j2;}
        inline UInt_t abschannelID1() const{return _channelID1;}
        inline UInt_t abschannelID2() const{return _channelID2;}
        inline Int_t xi1() const{return _xi1 ;}
        inline Int_t yi1() const{return _yi1;}
        inline Int_t chipID1() const{return (int)_channelID1 / 64;}
        inline Int_t xi2() const{return _xi2;}
        inline Int_t yi2() const{return _yi2;}
        inline Int_t chipID2() const{return (int)_channelID2 / 64;}
        inline Float_t energy1() const{return _energy1;}
        inline Float_t energy2() const{return _energy2;}
        inline Long64_t time1() const{return _time1;}
        inline Long64_t time2() const {return _time2;}
        inline Float_t x() const{return _x ;}
        inline Float_t y() const{return _y;}
        inline Float_t z() const{return _z;}
        inline Float_t x2() const{return _x2 ;}
        inline Float_t y2() const{return _y2;}
        inline Float_t z2() const{return _z2;}

        inline Int_t channel1(){
            Int_t chipID1 = (int)_channelID1 / 64;
            return _channelID1 - 64 * chipID1;
            }
        inline Int_t channel2(){
            Int_t chipID2 = (int)_channelID2 / 64;
            return _channelID2 - 64 * chipID2;
            }


    };
    


PetsysCoincidences::PetsysCoincidences() {}

PetsysCoincidences::PetsysCoincidences(const PetsysCoincidences& pc):
    file{pc.file}
    ,Leaves{pc.Leaves}
{
    PetsysCoincidenceBranchAddress();
}


PetsysCoincidences::PetsysCoincidences(std::string fileName)
{
    file = std::make_shared<TFile>(fileName.c_str(), "read");
    Leaves = std::shared_ptr<TTree>((TTree*)file->Get("data"));
    PetsysCoincidenceBranchAddress();
}

void PetsysCoincidences::PetsysCoincidenceBranchAddress() {
    _entries = (Int_t)Leaves->GetEntries();
    Leaves->SetBranchAddress("mh_n1", &_mh_n1);
    Leaves->SetBranchAddress("mh_j1", &_mh_j1);
    Leaves->SetBranchAddress("mh_n2", &_mh_n2);
    Leaves->SetBranchAddress("mh_j2", &_mh_j2);
    Leaves->SetBranchAddress("channelID1", &_channelID1);
    Leaves->SetBranchAddress("channelID2", &_channelID2);
    Leaves->SetBranchAddress("xi1", &_xi1);
    Leaves->SetBranchAddress("yi1", &_yi1);
    Leaves->SetBranchAddress("xi2", &_xi2);
    Leaves->SetBranchAddress("yi2", &_yi2);
    Leaves->SetBranchAddress("time1", &_time1);
    Leaves->SetBranchAddress("energy1", &_energy1);
    Leaves->SetBranchAddress("time2", &_time2);
    Leaves->SetBranchAddress("energy2", &_energy2);
    Leaves->SetBranchAddress("x2", &_x2);
    Leaves->SetBranchAddress("y2", &_y2);
    Leaves->SetBranchAddress("y2", &_y2);
    Leaves->SetBranchAddress("x1", &_x);
    Leaves->SetBranchAddress("y1", &_y);
    Leaves->SetBranchAddress("z1", &_z);

}