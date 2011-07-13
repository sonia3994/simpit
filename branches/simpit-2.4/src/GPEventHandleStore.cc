// $Id: GPEventHandleStore.cc,v 1.29 2008/01/17 17:31:32 maire Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
// 

#include "GPEventHandleStore.hh"
#include "GPEventHandle.hh"

#include "G4Event.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GPEventHandleStore* GPEventHandleStore::instance=NULL;
GPEventHandleStore* GPEventHandleStore::GetInstance()
{
  if(instance==NULL) 
    instance = new GPEventHandleStore();
  return instance;
}
void GPEventHandleStore::Delete()
{
  if(instance)  delete instance;
}
GPEventHandleStore::GPEventHandleStore()
{}
GPEventHandleStore::~GPEventHandleStore()
{
  GPEventHandleMap::iterator it;
  for(it=mStrEvtHangle.begin(); it!=mStrEvtHangle.end(); it++)
  {
    delete it->second;
  }
  mStrEvtHangle.clear();
}
void GPEventHandleStore::AddEventHandle(std::string skey, GPEventHandle* evtHandle)
{
  GPEventHandleMap::iterator it = mStrEvtHangle.find(skey);
  if(it!=mStrEvtHangle.end())
  {
    std::cout<<"Create GPEventHandle twice! Erase the old one."<<std::endl;
    mStrEvtHangle.erase(skey);
  }
  mStrEvtHangle[skey] = evtHandle;
}

void GPEventHandleStore::DelEventHandle(std::string skey)
{
  GPEventHandleMap::iterator it = mStrEvtHangle.find(skey);
  if(it==mStrEvtHangle.end())
  {
    std::cout<<"GPEventHandle: "<<skey<<" not exists."<<std::endl;
    return;
  }
  delete it->second;
  mStrEvtHangle.erase(skey);
}
void GPEventHandleStore::Clear()
{
  GPEventHandleMap::iterator it;
  for(it=mStrEvtHangle.begin(); it!=mStrEvtHangle.end(); it++)
  {
    delete it->second;
  }
  mStrEvtHangle.clear();
}
GPEventHandle* GPEventHandleStore::FindEventHandle(std::string skey)
{
  GPEventHandleMap::iterator it = mStrEvtHangle.find(skey);
  GPEventHandle* evtHandle;
  if(it==mStrEvtHangle.end())
    evtHandle=NULL;
  else
    evtHandle=it->second;
    
  return evtHandle;
}
void GPEventHandleStore::EraseItem(std::string skey)
{
  mStrEvtHangle.erase(skey);
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
