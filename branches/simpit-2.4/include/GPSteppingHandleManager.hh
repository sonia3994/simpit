// $Id: GPSteppingHandleManager.hh,v 1.10 2007/07/02 13:22:08 vnivanch Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef GPSTEPPINGHANDLEMANAGER_H
#define GPSTEPPINGHANDLEMANAGER_H 1

#include<string>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4Step;
class GPSteppingHandle;
class GPSteppingHandleManager 
{
  public:
    static GPSteppingHandleManager* GetInstance();
    static void Delete();
    void UserSteppingAction(const G4Step* aStep);
    GPSteppingHandle* FindAndBuildSteppingHandle(std::string ,std::string ,std::string );
    void Prepare();
    void CleanUp();
  protected:
    GPSteppingHandleManager();
    ~GPSteppingHandleManager();
  protected:
    static GPSteppingHandleManager* instance;
  
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
