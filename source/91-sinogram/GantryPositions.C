#include <vector>
#include <string>

class GimnPET
{

private:
    // Sinograms
    TH3F Sinogram3D;              // 3D Sinogram for reconstruction
    std::vector<TH2F> Sinogram2D; // Each slice will have a 2D sinogram

    // THings that will be read from the identificators
    // Estes caras vem do identificators, vale a pena criar uma variavel interna pra eles?
    int detectorNumber;
    std::vector<double> detectorAngles;     // { 0 , 180} The position of each detector in the gantry frame
    std::vector<double> detectorZRotations; // { 0,90,180,270} means the rotation around its z axis in the detector frame or around x in the gantry frame
    std::vector<std::string> detectorName;  // { Detector-1 , Detector-2}
    std::vector<std::string> detectorType;  // {Monolithic , Pixelated}

    std::string rotateAlong = "y";           // direction of the gantry rotation, can be x, y or z
    std::string crystalGantryRotation = "y"; // direction of the crystal rotation in the gantry frame, can be x, y or z during the positioning of the crystal in the gantry frame

    Identificator identificators; // This will be used to load the identificators from the identificators.conf file

    // a vector containing the position of each detector in the gantry frame
    std::map<int, TRotation> detectorAngle;
    std::map<int, TRotation> detectorZRotation;
    TRotation gantryRotation;

    // This will be used to rotate along detectors face z direction, for eventual rotation needed on the sipm
    // this rotation can be needed when the sipm is rotated.
    // Gantry rotation, for each source step

    double gantryAngle{0};
    double plaThickness = 2; // mm
    double gantryRadius = 35;

    TVector3 crystalDimension = TVector3(20.00, 26.88, 26.88);
    TVector3 globalCenter = TVector3(0, 0, 0);

    
public:

    void setCrystalGantryRotation(std::string crystalGantryRotation)
    {
        // This function will set the crystalGantryRotation variable
        // It will be used to define the rotation axis of the crystal in the gantry frame
        this->crystalGantryRotation = crystalGantryRotation;
    }


    void setRotateAlong(std::string rotateAlong)
    {
        // This function will set the rotateAlong variable
        // It will be used to define the rotation axis of the gantry
        this->rotateAlong = rotateAlong;
    }

    void setPLAThickness(double plaThickness)
    {
        // This function will set the plaThickness variable
        // It will be used to define the thickness of the PLA
        this->plaThickness = plaThickness;
    }
    void setGantryRadius(double gantryRadius)
    {
        // This function will set the gantryRadius variable
        // It will be used to define the radius of the gantry
        this->gantryRadius = gantryRadius;
    }
    void setCrystalDimension(TVector3 crystalDimension)
    {
        // This function will set the crystalDimension variable
        // It will be used to define the dimension of the crystal
        this->crystalDimension = crystalDimension;
    }
    void setGlobalCenter(TVector3 globalCenter)
    {
        // This function will set the globalCenter variable
        // It will be used to define the center of the gantry
        this->globalCenter = globalCenter;
    }



    void loadIdentificators(std::string fileName = "/home/gimn6/Documentos/Michel/produtos_moles/gimnpet/GimnROOT/petsys_conf/identificators.conf")
    {
        // This function will load the identificators from the identificators.conf file
        // It will use the Identificator class to load the data
        identificators.LoadIdentificators(fileName);
        initializeRotators();
    }



    std::vector<double> getDistanceAndAngle(TVector3 event1, TVector3 event2, std::string rotateAlong = "y")
    {
        // This function will calculate the angle and distance between two events
        // It will return a vector with the distance and angle in degrees
        // The angle is calculated based on the rotation axis defined by rotateAlong
        // If rotateAlong is empty, it will return an empty vector

        if (rotateAlong.empty())
        {
            std::cerr << "rotateAlong string is empty, cannot determine rotation axis." << std::endl;
            return {};
        }
        switch (rotateAlong[0])
        {
        case 'z':
        {
            double xm = (event2.Px() + event1.Px()) / 2;
            double ym = (event2.Py() + event1.Py()) / 2;
            double dx2 = event2.Px() - event1.Px();
            double dy2 = event2.Py() - event1.Py();
            // double theta = TMath::ATan2(dy2,dx2);
            double theta = TMath::ATan(dy2 / dx2);
            double phi = (TMath::Pi() / 2) + theta;
            double r = xm * TMath::Cos(phi) + ym * TMath::Sin(phi);
            phi = TMath::RadToDeg() * phi; // Convert to degrees
            return {static_cast<double>(r), static_cast<double>(phi)};
        }
        case 'y':
        {
            //std::cout << "Calculating angle and distance along y axis" << std::endl;
            double xm_y = (event2.Px() + event1.Px()) / 2;
            double ym_y = (event2.Pz() + event1.Pz()) / 2;
            double dx2_y = event2.Px() - event1.Px();
            double dy2_y = event2.Pz() - event1.Pz();
            // double theta_y = TMath::ATan2(dy2_y,dx2_y);
            double theta_y = TMath::ATan(dy2_y / dx2_y);

            double phi_y = (TMath::Pi() / 2) + theta_y;
            double r_y = xm_y * TMath::Cos(phi_y) + ym_y * TMath::Sin(phi_y);
            phi_y = TMath::RadToDeg() * phi_y; // Convert to degrees
            return {static_cast<double>(r_y), static_cast<double>(phi_y)};
        }
        case 'x':
        {
            double xm_x = (event2.Py() + event1.Py()) / 2;
            double ym_x = (event2.Pz() + event1.Pz()) / 2;
            double dx2_x = event2.Py() - event1.Py();
            double dy2_x = event2.Pz() - event1.Pz();
            // double theta_x = TMath::ATan2(dy2_x,dx2_x);
            double theta_x = TMath::ATan(dy2_x / dx2_x);
            double phi_x = (TMath::Pi() / 2) + theta_x;
            double r_x = xm_x * TMath::Cos(phi_x) + ym_x * TMath::Sin(phi_x);
            phi_x = TMath::RadToDeg() * phi_x; // Convert to degrees
            return {static_cast<double>(r_x), static_cast<double>(phi_x)};
        }
        default:
        {
            std::cerr << "Invalid rotation axis: " << rotateAlong << std::endl;
            return {};
        }
        }
    }

    TVector3 getZRotation(TVector3 event, int chipID)
    {
        return detectorZRotation[chipID] * event;
        }

    TVector3 getGlobalPositionPETSYS(TVector3 event, int chipID)
    {
        // the event comes as a 3D position x,y,doi.
        // the doi is the z dimension on the input vector
        // so the event will be:
        // TVector3 (x,y,doi or (z))
        // z dimension of the detector's frame will be equivalent
        // to the x dimension in  gantrys frame
        event = detectorZRotation[chipID] * event;
        TVector3 detectorReferential(event.Pz(), event.Py(), event.Px());
        TVector3 offset = TVector3(-crystalDimension.Px() / 2, 0, 0);
        TVector3 preGlob = detectorReferential + offset;
        TVector3 detectorCenter(gantryRadius + plaThickness + crystalDimension.Px() / 2, 0, 0);
        detectorReferential = preGlob + detectorCenter;
        TVector3 globalPosPreRotation = detectorAngle[chipID] * detectorReferential;
        TVector3 globalPos = gantryRotation * globalPosPreRotation; // Adding the global center to the position

        return globalPos;
    }

    void gantryAngleStep(double angle)
    {
        // rotates the gantry by a given angle
        // this will be used to rotate the gantry during the acquisition
        gantryAngle += angle;
        switch (rotateAlong[0])
        { // Use the first character of the string for comparison
        case 'x':
            gantryRotation.RotateX(angle * TMath::DegToRad());
            break;
        case 'y':
            gantryRotation.RotateY(angle * TMath::DegToRad());
            break;
        case 'z':
            gantryRotation.RotateZ(angle * TMath::DegToRad());
            break;
        default:
            std::cerr << "Invalid rotation axis: " << rotateAlong << std::endl;
            break;
        }
    }

    void initializeRotators()
    {
        // itializes the detectorAngle and detectorZRotation maps
        //  this will be used to rotate the detector in the gantry frame
        //  and the detector in its own frame rotating along its z axis

        int index = 0;

        // iterates trhough the connected ChipIDs
        for (auto chipID : identificators.chipID)
        {

            std::cout << "chip ID : " << chipID << std::endl;
            auto angle = identificators.detectorZRotation[index] * TMath::DegToRad();
            
            
            
            detectorZRotation[chipID] = TRotation();      // Rotation of a given detector in the gantry frame
            detectorZRotation[chipID].RotateZ(angle); // Rotation of a given detector in its own frame
            
        
            detectorAngle[chipID] = TRotation();      // Rotation of a given detector in the gantry frame




            switch (crystalGantryRotation[0])
            { // Use the first character of the string for comparison
            case 'x':
                detectorAngle[chipID].RotateX(identificators.detectorAngle[index] * TMath::DegToRad());
                break;
            case 'y':
                detectorAngle[chipID].RotateY(identificators.detectorAngle[index] * TMath::DegToRad());
                break;
            case 'z':
                detectorAngle[chipID].RotateZ(identificators.detectorAngle[index] * TMath::DegToRad());
                break;
            default:
                std::cerr << "Invalid rotation axis: " << crystalGantryRotation << std::endl;
                break;
            }
            index++;
        }

        // will define the gantry rotation direction
        switch (rotateAlong[0])
        { // Use the first character of the string for comparison
        case 'x':
            gantryRotation.RotateX(0 * TMath::DegToRad());
            break;
        case 'y':
            gantryRotation.RotateY(0 * TMath::DegToRad());
            break;
        case 'z':
            gantryRotation.RotateZ(0 * TMath::DegToRad());
            break;
        default:
            std::cerr << "Invalid rotation axis: " << rotateAlong << std::endl;
            break;
        }
    }

    GimnPET()
    {
        identificators.LoadIdentificators("/home/gimn6/Documentos/Michel/produtos_moles/gimnpet/GimnROOT/petsys_conf/identificators.conf");
        initializeRotators();
    }
    GimnPET(int detectorNumber, std::vector<int> detectorAngles, std::vector<double> detectorZRotations, std::vector<int> detectorName, std::vector<std::string> detectorType)
    {
        // this->detectorNumber = detectorNumber;
        // this->detectorAngles = detectorAngles;
        // this->detectorZRotations = detectorZRotations;

        // this->detectorType = detectorType;
        // identificators.LoadIdentificators("/home/gimn6/Documentos/Michel/produtos_moles/gimnpet/GimnROOT/petsys_conf/identificators.conf");
        // identificators.print();
    }

    float get_angle(std::string fileName = "Angle_30_1_time_300_t1_20_t2_20_te_15_HW_ON_qdc_QDC_OV_4_T1_20_T2_20_Te_15_360s_coinc.root")
    {

        auto s = splitString(fileName, '_');

        std::string angle = s[1] + "." + s[2];
        float out = std::stof(angle);

        return out;
    }


};

float get_angle(std::string fileName = "Angle_30_1_time_300_t1_20_t2_20_te_15_HW_ON_qdc_QDC_OV_4_T1_20_T2_20_Te_15_360s_coinc.root")
    {

        auto s = splitString(fileName, '_');

        std::string angle = s[1] + "." + s[2];
        float out = std::stof(angle);

        return out;
    }


 std::vector<std::string> checkIfTomographyc(std::string folderPath = "/home/gimn6/Documentos/Michel/DOUTORADO_CARACTERIZACAO/TOMOGRAFICO/")
    {
        // GEnerates a list to keep the file names as a TList
        TList *fileList = getFolderList(folderPath.c_str());
        TSystemFile *file;
        TString fileName;
        TIter next(fileList);

        std::vector<std::string> tomographycFiles;

        while ((file = (TSystemFile *)next()))
        {
            fileName = file->GetName();
            std::cout << "File name: " << fileName.Data() << std::endl;

            std::string fileNameStr = fileName.Data();
            if (fileNameStr.find("Angle") != -1)
            {
                tomographycFiles.push_back(fileNameStr);
                std::cout << "This is a tomographyc file: " << fileName.Data() << std::endl;
                std::cout << "It corresponds to Angle: " << get_angle(fileName.Data()) << std::endl;
            }
            else
            {
                std::cout << "This is not a tomographyc file: " << fileName.Data() << std::endl;
            }
        }
        return tomographycFiles;
    }


 std::vector<std::string> getListOfFiles(std::string folderPath = "/home/gimn6/Documentos/Michel/DOUTORADO_CARACTERIZACAO/TOMOGRAFICO/")
    {
        // GEnerates a list to keep the file names as a TList
        TList *fileList = getFolderList(folderPath.c_str());
        TSystemFile *file;
        TString fileName;
        TIter next(fileList);

        std::vector<std::string> tomographycFiles;

        while ((file = (TSystemFile *)next()))
        {
            fileName = file->GetName();
            std::string fileNameStr = fileName.Data();
            if (fileNameStr.size()<2) 
            {
                tomographycFiles.push_back(fileNameStr);
                std::cout << "file: " << fileName.Data() << std::endl;
            }
        }
        return tomographycFiles;
    }



