// $Id: GPModule.cc,v 1.19 2008/01/17 17:31:32 maire Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "GPModule.hh"
#include "GPModuleManager.hh"

#include "GPGeometryGeneral.hh"
#include "GPGeometryStore.hh"
#include "GPSteppingHandle.hh"
#include "GPSteppingHandleStore.hh"
#include "GPEventHandleGeneral.hh"
#include "GPEventHandleStore.hh"
#include "GPRunHandleGeneral.hh"
#include "GPRunHandleStore.hh"

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
  GPGeometry* geometry;
  for(itMod=mChildModule.begin();itMod!=mChildModule.end();itMod++)
  {
    geometry=(GPGeometry*)(itMod->second)->FindObject("geometry");
    if((itMod->second)->IsActive()&&geometry!=NULL)
      (itMod->second)->ConstructGeometry(motherLog);
  }
}
void GPModule::ConstructGeometryCompact(G4LogicalVolume* motherLog )
{
  GPModuleMap::iterator itMod;
  GPModule* module;
  GPGeometry* geometry;
  GPModule* moduleCenter=NULL;
  std::multimap<int,GPModule*> mIntGPModulePos;
  std::multimap<int,GPModule*>::iterator mIntModIt;
  std::multimap<int,GPModule*> mIntGPModuleNeg;
  std::multimap<int,GPModule*>::reverse_iterator mIntModRit;
  int iPriTmp;

  for(itMod=mChildModule.begin();itMod!=mChildModule.end();itMod++)
  {
    module=itMod->second;
    geometry=(GPGeometry*)module->FindObject("geometry");
    if(module->IsActive()&&geometry!=NULL)
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

  ((GPGeometry*) moduleCenter->FindObject("geometry"))
    ->SetPosition(vCenterChildPosition);
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
    ((GPGeometry*) module->FindObject("geometry"))
      ->SetPosition(vItPoint);
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
    ((GPGeometry*) module->FindObject("geometry"))
      ->SetPosition(vItPoint);
    module->ConstructGeometry(motherLog);
  }
  //G4cout<<"GP_DEBUG: Exit GPModule::ConstructGeometryCompact(G4LogicalVolume*): Object Name: "<<GetName()<<G4endl;
}
void GPModule::Print()
{
  std::string sGeoName;
  std::string sSteppingName;
  std::string sEventName;
  std::string sRunName;
  if(geometry)
    sGeoName=geometry->GetName();
  else 
    sGeoName="Not set";

  if(steppingHandle)
    sSteppingName=steppingHandle->GetName();
  else 
    sSteppingName="Not set";

  if(eventHandle)
    sEventName=eventHandle->GetName();
  else 
    sEventName="Not set";
  if(runHandle)
    sRunName=runHandle->GetName();
  else 
    sRunName="Not set";

  if(GetName()==GPModuleManager::GetInstance()->GetRootName())
  {
  std::cout
    <<"\n--------------------Begin Module tree-----------------------"
    <<std::endl;
  }
  std::cout
    <<"\nModule: "<<GetName()
    <<"\nGeometry Name: "<<sGeoName
    <<"\nStepping Handle: "<<sSteppingName
    <<"\nEvent Handle: "<<sEventName
    <<"\nRun Handle: "<<sRunName
    <<std::endl;
  if(mChildModule.size()>0)
  {
    std::cout
      <<"\nSub Module:" 
      <<std::endl;
    GPModuleMap::iterator it;
    for(it=mChildModule.begin();it!=mChildModule.end();it++)
    {
      if(int(iPrintRecursiveFlag)!=0)
	(it->second)->Print();
      else
	std::cout<<it->first<<"\n";
    }
  }
  if(GetName()==GPModuleManager::GetInstance()->GetRootName())
  {
  std::cout
    <<"\n--------------------End Module tree-----------------------"
    <<std::endl;
  }
}
void GPModule::Print(std::ofstream &ofs)
{
  std::string sGeoName;
  std::string sSteppingName;
  std::string sEventName;
  std::string sRunName;
  if(geometry)
    sGeoName=geometry->GetName();
  else 
    sGeoName="Not set";

  if(steppingHandle)
    sSteppingName=steppingHandle->GetName();
  else 
    sSteppingName="Not set";

  if(eventHandle)
    sEventName=eventHandle->GetName();
  else 
    sEventName="Not set";
  if(runHandle)
    sRunName=runHandle->GetName();
  else 
    sRunName="Not set";

  if(GetName()==GPModuleManager::GetInstance()->GetRootName())
  {
  ofs
    <<"\n--------------------Begin Module tree-----------------------"
    <<std::endl;
  }
  ofs
    <<"\nModule: "<<GetName()
    <<"\nGeometry Name: "<<sGeoName
    <<"\nStepping Handle: "<<sSteppingName
    <<"\nEvent Handle: "<<sEventName
    <<"\nRun Handle: "<<sRunName
    <<std::endl;
  if(mChildModule.size()>0)
  {
    ofs
      <<"\nSub Module:" 
      <<std::endl;
    GPModuleMap::iterator it;
    for(it=mChildModule.begin();it!=mChildModule.end();it++)
    {
      if(int(iPrintRecursiveFlag)!=0)
	(it->second)->Print(ofs);
      else
	ofs<<it->first<<"\n";
    }
  }
  if(GetName()==GPModuleManager::GetInstance()->GetRootName())
  {
  ofs
    <<"\n--------------------End Module tree-----------------------"
    <<std::endl;
  }
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
  else if(sKey=="priority")
  {
    iPriority=dValueNew;
    std::cout<<GetName()+": Set "+sKey<<": "<<dValueOrg<<std::endl;
    return;
  }
  else if(sKey=="center.z")
  {
    vCenterChildPosition.setZ(dValueNew);
    return;
  }
  else if(sKey=="set.geometry")
  {
    SetGeometry();
    return;
  }
  else if(sKey=="delete.geometry")
  {
    DelGeometry();
    return;
  }
  else if(sKey=="set.stepping")
  {
    SetSteppingHandle();
    return;
  }
  else if(sKey=="delete.stepping")
  {
    DelSteppingHandle();
    return;
  }
  else if(sKey=="set.event")
  {
    SetEventHandle();
    return;
  }
  else if(sKey=="delete.event")
  {
    DelEventHandle();
    return;
  }
  else if(sKey=="set.run")
  {
    SetRunHandle();
    return;
  }
  else if(sKey=="delete.run")
  {
    DelRunHandle();
    return;
  }
  else if(sKey=="new.child")
  {
    AddChild(sValueOrg);
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

  Update();
  ss.clear();
  ss.str(sGlobal);
  ss>>sKey;
  std::cout<<"Set "<<sKey<<" to "<< dValueOrg<<" "<<sUnit<<std::endl;
}
void GPModule::SetGeometry()
{
  if(geometry)
  {
    std::cout<<GetName()+"geometry/ has exist in: "+GetName()+". just return."<<std::endl; 
    return;
  }
  std::cout<<GetName()+"geometry/ to be added to: "+GetName()<<std::endl; 
  std::string sChildName=GetName()+"geometry/";
  geometry = new GPGeometryGeneral(sChildName,GetName());
}
void GPModule::DelGeometry()
{
  if(geometry)
  {
    std::cout<<GetName()+"geometry/ to be delete."<<std::endl; 
    delete geometry;
    return;
  }
  std::cout<<GetName()+"geometry/ does not exist in: "+GetName()<<std::endl; 
}
void GPModule::SetSteppingHandle()
{
}
void GPModule::DelSteppingHandle()
{
}
void GPModule::SetEventHandle()
{
  if(eventHandle)
  {
    std::cout<<GetName()+"event/ has exist in: "+GetName()+". just return."<<std::endl; 
    return;
  }
  std::cout<<GetName()+"event/ to be added to: "+GetName()<<std::endl; 
  std::string sChildName=GetName()+"event/";
  eventHandle = new GPEventHandleGeneral(sChildName,GetName());
}
void GPModule::DelEventHandle()
{
  if(eventHandle)
  {
    std::cout<<GetName()+"event/ to be delete."<<std::endl; 
    delete eventHandle;
    return;
  }
  std::cout<<GetName()+"event/ does not exist in: "+GetName()<<std::endl; 
}
void GPModule::SetRunHandle()
{
  if(runHandle)
  {
    std::cout<<GetName()+"run/ has exist in: "+GetName()+". just return."<<std::endl; 
    return;
  }
  std::cout<<GetName()+"run/ to be added to: "+GetName()<<std::endl; 
  std::string sChildName=GetName()+"run/";
  runHandle = new GPRunHandleGeneral(sChildName,GetName());
}
void GPModule::DelRunHandle()
{
  if(runHandle)
  {
    std::cout<<GetName()+"run/ to be delete."<<std::endl; 
    delete runHandle;
    return;
  }
  std::cout<<GetName()+"run/ does not exist in: "+GetName()<<std::endl; 
}
void GPModule::AddChild(std::string sValue)
{
  std::string sChildName=GetName()+sValue;
  GPModuleMap::iterator it= mChildModule.find(sChildName);
  if(it!=mChildModule.end()) 
  {
    std::cout<<sChildName+" has exist in: "+GetName()+". just return."<<std::endl; 
    return;
  }
  std::cout<<sChildName+": to be added to: "+GetName()<<std::endl; 
  GPModule* modu = new GPModule(sChildName,GetName());
  AddChild(modu);
}
void GPModule::DelChild(std::string sValue)
{
  std::string sChildName=GetName()+sValue;
  GPModuleMap::iterator it= mChildModule.find(sChildName);
  if(it!=mChildModule.end())
  {
    std::cout<<sChildName+": to be delete."<<std::endl; 
    delete it->second;
    mChildModule.erase(sChildName);
    return;
  }
  std::cout<<sChildName+" does not exist in: "+GetName()<<std::endl; 
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
