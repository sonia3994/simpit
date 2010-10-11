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
 iNumberOfCellsInX(hitDim[0]),iNumberOfCellsInY(hitDim[1]),iNumberOfCellsInZ(hitDim[2])
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPTargetSD::GPTargetSD(G4String,std::vector<G4int>)"<<G4endl;
#endif
  G4String HCname;
  collectionName.insert(HCname="EddCollection");
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPTargetSD::GPTargetSD(G4String,std::vector<G4int>)"<<G4endl;
#endif
}

GPTargetSD::~GPTargetSD()
{

#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPTargetSD::~GPTargetSD()"<<G4endl;
#endif
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPTargetSD::~GPTargetSD()"<<G4endl;
#endif
}

void GPTargetSD::Initialize(G4HCofThisEvent* HCE)
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPTargetSD::Initialize(G4HCofThisEvent*)"<<G4endl;
#endif
  vecIntCellID.clear();
  vecIntCellID.resize(iNumberOfCellsInX*iNumberOfCellsInY*iNumberOfCellsInZ); 
  EddCollection = new GPTargetHitsCollection
                      (SensitiveDetectorName,collectionName[0]); 
  G4int size_cell=iNumberOfCellsInX*iNumberOfCellsInY*iNumberOfCellsInZ;
  for(G4int i=0;i<size_cell;i++)
     {vecIntCellID[i] = -1;}
  verboseLevel = 0;
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPTargetSD::Initialize(G4HCofThisEvent*)"<<G4endl;
#endif
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
  G4int cellIndex=copyIDinZ*iNumberOfCellsInX*iNumberOfCellsInY+copyIDinY*iNumberOfCellsInX+copyIDinX;

  if (vecIntCellID[cellIndex]==-1)
  {
    GPTargetHit* calHit = new GPTargetHit (physVol->GetLogicalVolume(),copyIDinX,copyIDinY,copyIDinZ);
    calHit->SetEdep( edep );
    //G4AffineTransform aTrans = ROhist->GetHistory()->GetTopTransform();
    //aTrans.Invert();
    //calHit->SetPos(aTrans.NetTranslation());
    //calHit->SetRot(aTrans.NetRotation());
    G4int icell = EddCollection->insert( calHit );
    vecIntCellID[cellIndex]= icell - 1;
    if(verboseLevel>1)
    { G4cout << " New Calorimeter Hit on vecIntCellID " << copyIDinX << " " << copyIDinY <<" "<<copyIDinZ<< G4endl; }
  }
  else
  { 
    (*EddCollection)[vecIntCellID[cellIndex]]->AddEdep(edep);
    if(verboseLevel>1)
    { G4cout << " Energy added to vecIntCellID " << copyIDinX << " " << copyIDinY <<" "<<copyIDinZ<< G4endl; }
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

