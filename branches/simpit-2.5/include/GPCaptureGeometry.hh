//
// $Id: GPCaptureGeometry.hh,v 1.6 
// GEANT4 tag $Name: geant4-09-02 $
//

#ifndef GPCAPTUREGEOMETRY_H
#define GPCAPTUREGEOMETRY_H 1
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

class GPCaptureGeometry :public GPGeometry
{
  public:

    GPCaptureGeometry(std::string sFirst, std::string sSecond);
    GPCaptureGeometry();
    ~GPCaptureGeometry();

    G4VPhysicalVolume* Construct(G4LogicalVolume* motherLog, G4ThreeVector point);
    G4VPhysicalVolume* Construct(G4LogicalVolume* motherLog);

    void SetParameter(std::string, std::string);
    G4double GetParameter(std::string,std::string) const;
    void Print();
    void Print(std::ofstream& );

  protected:
    void SetLithiumLens(G4LogicalVolume*, G4ThreeVector, G4double dLength=1e-2,G4double dOuterRadius=1e-2,G4double dInnerRadius=0.0, G4double dStartAngle=0.0, G4double dSpanningAngle=360.0 );

  private:
    G4VPhysicalVolume* SetHit(G4LogicalVolume*);
    G4VPhysicalVolume* SetHitAtom(G4LogicalVolume*,G4ThreeVector);
    void Init();

  private:
    
    G4Tubs* captureTube;
    G4LogicalVolume* captureLog;
    G4VPhysicalVolume* capturePhys;

    G4double dCaptureTubeInnerRadius;
    G4double dCaptureTubeOuterRadius;
    G4double dCaptureTubeLength;
    G4double dCaptureTubeStartAngle;
    G4double dCaptureTubeSpanningAngle;
    G4double dCaptureLimitStepMax;
    G4double dLithiumTubeLength;
    G4double dLithiumTubeOuterRadius;
    G4double dGlobalLength;
    G4int    iCaptureLimitStepFlag;
    G4int    iCaptureType;
    G4int    iCaptureHitFlag;

    G4Material* capMaterial;
    G4Material* spaceMaterial;

    GPFieldManagerPool* fieldManagerPool;

};

#endif

