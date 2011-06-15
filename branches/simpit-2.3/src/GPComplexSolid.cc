// $Id: GPComplexSolid.cc,v 1.9 2006/06/29 17:47:19 gunter Granular $
// GEANT4 tag $Name: geant4-09-02 $
//

#include "GPComplexSolid.hh"
#include "GPSolidManager.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVReplica.hh"
#include "G4VisAttributes.hh"
#include "G4Orb.hh"
#include "G4UIcommand.hh"

#include "globals.hh"

#include <sstream>
#include <algorithm>
GPComplexSolid::GPComplexSolid(std::string sName, std::string sFatherName)
{
  SetActive(1);
  SetName(sName);
  SetFatherName(sFatherName);
}
GPComplexSolid::GPComplexSolid()
{
}

GPComplexSolid::~GPComplexSolid()
{
}

void GPComplexSolid::Construct(G4LogicalVolume* pMotherLog, GPSolidManager* pSolidManager,G4ThreeVector vPoint)
{
}

void GPComplexSolid::Init()
{
}

void GPComplexSolid::Print()
{
}

void GPComplexSolid::SetParameter(std::string str,std::string strGlobal)
{
}

G4double GPComplexSolid::GetParameter(std::string sKey,std::string sGlobal) const
{
}


void GPComplexSolid::Print(std::ofstream& fstOutput)
{
}
void GPComplexSolid::SetMaterial(std::string sValue)
{
}
