//
// $Id: GPHexagonalSolid.hh,v 1.6 2006/06/29 17:47:13 gunter Granular $
// GEANT4 tag $Name: geant4-09-02 $
//

#ifndef GPHEXAGONALSOLID_H
#define GPHEXAGONALSOLID_H 1
#include "G4ThreeVector.hh"
#include "GPComplexSolid.hh"
#include <string>
#include <vector>

class G4LogicalVolume;
class G4VPhysicalVolume;
class G4VSolid;
class G4Material;


class GPHexagonalSolid : public GPComplexSolid
{
  public:

    GPHexagonalSolid(std::string sFirst, std::string sSecond);
    ~GPHexagonalSolid();

    void Construct(G4LogicalVolume*,G4ThreeVector vPoint = G4ThreeVector());

    void SetParameter(std::string, std::string);
    G4double GetParameter(std::string,std::string) const;
    void Print();
    void Update();
    void Print(std::ofstream&);


  protected:
    void GranularHexagonal(G4LogicalVolume*);
    void GranularHexagonalCell(G4LogicalVolume* ,G4ThreeVector ,int, int, int);
    void SetMaterial(std::string);
    void Init();

  private:
    
    G4VSolid* pSolid;
    G4LogicalVolume* pLogicalVolume;
    G4VPhysicalVolume* pPhysicalVolume;

    G4Material* pMaterial;
    G4Material* pMaterialSpace;
    std::string  sMaterial;
    std::string  sBaseName;
    G4ThreeVector vPosition;
    G4double dSphereRadius;
    G4double dGlobalSolidX;
    G4double dGlobalSolidY;
    G4double dGlobalSolidZ;
    G4double dCellWidthX;
    G4double dCellWidthY;
    G4double dCellWidthZ;
    G4int    iCellNumberX;
    G4int    iCellNumberY;
    G4int    iCellNumberZ;

    G4double dIndexPoint;

};

#endif

