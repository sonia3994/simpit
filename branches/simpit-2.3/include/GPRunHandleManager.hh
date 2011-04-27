// $Id: GPRunHandleManager.hh,v 1.11 2007/07/02 13:22:08 vnivanch Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef GPRUNHANDLEMANAGER_H
#define GPRUNHANDLEMANAGER_H 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4Run;
class GPRunHandleManager 
{
  public:
    static GPRunHandleManager* GetInstance();
    static void Delete();
    void BeginOfRunAction(const G4Run*);
    void EndOfRunAction(const G4Run*);
  protected:
    GPRunHandleManager();
    ~GPRunHandleManager();
  protected:
    static GPRunHandleManager* instance;
  
};
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

