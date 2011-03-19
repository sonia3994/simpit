// $Id: GPEventActionMessenger.hh,v 1.7 2006/06/29 17:48:34 gunter Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef GPEventActionMessenger_h
#define GPEventActionMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class GPEventAction;
class G4UIdirectory;
class G4UIcommand;
class G4UIcmdWithAnInteger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class GPEventActionMessenger: public G4UImessenger
{
  public:
    GPEventActionMessenger				(GPEventAction* );
   ~GPEventActionMessenger				();
    
    void SetNewValue					(G4UIcommand*, G4String);
    
  private:
    GPEventAction*				eventAction;
    
    G4UIdirectory*					eventActionDir;
    G4UIcmdWithAnInteger*			trajectoryFlagCmd;    
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

