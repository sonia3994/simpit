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
:detector(det), eventAction(evt)					 
{ 
  	sParticle="e+";
  	steppingMessenger = new GPSteppingMessenger(this);
	verbose=0;
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
	acceleratorPhys = detector->GetPhysicalVolume("accelerator");

    dTargetX = detector->GetDetectorSize("target.x");
    dTargetY = detector->GetDetectorSize("target.y");
    dTargetL = detector->GetDetectorSize("target.z");
    dCaptureL = detector->GetDetectorSize("capture.z");
    dCaptureR = detector->GetDetectorSize("capture.or");
    dAcceleratorL = detector->GetDetectorSize("accelerator.l");
    dAcceleratorR = detector->GetDetectorSize("accelerator.or");

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
	static std::vector<G4double> outVector; 
	static G4double				globalTime; 
	static G4double				totalE; 
	static G4double				stepE;
	static G4double				stepL;
	static G4int   				stopFlag;
	static G4int   				trackID;
	static G4int   				eventID;
	static G4int				targetFilteredTrackID=-1;
	static G4int				targetFilteredEventID=-1;
	static G4int				captureFilteredTrackID=-1;
	static G4int				captureFilteredEventID=-1;
	static G4int				acceleratorFilteredTrackID=-1;
	static G4int				acceleratorFilteredEventID=-1;
	
	outVector.clear();
	static GPRunAction* userRunAction = (GPRunAction*)G4RunManager::GetRunManager()->GetUserRunAction();

    eventID=eventAction->GetEventID();
    trackID=aStep->GetTrack()->GetTrackID();
    currentTrack=aStep->GetTrack();
	currentTrackStatus=fStopAndKill;

	stepE=aStep->GetTotalEnergyDeposit();
	particleName = aStep->GetTrack()->GetDefinition()->GetParticleName();

	if(stepE<0) {return;}
	
	stepL = aStep->GetStepLength();
	if(stepL<=0)
	{
    	stopFlag++;
		if(stopFlag>=8)
		{
			currentTrack->SetTrackStatus(currentTrackStatus);
			if (verbose>=1)
			{
				G4cout<<"Kill a particle because it does not move more then 8 steps: "<<particleName<<G4endl;
				return;
			}
		}
	}

	else 
	{ stopFlag=0; }


	prevStepPoint=aStep->GetPreStepPoint();
	postStepPoint=aStep->GetPostStepPoint();

	prevPhys= prevStepPoint->GetPhysicalVolume();
	prevPos=prevStepPoint->GetPosition()/m;
	prevMom=prevStepPoint->GetMomentum()/MeV;
	totalE=prevStepPoint->GetTotalEnergy()/MeV;
	globalTime=prevStepPoint->GetGlobalTime()/picosecond;

	postPos=postStepPoint->GetPosition()/m;
	postMom=postStepPoint->GetMomentum()/MeV;
	
	outVector.push_back(eventID);
	outVector.push_back(trackID);
	outVector.push_back(prevPos.x()*m/mm);
	outVector.push_back(prevPos.y()*m/mm);
	//outVector.push_back(prevPos.z()*m/mm);
	outVector.push_back(prevMom.x());
	outVector.push_back(prevMom.y());
	outVector.push_back(prevMom.z());
	outVector.push_back(totalE);
	outVector.push_back(globalTime);
	
	

	if (particleName==sParticle)
	{
		///*	
		//if (prevPos.z()>=dTargetL/2)
		if (prevPhys == capturePhys&&postPhys==targetPhys)
		{  
			//if(trackID!=targetFilteredTrackID&&eventID!=targetFilteredEventID)
			//{
				userRunAction->OutPutData("target",outVector);
	  			eventAction->AddPositron(prevPos,prevMom,totalE);
				targetFilteredTrackID=trackID;
				targetFilteredEventID=eventID;
			//}
			
		}

		else if(prevPhys==acceleratorPhys&&postPhys==capturePhys)
		{
			if((prevPos.x()*prevPos.x()+prevPos.y()*prevPos.y())<=dCaptureR*dCaptureR&&prevPos.z()>=(dTargetL/2+dCaptureL))
			{
 				userRunAction->OutPutData("capture",outVector);
				captureFilteredTrackID=trackID;
				captureFilteredEventID=eventID;
			}
		}

		//*/	
		else if(prevPhys==vacuumPhys&&postPhys==acceleratorPhys)
		{
			if((prevPos.x()*prevPos.x()+prevPos.y()*prevPos.y())<=dAcceleratorR*dAcceleratorR&&prevPos.z()>=(dTargetL/2+dCaptureL+dAcceleratorL))
			{
 				userRunAction->OutPutData("accelerator",outVector);
				acceleratorFilteredTrackID=trackID;
				acceleratorFilteredEventID=eventID;
			}
		}

  	postPhys=prevPhys;

  	}

	else if(particleName=="gamma")
	{
		if (prevPhys == targetPhys&&postPhys==transferPhys)
		{  
			userRunAction->AddActualG(1);
		}

  		postPhys=prevPhys;
	}

	if (prevPhys==targetPhys) 
	{
	  eventAction->AddTargetED(stepE);
	  eventAction->AddTargetStep(stepL);
	}
	/*
	if (prevPhys!=targetPhys&&particleName=="e-") 
	{
		currentTrack->SetTrackStatus(currentTrackStatus);
		if(verbose>=1)
		{
			G4cout<<"Kill a e- because it isn't in target: "<<particleName<<G4endl;
		}
	}

	else if (prevPhys==vacuumPhys) 
	{
		currentTrack->SetTrackStatus(currentTrackStatus);
		if (verbose>=2)
		G4cout<<"Kill a gamma because it move out of tube "<<particleName<<G4endl;
	}

	if (prevPos.z()>dTargetL/2&&particleName!="e+") 
	{
		currentTrack->SetTrackStatus(currentTrackStatus);
		if (verbose>=2)
		{ 
			G4cout<<"Kill a particle because it is not a e+ after target: "<<particleName<<G4endl;
        	return;
		}
	}
    */
}

/*
void GPSteppingAction::WriteToFileDT(G4ThreeVector P,G4ThreeVector M,G4double T, G4double time)
{
	GPRunAction* userRunAction =
	(GPRunAction*)G4RunManager::GetRunManager()->GetUserRunAction();
	userRunAction->dataFileDT
	  <<P.x()<<" "<<P.y()<<" "
	  <<M.x()<<" "<<M.y()<<" "<<M.z()<<" "
	  <<T<<" "<<time<<G4endl;
	
}
void GPSteppingAction::WriteToFileDC(G4ThreeVector P,G4ThreeVector M,G4double T, G4double time)
{
	GPRunAction* userRunAction =
	(GPRunAction*)G4RunManager::GetRunManager()->GetUserRunAction();
	userRunAction->dataFileDC
	  <<P.x()<<" "<<P.y()<<" "
	  <<M.x()<<" "<<M.y()<<" "<<M.z()<<" "
	  <<T<<" "<<time<<G4endl;
	
}
*/

void GPSteppingAction::SetSelectedParticle(G4String tmpParticle)
{
	sParticle=tmpParticle;
        G4cout<<"The filter particle is:"<<sParticle<<G4endl;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
