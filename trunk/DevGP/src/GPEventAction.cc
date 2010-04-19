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
// $Id: GPEventAction.cc,v 1.29 2008/01/17 17:31:32 maire Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "GPEventAction.hh"
#include "GPTargetHit.hh"

#include "GPRunAction.hh"
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
:runAct(run),printModulo(1)//,eventMessenger(0)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GPEventAction::~GPEventAction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GPEventAction::BeginOfEventAction(const G4Event* evt)
{  
  G4int evtNb = evt->GetEventID();
//  if (evtNb%printModulo == 0) 
    if (G4EventManager::GetEventManager()->GetVerboseLevel()>=0)
    { 
    G4cout << "\n---> Begin of event: " << evtNb << G4endl;
    CLHEP::HepRandom::showEngineStatus();
    }
 
// initialisation per event
 EnergyTar = 0.;
 TrackL = 0.;
 positronPerEvt=0;
// de=0.;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GPEventAction::EndOfEventAction(const G4Event* evt)
{
  //accumulates statistic
  //
  runAct->FillPerEvent(EnergyTar, TrackL,positronPerEvt);
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
//  if (evtNb%printModulo == 0) 
    if (G4EventManager::GetEventManager()->GetVerboseLevel()>=1)
    {
    G4cout << "\n --------> End of event: " << evtNb << "  <--------"<<G4endl;	
    G4cout << "\n number of positron in this Event:"<<positronPerEvt<<"\n"
           << "\n Target: total energy: " << std::setw(7)<< G4BestUnit(EnergyTar,"Energy")<<"\n"
           << "\n total track length: " << std::setw(7)<< G4BestUnit(TrackL,"Length")
           << G4endl;

	  
    }
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  void GPEventAction::AddTargetED(G4double de) {EnergyTar+=de;}
  void GPEventAction::AddTargetStep(G4double TargetStep) {TrackL+=TargetStep;} 
  void GPEventAction::AddPositron(G4ThreeVector Position,G4ThreeVector MomentumDirection,G4double TotalEnergy)
  {
   positronPerEvt++;
if (G4EventManager::GetEventManager()->GetVerboseLevel()>=2)   G4cout << "positron" << positronPerEvt<<Position << MomentumDirection<<TotalEnergy<<G4endl;
   
  }
