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
  protected:
    GPGeometryManager();
    ~GPGeometryManager();
  protected:
    static GPGeometryManager* instance;
    int iCompactRangerFlag;
  
};

#endif

