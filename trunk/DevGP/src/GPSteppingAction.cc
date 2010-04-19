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
// $Id: GPSteppingAction.cc,v 1.15 2006/06/29 17:49:13 gunter Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "GPSteppingAction.hh"
#include "GPSteppingMessenger.hh"

#include "GPDetectorConstruction.hh"
#include "GPEventAction.hh"
#include "GPRunAction.hh"

#include "G4ThreeVector.hh"
#include "G4RunManager.hh"
#include "G4Step.hh"

//#include <fstream>
#include <sstream>
using namespace std;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GPSteppingAction::GPSteppingAction(GPDetectorConstruction* det,
                                         GPEventAction* evt)
:detector(det), eventaction(evt)					 
{ 
  particle="e+";
  steppingMessenger = new GPSteppingMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GPSteppingAction::~GPSteppingAction()
{ 
  delete steppingMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GPSteppingAction::Init()
{

  targetPhys=detector->GetTargetPhysical(); 
  capturePhys=detector->GetCapturePhysical(); 
  transferPhys=detector->GetTransferPhysical(); 
  vacuumPhys = detector->GetVacuumPhysical();

}

void GPSteppingAction::UserSteppingAction(const G4Step* aStep)
{
  static G4VPhysicalVolume* postPhys=NULL; 
  static G4VPhysicalVolume* previousPhys;
  static G4StepPoint* preStepPoint;
  static G4StepPoint* postStepPoint;
  static G4String particleName;
  static G4double stepE;
  static G4ThreeVector position;
  static G4ThreeVector momentum;
  static G4double totalE; 
  static G4double stepL;
  
  stepE=aStep->GetTotalEnergyDeposit();
  if(stepE<0)
    {return;}

  preStepPoint=aStep->GetPreStepPoint();
  postStepPoint=aStep->GetPostStepPoint();

/*
   		position=preStepPoint->GetPosition();
   		momentum=preStepPoint->GetMomentum();
        G4cout<<"x: "<<position.x()<<" y: "<<position.y()<<" z: "<<position.z()<<" px: "
			<<momentum.x()<<" py: "<<momentum.y()<<" pz: "<<momentum.z()<<G4endl; 
*/
  // get volume of the current step and the post volume
  // collect energy and track length step by step
  //= preStepPoint->GetPhysicalVolume();

  particleName = aStep->GetTrack()->GetDefinition()->GetParticleName();
  //previousPhys= preStepPoint->GetTouchableHandle()->GetVolume();
  previousPhys= preStepPoint->GetPhysicalVolume();
  //postPhys= postStepPoint->GetPhysicalVolume();
  //postPhys= postStepPoint->GetTouchableHandle()->GetVolume();

  if (aStep->GetTrack()->GetDefinition()->GetPDGCharge() != 0.)
    stepL = aStep->GetStepLength();
      
  if (previousPhys==targetPhys) 
  {
	eventaction->AddTargetED(stepE);
  }
 
  if (particleName==particle)
  {
/*	if (previousPhys == targetPhys&&postPhys==vacuumPhys)
	{  
   		position=preStepPoint->GetPosition();
   		momentum=preStepPoint->GetMomentum();
   		totalE=preStepPoint->GetTotalEnergy();
   		eventaction->AddPositron(position,momentum,totalE);
   		eventaction->AddTargetStep(stepL);
   		WriteToFileDT(position,momentum,totalE);
	}
	  
	if (previousPhys == targetPhys&&postPhys==transferPhys)
	{  
   		position=preStepPoint->GetPosition();
   		momentum=preStepPoint->GetMomentum();
   		totalE=preStepPoint->GetTotalEnergy();
   		eventaction->AddPositron(position,momentum,totalE);
   		eventaction->AddTargetStep(stepL);
   		WriteToFileDT(position,momentum,totalE);
	}
	
*/
///*	
	if (previousPhys == capturePhys&&postPhys==targetPhys)
	{  
   		position=preStepPoint->GetPosition();
   		momentum=preStepPoint->GetMomentum();
   		totalE=preStepPoint->GetTotalEnergy();
   		eventaction->AddPositron(position,momentum,totalE);
   		eventaction->AddTargetStep(stepL);
   		WriteToFileDT(position,momentum,totalE);
	}
	if(previousPhys==vacuumPhys&&postPhys==capturePhys)
 	{
   		position=preStepPoint->GetPosition();
   		momentum=preStepPoint->GetMomentum();
   		totalE=preStepPoint->GetTotalEnergy();
   		//eventaction->AddPositron(positon,momentum,totalE);
   		//eventaction->AddTargetStep(stepL);
   		if(((position.x()*position.x()+position.y()*position.y())<=(400*mm*mm))&&(position.z()>=detector->GetCaptureLength()))
   			WriteToFileDC(position,momentum,totalE);
	}
//*/	
  postPhys=previousPhys;
  }
	else if(particleName=="gamma")
	{
		if (previousPhys == targetPhys&&postPhys==transferPhys)
		{  
  			GPRunAction* user_run_action =
  			(GPRunAction*)G4RunManager::GetRunManager()->GetUserRunAction();
			user_run_action->AddActualG(1);
		}

  		postPhys=previousPhys;
	}

//  passedVolume=previousPhys;
  //example of saving random number seed of this event, under condition
  //// if (condition) G4RunManager::GetRunManager()->rndmSaveThisEvent(); 
}
void GPSteppingAction::WriteToFileDT(G4ThreeVector P,G4ThreeVector M,G4double T)
{
//  G4RunManager* theRunManager =  G4RunManager::GetRunManager();
  GPRunAction* user_run_action =
  (GPRunAction*)G4RunManager::GetRunManager()->GetUserRunAction();
//  writeFile=user_run_action->GetDataFile();

  user_run_action->dataFileDT
	<<P.x()<<" "<<P.y()<<" "
  	<<M.x()<<" "<<M.y()<<" "<<M.z()<<" "
	<<T<<G4endl;

}
void GPSteppingAction::WriteToFileDC(G4ThreeVector P,G4ThreeVector M,G4double T)
{
//  G4RunManager* theRunManager =  G4RunManager::GetRunManager();
  GPRunAction* user_run_action =
  (GPRunAction*)G4RunManager::GetRunManager()->GetUserRunAction();
//  writeFile=user_run_action->GetDataFile();
  user_run_action->dataFileDC
	<<P.x()<<" "<<P.y()<<" "
  	<<M.x()<<" "<<M.y()<<" "<<M.z()<<" "
	<<T<<G4endl;

}
void GPSteppingAction::SetSelectedParticle(G4String tmpParticle)
{
	particle=tmpParticle;
        G4cout<<"The filter particle is:"<<particle<<G4endl;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
