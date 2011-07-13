// $Id: GPRunHandleGeneral.cc,v 1.19 2008/01/17 17:31:32 maire Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "GPRunHandleGeneral.hh"
#include "GPRunHandleStore.hh"
#include "GPGeometryGeneral.hh"
#include "GPGeometryStore.hh"
#include "GPSensitiveHandle.hh"
#include "GPTargetSD.hh"
#include "GPRunAction.hh"

#include "G4Run.hh"
#include "G4SDManager.hh"
#include "G4RunManager.hh"

#include <algorithm>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GPRunHandleGeneral::GPRunHandleGeneral(std::string sName, std::string sFatherName)
{
  SetActive(1);
  SetName(sName);
  SetFatherName(sFatherName);
  GPRunHandleStore::GetInstance()->AddRunHandle(GetName(),this);
}


GPRunHandleGeneral::~GPRunHandleGeneral()
{
  GPRunHandleStore::GetInstance()->EraseItem(GetName());
}
void  GPRunHandleGeneral::BeginOfRunAction(const G4Run* run)
{
  iRunID=run->GetRunID();
  GPGeometryGeneral* geometry = 
    (GPGeometryGeneral*)GPGeometryStore::GetInstance()->FindGeometry(GetFatherName()+"geometry/");
  if(geometry==NULL)
    return;

  GPSensitiveHandle* sdHandle = geometry->GetSensitiveHandle();
  std::string sSDType = sdHandle->GetSDType();
  std::string sSDName = sdHandle->GetSDName();
  if(sSDType=="GPTargetSD")
  {
    GPTargetSD* targetSD = (GPTargetSD*)G4SDManager::GetSDMpointer()->FindSensitiveDetector(sSDName);
    if(targetSD)
    {
      int x = targetSD->GetCellNumber("x");
      int y = targetSD->GetCellNumber("y");
      int z = targetSD->GetCellNumber("z");
      vDouble.resize(x*y*z);
    }
    return;
  }

  else if(sSDType=="G4MultiFunctionalDetector")
  {
    MStrStrScorer* mStrStrScorer = sdHandle->GetMStrStrScorer();
    MStrStrScorer::iterator it;
    G4THitsMap<G4double>* oHitsMap;
    for(it=mStrStrScorer->begin();it!=mStrStrScorer->end();it++)
    {
      if(it->first=="GPSurfaceParticleScorer")
	continue;
      oHitsMap = new G4THitsMap<G4double>();  
      mStrG4THitsMap.insert(std::pair<std::string,G4THitsMap<G4double>* >(it->second,oHitsMap)) ;
    }
  }
  
}
void  GPRunHandleGeneral::EndOfRunAction(const G4Run* run)
{
  iRunID=run->GetRunID();
  GPRunAction* pRunAct = (GPRunAction*) G4RunManager::GetRunManager()->GetUserRunAction();
  std::string sPath = pRunAct->GetDataPath();
  std::string sFatherName=GetFatherName();
  replace(sFatherName.begin(),sFatherName.end(),'/','-');
  std::stringstream ss;
  std::string sRunID;
  std::ofstream ofs;
  ss<<iRunID;
  ss>>sRunID;

  GPGeometryGeneral* geometry = 
    (GPGeometryGeneral*)GPGeometryStore::GetInstance()->FindGeometry(GetFatherName()+"geometry/");
  if(geometry==NULL)
    return;

  std::cout<<"End of run: "<<GetName()<<std::endl;
  GPSensitiveHandle* sdHandle = geometry->GetSensitiveHandle();
  if(!sdHandle->IsActive())
    return;

  std::string sSDType = sdHandle->GetSDType();
  std::string sSDName = sdHandle->GetSDName();
  if(sSDType=="GPTargetSD")
  {
    ofs.open((sPath+"/"+sRunID+sFatherName+"GPTargetSD.dat").c_str());
    GPTargetSD* targetSD = (GPTargetSD*)G4SDManager::GetSDMpointer()->FindSensitiveDetector(sSDName);
    size_t iCellNumX,  iCellNumY, iCellNumZ, iIndex;

    iCellNumX = targetSD->GetCellNumber("x");
    iCellNumY = targetSD->GetCellNumber("y");
    iCellNumZ = targetSD->GetCellNumber("z");
    for(size_t i=0;i!=iCellNumX;i++)
    {
      for(size_t j=0;j!=iCellNumY;j++)
      {
	for(size_t k=0;k!=iCellNumZ;k++)
	{
	  iIndex=k*iCellNumX*iCellNumY+j*iCellNumX+i;
	  ofs<<i<<" "<<j<<" "<<k<<" "<<vDouble[iIndex]<<"\n";
	  //std::cout<<i<<" "<<j<<" "<<k<<" "<<vDouble[iIndex]<<"\n";
	}
      }
    }
    vDouble.resize(0);
    return;
  }

  else if(sSDType=="G4MultiFunctionalDetector")
  {
    ofs.open((sPath+"/"+sRunID+sFatherName+"G4MultiFunctional.dat").c_str());
    std::map<std::string,G4THitsMap<G4double>* >::iterator itStrTHit;
    for(itStrTHit = mStrG4THitsMap.begin();itStrTHit!=mStrG4THitsMap.end();itStrTHit++)
    {
      ofs<<"Value Type: "<<itStrTHit->first<<std::endl;
      ofs<<"Key	Value"<<std::endl;
      std::cout<<"Value Type: "<<itStrTHit->first<<std::endl;
      std::cout<<"Key	Value"<<std::endl;
      std::map<G4int,G4double*>::iterator itTHit 
	= (itStrTHit->second)->GetMap()->begin();
      for(;itTHit!=(itStrTHit->second)->GetMap()->end();itTHit++)
      {
	ofs<<itTHit->first<<"	"<<*(itTHit->second)<<std::endl;
	std::cout<<itTHit->first<<"	"<<*(itTHit->second)<<std::endl;
      }
      delete itStrTHit->second;
    }
    mStrG4THitsMap.clear();
  }
  ofs.close();
}
void  GPRunHandleGeneral::RecordPerEvent(std::string sKey,G4THitsMap<G4double>* pMap)
{
  (*(mStrG4THitsMap[sKey]))+=*pMap;
}
void  GPRunHandleGeneral::Add3dHit(G4int x, G4int y, G4int z, G4double e)
{
  GPGeometryGeneral* geometry = 
    (GPGeometryGeneral*)GPGeometryStore::GetInstance()->FindGeometry(GetFatherName()+"geometry/");
  if(geometry==NULL)
    return;

  GPSensitiveHandle* sdHandle = geometry->GetSensitiveHandle();
  std::string sSDName = sdHandle->GetSDName();
  GPTargetSD* targetSD = (GPTargetSD*)G4SDManager::GetSDMpointer()->FindSensitiveDetector(sSDName);

  size_t iCellNumX, iCellNumY, iCellNumZ;
  iCellNumX = targetSD->GetCellNumber("x");
  iCellNumY = targetSD->GetCellNumber("y");
  iCellNumZ = targetSD->GetCellNumber("z");
  size_t iIndex = x+y*iCellNumX+z*iCellNumX*iCellNumY; 
  vDouble[iIndex] +=e;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
