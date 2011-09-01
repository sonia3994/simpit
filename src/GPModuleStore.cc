// $Id: GPModuleStore.cc,v 1.19 2008/01/17 17:31:32 maire Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "GPModuleStore.hh"
#include <iostream>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GPModuleStore* GPModuleStore::instance=NULL;
GPModuleStore* GPModuleStore::GetInstance()
{
  if(instance==NULL) 
    instance = new GPModuleStore();
  return instance;
}
void GPModuleStore::Delete()
{
  if(instance)  delete instance;
}
GPModuleStore::GPModuleStore()
{}
GPModuleStore::~GPModuleStore()
{
  GPModuleMap::iterator it;
  for(it=mStrModule.begin(); it!=mStrModule.end(); it++)
  {
    delete it->second;
  }
  mStrModule.clear();
}
void GPModuleStore::AddModule(std::string sKey, GPModule* module)
{
  GPModuleMap::iterator it = mStrModule.find(sKey);
  if(it!=mStrModule.end())
  {
    std::cout<<"Create GPModule twice! Erase the old one."<<std::endl;
    mStrModule.erase(sKey);
  }
  mStrModule[sKey] = module;
}

void GPModuleStore::DelModule(std::string sKey)
{
  GPModuleMap::iterator it = mStrModule.find(sKey);
  if(it==mStrModule.end())
  {
    std::cout<<"GPModule: "<<sKey<<" not exists."<<std::endl;
    return;
  }
  delete it->second;
  mStrModule.erase(sKey);
}
void GPModuleStore::Clear()
{
  GPModuleMap::iterator it;
  for(it=mStrModule.begin(); it!=mStrModule.end(); it++)
  {
    delete it->second;
  }
  mStrModule.clear();
}
GPModule* GPModuleStore::FindModule(std::string sKey)
{
  GPModuleMap::iterator it = mStrModule.find(sKey);
  GPModule* module;
  if(it==mStrModule.end())
  {
    module=NULL;
  }
  else
    module=it->second;
    
  return module;
}
void GPModuleStore::Print()
{
  GPModuleMap::iterator it;
  std::cout<<"Module map:"<<std::endl;
  for(it=mStrModule.begin(); it!=mStrModule.end(); it++)
  {
    std::cout
      <<"key: "<<it->first <<"; value: "<<it->second
      <<std::endl;
  }
}
void GPModuleStore::EraseItem(std::string sKey)
{
  mStrModule.erase(sKey); 
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
