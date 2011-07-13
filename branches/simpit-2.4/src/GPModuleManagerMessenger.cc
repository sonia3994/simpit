// $Id: GPModuleManagerMessenger.cc,v 1.10 2006/06/29 17:48:58 gunter Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "GPModuleManagerMessenger.hh"

#include "GPModuleManager.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"

#include <sstream>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GPModuleManagerMessenger::GPModuleManagerMessenger()
{ 
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPModuleManagerMessenger::GPModuleManagerMessenger(GPModuleManager*)"<<G4endl;
#endif
  moduleDir = new G4UIdirectory("/GP/Module/");
  moduleDir->SetGuidance("Module control");
       
  setParameterCmd = new G4UIcmdWithAString("/GP/Module/SetParameter",this);
  setParameterCmd->SetGuidance("Set Module parameter.");
  setParameterCmd->SetGuidance("Usage: {object/key} {value} {unit}");
  setParameterCmd->SetGuidance("For example: SetParameter /target/geometry.length 8 mm");
  setParameterCmd->SetParameterName("choice",false);
  setParameterCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  updateCmd = new G4UIcmdWithoutParameter("/GP/Module/Update",this);
  updateCmd->SetGuidance("Update Module");
  updateCmd->SetGuidance("This command MUST be applied before \"/run/beamOn\" ");
  updateCmd->SetGuidance("if you changed geometrical value(s).");
  updateCmd->AvailableForStates(G4State_Idle);

  printCmd = new G4UIcmdWithoutParameter("/GP/Module/Print",this);
  printCmd->SetGuidance("Print Module parameters.");
  printCmd->AvailableForStates(G4State_Idle);      

#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPModuleManagerMessenger::GPModuleManagerMessenger(GPModuleManager*)"<<G4endl;
#endif
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GPModuleManagerMessenger::~GPModuleManagerMessenger()
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPModuleManagerMessenger::~GPModuleManagerMessenger()"<<G4endl;
#endif
  delete setParameterCmd; 
  delete updateCmd;
  delete printCmd;
  delete moduleDir;
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPModuleManagerMessenger::~GPModuleManagerMessenger()"<<G4endl;
#endif
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GPModuleManagerMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{ 
  GPModuleManager* moduleManager=GPModuleManager::GetInstance();
  if( command == setParameterCmd )
    moduleManager->SetParameter(newValue);
  if( command == printCmd )
    moduleManager->Print();
  if( command == updateCmd )
    moduleManager->Update();  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
