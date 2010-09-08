// $Id: GPEventActionMessenger.cc,v 1.10 2006/06/29 17:48:58 gunter Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "GPEventActionMessenger.hh"

#include "GPEventAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAnInteger.hh"

#include <sstream>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GPEventActionMessenger::GPEventActionMessenger(
                                           GPEventAction* evtAction)
:eventAction(evtAction)
{ 
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPEventActionMessenger::GPEventActionMessenger(GPEventAction*)"<<G4endl;
#endif
  eventActionDir = new G4UIdirectory("/GP/eventAction/");
  eventActionDir->SetGuidance("Event Action control");
  
  trajectoryFlagCmd = new G4UIcmdWithAnInteger("/GP/eventAction/numberOfEventToRecordTrajectory",this);
  trajectoryFlagCmd->SetGuidance("Set number of events to record trajectories.");
  trajectoryFlagCmd->SetParameterName("numberOfEventToRecordTrajectory",false);
  trajectoryFlagCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPEventActionMessenger::GPEventActionMessenger(GPEventAction*)"<<G4endl;
#endif
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GPEventActionMessenger::~GPEventActionMessenger()
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPEventActionMessenger::~GPEventActionMessenger()"<<G4endl;
#endif
  delete trajectoryFlagCmd;
  delete eventActionDir;
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPEventActionMessenger::~GPEventActionMessenger()"<<G4endl;
#endif
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GPEventActionMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{ 
  if( command == trajectoryFlagCmd )
   { eventAction->SetTrajectoryFlag((trajectoryFlagCmd->GetNewIntValue(newValue)));}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
