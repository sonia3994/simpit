//
// $Id: GPGeometryGeneral.hh,v 1.6 
// GEANT4 tag $Name: geant4-09-02 $
//

#ifndef GPGEOMETRYGENERAL_H
#define GPGEOMETRYGENERAL_H 1
#include "G4ThreeVector.hh"
#include "GPGeometry.hh"
#include <string>
#include <vector>
#include <map>

//#include "G4VUserDetectorConstruction.hh"

class G4VSolid;
class G4Material;
class G4VisAttributes;
class G4LogicalVolume;
class G4VPhysicalVolume;
class GPFieldManagerPool;
class GPSolidManager;
class GPSensitiveHandle;
class GPComplexSolid;

class GPGeometryGeneral : public GPGeometry
{
  public:

    GPGeometryGeneral(std::string, std::string);
    ~GPGeometryGeneral();

    G4VPhysicalVolume* Construct(G4LogicalVolume* motherLog, G4ThreeVector point);
    G4VPhysicalVolume* Construct(G4LogicalVolume* motherLog);

    void SetParameter(std::string, std::string);
    double GetParameter(std::string, std::string) const;
    void Print();
    GPSensitiveHandle* GetSensitiveHandle() const;
    GPSolidManager* GetSolidManager() const;
    void Print(std::ofstream& );
    void Update();
  protected:
    void SetMaterial(std::string);
  protected:
    void Init();
  private:
    GPSensitiveHandle* pSdHandle;
    GPSolidManager* pSolidManager;
    GPFieldManagerPool* pFieldManagerPool;
    GPComplexSolid* pComplexSolid;

    G4VSolid* pSolid;
    G4LogicalVolume* pLogicalVolume;
    G4VPhysicalVolume* pPhysicalVolume;
    G4Material* pMaterial;
    G4VisAttributes* pVisAttributes;

    std::string   sBaseNameChild;
    std::string   sMaterial;
    double dStepLimit;
    int iStepLimitFlag;
    int iHitFlag;

};

#endif

