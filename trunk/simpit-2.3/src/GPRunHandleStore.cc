// $Id: GPRunHandleStore.cc,v 1.19 2008/01/17 17:31:32 maire Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "GPRunHandleStore.hh"
#include "GPRunHandle.hh"
#include <iostream>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
GPRunHandleStore* GPRunHandleStore::instance=NULL;
GPRunHandleStore* GPRunHandleStore::GetInstance()
{
  if(instance==NULL) 
    instance = new GPRunHandleStore();
  return instance;
}
void GPRunHandleStore::Delete()
{
  if(instance)  delete instance;
}
GPRunHandleStore::GPRunHandleStore()
{}
GPRunHandleStore::~GPRunHandleStore()
{
  GPRunHandleMap::iterator it;
  for(it=mStrRunHandle.begin(); it!=mStrRunHandle.end(); it++)
  {
    delete it->second;
  }
  mStrRunHandle.clear();
}
void GPRunHandleStore::AddRunHandle(std::string skey, GPRunHandle* runHandle)
{
  GPRunHandleMap::iterator it = mStrRunHandle.find(skey);
  if(it!=mStrRunHandle.end())
  {
    std::cout<<"Create GPRunHandle twice! Erase the old one."<<std::endl;
    mStrRunHandle.erase(skey);
  }
  mStrRunHandle[skey] = runHandle;
}

void GPRunHandleStore::DelRunHandle(std::string skey)
{
  GPRunHandleMap::iterator it = mStrRunHandle.find(skey);
  if(it==mStrRunHandle.end())
  {
    std::cout<<"GPRunHandle: "<<skey<<" not exists."<<std::endl;
    return;
  }
  delete it->second;
  mStrRunHandle.erase(skey);
}
void GPRunHandleStore::EraseItem(std::string skey)
{
  mStrRunHandle.erase(skey);
}
void GPRunHandleStore::Clear()
{
  GPRunHandleMap::iterator it;
  for(it=mStrRunHandle.begin(); it!=mStrRunHandle.end(); it++)
  {
    delete it->second;
  }
  mStrRunHandle.clear();
}
GPRunHandle* GPRunHandleStore::FindRunHandle(std::string skey)
{
  GPRunHandleMap::iterator it = mStrRunHandle.find(skey);
  GPRunHandle* runHandle;
  if(it==mStrRunHandle.end())
    runHandle=NULL;
  else
    runHandle=it->second;
    
  return runHandle;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
