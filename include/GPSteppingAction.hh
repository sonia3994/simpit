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
class GPDetectorConstruction;
class GPTargetSteppingAction;
class GPEventAction;
class G4VPhysicalVolume;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class GPSteppingAction : public G4UserSteppingAction
{
public:
  GPSteppingAction(GPDetectorConstruction*, GPEventAction*);
  virtual ~GPSteppingAction();

  void UserSteppingAction(const G4Step*);
  void SetSelectedParticle(G4String);
  void Init();
  void CleanUp();
    
private:
  GPDetectorConstruction* detector;
  GPTargetSteppingAction* targetSteppingAction;
  GPEventAction*          eventAction;
  GPSteppingMessenger*	  steppingMessenger;
  const G4VPhysicalVolume* targetPhys;

  G4String 		sParticle;
  G4int   verbose;
  
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
