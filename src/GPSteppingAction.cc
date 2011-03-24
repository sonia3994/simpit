// $Id: GPSteppingAction.cc,v 1.15 2006/06/29 17:49:13 gunter Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "GPSteppingAction.hh"
#include "GPSteppingMessenger.hh"

#include "GPDetectorConstruction.hh"
#include "GPTargetSteppingAction.hh"
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
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPSteppingAction::GPSteppingAction(GPDetectorConstruction*)"<<G4endl;
#endif
  sParticle="e+";
  steppingMessenger = new GPSteppingMessenger(this);
  targetSteppingAction = new GPTargetSteppingAction();
  verbose=0;
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPSteppingAction::GPSteppingAction(GPDetectorConstruction*)"<<G4endl;
#endif
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GPSteppingAction::~GPSteppingAction()
{ 
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPSteppingAction::~GPSteppingAction()"<<G4endl;
#endif
  delete 	steppingMessenger;
  delete	targetSteppingAction;
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPSteppingAction::~GPSteppingAction()"<<G4endl;
#endif
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GPSteppingAction::Init()
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPSteppingAction::Init()"<<G4endl;
#endif
  targetSteppingAction->Init();
  targetPhys = detector->GetPhysicalVolume("target"); 
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPSteppingAction::Init()"<<G4endl;
#endif
}

void GPSteppingAction::CleanUp()
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPSteppingAction::CleanUp()"<<G4endl;
#endif
  targetSteppingAction->CleanUp();
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPSteppingAction::CleanUp()"<<G4endl;
#endif
}
void GPSteppingAction::UserSteppingAction(const G4Step* aStep)
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPSteppingAction::UserSteppingAction(const G4Step*)"<<G4endl;
#endif
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

  if (prevPhys==targetPhys) 
  {
    eventAction->AddTargetED(stepE);
    eventAction->AddTargetStep(stepL);
  }
  targetSteppingAction->UserSteppingAction(aStep);
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPSteppingAction:::UserSteppingAction(const G4Step*)"<<G4endl;
#endif
}

void GPSteppingAction::SetSelectedParticle(G4String tmpParticle)
{
  sParticle=tmpParticle;
  G4cout<<"The filter particle is:"<<sParticle<<G4endl;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
