//
// $Id: GPAcceleratorGeometry.hh,v 1.6 
// GEANT4 tag $Name: geant4-09-02 $
//

#ifndef GPACCELERATORGEOMETRY_H
#define GPACCELERATORGEOMETRY_H 1
#include "G4ThreeVector.hh"
#include <string>
#include <vector>

class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class G4Tubs;

//#include "G4VUserDetectorConstruction.hh"

class GPAcceleratorGeometry
{
  public:

    GPAcceleratorGeometry();
    ~GPAcceleratorGeometry();

    G4VPhysicalVolume* Construct(G4LogicalVolume* motherLog, G4ThreeVector point);

    void SetMaterial (G4String strMa);
    void SetParameter(std::string, std::string);
    G4double GetParameter(std::string) const;
    void Print();
    void Print(std::ofstream& );


  private:
    G4VPhysicalVolume* SetHit(G4LogicalVolume*);
    G4VPhysicalVolume* SetHitAtom(G4LogicalVolume*,G4ThreeVector);
    void Init();

  private:
    
    G4Tubs* acceleratorTube;
    G4LogicalVolume* acceleratorLog;
    G4VPhysicalVolume* acceleratorPhys;

    G4ThreeVector vecPosition;
    G4double dAcceleratorTubeInnerRadius;
    G4double dAcceleratorTubeOuterRadius;
    G4double dAcceleratorTubeLength;
    G4double dAcceleratorTubeStartAngle;
    G4double dAcceleratorTubeSpanningAngle;
    G4double dAcceleratorLimitStepMax;
    G4int    iAcceleratorLimitStepFlag;
    G4int    iAcceleratorHitFlag;

    G4Material* accMaterial;
    G4Material* spaceMaterial;


};

#endif

