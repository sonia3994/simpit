// $Id: GPEventHandleManager.cc,v 1.29 2008/01/17 17:31:32 maire Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
// 

#include "GPEventHandleManager.hh"
#include "GPEventHandleStore.hh"
#include "GPEventHandle.hh"
#include "GPModuleStore.hh"
#include "GPModule.hh"

#include "G4Event.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GPEventHandleManager* GPEventHandleManager::instance=NULL;
GPEventHandleManager* GPEventHandleManager::GetInstance()
{
  if(instance==NULL) 
    instance = new GPEventHandleManager();
  return instance;
}
void GPEventHandleManager::Delete()
{
  if(instance)  delete instance;
}
GPEventHandleManager::GPEventHandleManager()
{
}
GPEventHandleManager::~GPEventHandleManager()
{
}
void GPEventHandleManager::BeginOfEventAction(const G4Event* evt)
{
  GPEventHandleMap* mEvtHandle = 
    GPEventHandleStore::GetInstance()->GetEventHandleMap();
  GPEventHandle* evtHandle;
  std::string    sModuleName;
  GPEventHandleMap::iterator it;
  for(it = mEvtHandle->begin();it!=mEvtHandle->end();it++)
  {
    evtHandle=it->second;
    sModuleName=evtHandle->GetFatherName();
    GPModule* module = GPModuleStore::GetInstance()->FindModule(sModuleName);
    if(module==NULL)
      continue;
    if(module->IsActive()&&evtHandle->IsActive())
      evtHandle->BeginOfEventAction(evt);
  }
}

void GPEventHandleManager::EndOfEventAction(const G4Event* evt)
{
  GPEventHandleMap* mEvtHandle = 
    GPEventHandleStore::GetInstance()->GetEventHandleMap();
  GPEventHandle* evtHandle;
  std::string    sModuleName;
  GPEventHandleMap::iterator it;
  for(it = mEvtHandle->begin();it!=mEvtHandle->end();it++)
  {
    evtHandle=it->second;
    sModuleName=evtHandle->GetFatherName();
    GPModule* module = GPModuleStore::GetInstance()->FindModule(sModuleName);
    if(module==NULL)
      continue;
    if(module->IsActive()&&evtHandle->IsActive())
      evtHandle->EndOfEventAction(evt);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
