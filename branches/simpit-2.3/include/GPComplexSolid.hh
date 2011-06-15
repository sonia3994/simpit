//
// $Id: GPComplexSolid.hh,v 1.6 2006/06/29 17:47:13 gunter Granular $
// GEANT4 tag $Name: geant4-09-02 $
//

#ifndef GPCOMPLEXSOLID_H
#define GPCOMPLEXSOLID_H 1
#include "G4ThreeVector.hh"
#include "GPGeometry.hh"
#include <string>
#include <vector>

class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class GPSolidManager;


class GPComplexSolid : public GPObject
{
  public:

    GPComplexSolid(std::string sFirst, std::string sSecond);
    GPComplexSolid();
    ~GPComplexSolid();

    virtual void Construct(G4LogicalVolume*, GPSolidManager*, G4ThreeVector vPoint = G4ThreeVector());

    virtual void SetParameter(std::string, std::string);
    virtual G4double GetParameter(std::string,std::string) const;
    virtual void Print();
    virtual void Print(std::ofstream&);

  protected:
    virtual void Init();
    virtual void SetMaterial(std::string);
  private:
    
};

#endif

