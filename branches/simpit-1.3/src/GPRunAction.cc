// $Id: GPRunAction.cc,v 1.19 2008/01/17 17:31:32 maire Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "GPRunAction.hh"
#include "GPPrimaryGeneratorAction.hh"
#include "GPDetectorConstruction.hh"
#include "GPFieldSetup.hh"
#include "GPSteppingAction.hh"

#include "globals.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4FieldManager.hh"
#include "G4SDManager.hh"
#include "G4UnitsTable.hh"
#include <fstream>
#include <sstream>
#include <algorithm>

#include<unistd.h>
#include<dirent.h>
#include<sys/types.h>
#include <sys/stat.h>   //mkdir in this head file

using namespace std;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GPRunAction::GPRunAction(GPPrimaryGeneratorAction* generator,GPDetectorConstruction* detector)
:primaryGenerator(generator),mydetector(detector)
{
  time_t time_m=time(0);
  G4String tmpStr;
  tmpStr.insert(0,ctime(&time_m));
  tmpStr.resize(24);
  replace(tmpStr.begin(),tmpStr.end(),' ','-');
  //replace(tmpStr.begin(),tmpStr.end(),':','-');
  sFilePath="../"+tmpStr+"/";
	G4cout<<"mkdir: "<<sFilePath<<G4endl;  
  mkdir(sFilePath,0755);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GPRunAction::~GPRunAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GPRunAction::BeginOfRunAction(const G4Run* aRun)
{ 
//inform the runManager to save random number seed
  std::pair<std::string,std::ofstream* > pairHandle;
  G4String fileName;
  G4String chrunID;
  stringstream ss;
  G4int runID=aRun->GetRunID();
  G4int numEvt=aRun->GetNumberOfEventToBeProcessed();
  ss <<runID;
  ss >>chrunID;

  G4RunManager::GetRunManager()->SetRandomNumberStore(true);

  G4cout<<"\n========================Begin of Run: "<<runID<<"==================================\n"
  <<"Prepare run: "<<G4endl;
  GPSteppingAction* gpSteppingAction=(GPSteppingAction*)G4RunManager::GetRunManager()->GetUserSteppingAction();
  if(gpSteppingAction) gpSteppingAction->Init();
  G4cout<<"Init SteppingActionn."<<G4endl;

  GPFieldSetup* gpFieldSetup=(GPFieldSetup*)mydetector->GetFieldSetup();
  if(gpFieldSetup) gpFieldSetup->Init();
  G4cout<<"Init Field."<<G4endl;

  bTargetSDFlag=G4SDManager::GetSDMpointer()->FindSensitiveDetector("mydet/target")->isActive();
  G4cout<<"Target sensitive detector status: "<<bTargetSDFlag<<G4endl;

  mapStrOfsOutputHandler.clear();
  fileName=sFilePath +"SumAtExitOfTar.dat";
  ofsParaFile.open(fileName,ios::ate|ios::app);

  fileName=sFilePath+chrunID+"ExitOfTar.dat";
  ofsDataFileDT = new std::ofstream(fileName);
  pairHandle.first="target";
  pairHandle.second=ofsDataFileDT;
  mapStrOfsOutputHandler.insert(pairHandle);

  fileName=sFilePath+chrunID+"ExitOfCap.dat";  
  ofsDataFileDC = new std::ofstream(fileName);
  pairHandle.first="capture";
  pairHandle.second=ofsDataFileDC;
  mapStrOfsOutputHandler.insert(pairHandle);

  fileName=sFilePath+chrunID+"ExitOfAcc.dat";  
  ofsDataFileAC = new std::ofstream(fileName);
  pairHandle.first="accelerator";
  pairHandle.second=ofsDataFileAC;
  mapStrOfsOutputHandler.insert(pairHandle);


  if(bTargetSDFlag)
  {
  fileName=sFilePath+chrunID+"EddInTar.dat";
  ofsEddHandle.open(fileName);
  }

  G4cout<<"Created output files handlers"<<G4endl;
  mydetector->PrintDetectorParameters();
  primaryGenerator->PrintPrimaryMessage();

  G4cout << "Start run: " << runID<<G4endl;

  ofsParaFile 
	<<runID<<" "
//        RunID
	<<mydetector->GetTargetThickness()<<" "

//        target thickness
	<<numEvt*primaryGenerator->GetParticleInitNumber()<<" "
//        injection particles number
	<<numEvt<<" "
//	number of events
	<<primaryGenerator->GetParticleInitNumber()<<" "
//	number of this event
	<<primaryGenerator->GetParticleEnergyMean()<<" "
//	primary energy mean
	<<primaryGenerator->GetParticleEnergyRMS()<<" "
//	primary energy rms
	<<primaryGenerator->GetParticlePositionMean()<<" "	
//	primary positon mean
	<<primaryGenerator->GetParticlePositionRMS()<<" "
//	primary positon rms: 
	<<primaryGenerator->GetParticleMomentumMean()<<" "
//	primary momentum mean
	<<primaryGenerator->GetParticleMomentumRMS()<<" "
	;
//	nprimary momentum rms
  //initialize cumulative quantities
  //
  dSumETar = dSum2ETar = 0.;
  dSumLTrack = dSum2LTrack = 0.; 
  iPositronPerRun=0;
	iActualG=0;
  //
  vecDouEdd.clear();
  if(bTargetSDFlag)
  {
  vecIntEddDim=mydetector->GetEddDim();
  vecDouEdd.resize(vecIntEddDim[0]*vecIntEddDim[1]*vecIntEddDim[2]);
  for(size_t i=0;i!=vecDouEdd.size();i++)
  {vecDouEdd[i]=0;}
  }
#ifdef GP_DEBUG
  G4cout<<"Open new output file handls and begin to run."<<G4endl;
#endif
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
  G4int index=z*vecIntEddDim[0]*vecIntEddDim[1]+y*vecIntEddDim[0]+x;
  vecDouEdd[index]=vecDouEdd[index]+e;
  }
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GPRunAction::EndOfRunAction(const G4Run* aRun)
{
  G4int NbOfEvents = aRun->GetNumberOfEvent();
  if (NbOfEvents == 0) return;
  
  //compute statistics: mean and rms
  //
  //dSumETar /= NbOfEvents; dSum2ETar /= NbOfEvents;
  G4double rmsETar = dSum2ETar - dSumETar*dSumETar;
  if (rmsETar >0.) rmsETar = std::sqrt(rmsETar); else rmsETar = 0.;
  
  
  dSumLTrack /= NbOfEvents; dSum2LTrack /= NbOfEvents;
  G4double rmsLTrack = dSum2LTrack - dSumLTrack*dSumLTrack;
  if (rmsLTrack >0.) rmsLTrack = std::sqrt(rmsLTrack);
  else rmsLTrack = 0.;
  
  //print
  //
  G4cout
     <<"\n--------------------Results------------------------------\n"
     <<"The actual gamma impinged the target: "<<iActualG <<"\n"
     <<"Total number of this Run:"<<iPositronPerRun<<"\n"
     <<"Total energy deposited in Target of this run : " << G4BestUnit(dSumETar,"Energy")<<"\n"
     <<"Mean energy deposited in Target per event of this run : " << G4BestUnit(dSumETar/NbOfEvents,"Energy")
     <<" +- "<< G4BestUnit(rmsETar,"Energy")<<"\n"  
     <<"mean trackLength in Target : " << G4BestUnit(dSumLTrack,"Length")
     <<" +- "                               << G4BestUnit(rmsLTrack,"Length")<<"\n"  
     <<"---------------------End of results-------------------------\n"
     <<"==================================End of Run ===================================\n"
     << G4endl;
  ofsParaFile
	<<iPositronPerRun<<" "
	<<dSumETar<<" "
	<<rmsETar<<" "
	<<dSumLTrack<<" "
	<<rmsLTrack
     	<< G4endl;

  	ofsParaFile.close();
	for(std::map<std::string, std::ofstream* >::iterator iter=mapStrOfsOutputHandler.begin();iter!=mapStrOfsOutputHandler.end();iter++)
	{
		(iter->second)->close();
	}
	 mapStrOfsOutputHandler.clear();
	
  //G4double x,y,z;
  //G4double dx=mydetector->GetTargetXY()/vecIntEddDim[0];
  //G4double dy=mydetector->GetTargetXY()/vecIntEddDim[1];
  //G4double dz=mydetector->GetTargetThickness()/vecIntEddDim[2];
  long int index;
  if(bTargetSDFlag)
  {
	  for(G4int i=0;i!=vecIntEddDim[0];i++)
	  {
		//x=i*dx-(vecIntEddDim[0]-1)*dx*0.5;
	     for(G4int j=0;j!=vecIntEddDim[1];j++)
	     {
		//y=j*dy-(vecIntEddDim[1]-1)*dy*0.5;
	
	        for(G4int k=0;k!=vecIntEddDim[2];k++)
			{
				//z=k*dz-(vecIntEddDim[2]-1)*dz*0.5;
				index=k*vecIntEddDim[0]*vecIntEddDim[1]+j*vecIntEddDim[0]+i;
				ofsEddHandle<<i<<" "<<j<<" "<<k<<" "<<vecDouEdd[index]<<"\n";
				//ofsEddHandle<<x<<" "<<y<<" "<<z<<" "<<vecDouEdd[k*vecIntEddDim[0]*vecIntEddDim[1]+j*vecIntEddDim[0]+i]<<"\n";
			}
	     }
	  } 
	  ofsEddHandle<<G4endl;
	  ofsEddHandle.close();
  }
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
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
