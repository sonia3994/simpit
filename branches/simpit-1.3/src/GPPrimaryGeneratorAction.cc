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
// $Id: GPPrimaryGeneratorAction.cc,v 1.6 2006/06/29 17:47:23 gunter Exp $
// GEANT4 tag $Name: geant4-09-02 $
//

#include "GPPrimaryGeneratorAction.hh"
#include "GPDetectorConstruction.hh"
#include "GPPrimaryGeneratorMessenger.hh"
#include "GPHEPEvtInterface.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"

//#include "globals.hh"
//#include "Randomize.hh"

#include <sys/types.h>
#include <unistd.h>

#include <cmath>

GPPrimaryGeneratorAction::GPPrimaryGeneratorAction(GPDetectorConstruction* myDC):myDetector(myDC)
{

  ePositionMean=0.0*mm;
  ePositionRMS=2.0*mm;
  eEnergyMean=30.0*MeV;
  eEnergyRMS=10.0*MeV;
  eMomentumMean=0.0;
  eMomentumRMS=0.01;
  particlePosZ=-3.0*mm;
  bunchLength=6*ns;

  verbose=0;
  n_particle = 1;
  G4int index=getpid();

  CLHEP:: HepRandom::setTheSeed(time(0),index);
  randGauss = new CLHEP::RandGauss(&ranecuEngine,0.,2.);
//  randFlat=new CLHEP::RandFlat(&ranecuEngine);
  srand(time(0));

  particleGun = new G4ParticleGun(n_particle);
  primaryMessenger = new GPPrimaryGeneratorMessenger(this);

//default particle
  particleStyle = "gamma";
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* particle = particleTable->FindParticle(particleStyle);
  particleGun->SetParticleDefinition(particle);
//particleGun->SetParticleTime(0.0*ns);
  particleGun->SetParticlePosition(G4ThreeVector(0.0*mm,0.0*mm,particlePosZ));
  particleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
  particleGun->SetParticleEnergy(30.*MeV);

  HEPEvt = new GPHEPEvtInterface();
  HEPEvtFlag = false;
}

GPPrimaryGeneratorAction::~GPPrimaryGeneratorAction()
{
  if(particleGun) delete particleGun;
//  if(randGauss) delete randGauss;// we should not free randGausss, but why????
//  if(randFlat) delete randFlat;
  if(primaryMessenger) delete primaryMessenger;
  if(HEPEvt) delete HEPEvt;
}

void GPPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  if(HEPEvtFlag)
    { 
      HEPEvt->GeneratePrimaryVertex(anEvent);
      return;
    }

  G4double 	r=randGauss->shoot(ePositionMean,ePositionRMS);
  G4double 	theta=rand()%360;
  r=std::abs(r);
  G4double 	x0=r*cos(theta);
  G4double 	y0=r*sin(theta);
  G4double 	ptheta=rand()%360;
  G4double 	ptm=2.0;

  while(std::abs(ptm)>1.0)
  {
  ptm=randGauss->shoot(eMomentumMean,eMomentumRMS);
  }
  G4double 	px0=ptm*cos(ptheta);
  G4double 	py0=ptm*sin(ptheta);
  G4double 	pz0=sqrt(1.0-(px0*px0+py0*py0));
  G4double 	energy=randGauss->shoot(eEnergyMean,eEnergyRMS);

  if(verbose>=1)
  {
  G4cout<<"\nThe initial particle condition:"
		<<"\nx0: "<<x0<<" mm, y0: "<<y0<<" mm, z0: "<<particlePosZ<<""
		<<"\npx0: "<<px0<<" py0: "<<py0<<" pz0: "<<pz0
		<<"\nenergy: "<<energy<<" MeV"<<G4endl;
  }

  particleGun->SetParticlePosition(G4ThreeVector(x0,y0,particlePosZ));
  particleGun->SetParticleMomentumDirection(G4ThreeVector(px0,py0,pz0));
  particleGun->SetParticleEnergy(energy);
    
  particleGun->GeneratePrimaryVertex(anEvent);

}


void GPPrimaryGeneratorAction::SetParticleStyle(G4String tmpParticleStyle)
{
  particleStyle = tmpParticleStyle;
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* particle = particleTable->FindParticle(particleStyle);
  particleGun->SetParticleDefinition(particle);
  G4cout<<"\nThe initial Particle is :"<<particleStyle<<"\n"<<G4endl;
}

void GPPrimaryGeneratorAction::SetVerboseLevel(G4int ver)
{
  verbose=ver;
  G4cout<<"The verbose level is set to "<<ver<<G4endl;
}

void GPPrimaryGeneratorAction::SetParticleEnergyDistr(G4double tmpMean,G4double tmpRMS)
{
  eEnergyMean=tmpMean;
  eEnergyRMS=tmpRMS;
  G4cout<<"The initial energy distribution is set to mean: 	"
	 	<<tmpMean/MeV<<" MeV, rms: "<< tmpRMS/MeV<<" MeV."<<G4endl;
}

void GPPrimaryGeneratorAction::SetParticlePositionDistr(G4double tmpMean,G4double tmpRMS)
{
  ePositionMean=tmpMean;
  ePositionRMS=tmpRMS;
  G4cout<<"The initial position distribution is set to mean: 	"
	 	<<tmpMean/mm<<" mm, rms: "<< tmpRMS/mm<<" mm."<<G4endl;
}

void GPPrimaryGeneratorAction::SetParticlePositionZ(G4double tmp)
{
  particlePosZ=tmp;
  if(HEPEvt!=0) HEPEvt->SetParticlePosZ(tmp);
}

void GPPrimaryGeneratorAction::SetBunchLength(G4double tmp)
{
  bunchLength=tmp;
  if(HEPEvt!=0) HEPEvt->SetBunchLength(tmp);
}

void GPPrimaryGeneratorAction::SetParticleMomentumDistr(G4double tmpMean,G4double tmpRMS)
{
  if(std::abs(tmpMean>1.0)||std::abs(tmpRMS)>1.0)
  { G4cout<<" Wrong value, must set between [-1,1]!"<<G4endl;return;}

  eMomentumMean=tmpMean;
  eMomentumRMS=tmpRMS;
  G4cout<<"The initial momentum direction distribution is set to mean: 	"
	 	<<tmpMean<<", rms: "<< tmpRMS<<"."<<G4endl;
}

void GPPrimaryGeneratorAction::SetParticleInitNumber(G4int tmp)
{
  particleGun->SetNumberOfParticles(tmp);
  n_particle=tmp;
  G4cout<<"The initial number per event is set to "<<tmp<<G4endl;

}

void GPPrimaryGeneratorAction::PrintPrimaryMessage()
{ 
  if(HEPEvtFlag == true)
    {
  		G4cout
		<<"\n------------------Print primary status---------------------------\n"
		<<"Primary Generator is implemented from file: "<<inputFile<<"\n"
		<<"-----------------------------------------------------------------\n"
        <<G4endl;	
		return;
	}

  		G4cout
		<<"\n------------------Print primary status---------------------------\n"
  		<<"Selected distribution is: CLHEP::RandGauss\n	"
    	<<"primary particle \n"
    	<<"particle style: "<<particleStyle<<"\n"
    	<<"number per event: "<<n_particle<<"\n"
    	<<"energy mean: "<<eEnergyMean<<"*MeV\n"
    	<<"energy rms: "<<eEnergyRMS<<"*MeV\n"
    	<<"position mean(Transverse): "<<ePositionMean<<"*mm\n"
    	<<"position rms(Transverse): "<<ePositionRMS<<"*mm\n"
    	<<"position Z : "<<particlePosZ<<"*mm\n"
    	<<"momentum mean(Transverse): "<<eMomentumMean<<"\n"
    	<<"momentum rms(Transverse): "<<eMomentumRMS<<"\n"
		<<"-----------------------------------------------------------------\n"
    	<<G4endl;

}

void GPPrimaryGeneratorAction::SetInputFile(G4String tmp)
{ 
	if(HEPEvt) 
  	{
   		HEPEvt->SetInputFile(tmp); 
   		inputFile=tmp;
  	}
  	else
	{
   		HEPEvt = new GPHEPEvtInterface(tmp);
   		inputFile=tmp;
   	}
}

void GPPrimaryGeneratorAction::SetEnergyUnit(G4String tmp)
{
  if(HEPEvt!=0) HEPEvt->SetEnergyUnit(tmp);
}

void GPPrimaryGeneratorAction::SetMomentumUnit(G4String tmp)
{
  if(HEPEvt!=0) HEPEvt->SetMomentumUnit(tmp);
}

void GPPrimaryGeneratorAction::SetLengthUnit(G4String tmp)
{
  if(HEPEvt!=0) HEPEvt->SetLengthUnit(tmp);
}

void GPPrimaryGeneratorAction::SetInputFileRMSFactor(G4double tmp)
{
  if(HEPEvt!=0) HEPEvt->SetInputFileRMSFactor(tmp);
}
