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

  dPositionMean=0.0*mm;
  dPositionRMS=2.0*mm;
  dEnergyMean=30.0*MeV;
  dEnergyRMS=10.0*MeV;
  dMommentumMean=0.1;
  dMommentumRMS=0.1;
  dParticlePosZ=-3.0*mm;
  dBunchLength=6*ns;
  bFixedParticleGun=true;
  vectMommentumDirection=G4ThreeVector(0,0,1);

  verbose=0;
  iNParticles = 1;
  G4int index=getpid();

  CLHEP:: HepRandom::setTheSeed(time(0),index);
  randGauss = new CLHEP::RandGauss(&ranecuEngine,0.,2.);
//  randFlat=new CLHEP::RandFlat(&ranecuEngine);
  srand(time(0));

  particleGun = new G4ParticleGun(iNParticles);
  primaryMessenger = new GPPrimaryGeneratorMessenger(this);

//default particle
  sParticleStyle = "gamma";
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* particle = particleTable->FindParticle(sParticleStyle);
  particleGun->SetParticleDefinition(particle);
//particleGun->SetParticleTime(0.0*ns);
  particleGun->SetParticlePosition(G4ThreeVector(0.0*mm,0.0*mm,dParticlePosZ));
  particleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
  particleGun->SetParticleEnergy(30.*MeV);

  HEPEvt = new GPHEPEvtInterface();
  bHEPEvtFlag = false;
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
  if(bHEPEvtFlag)
    { 
      HEPEvt->GeneratePrimaryVertex(anEvent);
      return;
    }

  else if(bFixedParticleGun)
    {
      GeneratePrimariesFixedParticleGun(anEvent);
	  return;
    }

  G4double 	r=randGauss->shoot(dPositionMean,dPositionRMS);
  G4double 	theta=rand()%360;
  r=std::abs(r);
  G4double 	x0=r*cos(theta);
  G4double 	y0=r*sin(theta);
  G4double 	ptheta=rand()%360;
  G4double 	ptm=2.0;

  while(std::abs(ptm)>1.0)
  {
  ptm=randGauss->shoot(dMommentumMean,dMommentumRMS);
  }
  ptm=std::abs(ptm);
  G4double 	px0=ptm*cos(ptheta);
  G4double 	py0=ptm*sin(ptheta);
  G4double 	pz0=sqrt(1.0-(px0*px0+py0*py0));
  G4double 	energy=randGauss->shoot(dEnergyMean,dEnergyRMS);
  energy=std::abs(energy);

  if(verbose>=1)
  {
  G4cout<<"\nuse gauss to shoot particle initial parameters:"
        <<"\nThe initial particle condition:"
		<<"\nx0: "<<x0<<" mm, y0: "<<y0<<" mm, z0: "<<dParticlePosZ<<""
		<<"\npx0: "<<px0<<" py0: "<<py0<<" pz0: "<<pz0
		<<"\nenergy: "<<energy<<" MeV"<<G4endl;
  }

  particleGun->SetParticlePosition(G4ThreeVector(x0,y0,dParticlePosZ));
  particleGun->SetParticleMomentumDirection(G4ThreeVector(px0,py0,pz0));
  particleGun->SetParticleEnergy(energy);
    
  particleGun->GeneratePrimaryVertex(anEvent);

}

void GPPrimaryGeneratorAction::GeneratePrimariesFixedParticleGun(G4Event* anEvent)
{
  G4double 	r=dPositionMean;
  G4double 	theta=rand()%360;
  G4double 	x0=r*cos(theta);
  G4double 	y0=r*sin(theta);

  G4double 	energy=std::abs(dEnergyMean);

  if(verbose>=1)
  {
  G4cout<<"\nShoot "<<iNParticles<<" primary particle(s):"
		<<"\nx0: "<<x0<<" mm, y0: "<<y0<<" mm, z0: "<<dParticlePosZ<<""
		<<"\npx0: "<<vectMommentumDirection.x()<<" py0: "<<vectMommentumDirection.y()<<" pz0: "<<vectMommentumDirection.z()
		<<"\nenergy: "<<energy<<" MeV"<<G4endl;
  }

  particleGun->SetParticlePosition(G4ThreeVector(x0,y0,dParticlePosZ));
  particleGun->SetParticleMomentumDirection(vectMommentumDirection);
  particleGun->SetParticleEnergy(energy);
    
  particleGun->GeneratePrimaryVertex(anEvent);

}

void GPPrimaryGeneratorAction::SetParticleStyle(G4String tmpParticleStyle)
{
  sParticleStyle = tmpParticleStyle;
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* particle = particleTable->FindParticle(sParticleStyle);
  particleGun->SetParticleDefinition(particle);
  G4cout<<"\nThe initial Particle is :"<<sParticleStyle<<"\n"<<G4endl;
}

void GPPrimaryGeneratorAction::SetParticleMomentumDirection(G4ThreeVector t)
{
  vectMommentumDirection=t.unit();
  G4cout<<"Set particle momentum direction. px: "<<vectMommentumDirection.x()<<" px: "<<vectMommentumDirection.y()<<" pz: "<<vectMommentumDirection.z()<<G4endl;
}

void GPPrimaryGeneratorAction::SetVerboseLevel(G4int ver)
{
  verbose=ver;
  G4cout<<"The verbose level is set to "<<ver<<G4endl;
}

void GPPrimaryGeneratorAction::SetParticleEnergyDistr(G4double tmpMean,G4double tmpRMS)
{
  if (tmpRMS<0)
  { G4cout<<" Wrong value, RMS must set between [0,++]!"<<G4endl;return;}
  dEnergyMean=tmpMean;
  dEnergyRMS=tmpRMS;
  G4cout<<"The initial energy distribution is set to mean: 	"
	 	<<tmpMean/MeV<<" MeV, rms: "<< tmpRMS/MeV<<" MeV."<<G4endl;
}

void GPPrimaryGeneratorAction::SetParticlePositionDistr(G4double tmpMean,G4double tmpRMS)
{
  if (tmpRMS<0)
  { G4cout<<" Wrong value, RMS must set between [0,++]!"<<G4endl;return;}
    
  dPositionMean=tmpMean;
  dPositionRMS=tmpRMS;
  G4cout<<"The initial position distribution is set to mean: 	"
	 	<<tmpMean/mm<<" mm, rms: "<< tmpRMS/mm<<" mm."<<G4endl;
}

void GPPrimaryGeneratorAction::SetParticlePositionZ(G4double tmp)
{
  dParticlePosZ=tmp;
  if(HEPEvt!=0) HEPEvt->SetParticlePosZ(tmp);
}

void GPPrimaryGeneratorAction::SetBunchLength(G4double tmp)
{
  dBunchLength=tmp;
  if(HEPEvt!=0) HEPEvt->SetBunchLength(tmp);
}

void GPPrimaryGeneratorAction::SetParticleMomentumDistr(G4double tmpMean,G4double tmpRMS)
{
  if(std::abs(tmpMean>1.0)||std::abs(tmpRMS)>1.0)
  { G4cout<<" Wrong value, must set between [0,1]!"<<G4endl;return;}

  dMommentumMean=tmpMean;
  dMommentumRMS=tmpRMS;
  G4cout<<"The initial momentum direction distribution is set to mean: 	"
	 	<<tmpMean<<", rms: "<< tmpRMS<<"."<<G4endl;
}

void GPPrimaryGeneratorAction::SetParticleInitNumber(G4int tmp)
{
  particleGun->SetNumberOfParticles(tmp);
  iNParticles=tmp;
  G4cout<<"The initial number per event is set to "<<tmp<<G4endl;

}

void GPPrimaryGeneratorAction::PrintPrimaryMessage()
{ 
  if(bHEPEvtFlag == true)
    {
  		G4cout
		<<"\n------------------Print primary status---------------------------\n"
		<<"Primary Generator is implemented from file: "<<sInputFile<<"\n"
		<<"-----------------------------------------------------------------\n"
        <<G4endl;	
		return;
	}
  else if(bFixedParticleGun== true)
    {
  		G4cout
		<<"\n------------------Print primary status---------------------------\n"
  		<<"Fixed primary condition\n	"
    	<<"Primary particle \n"
    	<<"Particle style: "<<sParticleStyle<<"\n"
    	<<"Number per event: "<<iNParticles<<"\n"
    	<<"Energy : "<<dEnergyMean<<"*MeV\n"
    	<<"Position radius(Transverse): "<<dPositionMean<<"*mm\n"
    	<<"Position Z : "<<dParticlePosZ<<"*mm\n"
    	<<"Momentum direction:" 
		<<" px0: "<<vectMommentumDirection.x()
		<<" py0: "<<vectMommentumDirection.y()
		<<" pz0: "<<vectMommentumDirection.z()<<"\n"
		<<"-----------------------------------------------------------------\n"
        <<G4endl;	
		return;
	}

  		G4cout
		<<"\n------------------Print primary status---------------------------\n"
  		<<"Selected distribution is: CLHEP::RandGauss\n	"
    	<<"Primary particle \n"
    	<<"Particle style: "<<sParticleStyle<<"\n"
    	<<"Number per event: "<<iNParticles<<"\n"
    	<<"Energy mean: "<<dEnergyMean<<"*MeV\n"
    	<<"Energy rms: "<<dEnergyRMS<<"*MeV\n"
    	<<"Position mean(Transverse): "<<dPositionMean<<"*mm\n"
    	<<"Position rms(Transverse): "<<dPositionRMS<<"*mm\n"
    	<<"Position Z : "<<dParticlePosZ<<"*mm\n"
    	<<"Momentum mean(Transverse): "<<dMommentumMean<<"\n"
    	<<"Momentum rms(Transverse): "<<dMommentumRMS<<"\n"
		<<"-----------------------------------------------------------------\n"
    	<<G4endl;

}

void GPPrimaryGeneratorAction::SetInputFile(G4String tmp)
{ 
	if(HEPEvt) 
  	{
   		HEPEvt->SetInputFile(tmp); 
   		sInputFile=tmp;
  	}
  	else
	{
   		HEPEvt = new GPHEPEvtInterface(tmp);
   		sInputFile=tmp;
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
