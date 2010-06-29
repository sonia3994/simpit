// $Id: GPAcceleratorFieldMessenger.hh,v 1.4 2006/06/29 17:18:49 gunter Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#ifndef GPAcceleratorFieldMessenger_h
#define GPAcceleratorFieldMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class GPAcceleratorFieldManager;
class GPAcceleratorField;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADouble;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;
class G4UIcmdWithABool;



class GPAcceleratorFieldMessenger: public G4UImessenger
{
  public:
    GPAcceleratorFieldMessenger(GPAcceleratorFieldManager* );
   ~GPAcceleratorFieldMessenger();
    
    void SetNewValue(G4UIcommand*, G4String);
    void SetNewValue(G4UIcommand*, G4int);
    void SetFieldPoint(GPAcceleratorField* t){fieldPoint=t;};
    
  private:

    GPAcceleratorFieldManager*             	 fEMfieldManager;
    GPAcceleratorField*           	 fieldPoint;
    
    G4UIdirectory*            	 GPdetDir;

    G4UIcmdWithAnInteger*     	 StepperCmd;
    G4UIcmdWithAnInteger*     	 FieldType;
    G4UIcmdWithADoubleAndUnit*	 MagFieldB0Cmd;
    G4UIcmdWithADoubleAndUnit*	 EleFieldE0Cmd;
    G4UIcmdWithADoubleAndUnit*	 MinStepCmd;
    G4UIcmdWithoutParameter*  	 UpdateCmd;

    G4UIcmdWithABool*         	 FieldFlag;


};

#endif

