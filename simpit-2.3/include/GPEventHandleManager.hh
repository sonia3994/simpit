// $Id: GPEventHandleManager.hh,v 1.12 2007/07/02 13:22:08 vnivanch Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef GPEVENTHANDLEMANAGER_H
#define GPEVENTHANDLEMANAGER_H 1

class G4Event;
class GPEventHandleManager
{
  protected:
    GPEventHandleManager();
    ~GPEventHandleManager();
  
  public:
    static GPEventHandleManager* GetInstance();
    static void Delete();
    void BeginOfEventAction(const G4Event*);
    void EndOfEventAction(const G4Event*);
  protected:
    static GPEventHandleManager* instance;
      
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
