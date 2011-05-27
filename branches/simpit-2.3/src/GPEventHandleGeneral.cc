// $Id: GPEventHandleGeneral.cc,v 1.29 2008/01/17 17:31:32 maire Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
// 

#include "GPEventHandleGeneral.hh"
#include "GPGeometryStore.hh"
#include "GPGeometryGeneral.hh"
#include "GPEventHandleStore.hh"

#include "GPParticleHit.hh"
#include "GPEventAction.hh"
#include "GPRunAction.hh"
#include "G4RunManager.hh"

#include "G4Event.hh"
#include "G4SDManager.hh"
#include <sstream>
#include <fstream>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GPEventHandleGeneral::GPEventHandleGeneral(std::string sName, std::string sFatherName)
{
  SetActive(1);
  SetName(sName);
  SetFatherName(sFatherName);
  GPEventHandleStore::GetInstance()->AddEventHandle(GetName(),this);

}
GPEventHandleGeneral::~GPEventHandleGeneral()
{
  GPEventHandleStore::GetInstance()->EraseItem(GetName());
}

void  GPEventHandleGeneral::BeginOfEventAction(const G4Event* evt)
{
  iEvtID=evt->GetEventID();
}
void  GPEventHandleGeneral::EndOfEventAction(const G4Event* evt)
{
  G4HCofThisEvent* HCE = evt->GetHCofThisEvent();
  G4SDManager* SDM=G4SDManager::GetSDMpointer();
  GPGeometryGeneral* geometry = 
    (GPGeometryGeneral*)GPGeometryStore::GetInstance()->FindGeometry(GetFatherName()+"geometry/");
  MStrStrScorer* mStrStrScorer = geometry->GetMStrStrScorer();
  MStrStrScorer::iterator it;
  for(it=mStrStrScorer->begin();it!=mStrStrScorer->end();it++)
  {
    if(it->second=="GPSurfaceParticleScorer")
    {
      GPParticleHitsCollection* particleHitsCollection;

      G4int CollectionID;
      CollectionID=SDM->GetCollectionID(it->first); 
      particleHitsCollection =static_cast<GPParticleHitsCollection*>(HCE->GetHC(CollectionID));
      if(particleHitsCollection)
	ProcessParticleHits(particleHitsCollection,it->first);
    }
  }

}
void GPEventHandleGeneral::ProcessParticleHits(GPParticleHitsCollection* particleHitsCollection,std::string sCollectionName)
{
  GPRunAction* runAct = (GPRunAction*)G4RunManager::GetRunManager()->GetUserRunAction(); 
  std::string sPath = runAct->GetDataPath();
  int runID=runAct->GetRunID();
  GPParticleHit* particleHit;
  std::stringstream ss;
  std::string sFileName=sCollectionName;
  std::string sRun;
  ss<<runID;
  ss>>sRun;
  sFileName=sPath+"/"+sRun+sFileName+".dat";
  std::ofstream ofs;
  ofs.open(sFileName.c_str(),std::ios::ate|std::ios::app);
  
  static std::vector<G4double>   vecTrackInf;
  static G4ThreeVector  vecPos;
  static G4ThreeVector  vecMom;
  static G4String       particleName;
  static G4double	totalE; 
  static G4double	globalTime; 
  static G4int iTrackID=0;
  static G4int iLastTrackID=-1;
  static G4int iNumElectron=0;
  static G4int iNumPositron=0;
  static size_t numHit; 

  iNumPositron=0;
  iNumElectron=0;
  numHit=particleHitsCollection->GetSize(); 
  //std::cout<<"size: "<<numHit<<std::endl;
  for(size_t i=0;i!=numHit;i++)
  {
    particleHit=static_cast<GPParticleHit*>(particleHitsCollection->GetHit(i));
    iTrackID = particleHit->GetTrackID();
    if(iTrackID==iLastTrackID)	
    {
      continue;
    }
    iLastTrackID=iTrackID;
    particleName = particleHit->GetParticleName();
    vecPos = particleHit->GetPosition();
    vecMom = particleHit->GetMomentum();
    totalE = particleHit->GetTotalEnergy(); 
    globalTime = particleHit->GetGlobalTime(); 

    vecTrackInf.clear();
    vecTrackInf.push_back(particleHit->GetPDG());
    vecTrackInf.push_back(iEvtID);
    vecTrackInf.push_back(iTrackID);
    vecTrackInf.push_back(vecPos.x()*m/mm);
    vecTrackInf.push_back(vecPos.y()*m/mm);
    //vecTrackInf.push_back(vecPos.z()*m/mm);
    vecTrackInf.push_back(vecMom.x());
    vecTrackInf.push_back(vecMom.y());
    vecTrackInf.push_back(vecMom.z());
    vecTrackInf.push_back(totalE);
    vecTrackInf.push_back(globalTime*second/picosecond);

    for(size_t i=0;i!=vecTrackInf.size();i++)
    {
      ofs<<vecTrackInf[i]<<"  ";
    }
      ofs<<std::endl;
  }
  ofs.close();
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
