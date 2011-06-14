// $Id: GPGometry.cc,v 1.9 2006/06/29 17:47:19 gunter Granular $
// GEANT4 tag $Name: geant4-09-02 $
//
#include "GPGeometry.hh"

#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"

GPGeometry::GPGeometry()
{}
GPGeometry::~GPGeometry()
{}
void GPGeometry::SetPosition(G4ThreeVector pos)
{
  vPosition = pos;
}
void GPGeometry::SetMotherPositionInGlobalFrame(G4ThreeVector pos)
{
  vPositionInGlobalFrame=pos+vPosition;
}
G4ThreeVector GPGeometry::GetPositionInGlobalFrame()
{
  return vPositionInGlobalFrame;
}
