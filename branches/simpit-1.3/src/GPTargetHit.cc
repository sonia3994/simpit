
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
{pLogV=0;}

GPTargetHit::GPTargetHit(G4LogicalVolume* logVol,G4int x,G4int y,G4int z)
:XCellID(x),YCellID(y), ZCellID(z), pLogV(logVol)
{;}

GPTargetHit::~GPTargetHit()
{;}

GPTargetHit::GPTargetHit(const GPTargetHit &right)
  : G4VHit()
{
  XCellID = right.XCellID;
  YCellID = right.YCellID;
  ZCellID = right.ZCellID;
  edep = right.edep;
  pos = right.pos;
  rot = right.rot;
  pLogV = right.pLogV;
}

const GPTargetHit& GPTargetHit::operator=(const GPTargetHit &right)
{
  XCellID = right.XCellID;
  YCellID = right.YCellID;
  ZCellID = right.ZCellID;
  edep = right.edep;
  pos = right.pos;
  rot = right.rot;
  pLogV = right.pLogV;
  return *this;
}

G4int GPTargetHit::operator==(const GPTargetHit &right) const
{
  return ((XCellID==right.XCellID)&&(YCellID==right.YCellID)&&(ZCellID==right.ZCellID));
}

std::map<G4String,G4AttDef> GPTargetHit::fAttDefs;

void GPTargetHit::Draw()
{
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager)
  {
    G4Transform3D trans(rot,pos);
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
  if (fAttDefs.empty()) {
    fAttDefs["HitType"] = G4AttDef("HitType","Type of hit","Physics","","G4String");
    fAttDefs["XID"] = G4AttDef("XID","X Cell ID","Physics","","G4int");
    fAttDefs["YID"] = G4AttDef("YID","Y Cell ID","Physics","","G4int");
    fAttDefs["ZID"] = G4AttDef("ZID","Z Cell ID","Physics","","G4int");
    fAttDefs["EDep"] = G4AttDef("EDep","Energy deposited","Physics","G4BestUnit","G4double");
  }
  return &fAttDefs;
}

std::vector<G4AttValue>* GPTargetHit::CreateAttValues() const
{
  // Create expendable G4AttsValues for picking...
  std::vector<G4AttValue>* attValues = new std::vector<G4AttValue>;
  attValues->push_back (G4AttValue("HitType","GPTargetHit",""));
  attValues->push_back (G4AttValue("XID",G4UIcommand::ConvertToString(XCellID),""));
  attValues->push_back (G4AttValue("YID",G4UIcommand::ConvertToString(YCellID),""));
  attValues->push_back (G4AttValue("ZID",G4UIcommand::ConvertToString(ZCellID),""));
  attValues->push_back (G4AttValue("EDep",G4BestUnit(edep,"Energy"),""));
  //G4cout << "Checking...\n" << G4AttCheck(attValues, GetAttDefs());
  return attValues;
}

void GPTargetHit::Print()
{;}


