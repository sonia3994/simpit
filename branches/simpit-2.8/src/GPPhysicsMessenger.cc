// $Id: GPPhysicsMessenger.cc,v 1.10 2006/06/29 17:48:58 gunter Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "GPPhysicsMessenger.hh"
#include "GPPhysicsList.hh"

#include "G4RunManager.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"

#include <sstream>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GPPhysicsMessenger::GPPhysicsMessenger()
{ 
  pAppDir = new G4UIdirectory("/GP/Physics/");
  pAppDir->SetGuidance("Physics control");
  
  pParameterCmd = new G4UIcmdWithAString("/GP/Physics/SetParameter",this);
  pParameterCmd->SetGuidance("Set Physics parameter.");
  pParameterCmd->SetGuidance("Usage: {object/key} {value} {unit}");
  pParameterCmd->SetGuidance("For example: SetParameter cut_off.e- 1 mm");
  pParameterCmd->SetParameterName("choice",false);
  pParameterCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GPPhysicsMessenger::~GPPhysicsMessenger()
{
  delete pParameterCmd;
  delete pAppDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GPPhysicsMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{ 
  if( command == pParameterCmd )
   { 
     ((GPPhysicsList*)G4RunManager::GetRunManager()->GetUserPhysicsList())
     ->SetParameter(newValue);
   }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
