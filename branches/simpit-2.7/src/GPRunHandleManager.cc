// $Id: GPRunHandleManager.cc,v 1.19 2008/01/17 17:31:32 maire Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "GPRunHandleManager.hh"
#include "GPRunHandleStore.hh"
#include "GPRunHandle.hh"
#include "GPRunHandleGeneral.hh"
#include "GPModuleStore.hh"
#include "GPModule.hh"
#include "G4Run.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GPRunHandleManager* GPRunHandleManager::instance=NULL;
GPRunHandleManager* GPRunHandleManager::GetInstance()
{
  if(instance==NULL) 
    instance = new GPRunHandleManager();
  return instance;
}
void GPRunHandleManager::Delete()
{
  if(instance)  delete instance;
}
GPRunHandleManager::GPRunHandleManager()
{}


GPRunHandleManager::~GPRunHandleManager()
{}

void GPRunHandleManager::BeginOfRunAction(const G4Run* run)
{
  GPRunHandleMap* mRunHandle = 
    GPRunHandleStore::GetInstance()->GetRunHandleMap();
  GPRunHandle* runHandle;
  std::string    sModuleName;
  GPRunHandleMap::iterator it;
  for(it = mRunHandle->begin();it!=mRunHandle->end();it++)
  {
    runHandle=it->second;
    sModuleName=runHandle->GetFatherName();
    GPModule* module = GPModuleStore::GetInstance()->FindModule(sModuleName);
    if(module==NULL)
      continue;
    if(module->IsActive()&&runHandle->IsActive())
      runHandle->BeginOfRunAction(run);
  }
}
void GPRunHandleManager::EndOfRunAction(const G4Run* run)
{
  GPRunHandleMap* mRunHandle = 
    GPRunHandleStore::GetInstance()->GetRunHandleMap();
  GPRunHandle* runHandle;
  std::string    sModuleName;
  GPRunHandleMap::iterator it;
  for(it = mRunHandle->begin();it!=mRunHandle->end();it++)
  {
    runHandle=it->second;
    sModuleName=runHandle->GetFatherName();
    GPModule* module = GPModuleStore::GetInstance()->FindModule(sModuleName);
    if(module==NULL)
      continue;
    if(module->IsActive()&&runHandle->IsActive())
      runHandle->EndOfRunAction(run);
  }
}
GPRunHandle* GPRunHandleManager::FindAndBuildRunHandle(std::string sType,std::string sName,std::string sFather)
{
  GPRunHandle* pRunHandle;
  if(sType=="GPRunHandleGeneral"||sType==""||sType=="default")
    pRunHandle = new GPRunHandleGeneral(sName,sFather);
  else
    pRunHandle = NULL;

  return pRunHandle;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
