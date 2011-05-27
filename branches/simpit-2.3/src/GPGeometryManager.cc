// $Id: GPGometry.cc,v 1.9 2006/06/29 17:47:19 gunter Granular $
// GEANT4 tag $Name: geant4-09-02 $
//
#include "GPGeometryManager.hh"
#include "GPGeometryStore.hh"
#include "GPGeometry.hh"

#include "GPModule.hh"
#include "GPModuleStore.hh"

#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"

GPGeometryManager* GPGeometryManager::instance=NULL;
GPGeometryManager* GPGeometryManager::GetInstance()
{
  if(instance==NULL) 
    instance = new GPGeometryManager();
  return instance;
}
void GPGeometryManager::Delete()
{
  if(instance)  delete instance;
}
GPGeometryManager::GPGeometryManager()
{
  iCompactRangerFlag=1;
}


GPGeometryManager::~GPGeometryManager()
{}

