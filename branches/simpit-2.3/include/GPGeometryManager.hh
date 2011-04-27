//
// $Id: GPGeometryManager.hh,v 1.6 
// GEANT4 tag $Name: geant4-09-02 $
//

#ifndef GPGEOMETRYMANAGER_H
#define GPGEOMETRYMANAGER_H 1
#include "G4ThreeVector.hh"

class G4LogicalVolume;
class GPGeometryManager 
{
  public:
    static GPGeometryManager* GetInstance();
    static void Delete();
    void ConstructGeometry(G4LogicalVolume* motherLog , G4ThreeVector vPoint);
  protected:
    GPGeometryManager();
    ~GPGeometryManager();
    void ConstructGeometryCompact(G4LogicalVolume* , G4ThreeVector);
    void ConstructGeometryNormal(G4LogicalVolume* ,G4ThreeVector );
  protected:
    static GPGeometryManager* instance;
    int iCompactRangerFlag;
  
};

#endif

