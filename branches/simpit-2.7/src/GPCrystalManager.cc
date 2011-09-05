// $Id: GPCrystalManager.cc,v 1.9 2006/06/29 17:47:19 gunter Granular $
// GEANT4 tag $Name: geant4-09-02 $
//
#include "Crystal.h"
#include "RunParameters.h"

#include "GPCrystalManager.hh"
#include "GPCrystalManagerMessenger.hh"
#include "GPModuleManager.hh"

#include "G4UIcommand.hh"

#include "globals.hh"

#include <sstream>
GPCrystalManager* GPCrystalManager::instance=NULL;
GPCrystalManager* GPCrystalManager::GetInstance()
{
  if(instance==NULL) 
    instance = new GPCrystalManager();
  return instance;
}
void GPCrystalManager::Delete()
{
}
GPCrystalManager::GPCrystalManager()
{
  crysW = new Crystal("W",111);
  runPara = new RunParameters(*crysW);
  pMessenger = new GPCrystalManagerMessenger();
}

GPCrystalManager::~GPCrystalManager()
{
  if(crysW) delete crysW;
  if(runPara) delete runPara;
}

void GPCrystalManager::Clear()
{
}

void GPCrystalManager::SetCrystal(Crystal* value)
{
  crysW = value;
}

void GPCrystalManager::SetRunPara(RunParameters* value)
{
  runPara= value;
}

void GPCrystalManager::Print()
{

}

void GPCrystalManager::Print(std::ofstream& fstOutput)
{

}
void GPCrystalManager::SetParameter(std::string str)
{
  std::stringstream ss(str);
  std::string		  sUnit;
  std::string		  sKey;
  std::string		  sValue;
  G4double   		  dValueNew;
  G4double   		  dValueOrg;

  ss>>sKey>>sValue>>sUnit;
  ss.clear();
  ss.str(sValue);
  ss>>dValueOrg;
  if(sUnit!="")
    dValueNew=(dValueOrg*G4UIcommand::ValueOf(sUnit.c_str()))/m;
  else dValueNew=dValueOrg;

  if(sKey=="geometry")
  {
    return SetGeometry(sValue);
  }
  else 
  {
    std::cout<<"The Key is not exist."<<std::endl;
    return;
  }

  ss.clear();
  ss.str(str);
  ss>>sKey;
  std::cout<<"Set "<<sKey<<" to "<< sValue<<" "<<sUnit<<std::endl;
}

G4double GPCrystalManager::GetParameter(std::string name) const
{
}

void GPCrystalManager::SetGeometry(std::string sValue)
{
  G4double dLength = GPModuleManager::GetInstance()
    ->GetParameter(sValue+" length");
  if(dLength>0.0)
  {
    std::cout<<"CrystalManager Set crystal geometry: "+sValue
      <<"\nCrystal length: "<<dLength<<" m"
      <<std::endl;
    runPara->setZexit(dLength*m/angstrom); //?? G4Fot unit of length is angstrom;
    sGeometryName=sValue;
  }
  else
  {
    std::cout<<"CrystalManager Set crystal geometry fatal: "+sValue
      <<std::endl;
  }
}
std::string GPCrystalManager::GetGeometry()
{
  return sGeometryName;
}
