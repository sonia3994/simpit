//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
// $Id: exampleN01.cc,v 1.6 2006/06/29 17:47:10 gunter Exp $
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
#include "Randomize.hh"

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

//#include <fstream>
//using namespace std;
//ofstream positronfilename("/home/G4WorkShop/GammaPair1/positron.data");
  //CLHEP::HepRandomEngine* ranecuEngine=new CLHEP::RanecuEngine;
  CLHEP::RanecuEngine ranecuEngine;

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
  GPPrimaryGeneratorAction* gen_action = new GPPrimaryGeneratorAction(detector);
  runManager->SetUserAction(gen_action);

  GPRunAction* run_action = new GPRunAction(gen_action,detector);  
  runManager->SetUserAction(run_action);
  //
  GPEventAction* event_action = new GPEventAction(run_action);
  runManager->SetUserAction(event_action);
  //
  G4UserSteppingAction* stepping_action =
                    new GPSteppingAction(detector, event_action);
  runManager->SetUserAction(stepping_action);

  // Initialize G4 kernel
  //
  runManager->Initialize();

  // Get the pointer to the UI manager and set verbosities
  //
  G4UImanager* UI = G4UImanager::GetUIpointer();
  if (argc!=1)
	{
	G4String command="/control/execute";
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
#ifdef G4UI_USE_TCSH
	session = new G4UIterminal(new G4UItcsh);
#else
	session = new G4UIterminal();
#endif
//#ifdef G4VIS_USE
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


