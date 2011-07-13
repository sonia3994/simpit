#include "GPTargetSD.hh"
#include "GPTargetHit.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VSolid.hh"
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
}

GPTargetSD::~GPTargetSD()
{
}

void GPTargetSD::Initialize(G4HCofThisEvent* HCE)
{
  vecIntCellID.clear();
  vecIntCellID.resize(iNumberOfCellsInX*iNumberOfCellsInY*iNumberOfCellsInZ); 
  EddCollection = new GPTargetHitsCollection
                      (SensitiveDetectorName,collectionName[0]); 
  G4int size_cell=iNumberOfCellsInX*iNumberOfCellsInY*iNumberOfCellsInZ;
  for(G4int i=0;i<size_cell;i++)
     {vecIntCellID[i] = -1;}
  verboseLevel = 0;
  //std::cout<<SensitiveDetectorName+" Add new GPTargetHitsCollection: "+collectionName[0]<<std::endl;
}

G4bool GPTargetSD::ProcessHits(G4Step* aStep,G4TouchableHistory* ROhist)
{
  if(!ROhist) return false;
  G4double edep = aStep->GetTotalEnergyDeposit();
  //if(verboseLevel>1) 
    //G4cout << SensitiveDetectorName<< " :Deposite energy of this step: " << edep/MeV<<" MeV" << G4endl;
  if(edep==0.) return false;

  G4VPhysicalVolume* physVol = ROhist->GetVolume();
  //if(physVol->GetName()!="")
  //ROhist->MoveUpHistory();
  //G4VPhysicalVolume* mothVol = ROhist->GetVolume(1);
  G4double dCellVolume = ROhist->GetSolid()->GetCubicVolume();
  G4int copyIDinX = ROhist->GetReplicaNumber(2);
  G4int copyIDinY = ROhist->GetReplicaNumber(1);
  G4int copyIDinZ = ROhist->GetReplicaNumber(0);
  G4int cellIndex=copyIDinX+copyIDinY*iNumberOfCellsInX+copyIDinZ*iNumberOfCellsInX*iNumberOfCellsInY;

  if (vecIntCellID[cellIndex]==-1)
  {
    /*
    G4cout<<"Cell and Volume:\n"
      <<" Z: "<<copyIDinX
      <<" R: "<<copyIDinY
      <<" Phi: "<<copyIDinZ
      <<" Volume: "<<dCellVolume
      <<G4endl;

     */
    GPTargetHit* calHit = new GPTargetHit (physVol->GetLogicalVolume(),copyIDinX,copyIDinY,copyIDinZ);
    calHit->SetEdep( edep );
    //G4AffineTransform aTrans = ROhist->GetHistory()->GetTopTransform();
    //aTrans.Invert();
    //calHit->SetPos(aTrans.NetTranslation());
    //calHit->SetRot(aTrans.NetRotation());
    G4int icell = EddCollection->insert( calHit );
    vecIntCellID[cellIndex]= icell - 1;
    if(verboseLevel>1)
    { 
      G4cout 
	<< SensitiveDetectorName<< " New Calorimeter Hit on vecIntCellID " 
	<< copyIDinX << " " << copyIDinY <<" "<<copyIDinZ
	<< " Energy: "<<edep/MeV<<" MeV"
	<< G4endl; 
    }
  }
  else
  { 
    (*EddCollection)[vecIntCellID[cellIndex]]->AddEdep(edep);
    if(verboseLevel>1)
    { 
      G4cout 
	<< SensitiveDetectorName<< " Energy added to vecIntCellID " 
	<< copyIDinX << " " << copyIDinY <<" "<<copyIDinZ
	<< " Energy: "<<edep/MeV<<" MeV"
	<< G4endl; 
    }
  }

  return true;
}

void GPTargetSD::EndOfEvent(G4HCofThisEvent* HCE)
{
  //static G4int HCID = -1;
  //if(HCID<0)
  //{ HCID = GetCollectionID(0); }
  iCollectionID = GetCollectionID(0);
  HCE->AddHitsCollection( iCollectionID, EddCollection );
  /*
  std::cout
    <<SensitiveDetectorName<<": Add Collection: "<<collectionName[0]
    <<": Collection ID: "<<iCollectionID
    <<std::endl;
    */
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
int GPTargetSD::GetCellNumber(std::string sKey)
{
  if(sKey=="x")
    return iNumberOfCellsInX;
  else if(sKey=="y")
    return iNumberOfCellsInY;
  else if(sKey=="z")
    return iNumberOfCellsInZ;
  else
    return -1;
} 

void GPTargetSD::SetCollectionName(std::string sKey)
{
  collectionName.resize(1);
  collectionName[0]=sKey;
} 
