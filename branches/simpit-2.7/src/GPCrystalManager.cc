// $Id: GPCrystalManager.cc,v 1.9 2006/06/29 17:47:19 gunter Granular $
// GEANT4 tag $Name: geant4-09-02 $
//
#include "Crystal.h"
#include "RunParameters.h"

#include "GPCrystalManager.hh"

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
  if(instance)  delete instance;
}
GPCrystalManager::GPCrystalManager()
{
}

GPCrystalManager::~GPCrystalManager()
{
  if(crysW) delete crysW;
  if(runPara) delete runPara;
}

void GPCrystalManager::Clear()
{
  if(crysW) delete crysW;
  if(runPara) delete runPara;
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
void GPCrystalManager::SetParameter(std::string str,std::string strGlobal)
{
  /*
	std::stringstream ss(str);
	std::string		  unit;
	std::string		  key;
	G4double   		  dValueNew;
	G4double   		  dValueOrg;
	
	ss>>key>>dValueOrg>>unit;
    if(unit!="")
    dValueNew=(dValueOrg*G4UIcommand::ValueOf(unit.c_str()))/m;
    else dValueNew=dValueOrg;

    if(key=="ir")
    dCrystalTubeInnerRadius = dValueNew;
    else if(key=="or")
    dCrystalTubeOuterRadius = dValueNew;
    else if(key=="l")
    dCrystalTubeLength = dValueNew;
    else if(key=="sa")
    dCrystalTubeStartAngle = dValueNew;
    else if(key=="ea")
    dCrystalTubeSpanningAngle = dValueNew;
    else if(key=="hit.flag")
    iCrystalHitFlag = dValueNew;
    else if(key=="limit.step.max")
    dCrystalLimitStepMax = dValueNew;
    else if(key=="limit.step.flag")
    iCrystalLimitStepFlag = dValueNew;
    else 
    {
  	std::cout<<"the key is not exist."<<std::endl;
     	return;
    }

    Init();
    ss.clear();
    ss.str(strGlobal);
    ss>>key;
    std::cout<<"Set "<<key<<" to "<< dValueOrg<<" "<<unit<<std::endl;
    */
}

G4double GPCrystalManager::GetParameter(std::string name) const
{
  /*
    if(name=="ir")
    return dCrystalTubeInnerRadius;
    else if(name=="or")
    return dCrystalTubeOuterRadius;
    else if(name=="l")
    return dCrystalTubeLength;
    else if(name=="sa")
    return dCrystalTubeStartAngle;
    else if(name=="ea")
    return dCrystalTubeSpanningAngle;
    else if(name=="hit.flag")
    return iCrystalHitFlag;
    else if(name=="limit.step.max")
    return dCrystalLimitStepMax;
    else if(name=="limit.step.flag")
    return iCrystalLimitStepFlag;

    else
    {
      std::cout<<"key does not exist.\n"<<std::endl;
      return -1;
    }
    */
  return 0;
}

