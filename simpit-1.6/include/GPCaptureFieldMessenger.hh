// $Id: GPCaptureFieldMessenger.hh,v 1.4 2006/06/29 17:18:49 gunter Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#ifndef GPCaptureFieldMessenger_h
#define GPCaptureFieldMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class GPCaptureFieldManager;
class GPCaptureField;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADouble;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;
class G4UIcmdWithABool;


class GPCaptureFieldMessenger: public G4UImessenger
{
  public:
    GPCaptureFieldMessenger(GPCaptureFieldManager* );
   ~GPCaptureFieldMessenger();
    
    void SetNewValue(G4UIcommand*, G4String);
    void SetNewValue(G4UIcommand*, G4int);
    void SetFieldPoint(GPCaptureField* t){fieldPoint=t;};
    
  private:

    GPCaptureFieldManager*             	 fEMfieldManager;
    GPCaptureField*           	 fieldPoint;
    
    G4UIdirectory*            	 GPdetDir;

    G4UIcmdWithAnInteger*     	 StepperCmd;
    G4UIcmdWithAnInteger*     	 CaptureType;
    G4UIcmdWithADoubleAndUnit*	 MagFieldB0Cmd;
    G4UIcmdWithADoubleAndUnit*	 MinStepCmd;
    G4UIcmdWithADoubleAndUnit*	 LithiumFocalLengthCmd;
    G4UIcmdWithADoubleAndUnit*	 MagneticRigidityCmd;
    G4UIcmdWithADoubleAndUnit*	 LithiumCurrentCmd;
    G4UIcmdWithADouble*	 		 AMDAlphaCmd;
    G4UIcmdWithADouble*	 		 QWTFermiApproxAlphaCmd;
    G4UIcmdWithoutParameter*  	 UpdateCmd;

    G4UIcmdWithABool*         	 FieldFlag;


};

#endif

