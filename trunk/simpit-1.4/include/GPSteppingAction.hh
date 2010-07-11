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
#include <fstream>

class GPSteppingMessenger;
class GPDetectorConstruction;
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
    
private:
  //void WriteToFileDT(G4ThreeVector,G4ThreeVector,G4double,G4double);
  //void WriteToFileDC(G4ThreeVector,G4ThreeVector,G4double,G4double);

  GPDetectorConstruction* detector;
  GPEventAction*          eventAction;
  GPSteppingMessenger*	steppingMessenger;
    const G4VPhysicalVolume* capturePhys;
    const G4VPhysicalVolume* targetPhys;
    const G4VPhysicalVolume* transferPhys;
    const G4VPhysicalVolume* vacuumPhys;
    const G4VPhysicalVolume* acceleratorPhys;

  	G4String 		dParticle;
  	std::ofstream 	  ofsWriteFile;
    G4double dTargetL;
    G4double dTargetX;
    G4double dTargetY;
    G4double dCaptureL;
    G4double dCaptureR;
    G4double dAcceleratorL;
    G4double dAcceleratorR;

	G4int   verbose;
  
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
