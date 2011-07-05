//
// $Id: GPTrajectory.cc,v 1.3 2006/06/29 17:44:37 gunter Exp $
// GEANT4 tag $Name: geant4-09-02 $
//


#include "GPTrajectory.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleTypes.hh"
#include "G4Polyline.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4AttDefStore.hh"
#include "G4AttDef.hh"
#include "G4AttValue.hh"
#include "G4UIcommand.hh"
#include "G4VisAttributes.hh"
#include "G4VVisManager.hh"
#include "G4UnitsTable.hh"
#include "G4DynamicParticle.hh"
#include "G4PrimaryParticle.hh"
#include "GPTrackInformation.hh"

#include <cmath>
G4Allocator<GPTrajectory> GPTrajectoryAllocator;

GPTrajectory::GPTrajectory()
:G4VTrajectory()
{
  fpParticleDefinition = 0;
  sParticleName = "";
  dPDGCharge = 0;
  iPDGEncoding = 0;
  iTrackID = 0;
  iParentID = 0;
  iTrackStatus = 0;
  positionRecord = 0;
  vecMomentum = G4ThreeVector(0.,0.,0.);
  vecVertexPosition = G4ThreeVector(0.,0.,0.);
  dGlobalTime = 0.;
  dTrackLength = 0.;
  dZLength = 0.;
  vecFullInfor = new std::vector<std::vector<G4double>* >();
}

GPTrajectory::GPTrajectory(const G4Track* aTrack)
:G4VTrajectory()
{
  fpParticleDefinition = aTrack->GetDefinition();
  sParticleName = fpParticleDefinition->GetParticleName();
  dPDGCharge = fpParticleDefinition->GetPDGCharge();
  iPDGEncoding = fpParticleDefinition->GetPDGEncoding();
  if(sParticleName=="unknown")
  {
    G4PrimaryParticle*pp = aTrack->GetDynamicParticle()->GetPrimaryParticle();
    if(pp)
    {
      if(pp->GetCharge()<DBL_MAX) dPDGCharge = pp->GetCharge();
      iPDGEncoding = pp->GetPDGcode();
      if(pp->GetG4code()!=0)
      {
        sParticleName += " : ";
        sParticleName += pp->GetG4code()->GetParticleName();
      }
    }
  }
  iTrackID = aTrack->GetTrackID();
  GPTrackInformation* trackInfo
   = (GPTrackInformation*)(aTrack->GetUserInformation());
  iTrackStatus = trackInfo->GetTrackingStatus();
  if(iTrackStatus == 1)
  { iParentID = aTrack->GetParentID(); }
  else if(iTrackStatus == 2)
  { iParentID = trackInfo->GetSourceTrackID(); }
  else
  { iParentID = -1; }
  positionRecord = new GPTrajectoryPointContainer();
  positionRecord->push_back(new G4TrajectoryPoint(aTrack->GetPosition()));
  vecMomentum = aTrack->GetMomentum();
  vecVertexPosition = aTrack->GetPosition();
  dGlobalTime = aTrack->GetGlobalTime();
  dTrackLength = 0.;
  dZLength = 0.;

  vecFullInfor = new std::vector<std::vector<G4double>* >();
  std::vector<G4double>* vecItem = new std::vector<G4double>();
  vecItem->push_back(dGlobalTime);
  vecItem->push_back(vecVertexPosition.x());
  vecItem->push_back(vecVertexPosition.y());
  vecItem->push_back(vecVertexPosition.z());
  vecItem->push_back(vecMomentum.x());
  vecItem->push_back(vecMomentum.y());
  vecItem->push_back(vecMomentum.z());
  vecFullInfor->push_back(vecItem);
}

GPTrajectory::GPTrajectory(GPTrajectory & right)
:G4VTrajectory()
{
  sParticleName = right.sParticleName;
  fpParticleDefinition = right.fpParticleDefinition;
  dPDGCharge = right.dPDGCharge;
  iPDGEncoding = right.iPDGEncoding;
  iTrackID = right.iTrackID;
  iParentID = right.iParentID;
  iTrackStatus = right.iTrackStatus;
  positionRecord = new GPTrajectoryPointContainer();
  for(size_t i=0;i<right.positionRecord->size();i++)
  {
    G4TrajectoryPoint* rightPoint = (G4TrajectoryPoint*)((*(right.positionRecord))[i]);
    positionRecord->push_back(new G4TrajectoryPoint(*rightPoint));
  }
  vecMomentum = right.vecMomentum;
  vecVertexPosition = right.vecVertexPosition;
  dGlobalTime = right.dGlobalTime;
  dTrackLength = right.dTrackLength;
  dZLength = right.dZLength ;
///*
  vecFullInfor = new std::vector<std::vector<G4double>* >();
  std::vector<G4double>* vecItem;
  for(size_t i=0;i<right.vecFullInfor->size();i++)
  {
    vecItem = new std::vector<G4double>();
    for(size_t j=0;j<(*(right.vecFullInfor))[i]->size();j++)
    {
      vecItem->push_back((*(*vecFullInfor)[i])[j]);
    }
    vecFullInfor->push_back(vecItem);
  }
//*/
}

GPTrajectory::~GPTrajectory()
{
  size_t i;
  for(i=0;i<positionRecord->size();i++){
    delete  (*positionRecord)[i];
  }
  positionRecord->clear();
  delete positionRecord;

   for(size_t i=0;i<vecFullInfor->size();i++)
   {
      delete (*vecFullInfor)[i];
   }
   delete vecFullInfor;
}

void GPTrajectory::ProcessTrajectory(G4int code)
{
  if(code==-1)
    return;
  else if(code==0)
	ShowTrajectory();
  else if(code==1)
	DrawTrajectory();
  else if(code==2)
	return;          //TO DO: Save to disk
}

G4double GPTrajectory::GetRAtZ(G4double dZValue) const 
{
  size_t entries=vecFullInfor->size();
  if (entries==0) return -1;

  G4double dZLow=(*(*vecFullInfor)[0])[3];
  G4double dZHigh=(*(*vecFullInfor)[entries-1])[3];
  if(dZValue<dZLow||dZValue>dZHigh) return -1;

  G4double dXValue;
  G4double dYValue;
  size_t iLow;
  size_t iHigh;
  size_t iIndex=entries*(dZValue-dZLow)/(dZHigh-dZLow);
  G4double dCandidate=(*(*vecFullInfor)[iIndex])[3];
  if(dCandidate<dZValue)
  {
    iLow=iIndex;
    iHigh=entries;
  }
  else
  {
    iLow=0;
    iHigh=iIndex;
  }

  for(size_t i=iLow;i<iHigh;i++)
  {
    dCandidate=(*(*vecFullInfor)[i])[3];
    if(abs(dCandidate-dZValue)<=1)
    {
      dXValue=(*(*vecFullInfor)[i])[1];
      dYValue=(*(*vecFullInfor)[i])[2];
      return sqrt(dXValue*dXValue+dYValue*dYValue);
    }
  }
  return -1;
}
void GPTrajectory::ShowTrajectory(std::ostream& os) const
{
   os << G4endl << "TrackID =" << iTrackID 
        << " : ParentID=" << iParentID << " : TrackStatus=" << iTrackStatus << G4endl;
   os << "Particle name : " << sParticleName << "  PDG code : " << iPDGEncoding
        << "  Charge : " << dPDGCharge << G4endl;
   os << "Original vecMomentum : " <<
        G4BestUnit(vecMomentum,"Energy") << G4endl;
   os << "Vertex : " << G4BestUnit(vecVertexPosition,"Length")
        << "  Global time : " << G4BestUnit(dGlobalTime,"Time") << G4endl;
   os << "  Current trajectory has " << positionRecord->size() 
        << " points." << G4endl;
/*
   for( size_t i=0 ; i < positionRecord->size() ; i++)
   {
       G4TrajectoryPoint* aTrajectoryPoint = (G4TrajectoryPoint*)((*positionRecord)[i]);
       os << "Point[" << i << "]" 
            << " Position= " << aTrajectoryPoint->GetPosition() << G4endl;
   }
*/
   for( size_t i=0 ; i < vecFullInfor->size() ; i++)
   {
      std::vector<G4double>* vecItem=(*vecFullInfor)[i];
      os << "Slide[" << i << "]: ";
	 for(size_t j=0; j< vecItem->size(); j++) 
	 {
            os<<(*vecItem)[j]<<" ";
	 }
      os<<G4endl;
   }
}

void GPTrajectory::DrawTrajectory(G4int) const
{

   G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
   //if(pVVisManager) G4cout<<"G4VVisManager exists"<<G4endl;
   G4ThreeVector pos;

   G4Polyline pPolyline;
   for (size_t i = 0; i < positionRecord->size() ; i++) {
     G4TrajectoryPoint* aTrajectoryPoint = (G4TrajectoryPoint*)((*positionRecord)[i]);
     pos = aTrajectoryPoint->GetPosition();
     pPolyline.push_back( pos );
   }

   G4Colour colour(0.2,0.2,0.2);
   if(fpParticleDefinition==G4Gamma::GammaDefinition())
      colour = G4Colour(0.,1.,0.);
   else if(fpParticleDefinition==G4Electron::ElectronDefinition())
      colour = G4Colour(1.,0.,0.);
   else if(fpParticleDefinition==G4Positron::PositronDefinition())
      colour = G4Colour(0.,0.,1.);
   else if(fpParticleDefinition==G4MuonMinus::MuonMinusDefinition()
         ||fpParticleDefinition==G4MuonPlus::MuonPlusDefinition())
      colour = G4Colour(0.,1.,0.);
   else if(fpParticleDefinition->GetParticleType()=="meson")
   {
      if(dPDGCharge!=0.)
         colour = G4Colour(1.,0.,0.);
      else
         colour = G4Colour(0.5,0.,0.);
   }
   else if(fpParticleDefinition->GetParticleType()=="baryon")
   {
      if(dPDGCharge!=0.)
         colour = G4Colour(0.,1.,1.);
      else
         colour = G4Colour(0.,0.5,0.5);
   }

   G4VisAttributes attribs(colour);
   pPolyline.SetVisAttributes(attribs);
   if(pVVisManager) pVVisManager->Draw(pPolyline);
}

const std::map<G4String,G4AttDef>* GPTrajectory::GetAttDefs() const
{
  G4bool isNew;
  std::map<G4String,G4AttDef>* store
    = G4AttDefStore::GetInstance("GPTrajectory",isNew);
  if (isNew) {

    G4String ID("ID");
    (*store)[ID] = G4AttDef(ID,"Track ID","Bookkeeping","","G4int");

    G4String PID("PID");
    (*store)[PID] = G4AttDef(PID,"Parent ID","Bookkeeping","","G4int");

    G4String Status("Status");
    (*store)[Status] = G4AttDef(Status,"Track Status","Bookkeeping","","G4int");

    G4String PN("PN");
    (*store)[PN] = G4AttDef(PN,"Particle Name","Bookkeeping","","G4String");

    G4String Ch("Ch");
    (*store)[Ch] = G4AttDef(Ch,"Charge","Physics","e+","G4double");

    G4String PDG("PDG");
    (*store)[PDG] = G4AttDef(PDG,"PDG Encoding","Bookkeeping","","G4int");

    G4String IMom("IMom");
    (*store)[IMom] = G4AttDef(IMom, "vecMomentum of track at start of trajectory",
			      "Physics","G4BestUnit","G4ThreeVector");

    G4String IMag("IMag");
    (*store)[IMag] = 
      G4AttDef(IMag, "Magnitude of vecMomentum of track at start of trajectory",
	       "Physics","G4BestUnit","G4double");

    G4String VtxPos("VtxPos");
    (*store)[VtxPos] = G4AttDef(VtxPos, "Vertex position",
			      "Physics","G4BestUnit","G4ThreeVector");

    G4String NTP("NTP");
    (*store)[NTP] = G4AttDef(NTP,"No. of points","Bookkeeping","","G4int");

  }
  return store;
}

std::vector<G4AttValue>* GPTrajectory::CreateAttValues() const
{
  std::vector<G4AttValue>* values = new std::vector<G4AttValue>;

  values->push_back
    (G4AttValue("ID",G4UIcommand::ConvertToString(iTrackID),""));

  values->push_back
    (G4AttValue("PID",G4UIcommand::ConvertToString(iParentID),""));

  values->push_back
    (G4AttValue("Status",G4UIcommand::ConvertToString(iTrackStatus),""));

  values->push_back(G4AttValue("PN",sParticleName,""));

  values->push_back
    (G4AttValue("Ch",G4UIcommand::ConvertToString(dPDGCharge),""));

  values->push_back
    (G4AttValue("PDG",G4UIcommand::ConvertToString(iPDGEncoding),""));

  values->push_back
    (G4AttValue("IMom",G4BestUnit(vecMomentum,"Energy"),""));

  values->push_back
    (G4AttValue("IMag",G4BestUnit(vecMomentum.mag(),"Energy"),""));

  values->push_back
    (G4AttValue("VtxPos",G4BestUnit(vecVertexPosition,"Length"),""));

  values->push_back
    (G4AttValue("NTP",G4UIcommand::ConvertToString(GetPointEntries()),""));

  return values;
}

void GPTrajectory::AppendStep(const G4Step* aStep)
{
   G4ThreeVector pos=aStep->GetPostStepPoint()->GetPosition();
   G4ThreeVector mom=aStep->GetPostStepPoint()->GetMomentum();
   G4double      time=aStep->GetPostStepPoint()->GetGlobalTime();

   dTrackLength += aStep->GetStepLength();
   G4double dZLengthT =pos.z()-vecVertexPosition.z() ;
   dZLength = dZLength<dZLengthT ? dZLengthT : dZLength ;

   positionRecord->push_back( new G4TrajectoryPoint(pos));
   
   std::vector<G4double>* vecItem = new std::vector<G4double>();
   vecItem->push_back(time);
   vecItem->push_back(pos.x());
   vecItem->push_back(pos.y());
   vecItem->push_back(pos.z());
   vecItem->push_back(mom.x());
   vecItem->push_back(mom.y());
   vecItem->push_back(mom.z());
   vecFullInfor->push_back(vecItem);
}
  
G4ParticleDefinition* GPTrajectory::GetParticleDefinition()
{
   return (G4ParticleTable::GetParticleTable()->FindParticle(sParticleName));
}

void GPTrajectory::MergeTrajectory(G4VTrajectory* secondTrajectory)
{
  if(!secondTrajectory) return;

  GPTrajectory* seco = (GPTrajectory*)secondTrajectory;
  G4int ent = seco->GetPointEntries();
  for(int i=1;i<ent;i++) // initial point of the second trajectory should not be merged
  {
    positionRecord->push_back((*(seco->positionRecord))[i]);
  }
  delete (*seco->positionRecord)[0];
  seco->positionRecord->clear();

}


void GPTrajectory::SaveAs(std::ofstream* os) 
{
  size_t entries = vecFullInfor->size(); 
   *os << iPDGEncoding << " " <<entries<< G4endl;
   for( size_t i=0 ; i < entries ; i++)
   {
      std::vector<G4double>* vecItem=(*vecFullInfor)[i];
	 for(size_t j=0; j< vecItem->size(); j++) 
	 {
            *os<<(*vecItem)[j]<<" ";
	 }
      *os<<G4endl;
   }
}

