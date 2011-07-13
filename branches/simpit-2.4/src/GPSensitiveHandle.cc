// $Id: GPSensitiveHandle.cc,v 1.9 2006/06/29 17:47:19 gunter Granular $
// GEANT4 tag $Name: geant4-09-02 $
//
#include "GPSensitiveHandle.hh"
#include "GPSurfaceParticleScorer.hh"
#include "GPGeometryStore.hh"
#include "GPGeometry.hh"
#include "GPTargetSD.hh"
#include "GPTargetROGeometry.hh"

#include "G4PSEnergyDeposit.hh"
#include "G4SDParticleFilter.hh"
#include "G4LogicalVolume.hh"
#include "G4SDManager.hh"
#include "G4UIcommand.hh"

#include "globals.hh"

#include <sstream>
#include <algorithm>
GPSensitiveHandle::GPSensitiveHandle(std::string sName, std::string sFatherName)
{
  SetActive(0);
  SetName(sName);
  SetFatherName(sFatherName);
  //GPGeometryStore::GetInstance()->AddGeometry(GetName(),this);

  dCellX = 0.001; 
  dCellY = 0.001; 
  dCellZ = 0.001; 
  sSDType = "G4MultiFunctionalDetector";
  sSDName = GetFatherName()+"sd";
  sBaseNameChild = "f"+GetFatherName();
  std::replace(sBaseNameChild.begin(),sBaseNameChild.end(),'/','_');
  std::replace(sSDName.begin(),sSDName.end(),'/','_');
}

GPSensitiveHandle::~GPSensitiveHandle()
{
  //GPGeometryStore::GetInstance()->EraseItem(GetName());
}

void GPSensitiveHandle::Print()
{
  G4cout
    <<"\n[Begin Sensitive Detector: "+GetName()+"]"
    <<"\nSensitive status: "<<IsActive()
    <<"\nSensitive type: "+sSDType
    <<"\nSensitive name: "+sSDName;
    MStrStrScorer::iterator it;
    for(it=mStrStrScorer.begin();it!=mStrStrScorer.end();it++)
    {
  G4cout
    <<"\nScorer type: "+(it->first)+"; Scorer name: "+(it->second);
    }
  G4cout
    <<"\n[End Sensitive Detector: "+GetName()+"]"
    <<G4endl;
  
   
}

void GPSensitiveHandle::SetParameter(std::string str,std::string strGlobal)
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

    if(sKey=="active")
      SetActive(1);
    else if(sKey=="inactive")
      SetActive(0);
    else if(sKey=="readout.x")
      dReadOutX = dValueNew;
    else if(sKey=="readout.y")
      dReadOutY = dValueNew;
    else if(sKey=="readout.z")
      dReadOutZ = dValueNew;
    else if(sKey=="readout.cell.x")
      dCellX = dValueNew;
    else if(sKey=="readout.cell.y")
      dCellY = dValueNew;
    else if(sKey=="readout.cell.z")
      dCellZ = dValueNew;
    else if(sKey=="type")
    {
      SetSensitiveDetType(sValue);
      return;
    }
    else if(sKey=="scorer")
    {
      AddPrimitiveScorer(sValue,sUnit);
      return;
    }
    else if(sKey=="filter")
    {
      SetSDFilter(sValue,sUnit);
      return;
    }
    else 
    {
      std::cout<<GetName()<<": "+sKey+": Key does not exist."<<std::endl;
      return;
    }

    Init();
    std::cout<<GetName()<<": Set "<<sKey<<": "<< sValue<<" "<<sUnit<<std::endl;
}

G4double GPSensitiveHandle::GetParameter(std::string sKey, std::string sGlobal) const
{
  std::cout<<GetName()<<": "+sKey+": Key does not exist."<<std::endl;
  return -1;
}

//*/
void GPSensitiveHandle::SetSensitiveDet(G4LogicalVolume* motherLog)
{
  if(IsActive()==0)
    return;

  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  if(sSDType=="G4MultiFunctionalDetector")
  {
    G4MultiFunctionalDetector* multiFunDet=(G4MultiFunctionalDetector*)SDman->FindSensitiveDetector(sSDName);
    if(multiFunDet==NULL)
    {
      G4MultiFunctionalDetector* multiFunDet = new G4MultiFunctionalDetector(sSDName);
      SDman->AddNewDetector(multiFunDet);
      MStrStrScorer::iterator it;

      std::map<std::string,G4SDParticleFilter*>::iterator itSD;
      for(it=mStrStrScorer.begin();it!=mStrStrScorer.end();it++)
      {
	if(it->first=="GPSurfaceParticleScorer")
	{
	  GPSurfaceParticleScorer* particleScorer = new GPSurfaceParticleScorer(it->second,1,2);
	  multiFunDet->RegisterPrimitive(particleScorer);
	  itSD=mStrSDFilter.find("GPSurfaceParticleScorer");
	  if(itSD!=mStrSDFilter.end())
	    particleScorer->SetFilter(itSD->second);
	}
	else if(it->first=="G4PSEnergyDeposit")
	{
	  G4PSEnergyDeposit* pEnergyDeposit = new G4PSEnergyDeposit(it->second);
	  multiFunDet->RegisterPrimitive(pEnergyDeposit);
	  itSD=mStrSDFilter.find("G4PSEnergyDeposit");
	  if(itSD!=mStrSDFilter.end())
	    pEnergyDeposit->SetFilter(itSD->second);
	}
      }
    }
  motherLog->SetSensitiveDetector(multiFunDet); 
  }
 
  else if(sSDType=="GPTargetSD")
  {
    SetBoxCellSD(motherLog);
  }
 
}
         
void GPSensitiveHandle::SetBoxCellSD(G4LogicalVolume* motherLog)
{
  GPGeometry* geometry = 
    (GPGeometry*)GPGeometryStore::GetInstance()->FindGeometry(GetFatherName());

  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  std::string  targetROName=GetName()+"readout/";
  std::replace(targetROName.begin(),targetROName.end(),'/','_');

  std::vector<int> vDim(3,1);
  vDim[0]=(ceil(dReadOutX/dCellX));
  vDim[1]=(ceil(dReadOutY/dCellY));
  vDim[2]=(ceil(dReadOutZ/dCellZ));

  GPTargetSD* targetSD = (GPTargetSD*)SDman->FindSensitiveDetector(sSDName);
  if(targetSD==NULL)
  {
    targetSD = new GPTargetSD(sSDName,vDim);
    targetSD->SetCollectionName("GPTargetSD"); 
    SDman->AddNewDetector(targetSD);
  }

  targetSD->SetEddDim(vDim);

  GPTargetROGeometry* targetRO = (GPTargetROGeometry*)targetSD->GetROgeometry(); 
  if(targetRO)
    delete targetRO;

  targetRO=new GPTargetROGeometry(targetROName, dReadOutX, dReadOutY, dReadOutZ, vDim);
  targetRO->SetPosition(geometry->GetPositionInGlobalFrame());

  targetRO->BuildROGeometry();
  targetSD->SetROgeometry(targetRO);  
  motherLog->SetSensitiveDetector(targetSD); 

}

void GPSensitiveHandle::Print(std::ofstream& fstOutput)
{
  fstOutput
    <<"\n[Begin Sensitive Detector: "+GetName()+"]"
    <<"\nSensitive status: "<<IsActive()
    <<"\nSensitive type: "+sSDType
    <<"\nSensitive name: "+sSDName;
  if(sSDType=="GPTargetSD")
  {
    G4SDManager* SDman = G4SDManager::GetSDMpointer();
    GPTargetSD* targetSD = (GPTargetSD*)SDman->FindSensitiveDetector(sSDName);
    if(targetSD)
    {
      GPTargetROGeometry* targetRO = (GPTargetROGeometry*)targetSD->GetROgeometry(); 
      if(targetRO)
	targetRO->Print(fstOutput);
    }
  }
  MStrStrScorer::iterator it;
  for(it=mStrStrScorer.begin();it!=mStrStrScorer.end();it++)
  {
  fstOutput
    <<"\nScorer type: "+(it->first)+"; Scorer name: "+(it->second);
    }
  fstOutput
    <<"\n[End Sensitive Detector: "+GetName()+"]"
    <<G4endl;
  
}
void GPSensitiveHandle::SetSensitiveDetType(std::string sValue)
{
  if(sValue=="G4MultiFunctionalDetector")
  {
    sSDType=sValue;
    std::cout<<GetName()+": Set Sensitive detector: "+sSDType<<std::endl;
  }
  else if(sValue=="GPTargetSD")
  {
    sSDType=sValue;
    std::cout<<GetName()+": Set Sensitive detector: "+sSDType<<std::endl;
  }
  else
  {
    std::cout<<"This Sensitive detector is not support: "<<sValue<<std::endl;
  }

}
void GPSensitiveHandle::AddPrimitiveScorer(std::string sValue,std::string sIndex)
{
  if(sValue=="GPSurfaceParticleScorer")
  {
    mStrStrScorer.insert(std::pair<std::string,std::string>(sValue,sValue));
    std::cout<<GetName()+": Add scorer: "+sValue+". Name: "+sValue<<std::endl;
  }
  else if(sValue=="G4PSEnergyDeposit")
  {
    mStrStrScorer.insert(std::pair<std::string,std::string>(sValue,sValue));
    std::cout<<GetName()+": Add scorer: "+sValue+". Name: "+sValue<<std::endl;
  }
  else
  {
    std::cout<<"This scorer is not support: "<<sValue<<std::endl;
  }

}
MStrStrScorer* GPSensitiveHandle:: GetMStrStrScorer()
{
  return &mStrStrScorer;
}

std::string GPSensitiveHandle:: GetSDName()const
{
  return sSDName;
}
std::string GPSensitiveHandle:: GetSDType()const
{
  return sSDType;
}
void GPSensitiveHandle::SetSDFilter(std::string sValue,std::string sParticleName)
{
  MStrStrScorer::iterator it;
  it=mStrStrScorer.find(sValue);
  G4SDParticleFilter* pSDFilter ;
  if(it==mStrStrScorer.end())
  {
    std::cout<<GetName()+": This G4VPrimitiveScorer: "+sValue+", has not yet been created."<<std::endl;
    return;
  }
  std::map<std::string,G4SDParticleFilter*>::iterator itSD;
  itSD=mStrSDFilter.find(sValue);
  if(itSD==mStrSDFilter.end())
  {
    pSDFilter = new G4SDParticleFilter(sBaseNameChild+sValue);
    mStrSDFilter.insert(std::pair<std::string,G4SDParticleFilter*>(sValue,pSDFilter));
  }
  else
    pSDFilter=itSD->second;

  pSDFilter->add(sParticleName);

}
