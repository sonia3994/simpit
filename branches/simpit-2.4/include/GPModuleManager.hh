// $Id: GPModuleManager.hh,v 1.11 2007/07/02 13:22:08 vnivanch Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef GPMODULEMANAGER_H
#define GPMODULEMANAGER_H 1
#include <string>
#include <fstream>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class GPModule;
class GPModuleManagerMessenger; 
class G4VPhysicalVolume;
class GPModuleManager 
{
  public:
    static GPModuleManager* GetInstance();
    static void Delete();
    void ConstructModule();
    GPModule* FindAndBuildModule(std::string,std::string,std::string);
    G4VPhysicalVolume* ConstructGeometry();
    void ClearModule();
    void Print();
    void Print(std::ofstream&);
    void Update();
    void SetParameter(std::string);
    double GetParameter(std::string);
    inline std::string GetRootName(){return sRootName;};
  protected:
    GPModuleManager();
    void ConstructRoot();
    void ConstructCrystal();
    void ConstructSweeper();
    void ConstructTarget();
    void ConstructCapture();
    void ConstructAccelerator();
    ~GPModuleManager();
  protected:
    static GPModuleManager* instance;
    GPModuleManagerMessenger* moduleManagerMessenger;
    GPModule* worldModule;
    int iConstructedFlag;
    std::string sRootName;
  
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

