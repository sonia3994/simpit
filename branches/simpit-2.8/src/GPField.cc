// $Id: GPAcceleratorFieldManagerPool.cc,v 1.5 2007/04/28 01:31:12 gum Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
//  
//   Field Setup class implementation.
//

#include "GPField.hh"
#include <fstream>
#include <sstream>
//////////////////////////////////////////////////////////////////////////
GPField::GPField():G4ElectroMagneticField()
{
}

GPField::~GPField()
{
}

void GPField::Init()
{
}

void GPField::Print(std::ofstream& ofsOutput)
{
}
void GPField::Print()
{
}
void GPField::SetGeometryName(std::string s)
{
  sGeometryName=s;
}
void GPField::SetParameter(std::string sLocal, std::string sGlobal)
{
}
double GPField::GetParameter(std::string sLocal, std::string sGlobal) const
{
  return -1;
}
