
#include "GPTargetHit.hh"
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

G4Allocator<GPTargetHit> GPTargetHitAllocator;

GPTargetHit::GPTargetHit()
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPTargetHit::GPTargetHit()"<<G4endl;
#endif
pLogV=0;
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPTargetHit::GPTargetHit()"<<G4endl;
#endif
}

GPTargetHit::GPTargetHit(G4LogicalVolume* logVol,G4int x,G4int y,G4int z)
:iXCellID(x),iYCellID(y), iZCellID(z), pLogV(logVol)
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPTargetHit::GPTargetHit(G4LogicalVolume*,G4int,G4int,G4int)"<<G4endl;
#endif
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPTargetHit::GPTargetHit(G4LogicalVolume*,G4int,G4int,G4int)"<<G4endl;
#endif
}

GPTargetHit::~GPTargetHit()
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPTargetHit::~GPTargetHit()"<<G4endl;
#endif
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPTargetHit::~GPTargetHit()"<<G4endl;
#endif
}

GPTargetHit::GPTargetHit(const GPTargetHit &right)
  : G4VHit()
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPTargetHit::GPTargetHit(const GPTargetHit&)"<<G4endl;
#endif
  iXCellID = right.iXCellID;
  iYCellID = right.iYCellID;
  iZCellID = right.iZCellID;
  dEnergyDep = right.dEnergyDep;
  vecPos = right.vecPos;
  rot = right.rot;
  pLogV = right.pLogV;
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPTargetHit::~GPTargetHit(const GPTargetHit&)"<<G4endl;
#endif
}

const GPTargetHit& GPTargetHit::operator=(const GPTargetHit &right)
{
  iXCellID = right.iXCellID;
  iYCellID = right.iYCellID;
  iZCellID = right.iZCellID;
  dEnergyDep = right.dEnergyDep;
  vecPos = right.vecPos;
  rot = right.rot;
  pLogV = right.pLogV;
  return *this;
}

G4int GPTargetHit::operator==(const GPTargetHit &right) const
{
  return ((iXCellID==right.iXCellID)&&(iYCellID==right.iYCellID)&&(iZCellID==right.iZCellID));
}

std::map<G4String,G4AttDef> GPTargetHit::mapStrAttDef;

void GPTargetHit::Draw()
{
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager)
  {
    G4Transform3D trans(rot,vecPos);
    G4VisAttributes attribs;
    const G4VisAttributes* pVA = pLogV->GetVisAttributes();
    if(pVA) attribs = *pVA;
    G4Colour colour(1.,0.,0.);
    attribs.SetColour(colour);
    attribs.SetForceSolid(true);
    pVVisManager->Draw(*pLogV,attribs,trans);
  }
}

const std::map<G4String,G4AttDef>* GPTargetHit::GetAttDefs() const
{
  // G4AttDefs have to have long life.  Use static member...
  if (mapStrAttDef.empty()) {
    mapStrAttDef["HitType"] = G4AttDef("HitType","Type of hit","Physics","","G4String");
    mapStrAttDef["XID"] = G4AttDef("XID","X Cell ID","Physics","","G4int");
    mapStrAttDef["YID"] = G4AttDef("YID","Y Cell ID","Physics","","G4int");
    mapStrAttDef["ZID"] = G4AttDef("ZID","Z Cell ID","Physics","","G4int");
    mapStrAttDef["dEnergyDep"] = G4AttDef("dEnergyDep","Energy deposited","Physics","G4BestUnit","G4double");
  }
  return &mapStrAttDef;
}

std::vector<G4AttValue>* GPTargetHit::CreateAttValues() const
{
  // Create expendable G4AttsValues for picking...
  std::vector<G4AttValue>* attValues = new std::vector<G4AttValue>;
  attValues->push_back (G4AttValue("HitType","GPTargetHit",""));
  attValues->push_back (G4AttValue("XID",G4UIcommand::ConvertToString(iXCellID),""));
  attValues->push_back (G4AttValue("YID",G4UIcommand::ConvertToString(iYCellID),""));
  attValues->push_back (G4AttValue("ZID",G4UIcommand::ConvertToString(iZCellID),""));
  attValues->push_back (G4AttValue("dEnergyDep",G4BestUnit(dEnergyDep,"Energy"),""));
  //G4cout << "Checking...\n" << G4AttCheck(attValues, GetAttDefs());
  return attValues;
}

void GPTargetHit::Print()
{;}


