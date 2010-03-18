//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
// $Id: GPRunAction.cc,v 1.19 2008/01/17 17:31:32 maire Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "GPRunAction.hh"
#include "GPPrimaryGeneratorAction.hh"
#include "GPDetectorConstruction.hh"

#include "globals.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
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
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GPRunAction::~GPRunAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GPRunAction::BeginOfRunAction(const G4Run* aRun)
{ 
//inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(true);

  G4String filePath;
  G4String fileName;
  G4String chrunID;
  stringstream ss;
  G4int runID=aRun->GetRunID();
  G4int numEvt=aRun->GetNumberOfEventToBeProcessed();
  targetSDFlag=G4SDManager::GetSDMpointer()->FindSensitiveDetector("mydet/target")->isActive();
  ss <<runID;
  ss >>chrunID;
  //mkdir(filePath,0755);
  //
  
  time_t time_m=time(0);
  G4String tmpStr = primaryGenerator->GetInputFileName();
  if(tmpStr.empty())
  {
	  tmpStr="PGun";
  }
  else
  {
  	size_t pos1=tmpStr.find_last_of(".");
  	size_t pos2=tmpStr.size();
	if(pos1>0&&pos1<pos2)
	{
		tmpStr.erase(pos1,pos2-1);
	}
	pos1=tmpStr.find_last_of("/");
	if((pos1>=0)&&pos1<pos2)
	{ 
		tmpStr.erase(0,pos1+1);
	}
  }

  filePath="../out_"+tmpStr;

  tmpStr.clear();
  tmpStr.insert(0,ctime(&time_m));
  replace(tmpStr.begin(),tmpStr.end(),' ','-');
  tmpStr.resize(24);
  filePath+="_"+tmpStr+"/";
G4cout<<"mkdir: "<<filePath<<G4endl;  
  mkdir(filePath,0755);

  fileName=filePath +"SumAtExitOfTar.dat";
  paraFile.open(fileName,ios::ate|ios::app);

  fileName=filePath+chrunID+"ExitOfTar.dat";
  dataFileDT.open(fileName);

  fileName=filePath+chrunID+"ExitOfCap.dat";  
  dataFileDC.open(fileName);

  if(targetSDFlag)
  {
  fileName=filePath+chrunID+"EddInTar.dat";
  eddHandle.open(fileName);
  }

  G4cout << "### Run " << runID<< " start." <<"\n"
  <<"target sensitive detector status "<<targetSDFlag<<G4endl;
  mydetector->PrintDetectorParameters();
  primaryGenerator->PrintPrimaryMessage();

  paraFile 
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
  sumETar = sum2ETar = 0.;
  sumLTrack = sum2LTrack = 0.; 
  positronPerRun=0;
  //
  edd.clear();
  if(targetSDFlag)
  {
  eddDim=mydetector->GetEddDim();
  edd.resize(eddDim[0]*eddDim[1]*eddDim[2]);
  for(size_t i=0;i!=edd.size();i++)
  {edd[i]=0;}
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GPRunAction::FillPerEvent(G4double ETar,G4double LTrack,G4int positronPerEvt)
{
  //accumulate statistic
  //
  sumETar += ETar;  sum2ETar += ETar*ETar;
  sumLTrack += LTrack;  sum2LTrack += LTrack*LTrack;
  positronPerRun +=positronPerEvt;  
}

//
void GPRunAction::AddEddHit(G4int x, G4int y, G4int z, G4double e)
{
  if(targetSDFlag)
  {
  G4int index=z*eddDim[0]*eddDim[1]+y*eddDim[0]+x;
  edd[index]=edd[index]+e;
  }
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GPRunAction::EndOfRunAction(const G4Run* aRun)
{
  G4int NbOfEvents = aRun->GetNumberOfEvent();
  if (NbOfEvents == 0) return;
  
  //compute statistics: mean and rms
  //
  //sumETar /= NbOfEvents; sum2ETar /= NbOfEvents;
  G4double rmsETar = sum2ETar - sumETar*sumETar;
  if (rmsETar >0.) rmsETar = std::sqrt(rmsETar); else rmsETar = 0.;
  
  
  sumLTrack /= NbOfEvents; sum2LTrack /= NbOfEvents;
  G4double rmsLTrack = sum2LTrack - sumLTrack*sumLTrack;
  if (rmsLTrack >0.) rmsLTrack = std::sqrt(rmsLTrack);
  else rmsLTrack = 0.;
  
  //print
  //
  G4cout
     << "\n--------------------Results------------------------------\n"
     << "\n Total number of this Run:"<<positronPerRun<<"\n"
     << "\n Total energy deposited in Target of this run : " << G4BestUnit(sumETar,"Energy")
     << "\n Mean energy deposited in Target per event of this run : " << G4BestUnit(sumETar/NbOfEvents,"Energy")
     << " +- "                          << G4BestUnit(rmsETar,"Energy")  
     << G4endl;
     
  G4cout
     << "\n mean trackLength in Target : " << G4BestUnit(sumLTrack,"Length")
     << " +- "                               << G4BestUnit(rmsLTrack,"Length")  
     << "\n--------------------End of Run------------------------------\n"
     << G4endl;
  paraFile
	<<positronPerRun<<" "
	<<sumETar<<" "
	<<rmsETar<<" "
	<<sumLTrack<<" "
	<<rmsLTrack
     	<< G4endl;
  paraFile.close();
  dataFileDT.close();
  dataFileDC.close();
  //G4double x,y,z;
  //G4double dx=mydetector->GetTargetXY()/eddDim[0];
  //G4double dy=mydetector->GetTargetXY()/eddDim[1];
  //G4double dz=mydetector->GetTargetThickness()/eddDim[2];
  long int index;
  if(targetSDFlag)
  {
  for(G4int i=0;i!=eddDim[0];i++)
  {
	//x=i*dx-(eddDim[0]-1)*dx*0.5;
     for(G4int j=0;j!=eddDim[1];j++)
     {
	//y=j*dy-(eddDim[1]-1)*dy*0.5;

        for(G4int k=0;k!=eddDim[2];k++)
	{
	//z=k*dz-(eddDim[2]-1)*dz*0.5;
	index=k*eddDim[0]*eddDim[1]+j*eddDim[0]+i;
	eddHandle<<i<<" "<<j<<" "<<k<<" "<<edd[index]<<"\n";
	//eddHandle<<x<<" "<<y<<" "<<z<<" "<<edd[k*eddDim[0]*eddDim[1]+j*eddDim[0]+i]<<"\n";
	}
     }
  } 
  eddHandle<<G4endl;
  eddHandle.close();
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
