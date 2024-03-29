// $Id: GPRunAction.cc,v 1.19 2008/01/17 17:31:32 maire Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "GPRunAction.hh"
#include "GPTrajectoryAction.hh"
#include "GPPrimaryGeneratorAction.hh"
#include "GPDetectorConstruction.hh"
#include "GPFieldSetup.hh"
#include "GPSteppingAction.hh"
#include "GPModuleManager.hh"

#include "GPRunHandleManager.hh"

#include "globals.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4FieldManager.hh"
#include "G4SDManager.hh"
#include "G4UnitsTable.hh"


#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>

//#include<unistd.h>
//#include<dirent.h>
//#include<sys/types.h>
//#include <sys/stat.h>   //mkdir in this head file


#define MacRightAlign  std::setiosflags(std::ios_base::right)
#define MacLeftAlign  std::setiosflags(std::ios_base::left)
using namespace std;
namespace fs=boost::filesystem;
extern std::string  sProgramName;
extern std::string  sProgramVersion;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GPRunAction::GPRunAction()
{
  time_t time_m=time(0);
  G4String sTime;
  sTime=ctime(&time_m);
  sTime.resize(24);
  replace(sTime.begin(),sTime.end(),' ','-');
  replace(sTime.begin(),sTime.end(),':','-');
  sFilePath="./output/"+sTime;
  G4cout<<"Create directory: "<<sFilePath<<G4endl;  
  //fs::path path(sFilePath);
  bfsWorkPath=fs::path(sFilePath);
  fs::create_directories(bfsWorkPath);
  ofsParaFile.open((bfsWorkPath/"Summary.csv").string().c_str(),ios::ate|ios::app);
  ofsParaFile<<"Program infor: "+sProgramName+"-"+sProgramVersion<<std::endl;
  ofsParaFile.close();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GPRunAction::~GPRunAction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GPRunAction::BeginOfRunAction(const G4Run* aRun)
{ 
  //inform the runManager to save random number seed
  GPRunHandleManager::GetInstance()->BeginOfRunAction(aRun);
  std::pair<std::string,std::ofstream* > pairHandle;
  G4String chrunID;
  stringstream ss;
  iRunID=aRun->GetRunID();
  G4int numEvt=aRun->GetNumberOfEventToBeProcessed();
  ss <<iRunID;
  ss >>chrunID;

  G4RunManager::GetRunManager()->SetRandomNumberStore(true);

  mapStrOfsOutputHandler.clear();
  ofsParaFile.open((bfsWorkPath/"Summary.csv").string().c_str(),ios::ate|ios::app);

  std::ofstream* ofsTrajectoryHandle = new std::ofstream((bfsWorkPath/(chrunID+"Trajectory.dat")).string().c_str());
  GPTrajectoryAction::GetGPTrajectoryAction()->SetOfstream(ofsTrajectoryHandle);
  pairHandle.first="trajectory";
  pairHandle.second=ofsTrajectoryHandle;
  mapStrOfsOutputHandler.insert(pairHandle);

  GPDetectorConstruction* detector 
    = (GPDetectorConstruction*)G4RunManager::GetRunManager()->GetUserDetectorConstruction();
  GPPrimaryGeneratorAction* primaryGenerator
    =(GPPrimaryGeneratorAction*)G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction();

  GPSteppingAction* gpSteppingAction
    =(GPSteppingAction*)G4RunManager::GetRunManager()->GetUserSteppingAction();
  if(gpSteppingAction) gpSteppingAction->Prepare();

  time_t time_m=time(0);
  G4String sTime;
  sTime=ctime(&time_m);
  sTime.resize(24);

  G4cout<<"\n========================Begin of Run: "<<iRunID<<"==================================\n"
    <<"\nRun Time: "<<sTime
    <<"\nRun ID: "<<iRunID
    <<"\nEvents Number:"<<numEvt
    <<G4endl;
  GPModuleManager::GetInstance()->Print();
  primaryGenerator->Print();

  ofsParaFile 
    <<"\n------Begin Of Run: "<<iRunID<<"------"
    <<"\nRun Time: "<<sTime
    <<"\nRun ID: "<<iRunID
    <<"\nEvents Number:"<<numEvt
    <<G4endl;
  GPModuleManager::GetInstance()->Print(ofsParaFile);
  primaryGenerator->Print(ofsParaFile);

  G4cout<<"Init Field."<<G4endl;
  GPFieldSetup::GetGPFieldSetup()->Init();

//  mapElectron.clear();
//  mapElectron.insert(std::pair<G4String,G4int>("target",0));
//  mapElectron.insert(std::pair<G4String,G4int>("capture",0));
//  mapElectron.insert(std::pair<G4String,G4int>("accelerator",0));
//
//  mapPositron.clear();
//  mapPositron.insert(std::pair<G4String,G4int>("target",0));
//  mapPositron.insert(std::pair<G4String,G4int>("capture",0));
//  mapPositron.insert(std::pair<G4String,G4int>("accelerator",0));
//
//  ofsDataFileDT = new std::ofstream((bfsWorkPath/(chrunID+"ExitOfTar.dat")).string().c_str());
//  pairHandle.first="target";
//  pairHandle.second=ofsDataFileDT;
//  mapStrOfsOutputHandler.insert(pairHandle);
//
//  ofsDataFileDC = new std::ofstream((bfsWorkPath/(chrunID+"ExitOfCap.dat")).string().c_str());
//  pairHandle.first="capture";
//  pairHandle.second=ofsDataFileDC;
//  mapStrOfsOutputHandler.insert(pairHandle);
//
//  ofsDataFileAC = new std::ofstream((bfsWorkPath/(chrunID+"ExitOfAcc.dat")).string().c_str());
//  pairHandle.first="accelerator";
//  pairHandle.second=ofsDataFileAC;
//  mapStrOfsOutputHandler.insert(pairHandle);
//
//
//  if(bTargetSDFlag)
//  {
//    ofsEddHandle.open((bfsWorkPath/(chrunID+"EddInTar.dat")).string().c_str());
//  }
//
//
//  //initialize cumulative quantities
//  //
//  dSumETar = dSum2ETar = 0.;
//  dSumLTrack = dSum2LTrack = 0.; 
//  iPositronPerRun=0;
//  iActualG=0;
//  //
//  vecDouEdd.clear();
//  if(bTargetSDFlag)
//  {
//    vecIntEddDim=detector->GetEddDim();
//    vecDouEdd.resize(vecIntEddDim[0]*vecIntEddDim[1]*vecIntEddDim[2]);
//    for(size_t i=0;i!=vecDouEdd.size();i++)
//    {vecDouEdd[i]=0;}
//  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GPRunAction::FillPerEvent(G4double ETar,G4double LTrack,G4int positronPerEvt)
{
  //accumulate statistic
  //
  dSumETar += ETar;  dSum2ETar += ETar*ETar;
  dSumLTrack += LTrack;  dSum2LTrack += LTrack*LTrack;
  iPositronPerRun +=positronPerEvt;  
}

//
void GPRunAction::AddEddHit(G4int x, G4int y, G4int z, G4double e)
{
  if(bTargetSDFlag)
  {
    G4int index=x+y*vecIntEddDim[0]+z*vecIntEddDim[0]*vecIntEddDim[1];
    vecDouEdd[index]=vecDouEdd[index]+e;
  }
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GPRunAction::EndOfRunAction(const G4Run* aRun)
{
  GPRunHandleManager::GetInstance()->EndOfRunAction(aRun);
  GPSteppingAction* gpSteppingAction=(GPSteppingAction*)G4RunManager::GetRunManager()->GetUserSteppingAction();
  if(gpSteppingAction) gpSteppingAction->CleanUp();
  G4cout
    <<"\n==================================End of Run: "<<aRun->GetRunID()<<"==================================="
    << G4endl;
  ofsParaFile
    <<"\n-------End Of Run: "<<aRun->GetRunID()<<"-------"
    << G4endl;
//
//  G4int NbOfEvents = aRun->GetNumberOfEvent();
//  if (NbOfEvents == 0) return;
//
//  //compute statistics: mean and rms
//  //
//  //dSumETar /= NbOfEvents; dSum2ETar /= NbOfEvents;
//  G4double rmsETar = dSum2ETar - dSumETar*dSumETar;
//  if (rmsETar >0.) rmsETar = std::sqrt(rmsETar); else rmsETar = 0.;
//
//
//  dSumLTrack /= NbOfEvents; dSum2LTrack /= NbOfEvents;
//  G4double rmsLTrack = dSum2LTrack - dSumLTrack*dSumLTrack;
//  if (rmsLTrack >0.) rmsLTrack = std::sqrt(rmsLTrack);
//  else rmsLTrack = 0.;
//
//  //print
//  //
//  G4cout
//    <<"\n--------------------Results------------------------------\n"
//    //<<MacRightAlign<<std::setw(46)<<"The actual gammas impinged target: "<<iActualG <<"\n"
//    //<<MacRightAlign<<std::setw(46)<<"Number of e+ in this run: "<<iPositronPerRun<<"\n"
//    <<MacRightAlign<<std::setw(46)<<"Energy deposited in target in the run: " << G4BestUnit(dSumETar,"Energy")<<"\n"
//    <<MacRightAlign<<std::setw(46)<<"Mean energy deposited in target per event: " << G4BestUnit(dSumETar/NbOfEvents,"Energy") <<" +- "<< G4BestUnit(rmsETar,"Energy")<<"\n"  
//    <<MacRightAlign<<std::setw(46)<<"Mean track length in Target: " << G4BestUnit(dSumLTrack,"Length") <<" +- "<< G4BestUnit(rmsLTrack,"Length")<<"\n"  
//    <<"---------------------End of results-------------------------\n"
//    <<"==================================End of Run ===================================\n"
//    << G4endl;
//  ofsParaFile
//    <<"\nResult:"
//    <<"\nSum E deposited in target, deposited E rms, sum track length in target, track length rms"
//    <<"\n"<<dSumETar<<"," <<rmsETar<<"," <<dSumLTrack<<"," <<rmsLTrack
//    <<"\ne+ number,target,capture,accelerator"
//    <<"\nnumber,"<<mapPositron["target"]<<","<<mapPositron["capture"]<<","<<mapPositron["accelerator"]
//    <<"\ne- number,target,capture,accelerator"
//    <<"\nnumber,"<<mapElectron["target"]<<","<<mapElectron["capture"]<<","<<mapElectron["accelerator"]
//    <<"\n----End of a run----"
//    << G4endl;
//
//  ofsParaFile.close();
//  for(std::map<std::string, std::ofstream* >::iterator iter=mapStrOfsOutputHandler.begin();iter!=mapStrOfsOutputHandler.end();iter++)
//  {
//    (iter->second)->close();
//  }
//  mapStrOfsOutputHandler.clear();
//
//  long int index;
//  if(bTargetSDFlag)
//  {
//    for(G4int i=0;i!=vecIntEddDim[0];i++)
//    {
//      //x=i*dx-(vecIntEddDim[0]-1)*dx*0.5;
//      for(G4int j=0;j!=vecIntEddDim[1];j++)
//      {
//	//y=j*dy-(vecIntEddDim[1]-1)*dy*0.5;
//
//	for(G4int k=0;k!=vecIntEddDim[2];k++)
//	{
//	  //z=k*dz-(vecIntEddDim[2]-1)*dz*0.5;
//	  index=k*vecIntEddDim[0]*vecIntEddDim[1]+j*vecIntEddDim[0]+i;
//	  ofsEddHandle<<i<<" "<<j<<" "<<k<<" "<<vecDouEdd[index]<<"\n";
//	  //ofsEddHandle<<x<<" "<<y<<" "<<z<<" "<<vecDouEdd[k*vecIntEddDim[0]*vecIntEddDim[1]+j*vecIntEddDim[0]+i]<<"\n";
//	}
//      }
//    } 
//    ofsEddHandle<<G4endl;
//    ofsEddHandle.close();
//  }
//#ifdef GP_DEBUG
//  G4cout<<"GP_DEBUG: Exit GPRunAction::EndOfRunAction(const G4Run* aRun)"<<G4endl;
//#endif
}

void GPRunAction::OutPutData(std::string name,std::vector<G4double> value) 
{
  std::map<std::string, std::ofstream* >::iterator iter=mapStrOfsOutputHandler.find(name);
  if(iter!=mapStrOfsOutputHandler.end())
  {
    OutPut(iter,value);
  }
  else
  {
    G4cout<<"Wrong output key!"<<G4endl;
  }


}
void GPRunAction::OutPut(std::map<std::string, std::ofstream* >::iterator iter,std::vector<G4double> value) 
{

  for(size_t j=0;j!=value.size();j++)
  {
    *(iter->second)<<value[j]<<" ";
  }

  *(iter->second)<<G4endl;

}
void GPRunAction::AddElectronNumber(G4String key, G4int value) 
{
  std::map<G4String, G4int>::iterator iter=mapElectron.find(key);
  if(iter!=mapElectron.end())
  {
    mapElectron[key] += value;
  }
}
void GPRunAction::AddPositronNumber(G4String key, G4int value) 
{
  std::map<G4String, G4int>::iterator iter=mapPositron.find(key);
  if(iter!=mapPositron.end())
  {
    mapPositron[key] += value;
  }
}

G4int GPRunAction::GetRunID()
{
  return iRunID;
}

G4String GPRunAction::GetDataPath()
{
  return bfsWorkPath.string();
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
