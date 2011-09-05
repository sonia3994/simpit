//
// $Id: GPSolidManager.hh,v 1.6 
// GEANT4 tag $Name: geant4-09-02 $
//

#ifndef GPSOLIDMANAGER_H
#define GPSOLIDMANAGER_H 1
#include "G4ThreeVector.hh"
#include "GPObject.hh"
#include <string>
#include <vector>
#include <map>

//#include "G4VUserDetectorConstruction.hh"

class G4VSolid;
class GPSolidManager : public GPObject
{
  public:

    GPSolidManager(std::string, std::string);
    ~GPSolidManager();


    G4VSolid* ConstructSolid();
    void SetParameter(std::string, std::string);
    double GetParameter(std::string, std::string) const;
    void Print();
    void Print(std::ofstream& );
    void Update();
    bool IsInThisSolid(G4ThreeVector);
  protected:
    void SetSolidType(std::string);
  protected:
    void Init();
  private:
    G4VSolid* solid;
    std::string   sSolidType;
    std::string   sBaseNameChild;
    double dLength;
    double dWidth;
    double dHeight;
    double dRadiusInner;
    double dAngleStart;
    double dAngleEnd;

};

#endif

