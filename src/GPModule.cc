// $Id: GPModule.cc,v 1.19 2008/01/17 17:31:32 maire Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "GPModule.hh"
#include "GPModuleManager.hh"

#include "GPGeometryGeneral.hh"
#include "GPGeometryStore.hh"
#include "GPGeometryManager.hh"
#include "GPSteppingHandle.hh"
#include "GPSteppingHandleStore.hh"
#include "GPSteppingHandleManager.hh"
#include "GPEventHandleGeneral.hh"
#include "GPEventHandleStore.hh"
#include "GPEventHandleManager.hh"
#include "GPRunHandleGeneral.hh"
#include "GPRunHandleStore.hh"
#include "GPRunHandleManager.hh"

#include "GPModuleStore.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4UIcommand.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GPModule::GPModule(std::string sName, std::string sFatherName):
  geometry(0),
  steppingHandle(0),
  eventHandle(0),
  runHandle(0)
{
  SetActive(1);
  SetName(sName);
  SetFatherName(sFatherName);
  iCompactRangerFlag=1;
  iPrintRecursiveFlag=1;
  GPModuleStore::GetInstance()->AddModule(GetName(),this);
  vCenterChildPosition=G4ThreeVector(0,0,0);
}


GPModule::~GPModule()
{
  if(geometry) delete geometry;
  if(steppingHandle) delete steppingHandle;
  if(eventHandle) delete eventHandle;
  if(runHandle) delete runHandle;

  GPModuleMap::iterator it;
  for(it=mChildModule.begin();it!=mChildModule.end();it++)
  {
    delete it->second;
  }

  GPModuleStore::GetInstance()->EraseItem(GetName());
}
void GPModule::Update()
{
  geometry->Update();
  std::map<std::string,GPModule* >::iterator it;
  for(it=mChildModule.begin();it!=mChildModule.end();it++)
  {
    it->second->Update();
  }
}
void GPModule::AddChild(GPModule* child)
{
  mChildModule[child->GetName()]=child;
}
void GPModule::SetObject(std::string sValue, GPObject* object)
{
  if(sValue=="geometry")
    geometry=(GPGeometry*)object;
  else if(sValue=="stepping")
    steppingHandle=(GPSteppingHandle*)object;
  else if(sValue=="event")
    eventHandle=(GPEventHandle*)object;
  else if(sValue=="run")
    runHandle=(GPRunHandle*)object;
  else
  {
    std::cout<<"Can't find this sKey: "
      <<sValue<<std::endl;
  }

}

GPObject* GPModule::FindObject(std::string sValue) const
{
  if(sValue=="geometry")
    return geometry;
  else if(sValue=="stepping")
    return steppingHandle;
  else if(sValue=="event")
    return eventHandle;
  else if(sValue=="run")
    return runHandle;
  else
  {
    std::cout<<"Can't find this sKey: "
      <<sValue<<std::endl;
    return NULL;
  }
}
G4VPhysicalVolume* GPModule::ConstructGeometry(G4LogicalVolume* motherLog)
{
  G4VPhysicalVolume* phyCurrent=0;
  G4LogicalVolume* logCurrent;
  phyCurrent = geometry->Construct(motherLog);
  logCurrent = phyCurrent->GetLogicalVolume();

  if(mChildModule.size()>0)
  {
    if(iCompactRangerFlag)
      ConstructGeometryCompact(logCurrent);
    else
      ConstructGeometryNormal(logCurrent);
  }
  return phyCurrent;
}
void GPModule::ConstructGeometryNormal(G4LogicalVolume* motherLog)
{
  GPModuleMap::iterator itMod;
  GPGeometry* pDautherGeometry;
  for(itMod=mChildModule.begin();itMod!=mChildModule.end();itMod++)
  {
    pDautherGeometry=(GPGeometry*)(itMod->second)->FindObject("geometry");
    if((itMod->second)->IsActive()&&pDautherGeometry!=NULL)
      pDautherGeometry->SetMotherPositionInGlobalFrame(geometry->GetPositionInGlobalFrame());
      (itMod->second)->ConstructGeometry(motherLog);
  }
}
void GPModule::ConstructGeometryCompact(G4LogicalVolume* motherLog )
{
  GPModuleMap::iterator itMod;
  GPModule* module;
  GPGeometry* pDautherGeometry;
  GPModule* moduleCenter=NULL;
  std::multimap<int,GPModule*> mIntGPModulePos;
  std::multimap<int,GPModule*>::iterator mIntModIt;
  std::multimap<int,GPModule*> mIntGPModuleNeg;
  std::multimap<int,GPModule*>::reverse_iterator mIntModRit;
  int iPriTmp;

  for(itMod=mChildModule.begin();itMod!=mChildModule.end();itMod++)
  {
    module=itMod->second;
    pDautherGeometry=(GPGeometry*)module->FindObject("geometry");
    if(module->IsActive()&&pDautherGeometry!=NULL)
    {
      iPriTmp=module->GetPriority();
      if(iPriTmp>0)
        mIntGPModulePos.insert(std::pair<int,GPModule*>(iPriTmp,module));
      else if(iPriTmp<0)
        mIntGPModuleNeg.insert(std::pair<int,GPModule*>(iPriTmp,module));
      else
	moduleCenter=module;
    }
  }

  if(moduleCenter==NULL)
  {
    std::cout<<"Can't find center module!";
    if(mIntGPModulePos.size()>0)
    {
      mIntModIt=mIntGPModulePos.begin();
      moduleCenter=mIntModIt->second;
      std::cout<<"\n Move "<<moduleCenter->GetName()<<" to center!"
      <<std::endl;
      mIntGPModulePos.erase(mIntModIt);
    }
    else if(mIntGPModuleNeg.size()>0)
    {
      mIntModIt=mIntGPModuleNeg.end();
      mIntModIt--;
      moduleCenter=mIntModIt->second;
      std::cout<<"\n Move "<<moduleCenter->GetName()<<" to center!"
      <<std::endl;
      mIntGPModuleNeg.erase(mIntModIt);
    }
    else
    {
      std::cout<<"\nCan't find any module, just return!"
      <<std::endl;
      return;
    }
  }

  pDautherGeometry=(GPGeometry*)moduleCenter->FindObject("geometry");
  pDautherGeometry->SetPosition(vCenterChildPosition);
  pDautherGeometry->SetMotherPositionInGlobalFrame(geometry->GetPositionInGlobalFrame());
  moduleCenter->ConstructGeometry(motherLog);

  G4ThreeVector vItPoint=vCenterChildPosition;
  double dPreGeoHalfLength=moduleCenter->FindObject("geometry")->GetParameter("length","length")/2;
  double dCurGeoHalfLength=0;
  for(mIntModIt=mIntGPModulePos.begin();mIntModIt!=mIntGPModulePos.end();mIntModIt++)
  {
    module=mIntModIt->second;
    dCurGeoHalfLength=module->FindObject("geometry")->GetParameter("length","length")/2;
    vItPoint.setZ(vItPoint.z()+dPreGeoHalfLength+dCurGeoHalfLength);
    dPreGeoHalfLength=dCurGeoHalfLength;
    pDautherGeometry=(GPGeometry*)module->FindObject("geometry");
    pDautherGeometry->SetPosition(vItPoint);
    pDautherGeometry->SetMotherPositionInGlobalFrame(geometry->GetPositionInGlobalFrame());
    module->ConstructGeometry(motherLog);
  }

  vItPoint=vCenterChildPosition;
  dPreGeoHalfLength=-(moduleCenter->FindObject("geometry")->GetParameter("length","length")/2);
  dCurGeoHalfLength=0;
  for(mIntModRit=mIntGPModuleNeg.rbegin();mIntModRit!=mIntGPModuleNeg.rend();mIntModRit++)
  {
    module=mIntModRit->second;
    dCurGeoHalfLength=-(module->FindObject("geometry")->GetParameter("length","length")/2);
    vItPoint.setZ(vItPoint.z()+dPreGeoHalfLength+dCurGeoHalfLength);
    dPreGeoHalfLength=dCurGeoHalfLength;
    pDautherGeometry=(GPGeometry*)module->FindObject("geometry");
    pDautherGeometry->SetPosition(vItPoint);
    pDautherGeometry->SetMotherPositionInGlobalFrame(geometry->GetPositionInGlobalFrame());
    module->ConstructGeometry(motherLog);
  }
  //G4cout<<"GP_DEBUG: Exit GPModule::ConstructGeometryCompact(G4LogicalVolume*): Object Name: "<<GetName()<<G4endl;
}
void GPModule::Print()
{
  std::cout<<"\n[Begin Module: "+GetName()+"]";
  if(runHandle)
    runHandle->Print();
  else
    std::cout<<"\n[Run Handle: Not Set]";
  if(eventHandle)
    eventHandle->Print();
  else
    std::cout<<"\n[Event Handle: Not Set]";
  if(steppingHandle)
    steppingHandle->Print();
  else
    std::cout<<"\n[Stepping Handle: Not Set]";
  if(geometry)
    geometry->Print();
  else
    std::cout<<"\n[Geometry: Not Set]";
  if(mChildModule.size()>0)
  {
    std::cout
      <<"\n[Begin Sub Module: "+GetName()+"]";
    GPModuleMap::iterator it;
    for(it=mChildModule.begin();it!=mChildModule.end();it++)
    {
      if(int(iPrintRecursiveFlag)!=0)
	(it->second)->Print();
      else
	std::cout<<"\n["<<it->first<<"]";
    }
    std::cout
      <<"\n[End Sub Module: "+GetName()+"]";
  }
  std::cout<<"\n[End Module: "+GetName()+"]"<<std::endl;
}
void GPModule::Print(std::ofstream &ofs)
{
  ofs<<"\n[Begin Module: "+GetName()+"]";
  if(runHandle)
    runHandle->Print(ofs);
  else
    ofs<<"\n[Run Handle: Not Set]";
  if(eventHandle)
    eventHandle->Print(ofs);
  else
    ofs<<"\n[Event Handle: Not Set]";
  if(steppingHandle)
    steppingHandle->Print(ofs);
  else
    ofs<<"\n[Stepping Handle: Not Set]";
  if(geometry)
    geometry->Print(ofs);
  else
    ofs<<"\n[Geometry: Not Set]";
  if(mChildModule.size()>0)
  {
    ofs
      <<"\n[Begin Sub Module:]";
    GPModuleMap::iterator it;
    for(it=mChildModule.begin();it!=mChildModule.end();it++)
    {
      if(int(iPrintRecursiveFlag)!=0)
	(it->second)->Print(ofs);
      else
	ofs<<"\n["<<it->first<<"]";
    }
    ofs
      <<"\n[End Sub Module:]";
  }
  ofs<<"\n[End Module: "+GetName()+"]"<<std::endl;
}
void GPModule::SetParameter(std::string sPoolKeyValueUnit,std::string sGlobal)
{
  std::stringstream ss(sPoolKeyValueUnit);
  std::string		  sUnit;
  std::string		  sKey;
  std::string		  sValueOrg;
  G4double   		  dValueNew;
  G4double   		  dValueOrg;
      
  ss>>sKey>>sValueOrg>>sUnit;
  ss.clear();
  ss.str(sValueOrg);
  ss>>dValueOrg;

  if(sUnit!="")
  dValueNew=(dValueOrg*G4UIcommand::ValueOf(sUnit.c_str()))/m;
  else dValueNew=dValueOrg;

  if(sKey=="active.flag")
  {
    SetActive((int)dValueNew);
    return;
  }
  else if(sKey=="print")
  {
    iPrintRecursiveFlag=dValueNew;
    Print();
    return;
  }
  else if(sKey=="print.recursive.flag")
  {
    iPrintRecursiveFlag=dValueNew;
  }
  else if(sKey=="priority")
  {
    iPriority=dValueNew;
  }
  else if(sKey=="compact.flag")
  {
    iCompactRangerFlag=dValueNew;
  }
  else if(sKey=="center.z")
  {
    vCenterChildPosition.setZ(dValueNew);
    return;
  }
  else if(sKey=="set.geometry")
  {
    SetGeometry(sValueOrg);
    return;
  }
  else if(sKey=="delete.geometry")
  {
    DelGeometry();
    return;
  }
  else if(sKey=="set.stepping")
  {
    SetSteppingHandle(sValueOrg);
    return;
  }
  else if(sKey=="delete.stepping")
  {
    DelSteppingHandle();
    return;
  }
  else if(sKey=="set.event")
  {
    SetEventHandle(sValueOrg);
    return;
  }
  else if(sKey=="delete.event")
  {
    DelEventHandle();
    return;
  }
  else if(sKey=="set.run")
  {
    SetRunHandle(sValueOrg);
    return;
  }
  else if(sKey=="delete.run")
  {
    DelRunHandle();
    return;
  }
  else if(sKey=="new.child")
  {
    AddChild(sValueOrg,sUnit);
    return;
  }
  else if(sKey=="delete.child")
  {
    DelChild(sValueOrg);
    return;
  }
  else 
  {
    std::cout<<"The sKey is not exist."<<std::endl;
    return;
  }

  //Update();
  std::cout<<GetName()<<": Set "<<sKey<<": "<< sValueOrg<<" "<<sUnit<<std::endl;
}
void GPModule::SetGeometry(std::string sType)
{
  std::string sChildName=GetName()+"geometry/";
  if(geometry)
  {
    std::cout<<GetName()+": Add geometry handle: [Fatal; geometry handle has exist]"<<std::endl; 
    return;
  }
  geometry = 
    GPGeometryManager::GetInstance()->FindAndBuildGeometry(sType,sChildName,GetName());
  if(geometry)
    std::cout<<GetName()+": Add geometry handle: "+sChildName<<std::endl; 
  else
    std::cout<<GetName()+": Add geometry handle: [Fatal; geometry type does not exist]"<<std::endl; 
}
void GPModule::DelGeometry()
{
  std::string sChildName=GetName()+"geometry/";
  if(geometry)
  {
    std::cout<<GetName()+": Delete geometry handle: "+sChildName<<std::endl; 
    delete geometry;
    geometry=NULL;
    return;
  }
  std::cout<<GetName()+": Delete geometry handle: [Fatal; geometry handle does not exsit]"<<std::endl; 
}
void GPModule::SetSteppingHandle(std::string sType)
{
  std::string sChildName=GetName()+"stepping/";
  if(steppingHandle)
  {
    std::cout<<GetName()+": Add stepping handle: [Fatal; stepping handle has exist]"<<std::endl; 
    return;
  }
  steppingHandle = 
    GPSteppingHandleManager::GetInstance()->FindAndBuildSteppingHandle(sType,sChildName,GetName());
  if(steppingHandle)
    std::cout<<GetName()+": Add stepping handle: "+sChildName<<std::endl; 
  else
    std::cout<<GetName()+": Add stepping handle: [Fatal; stepping handle type does not exist]"<<std::endl; 
}
void GPModule::DelSteppingHandle()
{
  std::string sChildName=GetName()+"stepping/";
  if(steppingHandle)
  {
    std::cout<<GetName()+": Delete stepping handle: "+sChildName<<std::endl; 
    delete steppingHandle;
    steppingHandle=NULL;
    return;
  }
  std::cout<<GetName()+": Delete stepping handle: [Fatal; stepping handle does not exsit]"<<std::endl; 
}
void GPModule::SetEventHandle(std::string sType)
{
  std::string sChildName=GetName()+"event/";
  if(eventHandle)
  {
    std::cout<<GetName()+": Add event handle: [Fatal; event handle has exist]"<<std::endl; 
    return;
  }
  eventHandle = 
    GPEventHandleManager::GetInstance()->FindAndBuildEventHandle(sType,sChildName,GetName());
  if(eventHandle)
    std::cout<<GetName()+": Add event handle: "+sChildName<<std::endl; 
  else
    std::cout<<GetName()+": Add event handle: [Fatal; event handle type does not exist]"<<std::endl; 
}
void GPModule::DelEventHandle()
{
  std::string sChildName=GetName()+"event/";
  if(eventHandle)
  {
    std::cout<<GetName()+": Delete event handle: "+sChildName<<std::endl; 
    delete eventHandle;
    eventHandle=NULL;
    return;
  }
  std::cout<<GetName()+": Delete event handle: [Fatal; event handle does not exsit]"<<std::endl; 
}
void GPModule::SetRunHandle(std::string sType)
{
  std::string sChildName=GetName()+"run/";
  if(runHandle)
  {
    std::cout<<GetName()+": Add run handle: [Fatal; run handle has exist]"<<std::endl; 
    return;
  }
  runHandle = 
    GPRunHandleManager::GetInstance()->FindAndBuildRunHandle(sType,sChildName,GetName());
  if(runHandle)
    std::cout<<GetName()+": Add run handle: "+sChildName<<std::endl; 
  else
    std::cout<<GetName()+": Add run handle: [Fatal; run handle type does not exist]"<<std::endl; 
}
void GPModule::DelRunHandle()
{
  std::string sChildName=GetName()+"run/";
  if(runHandle)
  {
    std::cout<<GetName()+": Delete run handle: "+sChildName<<std::endl; 
    delete runHandle;
    runHandle=NULL;
    return;
  }
  std::cout<<GetName()+": Delete run handle: [Fatal; run handle does not exsit]"<<std::endl; 
}
void GPModule::AddChild(std::string sName,std::string sType)
{
  std::string sChildName=GetName()+sName;
  GPModuleMap::iterator it= mChildModule.find(sChildName);
  if(it!=mChildModule.end()) 
  {
    std::cout<<GetName()+": Add child: [Fatal; this child has exist]"<<std::endl; 
    return;
  }
  GPModule* modu = 
    GPModuleManager::GetInstance()->FindAndBuildModule(sType,sChildName,GetName());
  if(modu)
  {
    std::cout<<GetName()+": Add child: "+sChildName<<std::endl; 
    AddChild(modu);
  }
  else
    std::cout<<GetName()+": Add child: [Fatal; child module type does not exist]"<<std::endl; 
}
void GPModule::DelChild(std::string sValue)
{
  std::string sChildName=GetName()+sValue;
  GPModuleMap::iterator it= mChildModule.find(sChildName);
  if(it!=mChildModule.end())
  {
    std::cout<<GetName()+": Delete child: "+sChildName<<std::endl; 
    delete it->second;
    it->second=NULL;
    mChildModule.erase(sChildName);
    return;
  }
  std::cout<<GetName()+": Delete child: [Fatal; this child does not exsit]"<<std::endl; 
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
