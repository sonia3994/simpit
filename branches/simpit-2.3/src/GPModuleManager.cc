// $Id: GPModuleManager.cc,v 1.19 2008/01/17 17:31:32 maire Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "GPModuleManager.hh"
#include "GPModuleManagerMessenger.hh"
#include "GPModuleStore.hh"
#include "GPModule.hh"

#include "GPGeometryGeneral.hh"
#include "GPGeometryStore.hh"
#include "GPSteppingHandleStore.hh"
#include "GPSteppingHandle.hh"
#include "GPEventHandleStore.hh"
#include "GPEventHandle.hh"
#include "GPRunHandleStore.hh"
#include "GPRunHandle.hh"

#include "GPEventHandleCrystal.hh"
#include "GPCrystalGeometry.hh"
#include "GPSweeperGeometry.hh"
#include "GPTargetGeometry.hh"
#include "GPCaptureGeometry.hh"
#include "GPAcceleratorGeometry.hh"

#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GPModuleManager* GPModuleManager::instance=NULL;
GPModuleManager* GPModuleManager::GetInstance()
{
  if(instance==NULL) 
    instance = new GPModuleManager();
  return instance;
}
void GPModuleManager::Delete()
{
  if(instance)  delete instance;
}
GPModuleManager::GPModuleManager():
  worldModule(0)
{
  iConstructedFlag=0;
  sRootName="/";
  moduleManagerMessenger = new GPModuleManagerMessenger();
}


GPModuleManager::~GPModuleManager()
{
  if(worldModule)
  {
    delete worldModule;
  }
  delete moduleManagerMessenger;
}
void GPModuleManager::ClearModule()
{
  if(worldModule)
    delete worldModule;
}

void GPModuleManager::ConstructModule()
{
  if(worldModule)
  {
    delete worldModule;
    GPGeometryStore::GetInstance()->Clear();
    GPSteppingHandleStore::GetInstance()->Clear();
    GPEventHandleStore::GetInstance()->Clear();
    GPRunHandleStore::GetInstance()->Clear();
    GPModuleStore::GetInstance()->Clear();
  }
  ConstructRoot();
  ConstructCrystal();
  ConstructSweeper();
  ConstructTarget();
  ConstructCapture();
  ConstructAccelerator();
  iConstructedFlag=1;              
  //GPModuleStore::GetInstance()->Print();
}
G4VPhysicalVolume* GPModuleManager::ConstructGeometry()
{
  if(!worldModule)
    ConstructModule();

  G4LogicalVolume* log=NULL;
  return worldModule->ConstructGeometry(log);
}
void GPModuleManager::ConstructRoot()
{
  worldModule = new GPModule(sRootName,"");
  worldModule->SetPriority(0);
  GPGeometryGeneral* geometry = new GPGeometryGeneral(sRootName+"geometry/",sRootName);
  geometry->SetParameter("width 2 m","width 2 m");
  geometry->SetParameter("length 10 m","length 10 m");
  geometry->SetParameter("height 2 m","height 2 m");
  worldModule ->SetObject("geometry",geometry);
}
void GPModuleManager::ConstructCrystal()
{
  std::string sName=sRootName+"crystal/";
  GPModule* crystalModule = new GPModule(sName,sRootName);
  crystalModule->SetPriority(-2);
  worldModule->AddChild(crystalModule);

  GPCrystalGeometry* geometry = new GPCrystalGeometry(sName+"geometry/",sName);
  geometry->SetPriority(-2);
  crystalModule->SetObject("geometry",geometry);
  GPEventHandleCrystal* evtCrystal = new GPEventHandleCrystal(sName+"event/",sName);
  crystalModule->SetObject("event",evtCrystal);
}
void GPModuleManager::ConstructSweeper()
{
  std::string sName=sRootName+"sweeper/";
  GPModule* sweeperModule = new GPModule(sName,sRootName);
  sweeperModule->SetPriority(-1);
  worldModule->AddChild(sweeperModule);

  GPSweeperGeometry* geometry = new GPSweeperGeometry(sName+"geometry/",sName);
  geometry->SetPriority(-1);
  sweeperModule->SetObject("geometry",geometry);
  //GPEventHandleSweeper* evtSweeper = new GPEventHandleSweeper(sName+"event/",sName);
  //sweeperModule->SetObject("event",evtSweeper);
}
void GPModuleManager::ConstructTarget()
{
  std::string sName=sRootName+"target/";
  GPModule* targetModule = new GPModule(sName,sRootName);
  targetModule->SetPriority(0);
  worldModule->AddChild(targetModule);

  GPTargetGeometry* geometry = new GPTargetGeometry(sName+"geometry/",sName);
  geometry->SetPriority(0);
  targetModule->SetObject("geometry",geometry);
  //GPEventHandleTarget* evtTarget = new GPEventHandleTarget(sName+"event/",sName);
  //targetModule->SetObject("event",evtTarget);
}
void GPModuleManager::ConstructCapture()
{
  std::string sName=sRootName+"capture/";
  GPModule* captureModule = new GPModule(sName,sRootName);
  captureModule->SetPriority(1);
  worldModule->AddChild(captureModule);

  GPCaptureGeometry* geometry = new GPCaptureGeometry(sName+"geometry/",sName);
  geometry->SetPriority(1);
  captureModule->SetObject("geometry",geometry);
  //GPEventHandleCapture* evtCapture = new GPEventHandleCapture(sName+"event/",sName);
  //captureModule->SetObject("event",evtCapture);
}
void GPModuleManager::ConstructAccelerator()
{
  std::string sName=sRootName+"accelerator/";
  GPModule* acceleratorModule = new GPModule(sName,sRootName);
  acceleratorModule->SetPriority(2);
  worldModule->AddChild(acceleratorModule);

  GPAcceleratorGeometry* geometry = new GPAcceleratorGeometry(sName+"geometry/",sName);
  geometry->SetPriority(2);
  acceleratorModule->SetObject("geometry",geometry);
  //GPEventHandleAccelerator* evtAccelerator = new GPEventHandleAccelerator(sName+"event/",sName);
  //acceleratorModule->SetObject("event",evtAccelerator);
}

void GPModuleManager::Print()
{
  GPModuleStore::GetInstance()->FindModule(sRootName)->Print();
}
void GPModuleManager::Print(std::ofstream& ofs)
{
  GPModuleStore::GetInstance()->FindModule(sRootName)->Print(ofs);
}
void GPModuleManager::Update()
{
}
void GPModuleManager::SetParameter(std::string sGlobal)
{
  /*
    std::string sLocal=sGlobal;
    std::string sObjectName;
    std::string sPoolKeyValueUnit;
    size_t iFirstDot;
    iFirstDot = sLocal.find(".");
    if(iFirstDot!=std::string::npos)
    {
    sObjectName=sLocal.substr(0,iFirstDot);
    sPoolKeyValueUnit=sLocal.substr(iFirstDot+1);
    }
    */

    std::string sLocal=sGlobal;
    std::stringstream sstr(sLocal);
    std::string sObjectName;
    sstr>>sObjectName;
    std::string sPoolKeyValueUnit;
    size_t iFirstDot = sLocal.find(sObjectName);
    if(iFirstDot!=std::string::npos)
    {
    sPoolKeyValueUnit=sLocal.substr(iFirstDot+sObjectName.size());
    }

    if(GPModuleStore::GetInstance()->FindModule(sObjectName))
    {
      GPModuleStore::GetInstance()->FindModule(sObjectName)
	->SetParameter(sPoolKeyValueUnit,sGlobal);
    }
    else if(GPGeometryStore::GetInstance()->FindGeometry(sObjectName))
    {
      GPGeometryStore::GetInstance()->FindGeometry(sObjectName)
	->SetParameter(sPoolKeyValueUnit,sGlobal);
    }
    else if(GPSteppingHandleStore::GetInstance()->FindSteppingHandle(sObjectName))
    {
      GPSteppingHandleStore::GetInstance()->FindSteppingHandle(sObjectName)
	->SetParameter(sPoolKeyValueUnit,sGlobal);
    }
    else if(GPEventHandleStore::GetInstance()->FindEventHandle(sObjectName))
    {
      GPEventHandleStore::GetInstance()->FindEventHandle(sObjectName)
	->SetParameter(sPoolKeyValueUnit,sGlobal);
    }
    else if(GPRunHandleStore::GetInstance()->FindRunHandle(sObjectName))
    {
      GPRunHandleStore::GetInstance()->FindRunHandle(sObjectName)
	->SetParameter(sPoolKeyValueUnit,sGlobal);
    }
    else
    {
      std::cout<<"Can't find the Object: "<<sObjectName<<std::endl;
    }

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
