// $Id: GPEventAction.cc,v 1.29 2008/01/17 17:31:32 maire Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "GPEventAction.hh"
#include "GPTargetHit.hh"

#include "GPRunAction.hh"
#include "GPTrajectory.hh"

//#include "GPEventActionMessenger.hh"

#include "G4Event.hh"
#include "G4TrajectoryContainer.hh"
#include "G4VTrajectory.hh"
#include "G4VVisManager.hh"
#include "G4UnitsTable.hh"
#include "G4EventManager.hh"
#include "G4SDManager.hh"
#include "Randomize.hh"
#include <iomanip>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GPEventAction::GPEventAction(GPRunAction* run)
:runAct(run),iPrintModel(1)//,eventMessenger(0)
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPEventAction::GPEventAction(GPRunAction* )"<<G4endl;
#endif
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPEventAction::GPEventAction(GPRunAction* )"<<G4endl;
#endif
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GPEventAction::~GPEventAction()
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPEventAction::~GPEventAction()"<<G4endl;
#endif
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPEventAction::~GPEventAction()"<<G4endl;
#endif
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GPEventAction::BeginOfEventAction(const G4Event* evt)
{  
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPEventAction::BeginOfEventAction(const G4Event* )"<<G4endl;
#endif
  G4int evtNb = evt->GetEventID();
	iEventID=evtNb;
//  if (evtNb%iPrintModel == 0) 
    if (G4EventManager::GetEventManager()->GetVerboseLevel()>=1)
    { 
    G4cout << "\n------------------> Begin of event: " << evtNb<<" <--------------------" << G4endl;
    CLHEP::HepRandom::showEngineStatus();
    }
 
// initialisation per event
 dEnergyTar = 0.;
 dTrackL = 0.;
 iNPositronPerEvt=0;
// de=0.;
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPEventAction::BeginOfEventAction(const G4Event* )"<<G4endl;
#endif
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GPEventAction::EndOfEventAction(const G4Event* evt)
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPEventAction::EndOfEventAction(const G4Event* )"<<G4endl;
#endif
  //accumulates statistic
  //
  runAct->FillPerEvent(dEnergyTar, dTrackL,iNPositronPerEvt);
 // PEDD
  G4HCofThisEvent* HCE = evt->GetHCofThisEvent();
  G4SDManager* SDM=G4SDManager::GetSDMpointer();
  GPTargetHitsCollection* EddCollection;
  GPTargetHit* PEDDHit;
  G4int CollectionID=SDM->GetCollectionID("EddCollection"); 
  EddCollection =static_cast<GPTargetHitsCollection*>(HCE->GetHC(CollectionID));

  if(EddCollection)
  {
    size_t numHit=EddCollection->GetSize(); 
    for(size_t i=0;i!=numHit;i++)
    {
       PEDDHit=static_cast<GPTargetHit*>(EddCollection->GetHit(i));
       runAct->AddEddHit(PEDDHit->GetX(),PEDDHit->GetY(),PEDDHit->GetZ(),PEDDHit->GetEdep());
    }
  }
  //print per event (modulo n)
  //
  G4int evtNb = evt->GetEventID();
//  if (evtNb%iPrintModel == 0) 
    if (G4EventManager::GetEventManager()->GetVerboseLevel()>=1)
    {
    G4cout << "number of positron in this Event:"<<iNPositronPerEvt<<"\n"
           << "Target: total energy: " << std::setw(7)<< G4BestUnit(dEnergyTar,"Energy")<<"\n"
           << "total track length: " << std::setw(7)<< G4BestUnit(dTrackL,"Length")
           << G4endl;
    G4cout << "---------------> End of event: " << evtNb << " <--------------"<<G4endl;	

	  
    }

  //get number of stored trajectories
  G4TrajectoryContainer* trajectoryContainer = evt->GetTrajectoryContainer();
  G4int n_trajectories = 0;
  if (trajectoryContainer) n_trajectories = trajectoryContainer->entries();
  //extract the trajectories and print them out
  //G4cout << G4endl;
  //G4cout << "Trajectories in tracker --------------------------------------------------------------" << G4endl;
  for(G4int i=0; i<n_trajectories; i++) 
  {
  GPTrajectory* trj = (GPTrajectory*)((*trajectoryContainer)[i]);
  //trj->ShowTrajectory();
  //trj->DrawTrajectory();
  trj->ProcessTrajectory(1);
  }
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPEventAction::EndOfEventAction(const G4Event* )"<<G4endl;
#endif
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  void GPEventAction::AddPositron(G4ThreeVector Position,G4ThreeVector MomentumDirection,G4double TotalEnergy)
  {
   iNPositronPerEvt++;
if (G4EventManager::GetEventManager()->GetVerboseLevel()>=2)   G4cout << "positron num:" << iNPositronPerEvt<<", position: "<<Position <<"m, direction: "<<MomentumDirection<<", energy: "<<TotalEnergy<<" MeV"<<G4endl;
   
  }
