// $Id: GPTargetSteppingAction.cc,v 1.15 2006/06/29 17:49:13 gunter Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "GPTargetSteppingAction.hh"

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

GPTargetSteppingAction::GPTargetSteppingAction()
{ 
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPTargetSteppingAction::GPTargetSteppingAction(GPDetectorConstruction*)"<<G4endl;
#endif
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPTargetSteppingAction::GPTargetSteppingAction(GPDetectorConstruction*)"<<G4endl;
#endif
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GPTargetSteppingAction::~GPTargetSteppingAction()
{ 
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPTargetSteppingAction::~GPTargetSteppingAction()"<<G4endl;
#endif
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPTargetSteppingAction::~GPTargetSteppingAction()"<<G4endl;
#endif
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GPTargetSteppingAction::Init()
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPTargetSteppingAction::Init()"<<G4endl;
#endif
  G4cout<<"-------------------GPTargetSteppingAction----------------- "<<G4endl;
  GPDetectorConstruction* detector = (GPDetectorConstruction*)G4RunManager::GetRunManager()->GetUserDetectorConstruction();
  G4PhysicalVolumeStore* phyStore = G4PhysicalVolumeStore::GetInstance();
  G4ThreeVector vecPoint;
  std::vector<G4double>   vecTmp(4,0);
  if(detector->GetDetectorSize("target.granular.flag"))
  {
    G4int iXN = detector->GetDetectorSize("target.granular.x.number");
    G4int iYN = detector->GetDetectorSize("target.granular.y.number");
    G4int iZN = detector->GetDetectorSize("target.granular.z.number");
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
    for(G4int i=0;i<iXN*iYN*iZN;i++)
    {
      ss.clear();
      ss.str("");
      ss<<i;
      ss>>strSuffix;

      vecPoint = phyStore->GetVolume("sphPhyA_"+strSuffix)->GetTranslation()/mm;
      vecTmp[0]=vecPoint.x();
      vecTmp[1]=vecPoint.y();
      vecTmp[2]=vecPoint.z();
      mapSphereEddVec.insert(pair<G4String,std::vector<G4double> >("sphPhyA_"+strSuffix,vecTmp));

      vecPoint = phyStore->GetVolume("sphPhyB_"+strSuffix)->GetTranslation()/mm;
      vecTmp[0]=vecPoint.x();
      vecTmp[1]=vecPoint.y();
      vecTmp[2]=vecPoint.z();
      mapSphereEddVec.insert(pair<G4String,std::vector<G4double> >("sphPhyB_"+strSuffix,vecTmp));

      vecPoint = phyStore->GetVolume("sphPhyC_"+strSuffix)->GetTranslation()/mm;
      vecTmp[0]=vecPoint.x();
      vecTmp[1]=vecPoint.y();
      vecTmp[2]=vecPoint.z();
      mapSphereEddVec.insert(pair<G4String,std::vector<G4double> >("sphPhyC_"+strSuffix,vecTmp));

      vecPoint = phyStore->GetVolume("sphPhyD_"+strSuffix)->GetTranslation()/mm;
      vecTmp[0]=vecPoint.x();
      vecTmp[1]=vecPoint.y();
      vecTmp[2]=vecPoint.z();
      mapSphereEddVec.insert(pair<G4String,std::vector<G4double> >("sphPhyD_"+strSuffix,vecTmp));

      vecPoint = phyStore->GetVolume("sphPhyE_"+strSuffix)->GetTranslation()/mm;
      vecTmp[0]=vecPoint.x();
      vecTmp[1]=vecPoint.y();
      vecTmp[2]=vecPoint.z();
      mapSphereEddVec.insert(pair<G4String,std::vector<G4double> >("sphPhyE_"+strSuffix,vecTmp));

      vecPoint = phyStore->GetVolume("sphPhyF_"+strSuffix)->GetTranslation()/mm;
      vecTmp[0]=vecPoint.x();
      vecTmp[1]=vecPoint.y();
      vecTmp[2]=vecPoint.z();
      mapSphereEddVec.insert(pair<G4String,std::vector<G4double> >("sphPhyF_"+strSuffix,vecTmp));

      vecPoint = phyStore->GetVolume("sphPhyG_"+strSuffix)->GetTranslation()/mm;
      vecTmp[0]=vecPoint.x();
      vecTmp[1]=vecPoint.y();
      vecTmp[2]=vecPoint.z();
      mapSphereEddVec.insert(pair<G4String,std::vector<G4double> >("sphPhyG_"+strSuffix,vecTmp));

      vecPoint = phyStore->GetVolume("sphPhyH_"+strSuffix)->GetTranslation()/mm;
      vecTmp[0]=vecPoint.x();
      vecTmp[1]=vecPoint.y();
      vecTmp[2]=vecPoint.z();
      mapSphereEddVec.insert(pair<G4String,std::vector<G4double> >("sphPhyH_"+strSuffix,vecTmp));
    }
    G4cout
      <<"Insert granular target stepping map size: "<< mapSphereEddVec.size()<<G4endl;

  }
  G4cout<<"---------------------------------------------------------- "<<G4endl;
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPTargetSteppingAction::Init()"<<G4endl;
#endif
}

void GPTargetSteppingAction::CleanUp()
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPTargetSteppingAction::CleanUp()"<<G4endl;
#endif
  GPDetectorConstruction* detector = (GPDetectorConstruction*)G4RunManager::GetRunManager()->GetUserDetectorConstruction();

  if(!(detector->GetDetectorSize("target.granular.flag")))
  {
    fstHangle.close();
    return;
  }
  G4int iXN = detector->GetDetectorSize("target.granular.x.number");
  G4int iYN = detector->GetDetectorSize("target.granular.y.number");
  G4int iZN = detector->GetDetectorSize("target.granular.z.number");
  stringstream ss;
  G4String strSuffix;
  G4int iIndex;
  std::vector<G4double> vecEdd;

  for(G4int k=0;k<iZN;k++)
  {
    for(G4int j=0;j<iYN;j++)
    {
      for(G4int i=0;i<iXN;i++)
      {
	iIndex = i+j*iXN+k*iXN*iYN;
        ss.clear();
        ss.str("");
        ss<<iIndex;
        ss>>strSuffix;

	vecEdd=mapSphereEddVec["sphPhyA_"+strSuffix];
	fstHangle<<i<<" "<<j<<" "<<k<<" 1 "<<vecEdd[0]<<" "<<vecEdd[1]<<" "<<vecEdd[2]<<" "<<vecEdd[3]<<"\n";
	vecEdd=mapSphereEddVec["sphPhyB_"+strSuffix];
	fstHangle<<i<<" "<<j<<" "<<k<<" 2 "<<vecEdd[0]<<" "<<vecEdd[1]<<" "<<vecEdd[2]<<" "<<vecEdd[3]<<"\n";
	vecEdd=mapSphereEddVec["sphPhyC_"+strSuffix];
	fstHangle<<i<<" "<<j<<" "<<k<<" 3 "<<vecEdd[0]<<" "<<vecEdd[1]<<" "<<vecEdd[2]<<" "<<vecEdd[3]<<"\n";
	vecEdd=mapSphereEddVec["sphPhyD_"+strSuffix];
	fstHangle<<i<<" "<<j<<" "<<k<<" 4 "<<vecEdd[0]<<" "<<vecEdd[1]<<" "<<vecEdd[2]<<" "<<vecEdd[3]<<"\n";
	vecEdd=mapSphereEddVec["sphPhyE_"+strSuffix];
	fstHangle<<i<<" "<<j<<" "<<k<<" 5 "<<vecEdd[0]<<" "<<vecEdd[1]<<" "<<vecEdd[2]<<" "<<vecEdd[3]<<"\n";
	vecEdd=mapSphereEddVec["sphPhyF_"+strSuffix];
	fstHangle<<i<<" "<<j<<" "<<k<<" 6 "<<vecEdd[0]<<" "<<vecEdd[1]<<" "<<vecEdd[2]<<" "<<vecEdd[3]<<"\n";
	vecEdd=mapSphereEddVec["sphPhyG_"+strSuffix];
	fstHangle<<i<<" "<<j<<" "<<k<<" 7 "<<vecEdd[0]<<" "<<vecEdd[1]<<" "<<vecEdd[2]<<" "<<vecEdd[3]<<"\n";
	vecEdd=mapSphereEddVec["sphPhyH_"+strSuffix];
	fstHangle<<i<<" "<<j<<" "<<k<<" 8 "<<vecEdd[0]<<" "<<vecEdd[1]<<" "<<vecEdd[2]<<" "<<vecEdd[3]<<"\n";
      }
    }
  
  }


  fstHangle.close();
  mapSphereEddVec.clear();
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPTargetSteppingAction::CleanUp()"<<G4endl;
#endif
}

void GPTargetSteppingAction::UserSteppingAction(const G4Step* aStep)
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPTargetSteppingAction::UserSteppingAction(const G4Step*)"<<G4endl;
#endif
  G4String strPrevPhysName= aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName();
  map<G4String,std::vector<G4double> >::iterator mapEddIt = mapSphereEddVec.find(strPrevPhysName);
  if(mapEddIt!=mapSphereEddVec.end())
  {
    (mapSphereEddVec[strPrevPhysName])[3]=(mapSphereEddVec[strPrevPhysName])[3]+aStep->GetTotalEnergyDeposit()/MeV;
  }
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPTargetSteppingAction:::UserSteppingAction(const G4Step*)"<<G4endl;
#endif
}

void GPTargetSteppingAction::Print(std::ofstream fstOutput)
{
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
