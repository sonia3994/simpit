// $Id: GPCrystalManagerMessenger.cc,v 1.9 2006/06/29 17:49:09 gunter Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "GPCrystalManagerMessenger.hh"
#include "GPCrystalManager.hh"

#include "G4UIdirectory.hh"
#include "G4UIcommand.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UnitsTable.hh"


#include <sstream>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GPCrystalManagerMessenger::GPCrystalManagerMessenger()
{
  G4bool omitable;
  G4UIparameter* parameter;
  pCrystalDir = new G4UIdirectory("/GP/Crystal/");
  pCrystalDir->SetGuidance("Crystal manager");
  
  SetParameter = new G4UIcmdWithAString("/GP/Crystal/SetParameter",this);
  SetParameter->SetGuidance("Set Crystal parameter.");
  SetParameter->SetGuidance("Usage: {key} {value} {unit}");
  SetParameter->SetParameterName("choice",false);
  SetParameter->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  GetParameter = new G4UIcmdWithAString("/GP/Crystal/GetParameter",this);
  GetParameter->SetGuidance("Get Crystal parameter.");
  GetParameter->SetGuidance("Usage: {key}");
  GetParameter->SetParameterName("choice",false);
  GetParameter->AvailableForStates(G4State_PreInit,G4State_Idle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GPCrystalManagerMessenger::~GPCrystalManagerMessenger()
{
  delete SetParameter; 
  delete GetParameter; 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GPCrystalManagerMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{ 
  
   if( command == SetParameter )
   { GPCrystalManager::GetInstance()->SetParameter(newValue);}
   if( command == GetParameter )
   { GPCrystalManager::GetInstance()->GetParameter(newValue);}

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

