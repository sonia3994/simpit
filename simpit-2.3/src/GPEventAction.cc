// $Id: GPEventAction.cc,v 1.29 2008/01/17 17:31:32 maire Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "GPEventAction.hh"

#include "GPRunAction.hh"
#include "GPTrajectory.hh"
#include "GPTrajectoryAction.hh"

#include "GPEventActionMessenger.hh"

#include "GPEventHandleManager.hh"

#include "G4RunManager.hh"
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

GPEventAction::GPEventAction()
:iPrintModel(1)
{
  eventMessenger = new GPEventActionMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GPEventAction::~GPEventAction()
{
  if(eventMessenger)    delete eventMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GPEventAction::BeginOfEventAction(const G4Event* evt)
{  
  GPEventHandleManager::GetInstance()->BeginOfEventAction(evt);
//  G4int evtNb = evt->GetEventID();
//	iEventID=evtNb;
////  if (evtNb%iPrintModel == 0) 
//  if (G4EventManager::GetEventManager()->GetVerboseLevel()>=1)
//  { 
//    G4cout << "\n------------------> Begin of event: " << evtNb<<" <--------------------" << G4endl;
//    CLHEP::HepRandom::showEngineStatus();
//  }
// 
//// initialisation per event
// dEnergyTar = 0.;
// dTrackL = 0.;
// iNPositronPerEvt=0;
//// de=0.;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GPEventAction::EndOfEventAction(const G4Event* evt)
{
  //accumulates statistic
  //
  GPEventHandleManager::GetInstance()->EndOfEventAction(evt);
  G4int evtNb = evt->GetEventID();
//  GPRunAction* runAct = (GPRunAction*)G4RunManager::GetRunManager()->GetUserRunAction(); 
//  runAct->FillPerEvent(dEnergyTar, dTrackL,iNPositronPerEvt);
// // PEDD
//  G4HCofThisEvent* HCE = evt->GetHCofThisEvent();
//  G4SDManager* SDM=G4SDManager::GetSDMpointer();
//
//  GPTargetHitsCollection* EddCollection;
//  GPParticleHitsCollection* particleHitsCollection;
//
//  G4int CollectionID;
//  /*
//  CollectionID=SDM->GetCollectionID("/f_target_sd/TargetSD"); 
//  EddCollection =static_cast<GPTargetHitsCollection*>(HCE->GetHC(CollectionID));
//  if(EddCollection)
//    ProcessEdd(EddCollection);
//  */
//  
//  CollectionID=SDM->GetCollectionID("TargetParticleScorerZPlus"); 
//  particleHitsCollection =static_cast<GPParticleHitsCollection*>(HCE->GetHC(CollectionID));
//  if(particleHitsCollection)
//    ProcessParticleHits(particleHitsCollection,"target");
//
//  ///*
//  CollectionID=SDM->GetCollectionID("CaptureParticleScorerZPlus"); 
//  particleHitsCollection =static_cast<GPParticleHitsCollection*>(HCE->GetHC(CollectionID));
//  if(particleHitsCollection)
//    ProcessParticleHits(particleHitsCollection,"capture");
//
//  CollectionID=SDM->GetCollectionID("AcceleratorParticleScorerZPlus"); 
//  particleHitsCollection =static_cast<GPParticleHitsCollection*>(HCE->GetHC(CollectionID));
//  if(particleHitsCollection)
//    ProcessParticleHits(particleHitsCollection,"accelerator");
//   //*/
//
//  //print per event (modulo n)
//  //
////  if (evtNb%iPrintModel == 0) 
//    if (G4EventManager::GetEventManager()->GetVerboseLevel()>=1)
//    {
//    G4cout << "number of positron in this Event:"<<iNPositronPerEvt<<"\n"
//           << "Target: total energy: " << std::setw(7)<< G4BestUnit(dEnergyTar,"Energy")<<"\n"
//           << "total track length: " << std::setw(7)<< G4BestUnit(dTrackL,"Length")
//           << G4endl;
//    G4cout << "---------------> End of event: " << evtNb << " <--------------"<<G4endl;	
//
//	  
//    }
//
  //get number of stored trajectories
  G4TrajectoryContainer* trajectoryContainer = evt->GetTrajectoryContainer();
  if (trajectoryContainer) 
  {
    if(iTrajectoryFlag==-1)
    GPTrajectoryAction::GetGPTrajectoryAction()->ProcessTrajectory(trajectoryContainer);
    else if(evtNb<iTrajectoryFlag)
    GPTrajectoryAction::GetGPTrajectoryAction()->ProcessTrajectory(trajectoryContainer);
  }
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void GPEventAction::AddPositron(G4ThreeVector Position,G4ThreeVector MomentumDirection,G4double TotalEnergy)
{
   iNPositronPerEvt++;
if (G4EventManager::GetEventManager()->GetVerboseLevel()>=2)   G4cout << "positron num:" << iNPositronPerEvt<<", position: "<<Position <<"m, direction: "<<MomentumDirection<<", energy: "<<TotalEnergy<<" MeV"<<G4endl;
   
}

void GPEventAction::ProcessParticleHits(GPParticleHitsCollection* particleHitsCollection,G4String sVolume)
{
  GPRunAction* runAct = (GPRunAction*)G4RunManager::GetRunManager()->GetUserRunAction(); 
  GPParticleHit* particleHit;
  
  static std::vector<G4double>   vecTrackInf;
  static G4ThreeVector  vecPos;
  static G4ThreeVector  vecMom;
  static G4String       particleName;
  static G4double	totalE; 
  static G4double	globalTime; 
  static G4int iTrackID=0;
  static G4int iLastTrackID=-1;
  static G4int iNumElectron=0;
  static G4int iNumPositron=0;
  static size_t numHit; 

  iNumPositron=0;
  iNumElectron=0;
  numHit=particleHitsCollection->GetSize(); 
  for(size_t i=0;i!=numHit;i++)
  {
    particleHit=static_cast<GPParticleHit*>(particleHitsCollection->GetHit(i));
    iTrackID = particleHit->GetTrackID();
    if(iTrackID==iLastTrackID)	
    {
      continue;
    }
    iLastTrackID=iTrackID;
    particleName = particleHit->GetParticleName();
    if(particleName=="e-")
    {
      iNumElectron++;
    }
    //else if(particleName=="e+")
    //{
      iNumPositron++;
      vecPos = particleHit->GetPosition();
      vecMom = particleHit->GetMomentum();
      totalE = particleHit->GetTotalEnergy(); 
      globalTime = particleHit->GetGlobalTime(); 

      vecTrackInf.clear();
      vecTrackInf.push_back(particleHit->GetPDG());
      vecTrackInf.push_back(iEventID);
      vecTrackInf.push_back(iTrackID);
      vecTrackInf.push_back(vecPos.x()*m/mm);
      vecTrackInf.push_back(vecPos.y()*m/mm);
      //vecTrackInf.push_back(vecPos.z()*m/mm);
      vecTrackInf.push_back(vecMom.x());
      vecTrackInf.push_back(vecMom.y());
      vecTrackInf.push_back(vecMom.z());
      vecTrackInf.push_back(totalE);
      vecTrackInf.push_back(globalTime*second/picosecond);

      runAct->OutPutData(sVolume,vecTrackInf);
    //}
  }
  runAct->AddElectronNumber(sVolume,iNumElectron);
  runAct->AddPositronNumber(sVolume,iNumPositron);
    
}

void GPEventAction::ProcessEdd(GPTargetHitsCollection* EddCollection)
{
  GPRunAction* runAct = (GPRunAction*)G4RunManager::GetRunManager()->GetUserRunAction(); 
  GPTargetHit* PEDDHit;
  size_t numHit=EddCollection->GetSize(); 
  for(size_t i=0;i!=numHit;i++)
  {
     PEDDHit=static_cast<GPTargetHit*>(EddCollection->GetHit(i));
     runAct->AddEddHit(PEDDHit->GetX(),PEDDHit->GetY(),PEDDHit->GetZ(),PEDDHit->GetEdep());
  }
}
