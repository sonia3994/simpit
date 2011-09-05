// $Id: GPGeometryGeneral.cc,v 1.9 2006/06/29 17:47:19 gunter Granular $
// GEANT4 tag $Name: geant4-09-02 $
//
#include "GPGeometryGeneral.hh"

#include "GPGeometryStore.hh"

#include "GPSolidManager.hh"
#include "GPComplexSolid.hh"
#include "GPComplexSolidManager.hh"
#include "GPSurfaceParticleScorer.hh"
#include "GPFieldSetup.hh"
#include "GPFieldManagerPool.hh"
#include "GPSensitiveHandle.hh"


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
  :pSolid(0),pLogicalVolume(0),pPhysicalVolume(0),
  pFieldManagerPool(0),pVisAttributes(0),pSolidManager(0),
  pComplexSolid(0)
{
  SetActive(1);
  SetName(sName);
  SetFatherName(sFatherName);
  GPGeometryStore::GetInstance()->AddGeometry(GetName(),this);

  std::string   sMaterial = "G4_Galactic" ;
  pMaterial = G4NistManager::Instance()->FindOrBuildMaterial(sMaterial);

  pSdHandle = new GPSensitiveHandle(GetName()+"sd/",GetName());
  pSolidManager = new GPSolidManager(GetName()+"solid/",GetName());
  vPosition = G4ThreeVector(0,0,0) ;
  vPositionInGlobalFrame = G4ThreeVector(0,0,0) ;
  sBaseNameChild = "-"+GetName();
  std::replace(sBaseNameChild.begin(),sBaseNameChild.end(),'/','_');
  dStepLimit = 10e-3;
  iStepLimitFlag =0;
  iCompactRangerFlag =1;
  iPriority = 0;
}

GPGeometryGeneral::~GPGeometryGeneral()
{
  GPGeometryStore::GetInstance()->EraseItem(GetName());
  delete pSdHandle;
  delete pSolidManager;
  if(pComplexSolid) delete pComplexSolid;
  if(pFieldManagerPool) delete pFieldManagerPool;
}

void GPGeometryGeneral::Update()
{

  if(pComplexSolid)
  {
    pComplexSolid->Update();
  }
  if(pFieldManagerPool)
  {
    pFieldManagerPool->Update();
  }
  if(pSolidManager)
  {
    pSolidManager->Update();
  }

  /*
  if(pVisAttributes)
  {
    pVisAttributes->Update();
  }
  */
  Init();
}
void GPGeometryGeneral::Init()
{
  std::stringstream ss;
  std::string sValueX;
  std::string sValueY;
  std::string sValueZ;
  double dWidth; 
  double dHeight;
  double dLength;
  if(pComplexSolid)
  {
	  dWidth = pComplexSolid->GetParameter("width","width");
	  dHeight= pComplexSolid->GetParameter("height","height");
	  dLength= pComplexSolid->GetParameter("length","length");
  }
  else
  {
	  dWidth = pSolidManager->GetParameter("width","width");
	  dHeight= pSolidManager->GetParameter("height","height");
	  dLength= pSolidManager->GetParameter("length","length");
  }
  ss<<dWidth;
  ss>>sValueX;

  ss.clear();
  ss<<dHeight;
  ss>>sValueY;

  ss.clear();
  ss<<dLength;
  ss>>sValueZ;
  
  pSdHandle->SetParameter("readout.x "+sValueX+" m",GetName());
  pSdHandle->SetParameter("readout.y "+sValueY+" m",GetName());
  pSdHandle->SetParameter("readout.z "+sValueZ+" m",GetName());
  if(pComplexSolid)
  {
    pSolidManager->SetParameter("width "+sValueX+" m",GetName());
    pSolidManager->SetParameter("height "+sValueY+" m",GetName());
    pSolidManager->SetParameter("length "+sValueZ+" m",GetName());
  }
}
G4VPhysicalVolume* GPGeometryGeneral::Construct(G4LogicalVolume* motherLog)
{
  return Construct(motherLog,vPosition);
}
G4VPhysicalVolume* GPGeometryGeneral::Construct(G4LogicalVolume* motherLog,G4ThreeVector point)
{

  vPosition = point;
  //Init();

  //------------------------------ Geometry tube

  pSolid = pSolidManager->ConstructSolid();

  pLogicalVolume = new G4LogicalVolume(pSolid,pMaterial,sBaseNameChild+"logicalVolume");
  pPhysicalVolume = new G4PVPlacement(0,
             vPosition*m,
             pLogicalVolume,sBaseNameChild+"physicalVolume",motherLog,false,0);

  if(pFieldManagerPool)
  {
    pLogicalVolume->SetFieldManager(pFieldManagerPool->GetFieldManager(),true);
  }
  if(pComplexSolid)
    pComplexSolid->Construct(pLogicalVolume);

  if(iStepLimitFlag)
    pLogicalVolume->SetUserLimits(new G4UserLimits(dStepLimit*m));

  /*
  G4VisAttributes* pVisAttributes= new G4VisAttributes(G4Colour(1.0,0.6,0,0.3));
  pVisAttributes->SetVisibility(true);
  pVisAttributes->SetForceSolid(true);
  pLogicalVolume->SetVisAttributes(pVisAttributes);
  */

  pSdHandle->SetSensitiveDet(pLogicalVolume);

  return pPhysicalVolume;

}

void GPGeometryGeneral::Print()
{
  G4cout
    <<"\n[Begin Geometry: "+GetName()+"]"
    <<"\nMaterial: "+sMaterial
    <<"\nLocal Position(m): "<<vPosition
    <<"\nGlobal Position(m): "<<vPositionInGlobalFrame
    <<"\nStep Limit Flag of Geometry: "<<iStepLimitFlag
    <<"\nStep Limit of Geometry: "<<dStepLimit*m/mm<<" mm";
  pSolidManager->Print();
  if(pFieldManagerPool) pFieldManagerPool->Print();
  if(pComplexSolid) pComplexSolid->Print();
  pSdHandle->Print();
  G4cout
    <<"\n[End Geometry: "+GetName()+"]"
    <<G4endl;
  
   
}

void GPGeometryGeneral::SetParameter(std::string sLocal,std::string sGlobal)
{
    std::stringstream ss(sLocal);
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

    std::string sTotalKey=sKey;
    std::string sFirstKey;
    std::string sLeftKey;
    size_t iFirstDot;
    iFirstDot = sTotalKey.find(".");
    if(iFirstDot!=std::string::npos)
    {
      sFirstKey=sTotalKey.substr(0,iFirstDot);
      sLeftKey=sTotalKey.substr(iFirstDot+1);
    }

    if(sFirstKey=="solid")
    {
      pSolidManager->SetParameter(sLeftKey+" "+sValue+" "+sUnit,sGlobal);
      return;
    }
    else if(sFirstKey=="sd")
    {
      pSdHandle->SetParameter(sLeftKey+" "+sValue+" "+sUnit,sGlobal);
      return;
    }
    else if(sFirstKey=="field_manager"&&pFieldManagerPool!=NULL)
    {
      pFieldManagerPool->SetParameter(sLeftKey+" "+sValue+" "+sUnit,sGlobal);
      return;
    }
    else if(sFirstKey=="cs")
    {
      if(pComplexSolid)
      {
	pComplexSolid->SetParameter(sLeftKey+" "+sValue+" "+sUnit,sGlobal);
	return;
      }
      else
      {
	std::cout<<GetName()<<": Complex Solid model does not exist."<<std::endl;
	return;
      }
    }

    else if(sKey=="pos.x")
      vPosition.setX(dValueNew);
    else if(sKey=="pos.y")
      vPosition.setY(dValueNew);
    else if(sKey=="pos.z")
      vPosition.setZ(dValueNew);
    else if(sKey=="limit.step.max")
      dStepLimit = dValueNew;
    else if(sKey=="limit.step.flag")
      iStepLimitFlag = dValueNew;
    else if(sKey=="material")
    {
      SetMaterial(sValue);
      return;
    }
    else if(sKey=="set.cs")
    {
      if (pComplexSolid) delete pComplexSolid;
      pComplexSolid = GPComplexSolidManager::GetInstance()
	->FindAndBuildComplexSolid(sValue,GetName()+"cs/",GetName());

      if (pComplexSolid==NULL)
      return;
    }
    else if(sKey=="set.field_manager")
    {
      if(pFieldManagerPool) delete pFieldManagerPool;
      pFieldManagerPool = GPFieldSetup::GetInstance()
	->FindAndBuildFieldManagerPool(sValue,GetName());
      if (pFieldManagerPool==NULL) return;
    }
    else 
    {
      std::cout<<GetName()<<": "+sKey+": Key does not exist."<<std::endl;
      return;
    }
    //Init();

    std::cout<<GetName()<<": Set "<<sKey<<": "<< sValue<<" "<<sUnit<<std::endl;
}

G4double GPGeometryGeneral::GetParameter(std::string sKey, std::string sGlobal) const
{
  if(sKey=="inner.radius")
    return pSolidManager->GetParameter(sKey,sGlobal); 
  else if(sKey=="width")
    return pSolidManager->GetParameter(sKey,sGlobal); 
  else if(sKey=="height")
    return pSolidManager->GetParameter(sKey,sGlobal); 
  else if(sKey=="length")
    return pSolidManager->GetParameter(sKey,sGlobal); 
  else if(sKey=="agnle.start")
    return pSolidManager->GetParameter(sKey,sGlobal); 
  else if(sKey=="angle.end")
    return pSolidManager->GetParameter(sKey,sGlobal); 
  else if(sKey=="limit.step.max")
    return dStepLimit; 
  else if(sKey=="limit.step.flag")
    return iStepLimitFlag; 
  else if(sKey=="pos_in_world.x")
    return vPositionInGlobalFrame.x();
  else if(sKey=="pos_in_world.y")
    return vPositionInGlobalFrame.y();
  else if(sKey=="pos_in_world.z")
    return vPositionInGlobalFrame.z();
  //else if(sKey=="pSolid.type")
  //  return sSolidType;
  //else if(sKey=="pMaterial")
  //  return sMaterial;
  else
    {
      std::cout<<GetName()<<": "+sKey+": Key does not exist."<<std::endl;
      return -1;
    }
}

void GPGeometryGeneral::Print(std::ofstream& fstOutput)
{
  fstOutput
    <<"\n[Begin Geometry: "+GetName()+"]"
    <<"\nMaterial: "+sMaterial
    <<"\nLocal Position(m): "<<vPosition
    <<"\nGlobal Position(m): "<<vPositionInGlobalFrame
    <<"\nStep Limit Flag of Geometry: "<<iStepLimitFlag
    <<"\nStep Limit of Geometry: "<<dStepLimit*m/mm<<" mm";
  pSolidManager->Print(fstOutput);
  if(pFieldManagerPool) pFieldManagerPool->Print(fstOutput);
  if(pComplexSolid) pComplexSolid->Print(fstOutput);
  pSdHandle->Print(fstOutput);
  fstOutput
    <<"\n[End Geometry: "+GetName()+"]"
    <<G4endl;
}

void GPGeometryGeneral::SetMaterial(std::string sValue)
{
  pMaterial = G4NistManager::Instance()->FindOrBuildMaterial(sValue);
  if(pMaterial)
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
  return pSdHandle;
}
GPComplexSolid* GPGeometryGeneral::GetComplexSolid()const
{
  return pComplexSolid;
}
GPSolidManager* GPGeometryGeneral::GetSolidManager() const
{
  return pSolidManager;
}
bool GPGeometryGeneral::IsInThisGeometry(G4ThreeVector vGlobal)
{
  G4ThreeVector vLocal=vGlobal-vPositionInGlobalFrame;
  return pSolidManager->IsInThisSolid(vLocal);
}
