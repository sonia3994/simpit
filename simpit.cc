//
// $Id: GammaPair.cc,v 1.6 2006/06/29 17:47:10 gunter Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
// 
// --------------------------------------------------------------
//      GEANT 4 - exampleN01
// --------------------------------------------------------------
#include "GPDetectorConstruction.hh"
#include "GPPhysicsList.hh"
#include "GPPrimaryGeneratorAction.hh"
#include "GPRunAction.hh"
#include "GPEventAction.hh"
#include "GPSteppingAction.hh"
#include "GPTrackingAction.hh"

#include "Randomize.hh"
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#include "G4UIQt.hh"
#endif

  //CLHEP::HepRandomEngine* ranecuEngine=new CLHEP::RanecuEngine;
CLHEP::RanecuEngine ranecuEngine;
std::string sProgramName="simpit";
std::string sProgramVersion="2.6";

int main(int argc,char** argv)
{
//Choose the Random engine
//  

  CLHEP::HepRandom::setTheEngine(&ranecuEngine);

  // Construct the default run manager
  //
  G4RunManager* runManager = new G4RunManager;

  // set mandatory initialization classes
  //
  GPDetectorConstruction* detector = new GPDetectorConstruction;
  runManager->SetUserInitialization(detector);
  //
  G4VUserPhysicsList* physics = new GPPhysicsList;
  runManager->SetUserInitialization(physics);

  // set mandatory user action class
  //
  GPPrimaryGeneratorAction* gen_action = new GPPrimaryGeneratorAction();
  runManager->SetUserAction(gen_action);

  GPRunAction* run_action = new GPRunAction();  
  runManager->SetUserAction(run_action);
  //
  GPEventAction* event_action = new GPEventAction();
  runManager->SetUserAction(event_action);
  //
  G4UserTrackingAction* tracking_Action= new GPTrackingAction();
  runManager->SetUserAction(tracking_Action);

  G4UserSteppingAction* stepping_action =
                    new GPSteppingAction();
  runManager->SetUserAction(stepping_action);

  // Initialize G4 kernel
  //
  runManager->Initialize();

  // Get the pointer to the UI manager and set verbosities
  //
  G4UImanager* UI = G4UImanager::GetUIpointer();
  if (argc!=1)
	{
	G4String command="/control/execute ";
	G4String fileName=argv[1];
	UI->ApplyCommand(command+fileName);
	}
  else
    {
#ifdef G4VIS_USE
	G4VisManager * visManager = new G4VisExecutive;
	visManager->Initialize();
	visManager->SetVerboseLevel (1);
	
#endif

	G4UIsession * session=0;
#ifdef G4UI_USE_QT
	session = new G4UIQt(argc,argv);
#elif G4UI_USE_TCSH
	session = new G4UIterminal(new G4UItcsh);
#else
	session = new G4UIterminal();
#endif

	session->SessionStart();
	delete session;
#ifdef G4VIS_USE
	delete visManager;
#endif
    }
  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  //                 owned and deleted by the run manager, so they should not
  //                 be deleted in the main() program !
  delete runManager;

  return 0;
}

