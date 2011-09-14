//
// $Id: GPAcceleratorGeometry.hh,v 1.6 
// GEANT4 tag $Name: geant4-09-02 $
//

#ifndef GPACCELERATORGEOMETRY_H
#define GPACCELERATORGEOMETRY_H 1
#include "G4ThreeVector.hh"
#include "GPGeometry.hh"
#include <string>
#include <vector>

class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class G4Tubs;
class GPFieldManagerPool;

//#include "G4VUserDetectorConstruction.hh"

class GPAcceleratorGeometry : public GPGeometry
{
  public:

    GPAcceleratorGeometry(std::string sFirst, std::string sSecond);
    GPAcceleratorGeometry();
    ~GPAcceleratorGeometry();

    G4VPhysicalVolume* Construct(G4LogicalVolume* motherLog, G4ThreeVector point);
    G4VPhysicalVolume* Construct(G4LogicalVolume* motherLog);

    void SetParameter(std::string, std::string);
    G4double GetParameter(std::string,std::string) const;
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

    G4double dAcceleratorTubeInnerRadius;
    G4double dAcceleratorTubeOuterRadius;
    G4double dAcceleratorTubeLength;
    G4double dAcceleratorTubeStartAngle;
    G4double dAcceleratorTubeSpanningAngle;
    G4double dAcceleratorLimitStepMax;
    G4double dGlobalLength;
    G4int    iAcceleratorLimitStepFlag;
    G4int    iAcceleratorHitFlag;

    G4Material* accMaterial;
    G4Material* spaceMaterial;

    GPFieldManagerPool* fieldManagerPool;

};

#endif

