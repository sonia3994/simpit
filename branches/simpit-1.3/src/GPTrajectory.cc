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

G4Allocator<GPTrajectory> GPTrajectoryAllocator;

GPTrajectory::GPTrajectory()
:G4VTrajectory()
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPTrajectory::GPTrajectory()"<<G4endl;
#endif
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
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPTrajectory::GPTrajectory()"<<G4endl;
#endif
}

GPTrajectory::GPTrajectory(const G4Track* aTrack)
:G4VTrajectory()
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPTrajectory::GPTrajectory(const G4Track*)"<<G4endl;
#endif
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
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPTrajectory::GPTrajectory(const G4Track*)"<<G4endl;
#endif
}

GPTrajectory::GPTrajectory(GPTrajectory & right)
:G4VTrajectory()
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPTrajectory::GPTrajectory(GPTrajectory&)"<<G4endl;
#endif
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
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPTrajectory::GPTrajectory(GPTrajectory&)"<<G4endl;
#endif
}

GPTrajectory::~GPTrajectory()
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPTrajectory::~GPTrajectory()"<<G4endl;
#endif
  size_t i;
  for(i=0;i<positionRecord->size();i++){
    delete  (*positionRecord)[i];
  }
  positionRecord->clear();

  delete positionRecord;
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPTrajectory::~GPTrajectory()"<<G4endl;
#endif
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

   for( size_t i=0 ; i < positionRecord->size() ; i++){
       G4TrajectoryPoint* aTrajectoryPoint = (G4TrajectoryPoint*)((*positionRecord)[i]);
       os << "Point[" << i << "]" 
            << " Position= " << aTrajectoryPoint->GetPosition() << G4endl;
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
   positionRecord->push_back( new G4TrajectoryPoint(aStep->GetPostStepPoint()->
                                 GetPosition() ));
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



