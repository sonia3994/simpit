// $Id: GPDetectorMessenger.cc,v 1.10 2006/06/29 17:48:58 gunter Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "GPDetectorMessenger.hh"

#include "GPDetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"

#include <sstream>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GPDetectorMessenger::GPDetectorMessenger(
                                           GPDetectorConstruction* GPDet)
:GPDetector(GPDet)
{ 
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPDetectorMessenger::GPDetectorMessenger(GPDetectorConstruction*)"<<G4endl;
#endif
  GPDir = new G4UIdirectory("/GP/");
  GPDir->SetGuidance("UI commands of this example");
  
  detDir = new G4UIdirectory("/GP/detector/");
  detDir->SetGuidance("detector control");
       
  DetectorSizeCmd = new G4UIcmdWithAString("/GP/detector/detectorSize",this);
  DetectorSizeCmd->SetGuidance("Set detector size.");
  DetectorSizeCmd->SetGuidance("Usage: {key} {value} {unit}");
  DetectorSizeCmd->SetGuidance("For example: SetParameter target.l 8 mm");
  DetectorSizeCmd->SetParameterName("choice",false);
  DetectorSizeCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  UpdateCmd = new G4UIcmdWithoutParameter("/GP/detector/update",this);
  UpdateCmd->SetGuidance("Update Positron Resource geometry.");
  UpdateCmd->SetGuidance("This command MUST be applied before \"/run/beamOn\" ");
  UpdateCmd->SetGuidance("if you changed geometrical value(s).");
  UpdateCmd->AvailableForStates(G4State_Idle);

  PrintParaCmd = new G4UIcmdWithoutParameter("/GP/detector/printParameters",this);
  PrintParaCmd->SetGuidance("Print geometry parameters.");
  PrintParaCmd->AvailableForStates(G4State_Idle);      

#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPDetectorMessenger::GPDetectorMessenger(GPDetectorConstruction*)"<<G4endl;
#endif
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GPDetectorMessenger::~GPDetectorMessenger()
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPDetectorMessenger::~GPDetectorMessenger()"<<G4endl;
#endif
//  delete NbLayersCmd;
  delete DetectorSizeCmd; 
  delete UpdateCmd;
//  delete MagFieldCmd;
  delete PrintParaCmd;
  delete detDir;
  delete GPDir;  
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPDetectorMessenger::~GPDetectorMessenger()"<<G4endl;
#endif
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GPDetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{ 
  if( command == DetectorSizeCmd )
   { GPDetector->SetParameter(newValue);}

  if( command == PrintParaCmd )
   { GPDetector->PrintDetectorParameters();}


  if( command == UpdateCmd )
   { GPDetector->UpdateGeometry();  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
