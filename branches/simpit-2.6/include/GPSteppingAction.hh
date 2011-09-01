// $Id: GPSteppingAction.hh,v 1.10 2007/07/02 13:22:08 vnivanch Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef GPSteppingAction_h
#define GPSteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"

class GPSteppingMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class GPSteppingAction : public G4UserSteppingAction
{
public:
  GPSteppingAction();
  virtual ~GPSteppingAction();

  void UserSteppingAction(const G4Step*);
  void SetSelectedParticle(G4String);
  void Prepare();
  void CleanUp();
    
private:
  GPSteppingMessenger*	  steppingMessenger;

  G4String 		sParticle;
  G4int   verbose;
  
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
