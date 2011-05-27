// $Id: GPSteppingHandleManager.cc,v 1.15 2006/06/29 17:49:13 gunter Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "GPSteppingHandleManager.hh"
#include "GPSteppingHandleStore.hh"
#include "GPSteppingHandle.hh"
#include "GPModuleStore.hh"
#include "GPModule.hh"
#include "G4Step.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GPSteppingHandleManager* GPSteppingHandleManager::instance=NULL;
GPSteppingHandleManager* GPSteppingHandleManager::GetInstance()
{
  if(instance==NULL) 
    instance = new GPSteppingHandleManager();
  return instance;
}
void GPSteppingHandleManager::Delete()
{
  if(instance)  delete instance;
}
GPSteppingHandleManager::GPSteppingHandleManager()
{}


GPSteppingHandleManager::~GPSteppingHandleManager()
{}

void GPSteppingHandleManager::UserSteppingAction(const G4Step* aStep)
{
  GPSteppingHandleMap* mSteppingHandle = 
    GPSteppingHandleStore::GetInstance()->GetSteppingHandleMap();
  GPSteppingHandle* steppingHandle;
  std::string    sModuleName;
  GPSteppingHandleMap::iterator it;
  for(it = mSteppingHandle->begin();it!=mSteppingHandle->end();it++)
  {
    steppingHandle=it->second;
    sModuleName=steppingHandle->GetFatherName();
    GPModule* module = GPModuleStore::GetInstance()->FindModule(sModuleName);
    if(module==NULL)
      continue;
    if(module->IsActive()&&steppingHandle->IsActive())
      steppingHandle->UserSteppingAction(aStep);
  }
}

void GPSteppingHandleManager::Prepare()
{
  GPSteppingHandleMap* mSteppingHandle = 
    GPSteppingHandleStore::GetInstance()->GetSteppingHandleMap();
  GPSteppingHandle* steppingHandle;
  std::string    sModuleName;
  GPSteppingHandleMap::iterator it;
  for(it = mSteppingHandle->begin();it!=mSteppingHandle->end();it++)
  {
    steppingHandle=it->second;
    sModuleName=steppingHandle->GetFatherName();
    GPModule* module = GPModuleStore::GetInstance()->FindModule(sModuleName);
    if(module==NULL)
      continue;
    if(module->IsActive()&&steppingHandle->IsActive())
      steppingHandle->Prepare();
  }
}
void GPSteppingHandleManager::CleanUp()
{
  GPSteppingHandleMap* mSteppingHandle = 
    GPSteppingHandleStore::GetInstance()->GetSteppingHandleMap();
  GPSteppingHandle* steppingHandle;
  std::string    sModuleName;
  GPSteppingHandleMap::iterator it;
  for(it = mSteppingHandle->begin();it!=mSteppingHandle->end();it++)
  {
    steppingHandle=it->second;
    sModuleName=steppingHandle->GetFatherName();
    GPModule* module = GPModuleStore::GetInstance()->FindModule(sModuleName);
    if(module==NULL)
      continue;
    if(module->IsActive()&&steppingHandle->IsActive())
      steppingHandle->CleanUp();
  }
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
