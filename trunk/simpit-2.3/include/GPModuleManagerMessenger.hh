// $Id: GPModuleManagerMessenger.hh,v 1.7 2006/06/29 17:48:34 gunter Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef GPDETECTORMANAGERMESSENGER_H
#define GPDETECTORMANAGERMESSENGER_H 1

#include "globals.hh"
#include "G4UImessenger.hh"

class G4UIdirectory;
class G4UIcommand;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class GPModuleManagerMessenger: public G4UImessenger
{
  public:
    GPModuleManagerMessenger();
   ~GPModuleManagerMessenger();
    
    void SetNewValue	 (G4UIcommand*, G4String);
    
  private:
    
    G4UIdirectory*  moduleDir;
    G4UIcmdWithAString*	setParameterCmd;
    G4UIcmdWithoutParameter* updateCmd;
    G4UIcmdWithoutParameter* printCmd;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

