// $Id: GPMainMessenger.cc,v 1.10 2006/06/29 17:48:58 gunter Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "GPMainMessenger.hh"
#include "GPMain.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"

#include <sstream>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GPMainMessenger::GPMainMessenger()
{ 
  pAppDir = new G4UIdirectory("/GP/App/");
  pAppDir->SetGuidance("Application control");
  
  pParameterCmd = new G4UIcmdWithAString("/GP/App/SetParameter",this);
  pParameterCmd->SetGuidance("Set App parameter.");
  pParameterCmd->SetGuidance("Usage: {object/key} {value} {unit}");
  pParameterCmd->SetGuidance("For example: SetParameter app.out_dir output");
  pParameterCmd->SetParameterName("choice",false);
  pParameterCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GPMainMessenger::~GPMainMessenger()
{
  delete pParameterCmd;
  delete pAppDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GPMainMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{ 
  if( command == pParameterCmd )
   { GPMain::GetInstance()->SetParameter(newValue);}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
