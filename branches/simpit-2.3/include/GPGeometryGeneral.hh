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
class G4FieldManager;

class GPGeometryGeneral : public GPGeometry
{
  public:

    GPGeometryGeneral(std::string, std::string);
    ~GPGeometryGeneral();

    G4VPhysicalVolume* Construct(G4LogicalVolume* motherLog, G4ThreeVector point);
    G4VPhysicalVolume* Construct(G4LogicalVolume* motherLog);

    void SetParameter(std::string, std::string);
    double GetParameter(std::string, std::string) const;
    inline std::map<std::string,std::string>* 
      GetMStrStrScorer(){return &mStrStrScorer;};
    void Print();
    void Print(std::ofstream& );
    void Update();
  protected:
    G4VSolid* ConstructSolid();
    void SetSolidType(std::string);
    void SetMaterial(std::string);
    void SetSensitiveDetector(std::string);
    void AddPrimitiveScorer(std::string,std::string);


  protected:
    G4VPhysicalVolume* SetHit(G4LogicalVolume*);
    void Init();
    G4VSolid* solid;
    G4LogicalVolume* logicalVolume;
    G4VPhysicalVolume* physicalVolume;
    G4Material* material;
    G4FieldManager* fieldManager;
    G4VisAttributes* visAttributes;
    std::string   sSDType;
    std::string   sSDName;
    std::string   sSolidType;
    std::string   sBaseNameChild;
    std::string   sMaterial;
    std::map<std::string,std::string> mStrStrScorer;
    double dLength;
    double dWidth;
    double dHeight;
    double dRadiusInner;
    double dAngleStart;
    double dAngleEnd;
    double dStepLimit;
    int iStepLimitFlag;
    int iHitFlag;

};
typedef std::map<std::string,std::string> MStrStrScorer;

#endif

