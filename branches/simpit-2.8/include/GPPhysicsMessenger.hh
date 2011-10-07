// $Id: GPPhysicsMessenger.hh,v 1.7 2006/06/29 17:48:34 gunter Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef GPPhysicsMessenger_h
#define GPPhysicsMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class G4UIdirectory;
class G4UIcommand;
class G4UIcmdWithAString;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class GPPhysicsMessenger: public G4UImessenger
{
  public:
    GPPhysicsMessenger ();
   ~GPPhysicsMessenger ();
    
    void SetNewValue (G4UIcommand*, G4String);
    
  private:
    
    G4UIdirectory*	pAppDir;
    G4UIcmdWithAString*	pParameterCmd;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

