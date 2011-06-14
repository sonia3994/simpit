// $Id: GPGeometryGeneral.cc,v 1.9 2006/06/29 17:47:19 gunter Granular $
// GEANT4 tag $Name: geant4-09-02 $
//
#include "GPGeometryGeneral.hh"
#include "GPSurfaceParticleScorer.hh"
#include "GPFieldSetup.hh"
#include "GPSensitiveHandle.hh"

#include "GPGeometryStore.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Para.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVReplica.hh"
#include "G4VisAttributes.hh"
#include "G4Orb.hh"
#include "G4UIcommand.hh"
#include "G4SDManager.hh"
#include "G4UserLimits.hh"

#include "globals.hh"

#include <sstream>
#include <algorithm>
GPGeometryGeneral::GPGeometryGeneral(std::string sName, std::string sFatherName)
  :solid(0),logicalVolume(0),physicalVolume(0),fieldManager(0),visAttributes(0)
{
  SetActive(1);
  SetName(sName);
  SetFatherName(sFatherName);
  GPGeometryStore::GetInstance()->AddGeometry(GetName(),this);

  std::string   sMaterial = "G4_Galactic" ;
  material = G4NistManager::Instance()->FindOrBuildMaterial(sMaterial);

  sdHandle = new GPSensitiveHandle(GetName()+"sd/",GetName());
  vPosition = G4ThreeVector(0,0,0) ;
  vPositionInGlobalFrame = G4ThreeVector(0,0,0) ;
  sSolidType ="G4Box";
  sBaseNameChild = "-"+GetName();
  std::replace(sBaseNameChild.begin(),sBaseNameChild.end(),'/','_');
  dLength = 10e-3;
  dWidth  = 10e-3;
  dHeight = 10e-3;
  dRadiusInner = 0;
  dAngleStart  = 0;
  dAngleEnd = 360 ;
  dStepLimit = 10e-3;
  iStepLimitFlag =0;
  iCompactRangerFlag =1;
  iPriority = 0;
}

GPGeometryGeneral::~GPGeometryGeneral()
{
  GPGeometryStore::GetInstance()->EraseItem(GetName());
  delete sdHandle;
}

void GPGeometryGeneral::Update()
{
}
G4VSolid* GPGeometryGeneral::ConstructSolid()
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
void GPGeometryGeneral::Init()
{
  std::stringstream ss;
  std::string sValueX;
  std::string sValueY;
  std::string sValueZ;
  ss<<dWidth;
  ss>>sValueX;

  ss.clear();
  ss<<dHeight;
  ss>>sValueY;

  ss.clear();
  ss<<dLength;
  ss>>sValueZ;
  
  sdHandle->SetParameter("readout.x "+sValueX+" m",GetName());
  sdHandle->SetParameter("readout.y "+sValueY+" m",GetName());
  sdHandle->SetParameter("readout.z "+sValueZ+" m",GetName());
}
G4VPhysicalVolume* GPGeometryGeneral::Construct(G4LogicalVolume* motherLog)
{
  return Construct(motherLog,vPosition);
}
G4VPhysicalVolume* GPGeometryGeneral::Construct(G4LogicalVolume* motherLog,G4ThreeVector point)
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPGeometryGeneral::Construct(G4LogicalVolume,G4ThreeVector)"<<G4endl;
#endif

  vPosition = point;
  Init();

  //------------------------------ Geometry tube

  solid = ConstructSolid();

  logicalVolume = new G4LogicalVolume(solid,material,sBaseNameChild+"logicalVolume");
  physicalVolume = new G4PVPlacement(0,
             vPosition*m,
             logicalVolume,sBaseNameChild+"physicalVolume",motherLog,false,0);

  //logicalVolume->SetFieldManager(GPFieldSetup::GetGPFieldSetup()->GetLocalFieldManager(GetName()+"field/"),true);

  if(iStepLimitFlag)
    logicalVolume->SetUserLimits(new G4UserLimits(dStepLimit*m));

  /*
  G4VisAttributes* visAttributes= new G4VisAttributes(G4Colour(1.0,0.6,0,0.3));
  visAttributes->SetVisibility(true);
  visAttributes->SetForceSolid(true);
  logicalVolume->SetVisAttributes(visAttributes);
  */

  sdHandle->SetSensitiveDet(logicalVolume);

  return physicalVolume;

#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPGeometryGeneral::Construct(G4LogicalVolume,G4ThreeVector)"<<G4endl;
#endif
}

void GPGeometryGeneral::Print()
{
  G4cout
    <<"\n[Begin Geometry: "+GetName()+"]"
    <<"\nSolid: "+sSolidType
    <<"\nMaterial: "+sMaterial
    <<"\nLocal Position(m): "<<vPosition
    <<"\nGlobal Position(m): "<<vPositionInGlobalFrame
    <<"\nLength of Geometry: "<<dLength*m/mm<<" mm"
    <<"\nWidth of Geometry: "<<dWidth*m/mm<<" mm"
    <<"\nHeight of Geometry: "<<dHeight*m/mm<<" mm"
    <<"\nStep Limit Flag of Geometry: "<<iStepLimitFlag
    <<"\nStep Limit of Geometry: "<<dStepLimit*m/mm<<" mm"
    <<"\n[End Geometry: "+GetName()+"]"
    <<G4endl;
  sdHandle->Print();
  
   
}

void GPGeometryGeneral::SetParameter(std::string str,std::string sGlobal)
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
    else if(sKey=="pos.x")
      vPosition.setX(dValueNew);
    else if(sKey=="pos.y")
      vPosition.setY(dValueNew);
    else if(sKey=="pos.z")
      vPosition.setZ(dValueNew);
    else if(sKey=="agnle.start")
      dAngleStart = dValueNew;
    else if(sKey=="angle.end")
      dAngleEnd = dValueNew;
    else if(sKey=="limit.step.max")
      dStepLimit = dValueNew;
    else if(sKey=="limit.step.flag")
      iStepLimitFlag = dValueNew;
    else if(sKey=="solid.type")
    {
      SetSolidType(sValue);
      return;
    }
    else if(sKey=="material")
    {
      SetMaterial(sValue);
      return;
    }
    else if(sKey=="sd.active")
    {
      sdHandle->SetActive(1);
    }
    else if(sKey=="sd.inactive")
    {
      sdHandle->SetActive(0);
    }
    else if(sKey=="sd.type")
    {
      sdHandle->SetParameter(sKey+" "+sValue+" "+sUnit,sGlobal);
      return;
    }
    else if(sKey=="sd.scorer")
    {
      sdHandle->SetParameter(sKey+" "+sValue+" "+sUnit,sGlobal);
      return;
    }
    else if(sKey=="readout.cell.x")
    {
      sdHandle->SetParameter(sKey+" "+sValue+" "+sUnit,sGlobal);
      return;
    }
    else if(sKey=="readout.cell.y")
    {
      sdHandle->SetParameter(sKey+" "+sValue+" "+sUnit,sGlobal);
      return;
    }
    else if(sKey=="readout.cell.z")
    {
      sdHandle->SetParameter(sKey+" "+sValue+" "+sUnit,sGlobal);
      return;
    }
    else 
    {
      //std::cout<<((GPObject*) this)->GetName()<<": "+sKey+": Key does not exist."<<std::endl;
      std::cout<<GetName()<<": "+sKey+": Key does not exist."<<std::endl;
      return;
    }

    std::cout<<GetName()<<": Set "<<sKey<<": "<< sValue<<" "<<sUnit<<std::endl;
}

G4double GPGeometryGeneral::GetParameter(std::string sKey, std::string sGlobal) const
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
  else if(sKey=="limit.step.max")
    return dStepLimit; 
  else if(sKey=="limit.step.flag")
    return iStepLimitFlag; 
  //else if(sKey=="solid.type")
  //  return sSolidType;
  //else if(sKey=="material")
  //  return sMaterial;
  else
    {
      std::cout<<GetName()<<": "+sKey+": Key does not exist."<<std::endl;
      //std::cout<<((GPObject*) this)->GetName()<<": "+sKey+": Key does not exist."<<std::endl;
      return -1;
    }
}

void GPGeometryGeneral::Print(std::ofstream& fstOutput)
{
  fstOutput
    <<"\n[Begin Geometry: "+GetName()+"]"
    <<"\nSolid: "+sSolidType
    <<"\nMaterial: "+sMaterial
    <<"\nLocal Position(m): "<<vPosition
    <<"\nGlobal Position(m): "<<vPositionInGlobalFrame
    <<"\nLength of Geometry: "<<dLength*m/mm<<" mm"
    <<"\nWidth of Geometry: "<<dWidth*m/mm<<" mm"
    <<"\nHeight of Geometry: "<<dHeight*m/mm<<" mm"
    <<"\nStep Limit Flag of Geometry: "<<iStepLimitFlag
    <<"\nStep Limit of Geometry: "<<dStepLimit*m/mm<<" mm"
    <<"\n[End Geometry: "+GetName()+"]"
    <<G4endl;
  sdHandle->Print(fstOutput);
}
void GPGeometryGeneral::SetSolidType(std::string sValue)
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

void GPGeometryGeneral::SetMaterial(std::string sValue)
{
  material = G4NistManager::Instance()->FindOrBuildMaterial(sValue);
  if(material)
  {
    sMaterial=sValue;
    std::cout<<GetName()+": Set Material: "+sMaterial<<std::endl;
  }
  else
  {
    std::cout<<"This Material does not exist: "<<sValue<<std::endl;
  }

}

GPSensitiveHandle* GPGeometryGeneral::GetSensitiveHandle()const
{
  return sdHandle;
}
