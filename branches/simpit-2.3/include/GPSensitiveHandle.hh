//
// $Id: GPSensitiveHandle.hh,v 1.6 
// GEANT4 tag $Name: geant4-09-02 $
//

#ifndef GPSENSITIVEHANDLE_H
#define GPSENSITIVEHANDLE_H 1
#include "G4ThreeVector.hh"
#include "GPObject.hh"
#include <string>
#include <vector>
#include <map>

//#include "G4VUserDetectorConstruction.hh"

class G4LogicalVolume;

class GPSensitiveHandle : public GPObject
{
  public:

    GPSensitiveHandle(std::string, std::string);
    ~GPSensitiveHandle();

    void SetParameter(std::string, std::string);
    double GetParameter(std::string, std::string) const;
    std::map<std::string,std::string>* 
      GetMStrStrScorer();
    std::string GetSDName()const;
    std::string GetSDType()const;
    void Print();
    void Print(std::ofstream& );
    void SetSensitiveDet(G4LogicalVolume*);
  protected:
    void AddPrimitiveScorer(std::string,std::string);
    void SetSensitiveDetType(std::string);
    void SetBoxCellSD(G4LogicalVolume*);


  protected:
    std::string   sSDType;
    std::string   sSDName;
    std::string   sBaseNameChild;
    std::map<std::string,std::string> mStrStrScorer;
    double dReadOutX;
    double dReadOutY;
    double dReadOutZ;
    double dCellX; 
    double dCellY; 
    double dCellZ; 
};
typedef std::map<std::string,std::string> MStrStrScorer;

#endif

