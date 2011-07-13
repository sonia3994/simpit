// $Id: GPSteppingAction.cc,v 1.15 2006/06/29 17:49:13 gunter Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "GPSteppingAction.hh"
#include "GPSteppingMessenger.hh"
#include "GPSteppingHandleManager.hh"

#include "G4ThreeVector.hh"
#include "G4RunManager.hh"
#include "G4Step.hh"

//#include <fstream>
#include <sstream>
using namespace std;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GPSteppingAction::GPSteppingAction()
{ 
  sParticle="e+";
  steppingMessenger = new GPSteppingMessenger(this);
  verbose=0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GPSteppingAction::~GPSteppingAction()
{ 
  delete 	steppingMessenger;
  //delete	targetSteppingAction;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GPSteppingAction::Prepare()
{
	GPSteppingHandleManager::GetInstance()->Prepare();
  //targetSteppingAction->Prepare();
}

void GPSteppingAction::CleanUp()
{
	GPSteppingHandleManager::GetInstance()->CleanUp();
  //targetSteppingAction->CleanUp();
}
void GPSteppingAction::UserSteppingAction(const G4Step* aStep)
{
  GPSteppingHandleManager::GetInstance()->UserSteppingAction(aStep);

  static G4VPhysicalVolume* 	prevPhys;
  static G4Track*		currentTrack;
  static G4TrackStatus		currentTrackStatus;
  static G4double		stepE;
  static G4double		stepL;
  static G4String 		particleName;
  static G4int   		stopFlag;

  prevPhys= aStep->GetPreStepPoint()->GetPhysicalVolume();
  currentTrack=aStep->GetTrack();
  currentTrackStatus=fStopAndKill;
  stepE=aStep->GetTotalEnergyDeposit();

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

}

void GPSteppingAction::SetSelectedParticle(G4String tmpParticle)
{
  sParticle=tmpParticle;
  G4cout<<"The filter particle is:"<<sParticle<<G4endl;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
