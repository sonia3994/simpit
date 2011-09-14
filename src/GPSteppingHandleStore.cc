// $Id: GPSteppingHandleStore.cc,v 1.15 2006/06/29 17:49:13 gunter Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "GPSteppingHandleStore.hh"
#include "GPSteppingHandle.hh"
#include <iostream>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GPSteppingHandleStore* GPSteppingHandleStore::instance=NULL;
GPSteppingHandleStore* GPSteppingHandleStore::GetInstance()
{
  if(instance==NULL) 
    instance = new GPSteppingHandleStore();
  return instance;
}
void GPSteppingHandleStore::Delete()
{
  if(instance)  delete instance;
}
GPSteppingHandleStore::GPSteppingHandleStore()
{}
GPSteppingHandleStore::~GPSteppingHandleStore()
{
  GPSteppingHandleMap::iterator it;
  for(it=mStrSteppingHandle.begin(); it!=mStrSteppingHandle.end(); it++)
  {
    delete it->second;
  }
  mStrSteppingHandle.clear();
}
void GPSteppingHandleStore::AddSteppingHandle(std::string skey, GPSteppingHandle* steppingHandle)
{
  GPSteppingHandleMap::iterator it = mStrSteppingHandle.find(skey);
  if(it!=mStrSteppingHandle.end())
  {
    std::cout<<"Create GPSteppingHandle twice! Erase the old one."<<std::endl;
    mStrSteppingHandle.erase(skey);
  }
  mStrSteppingHandle[skey] = steppingHandle;
}

void GPSteppingHandleStore::DelSteppingHandle(std::string skey)
{
  GPSteppingHandleMap::iterator it = mStrSteppingHandle.find(skey);
  if(it==mStrSteppingHandle.end())
  {
    std::cout<<"GPSteppingHandle: "<<skey<<" not exists."<<std::endl;
    return;
  }
  delete it->second;
  mStrSteppingHandle.erase(skey);
}
void GPSteppingHandleStore::Clear()
{
  GPSteppingHandleMap::iterator it;
  for(it=mStrSteppingHandle.begin(); it!=mStrSteppingHandle.end(); it++)
  {
    delete it->second;
  }
  mStrSteppingHandle.clear();
}
GPSteppingHandle* GPSteppingHandleStore::FindSteppingHandle(std::string skey)
{
  GPSteppingHandleMap::iterator it = mStrSteppingHandle.find(skey);
  GPSteppingHandle* steppingHandle;
  if(it==mStrSteppingHandle.end())
    steppingHandle=NULL;
  else
    steppingHandle=it->second;
    
  return steppingHandle;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
