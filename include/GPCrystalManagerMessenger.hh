// $Id: GPCrystalManagerMessenger.hh,v 1.8 2007/07/02 13:22:08 vnivanch Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef GPCrystalManagerMessenger_h
#define GPCrystalManagerMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class G4UIdirectory;
class G4UIcommand;
class G4UIcmdWithAString;
class G4UIcmdWithABool;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class GPCrystalManagerMessenger: public G4UImessenger
{
public:
  GPCrystalManagerMessenger();
  virtual ~GPCrystalManagerMessenger();
    
  void SetNewValue(G4UIcommand*, G4String);
private:
  G4UIdirectory*       pCrystalDir; 

  G4UIcmdWithAString*	SetParameter;
  G4UIcmdWithAString*	GetParameter;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

