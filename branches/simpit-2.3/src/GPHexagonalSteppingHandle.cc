// $Id: GPHexagonalSteppingHandle.cc,v 1.15 2006/06/29 17:49:13 gunter Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "GPHexagonalSteppingHandle.hh"
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
using namespace std;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GPHexagonalSteppingHandle::GPHexagonalSteppingHandle(std::string sName,std::string sFatherName)
{ 
  SetActive(1);
  SetName(sName);
  SetFatherName(sFatherName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GPHexagonalSteppingHandle::~GPHexagonalSteppingHandle()
{ 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GPHexagonalSteppingHandle::Init()
{
  GPRunAction* runAct = (GPRunAction*)G4RunManager::GetRunManager()->GetUserRunAction();
  G4String strFolder = runAct->GetDataPath();
  G4int iRunID = runAct->GetRunID();
  stringstream ss;
  string  strRunID;
  string  strSuffix;
  ss<<iRunID;
  ss>>strRunID;
  fstHangle.open((strFolder+"/"+strRunID+"GranularEDD.dat").c_str());
  G4cout<<"Create Target granular edd data file: "<<strFolder+"/"+strRunID+"GranularEDD.dat"<<G4endl;
  mapSphereEddVec.clear();
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
      std::vector<G4double>   vTmp(4,0);
      std::string sPhyName;
      for(iIndex=0;iIndex!=vsPhysicalVector->size();iIndex++)
      {
	vPoint = (*vsPhysicalVector)[iIndex]->GetTranslation()/mm;
	vTmp[0]=vPoint.x();
	vTmp[1]=vPoint.y();
	vTmp[2]=vPoint.z();
	sPhyName = (*vsPhysicalVector)[iIndex]->GetName();
	mapSphereEddVec.insert(pair<G4String,std::vector<G4double> >(sPhyName,vTmp));
      }

    }
  }
}

void GPHexagonalSteppingHandle::CleanUp()
{
  std::map<G4String,std::vector<G4double> >::iterator it=mapSphereEddVec.begin();
  std::vector<double> vTmp;
  for(it=mapSphereEddVec.begin();it!=mapSphereEddVec.end();it++);
  {    
    vTmp = it->second;
  fstHangle
    <<vTmp[0]<<"\t"<<vTmp[1]<<"\t"<<vTmp[2]<<"\t"<<vTmp[3]
    <<"\n";
  }

  fstHangle.close();
  mapSphereEddVec.clear();
}

void GPHexagonalSteppingHandle::UserSteppingAction(const G4Step* aStep)
{
  G4double dVolume;
  G4String strPrevPhysName= aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName();
  map<G4String,std::vector<G4double> >::iterator mapEddIt = mapSphereEddVec.find(strPrevPhysName);
  if(mapEddIt!=mapSphereEddVec.end())
  {
    dVolume = 
      aStep->GetPreStepPoint()->GetPhysicalVolume()->GetLogicalVolume()->GetSolid()->GetCubicVolume()/mm3;
    (mapSphereEddVec[strPrevPhysName])[3]+=aStep->GetTotalEnergyDeposit()/MeV/dVolume;
  }
}

void GPHexagonalSteppingHandle::Print(std::ofstream fstOutput)
{
}
void GPHexagonalSteppingHandle::Print()
{
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
