//
// $Id: GPTargetGeometry.hh,v 1.6 2006/06/29 17:47:13 gunter Granular $
// GEANT4 tag $Name: geant4-09-02 $
//

#ifndef GPTARGETGEOMETRY_H
#define GPTARGETGEOMETRY_H 1
#include "G4ThreeVector.hh"
#include <string>
#include <vector>

class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;

//#include "G4VUserDetectorConstruction.hh"

class GPTargetGeometry
{
  public:

    GPTargetGeometry();
    ~GPTargetGeometry();

    G4VPhysicalVolume* Construct(G4LogicalVolume* motherLog, G4ThreeVector point);

    void SetTargetMaterial (G4String strMa);
    void SetDetectorSize(std::string, std::string);
    G4double GetDetectorSize(std::string) const;
    std::vector<G4int> GetEddDim();
    void Print();


  private:
    G4VPhysicalVolume* Granular(G4LogicalVolume*);
    G4VPhysicalVolume* GranularCell(G4LogicalVolume*);
    G4VPhysicalVolume* GranularHexagonal(G4LogicalVolume* motherLog,G4ThreeVector point);
    G4VPhysicalVolume* SetTargetHit(G4LogicalVolume*,G4ThreeVector);
    G4VPhysicalVolume* TubularTarget(G4LogicalVolume*,G4ThreeVector);
    void GranularHexagonalCell(G4LogicalVolume* motherLog,G4ThreeVector point,long iIndex);
    void GranularHexagonalInit();
    void SetTargetSD(G4LogicalVolume*);
    void PrintSD();

  private:
    
    G4ThreeVector vecPoint;
    G4double dTargetGlobalSolidX;
    G4double dTargetGlobalSolidY;
    G4double dTargetGlobalSolidZ;
    G4double dTargetHexagonalSphereRadius;
    G4double dTargetSolidX;
    G4double dTargetSolidY;
    G4double dTargetSolidZ;
    G4double dTargetHitL;
    G4double dWidthX;
    G4double dWidthY;
    G4double dWidthZ;
    G4int    iTargetGranularXNumber;
    G4int    iTargetGranularYNumber;
    G4int    iTargetGranularZNumber;
    G4int    iTargetGranularFlag;
    G4int    iTargetHitFlag;

    std::vector<G4int> vecEddDim;
    G4double dTargetSDSolidR;
    G4double dTargetSDSolidZ;
    G4double dTargetSDSolidAngle;

    G4double dTargetSDCellZ;
    G4double dTargetSDCellR;
    G4double dTargetSDCellPhi;

    
    G4Material* targetMaterial;
    G4Material* spaceMaterial;


};

#endif

