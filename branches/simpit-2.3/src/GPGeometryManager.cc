// $Id: GPGometry.cc,v 1.9 2006/06/29 17:47:19 gunter Granular $
// GEANT4 tag $Name: geant4-09-02 $
//
#include "GPGeometryManager.hh"
#include "GPGeometryStore.hh"
#include "GPGeometry.hh"

#include "GPModule.hh"
#include "GPModuleStore.hh"

#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"

GPGeometryManager* GPGeometryManager::instance=NULL;
GPGeometryManager* GPGeometryManager::GetInstance()
{
  if(instance==NULL) 
    instance = new GPGeometryManager();
  return instance;
}
void GPGeometryManager::Delete()
{
  if(instance)  delete instance;
}
GPGeometryManager::GPGeometryManager()
{
  iCompactRangerFlag=1;
}


GPGeometryManager::~GPGeometryManager()
{}

void GPGeometryManager::ConstructGeometry(G4LogicalVolume* motherLog , G4ThreeVector vPoint)
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPGeometryManager::ConstructGeometry(G4LogicalVolume* motherLog , G4ThreeVector vPoint)"<<G4endl;
#endif
  if(iCompactRangerFlag)
    ConstructGeometryCompact(motherLog,vPoint);
  else
    ConstructGeometryNormal(motherLog,vPoint);
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPGeometryManager::ConstructGeometry(G4LogicalVolume* motherLog , G4ThreeVector vPoint)"<<G4endl;
#endif
}
void GPGeometryManager::ConstructGeometryNormal(G4LogicalVolume* motherLog,G4ThreeVector vPoint)
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPGeometryManager::ConstructGeometryNormal(G4LogicalVolume* motherLog,G4ThreeVector vPoint)"<<G4endl;
#endif
  GPGeometryMap* mStrGeometry
    =GPGeometryStore::GetInstance()->GetGeometryMap();
  GPGeometryMap::iterator it;
  GPGeometry* geometry;
  GPModule* module;
  std::string sModuleName;
  for(it=mStrGeometry->begin();it!=mStrGeometry->end();it++)
  {
    geometry=it->second;
    sModuleName = geometry->GetFatherName();
    module = GPModuleStore::GetInstance()->FindModule(sModuleName);
    if(module==NULL)
      continue;
    if(module->IsActive()&&geometry->IsActive()) 
      geometry->Construct(motherLog,vPoint);
  }
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPGeometryManager::ConstructGeometryNormal(G4LogicalVolume* motherLog,G4ThreeVector vPoint)"<<G4endl;
#endif
}
void GPGeometryManager::ConstructGeometryCompact(G4LogicalVolume* motherLog , G4ThreeVector vPoint)
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPGeometryManager::ConstructGeometryCompact(G4LogicalVolume* motherLog , G4ThreeVector vPoint)"<<G4endl;
#endif
  GPGeometryMap* mStrGeometry
    =GPGeometryStore::GetInstance()->GetGeometryMap();
  GPGeometryMap::iterator it;
  GPGeometry* geometry;
  GPGeometry* geometryCenter;
  GPModule* module;
  std::string sModuleName;
  std::map<int,GPGeometry*> mIntGeometryPos;
  std::map<int,GPGeometry*>::iterator mIntGeoIt;
  std::map<int,GPGeometry*> mIntGeometryNeg;
  std::map<int,GPGeometry*>::reverse_iterator mIntGeoRit;
  int iPriority;
  for(it=mStrGeometry->begin();it!=mStrGeometry->end();it++)
  {
    geometry=it->second;
    sModuleName = geometry->GetFatherName();
    module = GPModuleStore::GetInstance()->FindModule(sModuleName);
    if(module==NULL)
    {
      continue;
    }
    if(module->IsActive()&&geometry->IsActive()) 
    {
      iPriority=module->GetPriority();
      if(iPriority>0)
        mIntGeometryPos[iPriority]=geometry;
      else if(iPriority<0)
        mIntGeometryNeg[iPriority]=geometry;
      else
	geometryCenter=geometry;
    }
  }
  
  if(geometryCenter==NULL)
  {
    std::cout<<"Can't find center geometry!";
    if(mIntGeometryPos.size()>0)
    {
      mIntGeoIt=mIntGeometryPos.begin();
      geometryCenter=mIntGeoIt->second;
      std::cout<<"\n Move "<<mIntGeoIt->first<<" to center!"
      <<std::endl;
    }
    else if(mIntGeometryNeg.size()>0)
    {
      mIntGeoIt=mIntGeometryNeg.end();
      mIntGeoIt--;
      geometryCenter=mIntGeoIt->second;
      std::cout<<"\n Move "<<mIntGeoIt->first<<" to center!"
      <<std::endl;
      mIntGeometryNeg.erase(mIntGeoIt);
    }
    else
    {
      std::cout<<"\nCan't find any geometry, just return!"
      <<std::endl;
      return;
    }
  }

  G4ThreeVector vItPoint=vPoint;
  geometryCenter->Construct(motherLog,vItPoint);

  double dPreGeoHalfLength=geometryCenter->GetParameter("length","length")/2;
  double dCurGeoHalfLength=0;
  for(mIntGeoIt=mIntGeometryPos.begin();mIntGeoIt!=mIntGeometryPos.end();mIntGeoIt++)
  {
    geometry=mIntGeoIt->second;
    dCurGeoHalfLength=geometry->GetParameter("length","length")/2;
    vItPoint.setZ(vItPoint.z()+dPreGeoHalfLength+dCurGeoHalfLength);
    dPreGeoHalfLength=dCurGeoHalfLength;
    geometry->Construct(motherLog,vItPoint);
  }

  vItPoint=vPoint;
  dPreGeoHalfLength=-(geometryCenter->GetParameter("length","length")/2);
  dCurGeoHalfLength=0;
  for(mIntGeoRit=mIntGeometryNeg.rbegin();mIntGeoRit!=mIntGeometryNeg.rend();mIntGeoRit++)
  {
    geometry=mIntGeoRit->second;
    dCurGeoHalfLength=-(geometry->GetParameter("length","length")/2);
    vItPoint.setZ(vItPoint.z()+dPreGeoHalfLength+dCurGeoHalfLength);
    dPreGeoHalfLength=dCurGeoHalfLength;
    geometry->Construct(motherLog,vItPoint);
  }
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPGeometryManager::ConstructGeometryCompact(G4LogicalVolume* motherLog , G4ThreeVector vPoint)"<<G4endl;
#endif
}
