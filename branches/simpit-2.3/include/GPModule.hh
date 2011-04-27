// $Id: GPModule.hh,v 1.11 2007/07/02 13:22:08 vnivanch Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef GPMODULE_H
#define GPMODULE_H 1

#include "GPObject.hh"
#include "G4ThreeVector.hh"
#include <sstream>
#include <string>
#include <fstream>
#include <map>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class GPGeometry;
class GPSteppingHandle;
class GPEventHandle;
class GPRunHandle;
class G4VPhysicalVolume;
class G4LogicalVolume;
class GPModule : public GPObject
{
  public:
    GPModule(std::string,std::string);
    virtual ~GPModule();
    void SetObject(std::string sValue, GPObject* object);
    GPObject* FindObject(std::string)const;
    G4VPhysicalVolume* ConstructGeometry(G4LogicalVolume*);
    void AddChild(GPModule*);
    void Print();
    void Print(std::ofstream&);
    void Update();
    void SetParameter(std::string,std::string);
    inline int GetPriority(){return iPriority;};
    inline void SetPriority(int i){iPriority=i;};
    inline int GetCompactRanger(){return iCompactRangerFlag;};
    inline void SetCompactRanger(int i){iCompactRangerFlag=i;};
  protected:
    void ConstructGeometryCompact(G4LogicalVolume*);
    void ConstructGeometryNormal(G4LogicalVolume*);
    void SetGeometry();
    void DelGeometry();
    void SetSteppingHandle();
    void DelSteppingHandle();
    void SetEventHandle();
    void DelEventHandle();
    void SetRunHandle();
    void DelRunHandle();
    void AddChild(std::string sValue);
    void DelChild(std::string sValue);
  protected:
    int iPriority;
    int iCompactRangerFlag;
    GPGeometry*       geometry;
    GPSteppingHandle* steppingHandle;
    GPEventHandle*    eventHandle;
    GPRunHandle*      runHandle;
    std::map<std::string,GPModule* > mChildModule;
    G4ThreeVector vCenterChildPosition;

};

typedef std::map<std::string,GPModule* > GPModuleMap;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

