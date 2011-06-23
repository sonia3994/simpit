//
// $Id: GPGeometryManager.hh,v 1.6 
// GEANT4 tag $Name: geant4-09-02 $
//

#ifndef GPGEOMETRYMANAGER_H
#define GPGEOMETRYMANAGER_H 1
#include "G4ThreeVector.hh"
#include <string>

class G4LogicalVolume;
class GPGeometry;
class GPGeometryManager 
{
  public:
    static GPGeometryManager* GetInstance();
    static void Delete();
    GPGeometry* FindAndBuildGeometry(std::string,std::string,std::string);
  protected:
    GPGeometryManager();
    ~GPGeometryManager();
  protected:
    static GPGeometryManager* instance;
    int iCompactRangerFlag;
  
};

#endif

