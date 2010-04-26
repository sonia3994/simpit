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
  	delete 	steppingMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GPSteppingAction::Init()
{

	targetPhys = detector->GetTargetPhysical(); 
	capturePhys = detector->GetCapturePhysical(); 
	transferPhys = detector->GetTransferPhysical(); 
	vacuumPhys = detector->GetVacuumPhysical();
    targetL = detector->GetTargetLength();
    targetX = detector->GetTargetX();
    targetY = detector->GetTargetY();
    captureL = detector->GetCaptureLength();
    captureR = detector->GetCaptureRadius();

}

void GPSteppingAction::UserSteppingAction(const G4Step* aStep)
{
	static G4VPhysicalVolume* 	postPhys=NULL; 
	static G4VPhysicalVolume* 	prevPhys;
	static G4StepPoint* 		prevStepPoint;
	static G4StepPoint* 		postStepPoint;
	static G4Track*				currentTrack;
	static G4TrackStatus		currentTrackStatus;

	static G4String 			particleName;
	static G4ThreeVector		prevPos;
	static G4ThreeVector		postPos;
	static G4ThreeVector		prevMom;
	static G4ThreeVector		postMom;
	static G4double				globalTime; 
	static G4double				totalE; 
	static G4double				stepE;
	static G4double				stepL;
	static G4int   				stopFlag;
	
	stepE=aStep->GetTotalEnergyDeposit();
	particleName = aStep->GetTrack()->GetDefinition()->GetParticleName();

	if(stepE<0) {return;}
	
	stepL = aStep->GetStepLength();
	if(stepL<=0.0)
	{
    	stopFlag++;
		if(stopFlag>=10)
		{
        	currentTrack=aStep->GetTrack();
			currentTrackStatus=fStopAndKill;
			currentTrack->SetTrackStatus(currentTrackStatus);
			G4cout<<"Kill a particle because it does not move more then 10 steps, particle type: "<<particleName<<G4endl;
		}
		
	}
	else 
	{
    	stopFlag=0;
	}
	//if (aStep->GetTrack()->GetDefinition()->GetPDGCharge() != 0.)


	prevStepPoint=aStep->GetPreStepPoint();
	postStepPoint=aStep->GetPostStepPoint();

	prevPhys= prevStepPoint->GetPhysicalVolume();
	prevPos=prevStepPoint->GetPosition();
	prevMom=prevStepPoint->GetMomentum();
	totalE=prevStepPoint->GetTotalEnergy();
	globalTime=prevStepPoint->GetGlobalTime();

	postPos=postStepPoint->GetPosition();
	postMom=postStepPoint->GetMomentum();
	
	if (prevPhys==targetPhys) 
	{
	  eventaction->AddTargetED(stepE);
	}
	
	if (particleName==particle)
	{
		///*	
		if (prevPhys == capturePhys&&postPhys==targetPhys)
		{  
			eventaction->AddPositron(prevPos,prevMom,totalE);
			eventaction->AddTargetStep(stepL);
			WriteToFileDT(prevPos,prevMom,totalE,globalTime);
		}
		if(prevPhys==vacuumPhys&&postPhys==capturePhys)
		{
			//eventaction->AddPositron(positon,prevMom,totalE);
			//eventaction->AddTargetStep(stepL);
			if((prevPos.x()*prevPos.x()+prevPos.y()*prevPos.y())<=4*cm*cm&&prevPos.z()>=(captureL+targetL/2))
				WriteToFileDC(prevPos,prevMom,totalE,globalTime);
		}
		//*/	
  	postPhys=prevPhys;
  	}

	if(particleName=="gamma")
	{
		if (prevPhys == targetPhys&&postPhys==transferPhys)
		{  
  			GPRunAction* user_run_action =
  			(GPRunAction*)G4RunManager::GetRunManager()->GetUserRunAction();
			user_run_action->AddActualG(1);
		}

  		postPhys=prevPhys;
	}

}

void GPSteppingAction::WriteToFileDT(G4ThreeVector P,G4ThreeVector M,G4double T, G4double time)
{
	GPRunAction* user_run_action =
	(GPRunAction*)G4RunManager::GetRunManager()->GetUserRunAction();
	user_run_action->dataFileDT
	  <<P.x()<<" "<<P.y()<<" "
	  <<M.x()<<" "<<M.y()<<" "<<M.z()<<" "
	  <<T<<" "<<time<<G4endl;
	
}
void GPSteppingAction::WriteToFileDC(G4ThreeVector P,G4ThreeVector M,G4double T, G4double time)
{
	GPRunAction* user_run_action =
	(GPRunAction*)G4RunManager::GetRunManager()->GetUserRunAction();
	user_run_action->dataFileDC
	  <<P.x()<<" "<<P.y()<<" "
	  <<M.x()<<" "<<M.y()<<" "<<M.z()<<" "
	  <<T<<" "<<time<<G4endl;
	
}
void GPSteppingAction::SetSelectedParticle(G4String tmpParticle)
{
	particle=tmpParticle;
        G4cout<<"The filter particle is:"<<particle<<G4endl;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
