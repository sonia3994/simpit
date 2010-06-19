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

#include "GPTargetSD.hh"
#include "GPTargetHit.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4ParticleDefinition.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4ios.hh"

GPTargetSD::GPTargetSD(G4String name,std::vector<G4int> hitDim)
:G4VSensitiveDetector(name),
 numberOfCellsInX(hitDim[0]),numberOfCellsInY(hitDim[1]),numberOfCellsInZ(hitDim[2])
{
  G4String HCname;
  collectionName.insert(HCname="EddCollection");
}

GPTargetSD::~GPTargetSD()
{;}

void GPTargetSD::Initialize(G4HCofThisEvent* HCE)
{
  CellID.clear();
  CellID.resize(numberOfCellsInX*numberOfCellsInY*numberOfCellsInZ); 
  EddCollection = new GPTargetHitsCollection
                      (SensitiveDetectorName,collectionName[0]); 
  G4int size_cell=numberOfCellsInX*numberOfCellsInY*numberOfCellsInZ;
  for(G4int i=0;i<size_cell;i++)
     {CellID[i] = -1;}
  verboseLevel = 0;
}

G4bool GPTargetSD::ProcessHits(G4Step* aStep,G4TouchableHistory* ROhist)
{
  if(!ROhist) return false;
  G4double edep = aStep->GetTotalEnergyDeposit();
  if(verboseLevel>1) G4cout << "Next step edep(MeV) = " << edep/MeV << G4endl;
  if(edep==0.) return false;

  G4VPhysicalVolume* physVol = ROhist->GetVolume();
  //if(physVol->GetName()!="")
  //ROhist->MoveUpHistory();
  //G4VPhysicalVolume* mothVol = ROhist->GetVolume(1);
  G4int copyIDinX = ROhist->GetReplicaNumber(2);
  G4int copyIDinY = ROhist->GetReplicaNumber(1);
  G4int copyIDinZ = ROhist->GetReplicaNumber(0);
  G4int cellIndex=copyIDinZ*numberOfCellsInX*numberOfCellsInY+copyIDinY*numberOfCellsInX+copyIDinX;

  if (CellID[cellIndex]==-1)
  {
    GPTargetHit* calHit = new GPTargetHit (physVol->GetLogicalVolume(),copyIDinX,copyIDinY,copyIDinZ);
    calHit->SetEdep( edep );
    //G4AffineTransform aTrans = ROhist->GetHistory()->GetTopTransform();
    //aTrans.Invert();
    //calHit->SetPos(aTrans.NetTranslation());
    //calHit->SetRot(aTrans.NetRotation());
    G4int icell = EddCollection->insert( calHit );
    CellID[cellIndex]= icell - 1;
    if(verboseLevel>1)
    { G4cout << " New Calorimeter Hit on CellID " << copyIDinX << " " << copyIDinY <<" "<<copyIDinZ<< G4endl; }
  }
  else
  { 
    (*EddCollection)[CellID[cellIndex]]->AddEdep(edep);
    if(verboseLevel>1)
    { G4cout << " Energy added to CellID " << copyIDinX << " " << copyIDinY <<" "<<copyIDinZ<< G4endl; }
  }

  return true;
}

void GPTargetSD::EndOfEvent(G4HCofThisEvent* HCE)
{
  static G4int HCID = -1;
  if(HCID<0)
  { HCID = GetCollectionID(0); }
  HCE->AddHitsCollection( HCID, EddCollection );
}

void GPTargetSD::clear()
{
} 

void GPTargetSD::DrawAll()
{
} 

void GPTargetSD::PrintAll()
{
} 

