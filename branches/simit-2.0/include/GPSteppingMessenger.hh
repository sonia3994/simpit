#ifndef GPSteppingMessenger_h
#define GPSteppingMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class GPSteppingAction;
class G4UIdirectory;
class G4UIcmdWithAString;

class GPSteppingMessenger: public G4UImessenger
{
public:
  GPSteppingMessenger(GPSteppingAction*);
  virtual ~GPSteppingMessenger();
    
  void SetNewValue(G4UIcommand*, G4String);
private:
  GPSteppingAction* GPAction;
  G4UIdirectory*               generatorDir; 
  G4UIcmdWithAString*          selectParCmd;
 // G4UIcmdWithAnInteger*        InitNumbCmd;
 // G4UIcommand*		EneDistrCmd;
 // G4UIcommand*		PosDistrCmd;
 // G4UIcommand*		MomDistrCmd;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

