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
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GPEventAction::EndOfEventAction(const G4Event* evt)
{
  //accumulates statistic
  //
  GPEventHandleManager::GetInstance()->EndOfEventAction(evt);
  G4int evtNb = evt->GetEventID();

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

