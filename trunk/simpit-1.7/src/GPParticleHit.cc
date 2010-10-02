
#include "GPParticleHit.hh"
#include "G4ios.hh"
#include "G4VVisManager.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalVolume.hh"
#include "G4UIcommand.hh"
#include "G4UnitsTable.hh"
#include "G4AttValue.hh"
#include "G4AttDef.hh"
#include "G4AttCheck.hh"

G4Allocator<GPParticleHit> GPParticleHitAllocator;

GPParticleHit::GPParticleHit()
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPParticleHit::GPParticleHit()"<<G4endl;
#endif
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPParticleHit::GPParticleHit()"<<G4endl;
#endif
}

GPParticleHit::GPParticleHit(G4Track* track)
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPParticleHit::GPParticleHit(G4Track* track)"<<G4endl;
#endif
  trackHit = new G4Track((*track));
  trackHit->SetTrackID(track->GetTrackID());
  trackHit->SetParentID(track->GetParentID());
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPParticleHit::GPParticleHit(G4Track* track)"<<G4endl;
#endif
}

GPParticleHit::~GPParticleHit()
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPParticleHit::~GPParticleHit()"<<G4endl;
#endif
  delete trackHit;
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPParticleHit::~GPParticleHit()"<<G4endl;
#endif
}

GPParticleHit::GPParticleHit(const GPParticleHit &right)
  : G4VHit()
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPParticleHit::GPParticleHit(const GPParticleHit&)"<<G4endl;
#endif
  trackHit = new G4Track(*(right.trackHit));
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPParticleHit::~GPParticleHit(const GPParticleHit&)"<<G4endl;
#endif
}

const GPParticleHit& GPParticleHit::operator=(const GPParticleHit &right)
{
  trackHit = new G4Track(*(right.trackHit));
  return *this;
}


std::map<G4String,G4AttDef> GPParticleHit::mapStrAttDef;

void GPParticleHit::Draw()
{
}

const std::map<G4String,G4AttDef>* GPParticleHit::GetAttDefs() const
{
  // G4AttDefs have to have long life.  Use static member...
  if (mapStrAttDef.empty()) {
    mapStrAttDef["HitType"] = G4AttDef("HitType","Type of hit","Physics","","G4String");
  }
  return &mapStrAttDef;
}

std::vector<G4AttValue>* GPParticleHit::CreateAttValues() const
{
  // Create expendable G4AttsValues for picking...
  std::vector<G4AttValue>* attValues = new std::vector<G4AttValue>;
  attValues->push_back (G4AttValue("HitType","GPParticleHit",""));
  return attValues;
}

void GPParticleHit::Print()
{;}

