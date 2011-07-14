// $Id: GPSolidManager.cc,v 1.9 2006/06/29 17:47:19 gunter Granular $
// GEANT4 tag $Name: geant4-09-02 $
//
#include "GPSolidManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Orb.hh"
#include "G4UIcommand.hh"
#include "globals.hh"

#include <sstream>
#include <cmath>
#include <algorithm>
GPSolidManager::GPSolidManager(std::string sName, std::string sFatherName)
  :solid(0)
{
  SetActive(1);
  SetName(sName);
  SetFatherName(sFatherName);

  sSolidType ="G4Box";
  sBaseNameChild = "-"+GetName();
  std::replace(sBaseNameChild.begin(),sBaseNameChild.end(),'/','_');
  dLength = 10e-3;
  dWidth  = 10e-3;
  dHeight = 10e-3;
  dRadiusInner = 0;
  dAngleStart  = 0;
  dAngleEnd = 360 ;
}

GPSolidManager::~GPSolidManager()
{
}

void GPSolidManager::Update()
{
  Init();
}
G4VSolid* GPSolidManager::ConstructSolid()
{
  if(sSolidType=="G4Box")
  {
    G4VSolid* sol = new G4Box(sBaseNameChild+"solid",
	m*dWidth/2,
	m*dHeight/2,
	m*dLength/2);
    return sol;
  }
  else 
  {
    G4VSolid* sol= new G4Tubs(sBaseNameChild+"solid",
	m*dRadiusInner,
	m*dWidth/2.0,
	m*dLength/2.0,
	deg*dAngleStart,
	deg*dAngleEnd);
    return sol;
  }

  return NULL;

}
void GPSolidManager::Init()
{
}

void GPSolidManager::Print()
{
  G4cout
    <<"\n[Begin Solid: "+GetName()+"]"
    <<"\nSolid: "+sSolidType
    <<"\nLength of Geometry: "<<dLength*m/mm<<" mm"
    <<"\nWidth of Geometry: "<<dWidth*m/mm<<" mm"
    <<"\nHeight of Geometry: "<<dHeight*m/mm<<" mm"
    <<"\n[End Solid: "+GetName()+"]"
    <<G4endl;
   
}

void GPSolidManager::SetParameter(std::string str,std::string sGlobal)
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

    if(sKey=="inner.radius")
      dRadiusInner = dValueNew;
    else if(sKey=="width")
      dWidth = dValueNew;
    else if(sKey=="height")
      dHeight = dValueNew;
    else if(sKey=="length")
      dLength = dValueNew;
    else if(sKey=="agnle.start")
      dAngleStart = dValueNew;
    else if(sKey=="angle.end")
      dAngleEnd = dValueNew;
    else if(sKey=="type")
    {
      SetSolidType(sValue);
      return;
    }
    else 
    {
      std::cout<<GetName()<<": "+sKey+": Key does not exist."<<std::endl;
      return;
    }

    std::cout<<GetName()<<": Set "<<sKey<<": "<< sValue<<" "<<sUnit<<std::endl;
}

G4double GPSolidManager::GetParameter(std::string sKey, std::string sGlobal) const
{
  if(sKey=="inner.radius")
    return dRadiusInner; 
  else if(sKey=="width")
    return dWidth; 
  else if(sKey=="height")
    return dHeight; 
  else if(sKey=="length")
    return dLength; 
  else if(sKey=="agnle.start")
    return dAngleStart; 
  else if(sKey=="angle.end")
    return dAngleEnd; 
  //else if(sKey=="solid.type")
  //  return sSolidType;
  //else if(sKey=="material")
  //  return sMaterial;
  else
    {
      std::cout<<GetName()<<": "+sKey+": Key does not exist."<<std::endl;
      return -1;
    }
}

void GPSolidManager::Print(std::ofstream& fstOutput)
{
  fstOutput
    <<"\n[Begin Solid: "+GetName()+"]"
    <<"\nSolid: "+sSolidType
    <<"\nLength of Geometry: "<<dLength*m/mm<<" mm"
    <<"\nWidth of Geometry: "<<dWidth*m/mm<<" mm"
    <<"\nHeight of Geometry: "<<dHeight*m/mm<<" mm"
    <<"\n[End Solid: "+GetName()+"]"
    <<G4endl;
}
void GPSolidManager::SetSolidType(std::string sValue)
{
  if(sValue=="G4Box")
  {
    sSolidType=sValue;
    std::cout<<GetName()+": Set Solid Type: "+sSolidType<<std::endl;
  }
  else if(sValue=="G4Tubs")
  {
    sSolidType=sValue;
    std::cout<<GetName()+": Set Solid Type: "+sSolidType<<std::endl;
  }
  else
  {
    std::cout<<"This Solid Type does not exist: "<<sValue<<std::endl;
  }

}

bool GPSolidManager::IsInThisSolid(G4ThreeVector vLocal)
{
  if(abs(vLocal.x())<=dWidth/2&&abs(vLocal.y())<=dHeight/2&&abs(vLocal.z())<=dLength/2)
    return true;
  else return false;
}
