// $Id: GPHexagonalSteppingHandle.cc,v 1.15 2006/06/29 17:49:13 gunter Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "GPHexagonalSteppingHandle.hh"
#include "GPSteppingHandleStore.hh"
#include "GPGeometryStore.hh"
#include "GPGeometryGeneral.hh"
#include "GPHexagonalSolid.hh"

#include "GPDetectorConstruction.hh"
#include "GPEventAction.hh"
#include "GPRunAction.hh"

#include "G4PhysicalVolumeStore.hh"
#include "G4ThreeVector.hh"
#include "G4RunManager.hh"
#include "G4Step.hh"

//#include <fstream>
#include <sstream>
#include <algorithm>
using namespace std;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GPHexagonalSteppingHandle::GPHexagonalSteppingHandle(std::string sName,std::string sFatherName)
{ 
  SetActive(1);
  SetName(sName);
  SetFatherName(sFatherName);
  GPSteppingHandleStore::GetInstance()->AddSteppingHandle(GetName(),this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GPHexagonalSteppingHandle::~GPHexagonalSteppingHandle()
{ 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GPHexagonalSteppingHandle::Init()
{
  GPRunAction* runAct = (GPRunAction*)G4RunManager::GetRunManager()->GetUserRunAction();
  std::string sFolder = runAct->GetDataPath();
  G4int iRunID = runAct->GetRunID();
  stringstream ss;
  string  sRunID;
  string  sFather=GetFatherName();
  string  sFileName;
  ss<<iRunID;
  ss>>sRunID;
  std::replace(sFather.begin(),sFather.end(),'/','-');
  sFileName=sFolder+"/"+sRunID+sFather+"HexagonalEDD.dat";
  fstHangle.open(sFileName.c_str());
  G4cout<<"Create Target granular edd data file: "<<sFileName<<G4endl;
  msvdHexagonalEdd.clear();
  GPGeometryGeneral* pGeometry
    = (GPGeometryGeneral*) GPGeometryStore::GetInstance()->FindGeometry(GetFatherName()+"geometry/");  
  if(pGeometry)
  {
    GPComplexSolid* pComplexSolid

      = pGeometry->GetComplexSolid();
    if(pComplexSolid)
    {
      std::vector<G4VPhysicalVolume*>* vsPhysicalVector
	= pComplexSolid->GetPhysicalVector();
      size_t iIndex; 
      G4ThreeVector vPoint;
      std::vector<double>*   vTmp;
      std::string sPhyName;
      for(iIndex=0;iIndex!=vsPhysicalVector->size();iIndex++)
      {
	vPoint = (*vsPhysicalVector)[iIndex]->GetTranslation()/mm;
	vTmp = new std::vector<double>(4,0);
	(*vTmp)[0]=vPoint.x();
	(*vTmp)[1]=vPoint.y();
	(*vTmp)[2]=vPoint.z();
	sPhyName = (*vsPhysicalVector)[iIndex]->GetName();
	msvdHexagonalEdd.insert(pair<std::string,std::vector<double>* >(sPhyName,vTmp));
      }

    }
  }
}

void GPHexagonalSteppingHandle::CleanUp()
{
  std::map<std::string,std::vector<double>* >::iterator it;
  std::vector<double>* vTmp;
  for(it=msvdHexagonalEdd.begin();it!=msvdHexagonalEdd.end();it++)
  {    
    vTmp = it->second;
  fstHangle
    <<it->first<<"\t"
    <<std::setw(10)<<(*vTmp)[0]<<"\t"
    <<std::setw(10)<<(*vTmp)[1]<<"\t"
    <<std::setw(10)<<(*vTmp)[2]<<"\t"
    <<std::setw(10)<<(*vTmp)[3]<<"\t" 
    <<"\n";
  delete vTmp;
  }

  fstHangle.close();
  msvdHexagonalEdd.clear();
}

void GPHexagonalSteppingHandle::UserSteppingAction(const G4Step* aStep)
{
  double dVolume;
  std::string strPrevPhysName= aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName();
  map<std::string,std::vector<double>* >::iterator mapEddIt = msvdHexagonalEdd.find(strPrevPhysName);
  if(mapEddIt!=msvdHexagonalEdd.end())
  {
    dVolume = 
      aStep->GetPreStepPoint()->GetPhysicalVolume()->GetLogicalVolume()->GetSolid()->GetCubicVolume()/mm3;
    (*(msvdHexagonalEdd[strPrevPhysName]))[3]+=aStep->GetTotalEnergyDeposit()/MeV/dVolume;
  }
}

void GPHexagonalSteppingHandle::Print(std::ofstream fstOutput)
{
}
void GPHexagonalSteppingHandle::Print()
{
}
void GPHexagonalSteppingHandle::Prepare()
{
  Init();
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
