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
#include "GPMain.hh"

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
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GPRunAction::GPRunAction()
{
  /*
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
  //ofsParaFile.close();
  */
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GPRunAction::~GPRunAction()
{
  ofsParaFile.close();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GPRunAction::BeginOfRunAction(const G4Run* aRun)
{ 
  sFilePath=GPMain::GetInstance()->GetDataDir();
  bfsWorkPath=fs::path(sFilePath);
  fs::create_directories(bfsWorkPath);
  std::string sProgramVersion=GPMain::GetInstance()->GetValueInString("app.version");
  std::string sProgramName=GPMain::GetInstance()->GetValueInString("app.name");
  ofsParaFile.open((bfsWorkPath/"Summary.csv").string().c_str(),ios::ate|ios::app);
  ofsParaFile<<"Program infor: "+sProgramName+"-"+sProgramVersion<<std::endl;

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
  //ofsParaFile.open((bfsWorkPath/"Summary.csv").string().c_str(),ios::ate|ios::app);

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
  G4SDManager::GetSDMpointer()->ListTree();

  ofsParaFile 
    <<"\n------Begin Of Run: "<<iRunID<<"------"
    <<"\nRun Time: "<<sTime
    <<"\nRun ID: "<<iRunID
    <<"\nEvents Number:"<<numEvt
    <<G4endl;
  G4cout<<"Init Field."<<G4endl;
  GPFieldSetup::GetGPFieldSetup()->Init();
  GPModuleManager::GetInstance()->Print(ofsParaFile);
  primaryGenerator->Print(ofsParaFile);


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
