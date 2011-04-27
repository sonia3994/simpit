//
// $Id: GPGeometry.hh,v 1.6 
// GEANT4 tag $Name: geant4-09-02 $
//

#ifndef GPGEOMETRY_H
#define GPGEOMETRY_H 1
#include "GPObject.hh"
#include "G4ThreeVector.hh"
#include <vector>

class G4LogicalVolume;
class G4VPhysicalVolume;

//#include "G4VUserDetectorConstruction.hh"

class GPGeometry : public GPObject
{
  public:

    GPGeometry();
    GPGeometry(std::string, std::string);
    virtual ~GPGeometry();
    virtual G4VPhysicalVolume* Construct(G4LogicalVolume* , G4ThreeVector )=0;
    virtual G4VPhysicalVolume* Construct(G4LogicalVolume*){return NULL;};
    int GetPriority(){return iPriority;};
    void SetPriority(int i){iPriority=i;};
    void SetPosition(G4ThreeVector pos){vPosition=pos;};
    void SetCompactRanger(int i){iCompactRangerFlag=i;};
    int GetCompactRanger(){return iCompactRangerFlag;};
  protected:

    G4ThreeVector vPosition;
    int iCompactRangerFlag;
    int iPriority;

};

#endif

