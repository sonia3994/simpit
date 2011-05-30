// $Id: GPCrystalPrimaryGA.cc,v 1.11 2006/06/29 18:09:48 gunter Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
// 
// --------------------------------------------------------------------
#include "GPCrystalPrimaryGA.hh"

#include "GPModuleManager.hh"
#include "GPCrystalManager.hh"

#include "Crystal.h"
#include "RunParameters.h"
#include "Fot.h"
#include "Particle.h"

#include "G4Types.hh"
#include "G4RunManager.hh"
#include "G4ios.hh"
#include "G4PrimaryVertex.hh"
#include "G4PrimaryParticle.hh"
#include "G4HEPEvtParticle.hh"
#include "G4Event.hh"
#include "globals.hh"

//#include "G4UIcommand.hh"

//#include <sys/types.h>
//#include <unistd.h>

#include <cmath>
#include <list>


extern CLHEP::RanecuEngine ranecuEngine;
GPCrystalPrimaryGA::GPCrystalPrimaryGA()
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPCrystalPrimaryGA::GPCrystalPrimaryGA()"<<G4endl;
#endif
  Init();
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPCrystalPrimaryGA::GPCrystalPrimaryGA()"<<G4endl;
#endif
}

void GPCrystalPrimaryGA::Init()
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPCrystalPrimaryGA::Init()"<<G4endl;
#endif
  dUnitE=MeV;
  dUnitP=MeV;
  dUnitL=cm;
  dMomentumX=0;
  dMomentumY=0;
  dEnergyRMS = 0;
  dEnergyMean = 10000;
  dMomentumTranAngleMean=0;
  dMomentumTranAgngleRMS=0;
  dRadiusRMSFactor=2.5e-3;
  dBunchLength=10;
  bRadiusRMSFactorFlag=TRUE;
  particle_time = 0.0;
  dUnitTime=picosecond;
  dParticlePosZ=-4e-3;
  iUseOriginalElectronFlag = 0;
  iProducedPhoton = 0;
  CLHEP:: HepRandom::setTheSeed(time(0),time(0));
  randGauss = new CLHEP::RandGauss(&ranecuEngine,0.,2.5);
	
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPCrystalPrimaryGA::Init()"<<G4endl;
#endif
}

GPCrystalPrimaryGA::~GPCrystalPrimaryGA()
{  
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPCrystalPrimaryGA::~GPCrystalPrimaryGA()"<<G4endl;
#endif

#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPCrystalPrimaryGA::~GPCrystalPrimaryGA()"<<G4endl;
#endif
}

void GPCrystalPrimaryGA::GeneratePrimaryVertex(G4Event* evt)
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPCrystalPrimaryGA::GeneratePrimaryVertex()"<<G4endl;
#endif
    //G4double	dECharge=CLHEP::electron_charge;
  GPModuleManager* moduleManager = GPModuleManager::GetInstance();
  G4double dCoordinateZG4FotCrystalToWorld =
    moduleManager->GetParameter("/crystal/geometry/ coordG4FotToGeant4.z");
  G4double dCrystalBorderPositionZPositive =
    moduleManager->GetParameter("/crystal/geometry/ border.z.positive");
  G4double	dECharge=-1.0;
  G4double	dGamma;

  //PARAMETRES DU RUN
  double phomin = 2.0e-3;
  double etmax = 100000.0;
  double vtmax = 1e-2;
  double poimin = 1.0;
  double frekuma = 3.0/MC_OVER_HBAR;
  RunParameters* runPara = GPCrystalManager::GetInstance()->GetRunPara();
  runPara->set( phomin, etmax, vtmax,poimin, frekuma); 

  Fot fot(*runPara);

  dPositionX=randGauss->shoot(0.0,dRadiusRMSFactor);
  dPositionY=randGauss->shoot(0.0,dRadiusRMSFactor);
  G4double dEnergy   =randGauss->shoot(dEnergyMean,dEnergyRMS);
  dGamma = dEnergy*MeV/GeV/ELECTRON_MASS_GEV;

  //srand(time(0));
  //G4double dPhi = rand()%360;
  G4double dTheta;
  dTheta = abs(randGauss->shoot(dMomentumTranAngleMean,dMomentumTranAgngleRMS));
  dMomentumX = sin(dTheta);
  dTheta = abs(randGauss->shoot(dMomentumTranAngleMean,dMomentumTranAgngleRMS));
  dMomentumY = sin(dTheta);
  //dMomentumX = sin(dTheta)*cos(dPhi);
  //dMomentumY = sin(dTheta)*sin(dPhi);

  Particle* particleFot = new Particle(dECharge,
      1.0,
      1.0,
      0.0,
      dMomentumX,
      dMomentumY,
      dGamma);
  //Particle* particleFot = new Particle(dECharge,
  //    dPositionX*m/angstrom,
  //    dPositionY*m/angstrom,
  //    0,
  //    dMomentumX,
  //    dMomentumY,
  //    dGamma);

  ParticleInCrystal partCrys = fot.makeSingleParticleKumakhov(particleFot);
  std::list<struct Photon>::iterator it;
  std::list<struct Photon> lisPhotonKumakov = fot.getPhotonCollection().getPhotonList();
  iProducedPhoton+=lisPhotonKumakov.size();
  IDHEP = 22;
  for(it=lisPhotonKumakov.begin();it!=lisPhotonKumakov.end();it++)
  {
    polX=G4UniformRand();
    polY=G4UniformRand();
    polZ=std::sqrt(1.0-polX*polX-polY*polY);
    
    dPositionX = it->getXemis()*angstrom/m+randGauss->shoot(0.0,dRadiusRMSFactor);
    dPositionY = it->getYemis()*angstrom/m+randGauss->shoot(0.0,dRadiusRMSFactor);
    //dPositionZ = it->getZemis()*angstrom/m+dParticlePosZ;
    dPositionZ = it->getZemis()*angstrom/m + dCoordinateZG4FotCrystalToWorld;

    //dPositionX = it->getXemis()*angstrom/m;
    //dPositionY = it->getYemis()*angstrom/m;
    //dPositionZ = it->getZemis()*angstrom/m + dCoordinateZG4FotCrystalToWorld;
    if(dPositionZ>dCrystalBorderPositionZPositive) 
      dPositionZ = dCrystalBorderPositionZPositive;
    
    dEnergy = it->getEnergy()*GeV;
    //dMass = it->getWeight();
    dMomentumX = it->getThetax()*dEnergy;
    dMomentumY = it->getThetay()*dEnergy;
    dMomentumZ = sqrt(dEnergy*dEnergy-dMomentumX*dMomentumX-dMomentumY*dMomentumY);

    particle_position=G4ThreeVector(dPositionX*m,dPositionY*m,dPositionZ*m);

    // create G4PrimaryParticle object
    G4PrimaryParticle* particle = new G4PrimaryParticle( IDHEP, dMomentumX, dMomentumY, dMomentumZ);
    particle->SetMass(0);
    particle->SetPolarization(polX,polY,polZ);
    
    particle_time=randGauss->shoot(0.0,dBunchLength)*picosecond;
    
    G4PrimaryVertex* vertex = new G4PrimaryVertex(particle_position,particle_time);
    vertex->SetPrimary( particle);
    evt->AddPrimaryVertex( vertex );
  }
  if(iUseOriginalElectronFlag==1)
  {
    IDHEP=11;
    dMomentumX = ELECTRON_MASS_EV*partCrys.getPx();
    dMomentumY = ELECTRON_MASS_EV*partCrys.getPy();
    dGamma = partCrys.getGamma();
    dMomentumZ = sqrt((dGamma*dGamma-1)*ELECTRON_MASS_EV*ELECTRON_MASS_EV
        -dMomentumX*dMomentumX
        -dMomentumY*dMomentumY);
    dPositionX = partCrys.getXPosition()*angstrom/m;
    dPositionY = partCrys.getYPosition()*angstrom/m;
    dPositionZ = partCrys.getZPosition()*angstrom/m+dCoordinateZG4FotCrystalToWorld;
    if(dPositionZ>dCrystalBorderPositionZPositive) 
      dPositionZ = dCrystalBorderPositionZPositive; 
    particle_position = G4ThreeVector(dPositionX*m,dPositionY*m,dPositionZ*m);

    polX=G4UniformRand();
    polY=G4UniformRand();
    polZ=std::sqrt(1.0-polX*polX-polY*polY);
    particle_time=randGauss->shoot(0.0,dBunchLength)*picosecond;
    
    // create G4PrimaryParticle object
    G4PrimaryParticle* particle = new G4PrimaryParticle( IDHEP, dMomentumX, dMomentumY, dMomentumZ);
    particle->SetMass(ELECTRON_MASS_GEV);
    particle->SetPolarization(polX,polY,polZ);
    G4PrimaryVertex* vertex = new G4PrimaryVertex(particle_position,particle_time);
    vertex->SetPrimary( particle);
    evt->AddPrimaryVertex( vertex );
  }

#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPCrystalPrimaryGA::GeneratePrimaryVertex()"<<G4endl;
#endif
}

void GPCrystalPrimaryGA::Print()
{

  G4cout
    <<"\nPrimary Generator type: G4Fot"
    <<"\nBeam length: "<<dBunchLength<<" ps"
    <<"\nBeam energy mean: "<<dEnergyMean<<" MeV"
    <<"\nBeam energy rms: "<<dEnergyRMS<<" MeV"
    <<"\nBeam transverse momentum direction mean: "<<dMomentumTranAngleMean
    <<"\nBeam transverse momentum direction rms: "<<dMomentumTranAgngleRMS
    <<"\nBeam transverse dimension rms: "<<dRadiusRMSFactor<<" m"
    <<"\nDistance From crystal to amorphous target: "<<dParticlePosZ<<" m"
    <<G4endl;

}

void GPCrystalPrimaryGA::Print(std::ofstream& ofsOutput)
{

  ofsOutput
    <<"\nPrimary status:"
    <<"\nPrimary Generator type, G4Fot"
    <<"\nBeam length, "<<dBunchLength<<" ps"
    <<"\nBeam energy mean, "<<dEnergyMean<<" MeV"
    <<"\nBeam energy rms, "<<dEnergyRMS<<" MeV"
    <<"\nBeam transverse momentum direction mean, "<<dMomentumTranAngleMean
    <<"\nBeam transverse momentum direction rms, "<<dMomentumTranAgngleRMS
    <<"\nBeam transverse dimension rms, "<<dRadiusRMSFactor<<" m"
    <<"\nDistance From crystal to amorphous target, "<<dParticlePosZ<<" m"
    <<G4endl;

}
void GPCrystalPrimaryGA::SetParameter(std::string sLocal,std::string sGlobal)
{
    std::stringstream ss(sLocal);
    std::string		  sLocalKey;
    std::string		  sGlobalKey;
    std::string		  sValueOrg;
    std::string		  sUnit;

    G4double   		  dValueNew;
    G4double   		  dValueOrg;
    
    ss>>sLocalKey>>sValueOrg>>sUnit;
    ss.clear();
    ss.str(sValueOrg);
    ss>>dValueOrg;
    ss.clear();
    ss.str(sGlobal);
    ss>>sGlobalKey;

    if(sUnit!="")
    dValueNew=(dValueOrg*G4UIcommand::ValueOf(sUnit.c_str()));
    else dValueNew=dValueOrg;

    if(sLocalKey=="energy.mean")
    {
      dEnergyMean = dValueNew/MeV;
    }
    else if(sLocalKey=="energy.rms")
    {
      dEnergyRMS = dValueNew/MeV;
    }
    else if(sLocalKey=="momentum.direction.theta.mean")
    {
      dMomentumTranAngleMean= dValueNew;
    }
    else if(sLocalKey=="momentum.direction.theta.rms")
    {
      dMomentumTranAgngleRMS= dValueNew;
    }
    else if(sLocalKey=="position.z")
    {
      dParticlePosZ = dValueNew/m;
    }
    else if(sLocalKey=="position.transverse.rms")
    {
      dRadiusRMSFactor = dValueNew/m;
    }
    else if(sLocalKey=="time.rms")
    {
      dBunchLength =  dValueNew/picosecond;
    }
    else if(sLocalKey == "photon.number")
    {
      iProducedPhoton=0;
    }
    else 
    {
  	std::cout<<"the Key: "<<sGlobalKey<<" is not exist."<<std::endl;
     	return;
    }

    std::cout<<"Set: "<<sGlobalKey<<" to "<< dValueOrg<<" "<<sUnit<<std::endl;
}

G4double GPCrystalPrimaryGA::GetParameter(std::string sLocalKey, std::string sGlobalKey) const
{
  if(sLocalKey == "photon.number")
  {
    G4cout<<"The number of produced photon: "<<iProducedPhoton<<G4endl;
    return iProducedPhoton;
  }
  return 0;
}
