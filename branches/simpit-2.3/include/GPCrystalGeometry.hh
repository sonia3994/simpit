//
// $Id: GPCrystalGeometry.hh,v 1.6 
// GEANT4 tag $Name: geant4-09-02 $
//

#ifndef GPCRYSTALGEOMETRY_H
#define GPCRYSTALGEOMETRY_H 1
#include "G4ThreeVector.hh"
#include "GPGeometry.hh"
#include <string>
#include <vector>

class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class G4Tubs;
class Crystal;
class RunParameters;

//#include "G4VUserDetectorConstruction.hh"

class GPCrystalGeometry : public GPGeometry
{
  public:

    GPCrystalGeometry();
    GPCrystalGeometry(std::string, std::string);
    ~GPCrystalGeometry();

    G4VPhysicalVolume* Construct(G4LogicalVolume* motherLog, G4ThreeVector point);
    G4VPhysicalVolume* Construct(G4LogicalVolume* motherLog);

    void ConstructCrystal();
    void SetParameter(std::string, std::string);
    double GetParameter(std::string, std::string) const;
    void Print();
    void Print(std::ofstream& );


  private:
    G4VPhysicalVolume* SetHit(G4LogicalVolume*);
    G4VPhysicalVolume* SetHitAtom(G4LogicalVolume*,G4ThreeVector);
    void Init();

  private:
    
    G4Tubs* crystalTube;
    G4LogicalVolume* crystalLog;
    G4VPhysicalVolume* crystalPhys;

    G4double dCrystalTubeInnerRadius;
    G4double dCrystalTubeOuterRadius;
    G4double dCrystalTubeLength;
    G4double dCrystalTubeStartAngle;
    G4double dCrystalTubeSpanningAngle;
    G4double dCrystalLimitStepMax;
    G4double dGlobalLength;
    G4int    iCrystalLimitStepFlag;
    G4int    iCrystalHitFlag;

    G4Material* cryMaterial;
    G4Material* spaceMaterial;
    Crystal*	crysW;
    RunParameters* runPara;


};

#endif

