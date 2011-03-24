//
// $Id: GPSweeperGeometry.hh,v 1.6 
// GEANT4 tag $Name: geant4-09-02 $
//

#ifndef GPSWEEPERGEOMETRY_H
#define GPSWEEPERGEOMETRY_H 1
#include "G4ThreeVector.hh"
#include <string>
#include <vector>

class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class G4Tubs;

//#include "G4VUserDetectorConstruction.hh"

class GPSweeperGeometry
{
  public:

    GPSweeperGeometry();
    ~GPSweeperGeometry();

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
    
    G4Tubs* sweeperTube;
    G4LogicalVolume* sweeperLog;
    G4VPhysicalVolume* sweeperPhys;

    G4ThreeVector vecPosition;
    G4double dSweeperTubeInnerRadius;
    G4double dSweeperTubeOuterRadius;
    G4double dSweeperTubeLength;
    G4double dSweeperTubeStartAngle;
    G4double dSweeperTubeSpanningAngle;
    G4double dSweeperLimitStepMax;
    G4int    iSweeperLimitStepFlag;
    G4int    iSweeperHitFlag;

    G4Material* swpMaterial;
    G4Material* spaceMaterial;


};

#endif

