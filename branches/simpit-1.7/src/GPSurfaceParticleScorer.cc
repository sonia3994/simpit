// $Id: GPSurfaceParticleScorer.cc,v 1.3 2008/12/29 00:17:14 asaim Exp $
// GEANT4 tag $Name: geant4-09-03 $
//
// GPSurfaceParticleScorer
#include "GPSurfaceParticleScorer.hh"

#include "G4StepStatus.hh"
#include "G4Track.hh"
#include "G4VSolid.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VPVParameterisation.hh"
#include "G4UnitsTable.hh"
#include "G4GeometryTolerance.hh"


GPSurfaceParticleScorer::GPSurfaceParticleScorer(G4String name,G4int surface, G4int direction, G4int depth)
    :G4VPrimitiveScorer(name,depth)
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPSurfaceParticleScorer::GPSurfaceParticleScorer(G4String, G4int, G4int, G4int)"<<G4endl;
#endif
  HCID=-1;
  iSelectedSurface=surface;
  iSelectedDirection=direction;
  iSurface=-1;
  iDirection=-1;
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPSurfaceParticleScorer::GPSurfaceParticleScorer(G4String, G4int, G4int, G4int)"<<G4endl;
#endif
}

GPSurfaceParticleScorer::~GPSurfaceParticleScorer()
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPSurfaceParticleScorer::~GPSurfaceParticleScorer()"<<G4endl;
#endif
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPSurfaceParticleScorer::~GPSurfaceParticleScorer()"<<G4endl;
#endif
}

G4bool GPSurfaceParticleScorer::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  //hitsCollection = (GPParticleHitsCollection*)(hitCE->GetHC(HCID));
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPSurfaceParticleScorer::ProcessHits(G4Step*,G4TouchableHistory*)"<<G4endl;
  //G4cout<<"GP_DEBUG: GPSurfaceParticleScorer::hitsCollection="<<hitsCollection<<G4endl;
  //G4cout<<"GP_DEBUG: GPSurfaceParticleScorer::hitsCollection::size="<<hitsCollection->GetSize()<<G4endl;
#endif
  G4StepPoint* preStep = aStep->GetPreStepPoint();
  G4VPhysicalVolume* physVol = preStep->GetPhysicalVolume();
  G4VPVParameterisation* physParam = physVol->GetParameterisation();
  G4VSolid * solid = 0;
  if(physParam)
  { // for parameterized volume
    G4int idx = ((G4TouchableHistory*)(aStep->GetPreStepPoint()->GetTouchable()))
                ->GetReplicaNumber(indexDepth);
    solid = physParam->ComputeSolid(idx, physVol);
    solid->ComputeDimensions(physParam,idx,physVol);
  }
  else
  { // for ordinary volume
    solid = physVol->GetLogicalVolume()->GetSolid();
  }

  CheckSurfaceAndDirection(aStep,solid);

  if(iSurface==iSelectedSurface)
  {
    if(iSelectedDirection==iDirection)
    {
      GPParticleHit* particleHit = new GPParticleHit(aStep->GetTrack());
      hitsCollection->insert(particleHit);
    }
    else if(iSelectedDirection==fCurrent_InOut&&(iDirection==fCurrent_Out||iDirection==fCurrent_In))
    {
      GPParticleHit* particleHit = new GPParticleHit(aStep->GetTrack());
      hitsCollection->insert(particleHit);
    }
  }
  iSurface=-1;
  iDirection=-1;

#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPSurfaceParticleScorer::ProcessHits(G4Step*,G4TouchableHistory*)"<<G4endl;
#endif
  return true;
}

void GPSurfaceParticleScorer::CheckSurfaceAndDirection(G4Step* aStep, G4VSolid* solid)
{
  if(solid->GetEntityType()=="G4Box")
  {
    //G4cout<<"current solid is G4Box."<<G4endl;
    G4Box* boxSolid = (G4Box*)(solid);
    CheckForBox(aStep,boxSolid);
  }
  else if (solid->GetEntityType()=="G4Tubs")
  {
    //G4cout<<"current solid is G4Tubs."<<G4endl;
    G4Tubs* tubeSolid = (G4Tubs*)(solid);
    CheckForTube(aStep,tubeSolid);
  }
}

void GPSurfaceParticleScorer::CheckForBox(G4Step* aStep, G4Box* boxSolid)
{
  G4TouchableHandle theTouchable = 
    aStep->GetPreStepPoint()->GetTouchableHandle();
  G4double kCarTolerance = G4GeometryTolerance::GetInstance()->GetSurfaceTolerance();

  if (aStep->GetPreStepPoint()->GetStepStatus() == fGeomBoundary )
  {
    // Entering Geometry
    G4ThreeVector stppos1= aStep->GetPreStepPoint()->GetPosition();
    G4ThreeVector localpos1 = 
      theTouchable->GetHistory()->GetTopTransform().TransformPoint(stppos1);
    if(std::fabs( localpos1.z() - boxSolid->GetZHalfLength())<kCarTolerance )
    {
      iDirection = fCurrent_In;
      iSurface   = 1;
    }
  }

  if (aStep->GetPostStepPoint()->GetStepStatus() == fGeomBoundary )
  {
    // Exiting Geometry
    G4ThreeVector stppos2= aStep->GetPostStepPoint()->GetPosition();
    G4ThreeVector localpos2 = 
      theTouchable->GetHistory()->GetTopTransform().TransformPoint(stppos2);
    if(std::fabs( localpos2.z() - boxSolid->GetZHalfLength())<kCarTolerance )
    {
      iDirection = fCurrent_Out;
      iSurface   = 1;
    }
  }

}

void GPSurfaceParticleScorer::CheckForTube(G4Step* aStep, G4Tubs* tubeSolid)
{
  G4TouchableHandle theTouchable = 
    aStep->GetPreStepPoint()->GetTouchableHandle();
  G4double kCarTolerance = G4GeometryTolerance::GetInstance()->GetSurfaceTolerance();

  if (aStep->GetPreStepPoint()->GetStepStatus() == fGeomBoundary )
  {
    // Entering Geometry
    G4ThreeVector stppos1= aStep->GetPreStepPoint()->GetPosition();
    G4ThreeVector localpos1 = 
      theTouchable->GetHistory()->GetTopTransform().TransformPoint(stppos1);
    if(std::fabs( localpos1.z() - tubeSolid->GetZHalfLength())<kCarTolerance )
    {
      iDirection = fCurrent_In;
      iSurface   = 1;
    }
  }

  if (aStep->GetPostStepPoint()->GetStepStatus() == fGeomBoundary )
  {
    // Exiting Geometry
    G4ThreeVector stppos2= aStep->GetPostStepPoint()->GetPosition();
    G4ThreeVector localpos2 = 
      theTouchable->GetHistory()->GetTopTransform().TransformPoint(stppos2);
    if(std::fabs( localpos2.z() - tubeSolid->GetZHalfLength())<kCarTolerance )
    {
      iDirection = fCurrent_Out;
      iSurface   = 1;
    }
  }

}


void GPSurfaceParticleScorer::Initialize(G4HCofThisEvent* HCE)
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPSurfaceParticleScorer::Initialize(G4HCofThisEvent*)"<<G4endl;
#endif
  hitsCollection = new GPParticleHitsCollection(detector->GetName(), GetName());
  if ( HCID < 0 ) {HCID = GetCollectionID(0);}
  HCE->AddHitsCollection(HCID,hitsCollection);
  //hitCE=HCE;
#ifdef GP_DEBUG
  //G4cout<<"GP_DEBUG: GPSurfaceParticleScorer::hitsCollection="<<hitsCollection<<G4endl;
  //G4cout<<"GP_DEBUG: GPSurfaceParticleScorer::hitsCollection::size="<<hitsCollection->GetSize()<<G4endl;
  G4cout<<"GP_DEBUG: Exit GPSurfaceParticleScorer::Initialize(G4HCofThisEvent*)"<<G4endl;
#endif
}

void GPSurfaceParticleScorer::EndOfEvent(G4HCofThisEvent* HCE)
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPSurfaceParticleScorer::EndOfEvent(G4HCofThisEvent* HCE)"<<G4endl;
#endif
#ifdef GP_DEBUG
  //G4cout<<"GP_DEBUG: GPSurfaceParticleScorer::hitsCollection="<<hitsCollection<<G4endl;
  //G4cout<<"GP_DEBUG: GPSurfaceParticleScorer::hitsCollection::size="<<hitsCollection->GetSize()<<G4endl;
  G4cout<<"GP_DEBUG: Exit GPSurfaceParticleScorer::EndOfEvent(G4HCofThisEvent* HCE)"<<G4endl;
#endif
}

void GPSurfaceParticleScorer::clear()
{}

void GPSurfaceParticleScorer::DrawAll()
{}

void GPSurfaceParticleScorer::PrintAll()
{}

