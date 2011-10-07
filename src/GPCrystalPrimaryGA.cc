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
  Init();
}

void GPCrystalPrimaryGA::Init()
{
  GPCrystalManager::GetInstance()->GetGeometry();
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
	
}

GPCrystalPrimaryGA::~GPCrystalPrimaryGA()
{  
}

void GPCrystalPrimaryGA::GeneratePrimaryVertex(G4Event* evt)
{
    //G4double	dECharge=CLHEP::electron_charge;
  GPModuleManager* moduleManager = GPModuleManager::GetInstance();
  /*
  G4double dUpstreamOfCrystal =
    moduleManager->GetParameter("/crystal/geometry/ coordG4FotToGeant4.z");
  G4double dDownstreamOfCrystal =
    moduleManager->GetParameter("/crystal/geometry/ border.z.positive");
    */
  std::string sCrystalName 
    = GPCrystalManager::GetInstance()->GetGeometry();
  G4double dCrystalLength 
    = moduleManager->GetParameter(sCrystalName+" length");
  G4double dCrystalPosInWorld_Z 
    = moduleManager->GetParameter(sCrystalName+" pos_in_world.z");
  G4double dUpstreamOfCrystal=dCrystalPosInWorld_Z-dCrystalLength/2;
  G4double dDownstreamOfCrystal=dCrystalPosInWorld_Z+dCrystalLength/2;
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
    /*
    dPolX=G4UniformRand();
    dPolY=G4UniformRand();
    dPolZ=std::sqrt(1.0-dPolX*dPolX-dPolY*dPolY);
    */
    
    dPositionX = it->getXemis()*angstrom/m+randGauss->shoot(0.0,dRadiusRMSFactor);
    dPositionY = it->getYemis()*angstrom/m+randGauss->shoot(0.0,dRadiusRMSFactor);
    //dPositionZ = it->getZemis()*angstrom/m+dParticlePosZ;
    dPositionZ = it->getZemis()*angstrom/m + dUpstreamOfCrystal;

    //dPositionX = it->getXemis()*angstrom/m;
    //dPositionY = it->getYemis()*angstrom/m;
    //dPositionZ = it->getZemis()*angstrom/m + dUpstreamOfCrystal;
    if(dPositionZ>dDownstreamOfCrystal) 
      dPositionZ = dDownstreamOfCrystal;
    
    dEnergy = it->getEnergy()*GeV;
    //dMass = it->getWeight();
    dMomentumX = it->getThetax()*dEnergy;
    dMomentumY = it->getThetay()*dEnergy;
    dMomentumZ = sqrt(dEnergy*dEnergy-dMomentumX*dMomentumX-dMomentumY*dMomentumY);

    particle_position=G4ThreeVector(dPositionX*m,dPositionY*m,dPositionZ*m);

    // create G4PrimaryParticle object
    G4PrimaryParticle* particle = new G4PrimaryParticle( IDHEP, dMomentumX, dMomentumY, dMomentumZ);
    particle->SetMass(0);
    particle->SetPolarization(dPolX,dPolY,dPolZ);
    
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
    dPositionZ = partCrys.getZPosition()*angstrom/m+dUpstreamOfCrystal;
    if(dPositionZ>dDownstreamOfCrystal) 
      dPositionZ = dDownstreamOfCrystal; 
    particle_position = G4ThreeVector(dPositionX*m,dPositionY*m,dPositionZ*m);

    /*
    dPolX=G4UniformRand();
    dPolY=G4UniformRand();
    dPolZ=std::sqrt(1.0-dPolX*dPolX-dPolY*dPolY);
    */
    particle_time=randGauss->shoot(0.0,dBunchLength)*picosecond;
    
    // create G4PrimaryParticle object
    G4PrimaryParticle* particle = new G4PrimaryParticle( IDHEP, dMomentumX, dMomentumY, dMomentumZ);
    particle->SetMass(ELECTRON_MASS_GEV);
    particle->SetPolarization(dPolX,dPolY,dPolZ);
    G4PrimaryVertex* vertex = new G4PrimaryVertex(particle_position,particle_time);
    vertex->SetPrimary( particle);
    evt->AddPrimaryVertex( vertex );
  }

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
    <<"\nPolarization(Sx,Sy,Sz): "<<"("<<dPolX<<","<<dPolY<<","<<dPolZ<<")"
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
    <<"\nPolarization(Sx,Sy,Sz): "<<"("<<dPolX<<","<<dPolY<<","<<dPolZ<<")"
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
    else if(sLocalKey=="polarization")
    {
      std::string  sTmp;
      ss.clear();
      ss.str(sLocal);
      ss>>sTmp>>dPolX>>dPolY>>dPolZ;
      std::cout<<"Set polarization(Sx,Sy,Sz): "
	<<"("<<dPolX<<","<<dPolY<<","<<dPolZ<<")"
	<<std::endl;
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
