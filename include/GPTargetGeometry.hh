//
// $Id: GPTargetGeometry.hh,v 1.6 2006/06/29 17:47:13 gunter Granular $
// GEANT4 tag $Name: geant4-09-02 $
//

#ifndef GPTARGETGEOMETRY_H
#define GPTARGETGEOMETRY_H 1
#include "G4ThreeVector.hh"

class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;

//#include "G4VUserDetectorConstruction.hh"

class GPTargetGeometry
{
  public:

    GPTargetGeometry();
    ~GPTargetGeometry();

G4VPhysicalVolume* Construct(G4LogicalVolume* motherLog,
    G4ThreeVector point,
    G4double dSphereR,
    G4double dLengthX,G4double dLengthY,G4double dLengthZ,
    int iIndex);

  private:
    G4VPhysicalVolume* Granular(G4LogicalVolume*);
    G4VPhysicalVolume* GranularCell(G4LogicalVolume*);
    G4VPhysicalVolume* GranularHexagonal(G4LogicalVolume* motherLog,G4ThreeVector point);
    void GranularHexagonalCell(G4LogicalVolume* motherLog,G4ThreeVector point,long iIndex);
  private:
    
    G4ThreeVector vecPoint;
    G4double dSphereRadiu;
    G4double dLengthX;
    G4double dLengthY;
    G4double dLengthZ;
    G4double dWidthX;
    G4double dWidthY;
    G4double dWidthZ;
    G4int    iNumX;
    G4int    iNumY;
    G4int    iNumZ;
    
    G4Material* targetMaterial;
    G4Material* spaceMaterial;


};

#endif

