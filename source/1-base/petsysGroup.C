//
// Created by michelraed on 15/11/2021.
//

class PetsysGroup {
protected:

    Int_t _entries;
    Float_t _step1,_step2;
    UShort_t _mh_n, _mh_j;
    UInt_t _channelID;
    Int_t _xi, _yi, _chipID;
    Float_t _energy;
    Long64_t _time;
    Float_t _x, _y, _z;

    std::shared_ptr<TFile> file;
    std::shared_ptr<TTree> Leaves;
    bool state = false;
    inline void switchState(){state = !state;}
    void PetsysGroupBranchAddress();

public:
    PetsysGroup();
    PetsysGroup(const PetsysGroup & pg );
    PetsysGroup(std::string filename);
    
    inline std::shared_ptr<const TTree> GetGroupTrees() const { return Leaves; }


    inline Int_t Entries()const{return _entries;}
    inline void getEntry(Int_t entry){Leaves->GetEntry(entry);}
    inline Float_t step1 () const{return _step1;}
    inline Float_t step2 () const{return _step2;}
    inline UShort_t mh_n() const{return _mh_n;}
    inline UShort_t mh_j() const{return _mh_j;}
    inline  UInt_t channelID() const{return _channelID;}
    inline Int_t xi() const{return _xi ;}
    inline Int_t yi() const{return _yi;}
    inline Float_t x() const{return _x ;}
    inline Float_t y() const{return _y;}
    inline Float_t z() const{return _z;}
    inline Int_t chipID() const{return (int)_channelID / 64;}
    inline Float_t energy() const{return _energy;}
    inline Long64_t time() const{return _time;}

    inline Int_t channel(){
        Int_t chipID = (int)_channelID / 64;
        return _channelID - 64 * chipID;
        }
};



//
// Created by michelraed on 15/11/2021.
//


PetsysGroup::PetsysGroup() {}

PetsysGroup::PetsysGroup(const PetsysGroup& pg):
        file{pg.file}
        ,Leaves {pg.Leaves}

{
    PetsysGroupBranchAddress();
}


PetsysGroup::PetsysGroup(std::string fileName)
{
    file = std::make_shared<TFile>(fileName.c_str(), "read");
    Leaves = std::shared_ptr<TTree>((TTree*)file->Get("data"));
    PetsysGroupBranchAddress();

}

void PetsysGroup::PetsysGroupBranchAddress() {
    _entries = (Int_t)Leaves->GetEntries();
    Leaves->SetBranchAddress("step1", &_step1);
    Leaves->SetBranchAddress("step2", &_step2);
    Leaves->SetBranchAddress("mh_n", &_mh_n);
    Leaves->SetBranchAddress("mh_j", &_mh_j);
    Leaves->SetBranchAddress("channelID", &_channelID);
    Leaves->SetBranchAddress("xi", &_xi);
    Leaves->SetBranchAddress("yi", &_yi);
    Leaves->SetBranchAddress("time", &_time);
    Leaves->SetBranchAddress("energy", &_energy);
    Leaves->SetBranchAddress("x", &_x);
    Leaves->SetBranchAddress("y", &_y);
    Leaves->SetBranchAddress("z", &_z);
}

