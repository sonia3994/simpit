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
#include <iomanip>

#define MacRightAlign  std::setiosflags(std::ios_base::right)
#define MacLeftAlign  std::setiosflags(std::ios_base::left)

GPPrimaryGeneratorAction::GPPrimaryGeneratorAction(GPDetectorConstruction* myDC):myDetector(myDC)
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPPrimaryGeneratorAction::GPPrimaryGeneratorAction(GPDetectorConstruction*)"<<G4endl;
#endif

  dPositionMean=0.0e-3;
  dPositionRMS=2.0e-3;
  dEnergyMean=30.0;
  dEnergyRMS=10.0;
  dMommentumMean=0.1;
  dMommentumRMS=0.1;
  dParticlePosZ=-3.0e-3;
  dBunchLength=6;
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
  particleGun->SetParticlePosition(G4ThreeVector(0.0*m,0.0*m,dParticlePosZ*m));
  particleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
  particleGun->SetParticleEnergy(dEnergyMean*MeV);

  HEPEvt = new GPHEPEvtInterface();
  bHEPEvtFlag = false;
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPPrimaryGeneratorAction::GPPrimaryGeneratorAction(GPDetectorConstruction*)"<<G4endl;
#endif
}

GPPrimaryGeneratorAction::~GPPrimaryGeneratorAction()
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPPrimaryGeneratorAction::~GPPrimaryGeneratorAction()"<<G4endl;
#endif
  if(particleGun) delete particleGun;
//  if(randGauss) delete randGauss;// we should not free randGausss, but why????
//  if(randFlat) delete randFlat;
  if(primaryMessenger) delete primaryMessenger;
  if(HEPEvt) delete HEPEvt;
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPPrimaryGeneratorAction::~GPPrimaryGeneratorAction()"<<G4endl;
#endif
}

void GPPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPPrimaryGeneratorAction::GeneratePrimaries(G4Event*)"<<G4endl;
#endif
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
		<<"\nx0: "<<x0<<" m, y0: "<<y0<<" m, z0: "<<dParticlePosZ<<" m"
		<<"\npx0: "<<px0<<" py0: "<<py0<<" pz0: "<<pz0
		<<"\nenergy: "<<energy<<" MeV"<<G4endl;
  }

  particleGun->SetParticlePosition(G4ThreeVector(x0*m,y0*m,dParticlePosZ*m));
  particleGun->SetParticleMomentumDirection(G4ThreeVector(px0,py0,pz0));
  particleGun->SetParticleEnergy(energy*MeV);
    
  particleGun->GeneratePrimaryVertex(anEvent);

#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPPrimaryGeneratorAction::GeneratePrimaries(G4Event*)"<<G4endl;
#endif
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
		<<"\nx0: "<<x0<<" m, y0: "<<y0<<" m, z0: "<<dParticlePosZ<<" m"
		<<"\npx0: "<<vectMommentumDirection.x()<<" py0: "<<vectMommentumDirection.y()<<" pz0: "<<vectMommentumDirection.z()
		<<"\nenergy: "<<energy<<" MeV"<<G4endl;
  }

  particleGun->SetParticlePosition(G4ThreeVector(x0*m,y0*m,dParticlePosZ*m));
  particleGun->SetParticleMomentumDirection(vectMommentumDirection);
  particleGun->SetParticleEnergy(energy*MeV);
    
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
	 	<<tmpMean*m/mm<<" mm, rms: "<< tmpRMS*m/mm<<" mm."<<G4endl;
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
  	<<MacRightAlign<<std::setw(32)<<"Fixed primary condition:\n"
    	<<MacRightAlign<<std::setw(32)<<"Particle style: "<<sParticleStyle<<"\n"
    	<<MacRightAlign<<std::setw(32)<<"Number per event: "<<iNParticles<<"\n"
    	<<MacRightAlign<<std::setw(32)<<"Energy : "<<dEnergyMean<<"MeV\n"
    	<<MacRightAlign<<std::setw(32)<<"Position radius(Transverse): "<<dPositionMean<<" m\n"
    	<<MacRightAlign<<std::setw(32)<<"Position Z : "<<dParticlePosZ<<" m\n"
    	<<MacRightAlign<<std::setw(32)<<"Momentum direction: "<<"\n" 
	<<MacRightAlign<<std::setw(32)<<"px0: "<<vectMommentumDirection.x()<<"\n"
	<<MacRightAlign<<std::setw(32)<<"py0: "<<vectMommentumDirection.y()<<"\n"
	<<MacRightAlign<<std::setw(32)<<"pz0: "<<vectMommentumDirection.z()<<"\n"
	<<"-----------------------------------------------------------------\n"
        <<G4endl;	
	return;
     }

       G4cout
        <<"\n------------------Print primary status---------------------------\n"
        <<MacRightAlign<<std::setw(30)<<"Selected distribution is: "<<"CLHEP::RandGauss\n	"
    	<<MacRightAlign<<std::setw(30)<<"Particle style: "<<sParticleStyle<<"\n"
    	<<MacRightAlign<<std::setw(30)<<"Number per event: "<<iNParticles<<"\n"
    	<<MacRightAlign<<std::setw(30)<<"Energy mean: "<<dEnergyMean<<" MeV\n"
    	<<MacRightAlign<<std::setw(30)<<"Energy rms: "<<dEnergyRMS<<" MeV\n"
    	<<MacRightAlign<<std::setw(30)<<"Position mean(Transverse): "<<dPositionMean<<" m\n"
    	<<MacRightAlign<<std::setw(30)<<"Position rms(Transverse): "<<dPositionRMS<<" m\n"
    	<<MacRightAlign<<std::setw(30)<<"Position Z : "<<dParticlePosZ<<" m\n"
    	<<MacRightAlign<<std::setw(30)<<"Momentum mean(Transverse): "<<dMommentumMean<<"\n"
    	<<MacRightAlign<<std::setw(30)<<"Momentum rms(Transverse): "<<dMommentumRMS<<"\n"
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
