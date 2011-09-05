// $Id: GPModuleStore.hh,v 1.11 2007/07/02 13:22:08 vnivanch Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef GPMODULESTORE_H
#define GPMODULESTORE_H 1

#include "GPModule.hh"
#include <map>
#include <string>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class GPModuleStore
{
  protected:
    GPModuleStore();
    ~GPModuleStore();
  public:
    static GPModuleStore* GetInstance();
    static void Delete();
    GPModuleMap*  GetModuleMap(){return &mStrModule;};
    GPModule* FindModule(std::string);
    void AddModule(std::string, GPModule*);
    void DelModule(std::string);
    void EraseItem(std::string);
    void Clear();
    void Print();

  protected:
    static GPModuleStore* instance;
    GPModuleMap mStrModule;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

