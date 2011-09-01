// $Id: GPGometry.cc,v 1.9 2006/06/29 17:47:19 gunter Granular $
// GEANT4 tag $Name: geant4-09-02 $
//
#include "GPGeometryStore.hh"
#include "GPGeometry.hh"

GPGeometryStore* GPGeometryStore::instance=NULL;
GPGeometryStore* GPGeometryStore::GetInstance()
{
  if(instance==NULL) 
    instance = new GPGeometryStore();
  return instance;
}
void GPGeometryStore::Delete()
{
  if(instance)  delete instance;
}
GPGeometryStore::GPGeometryStore()
{}
GPGeometryStore::~GPGeometryStore()
{
  GPGeometryMap::iterator it;
  for(it=mStrGeometry.begin(); it!=mStrGeometry.end(); it++)
  {
    delete it->second;
  }
  mStrGeometry.clear();
}
void GPGeometryStore::AddGeometry(std::string skey, GPGeometry* geometry)
{
  GPGeometryMap::iterator it = mStrGeometry.find(skey);
  if(it!=mStrGeometry.end())
  {
    std::cout<<"Create GPGeometry twice! Erase the old one."<<std::endl;
    mStrGeometry.erase(skey);
  }
  mStrGeometry[skey] = geometry;
}

void GPGeometryStore::DelGeometry(std::string skey)
{
  GPGeometryMap::iterator it = mStrGeometry.find(skey);
  if(it==mStrGeometry.end())
  {
    std::cout<<"GPGeometry: "<<skey<<" not exists."<<std::endl;
    return;
  }
  delete it->second;
  mStrGeometry.erase(skey);
}
void GPGeometryStore::Clear()
{
  GPGeometryMap::iterator it;
  for(it=mStrGeometry.begin(); it!=mStrGeometry.end(); it++)
  {
    delete it->second;
  }
  mStrGeometry.clear();
}
GPGeometry* GPGeometryStore::FindGeometry(std::string skey)
{
  GPGeometryMap::iterator it = mStrGeometry.find(skey);
  GPGeometry* geometry;
  if(it==mStrGeometry.end())
    geometry=NULL;
  else
    geometry=it->second;
    
  return geometry;
}
void GPGeometryStore::EraseItem(std::string skey)
{
  mStrGeometry.erase(skey);
}
