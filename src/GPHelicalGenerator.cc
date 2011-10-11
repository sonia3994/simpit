// $Id: GPHelicalGenerator.cc,v 1.6 2006/06/29 17:47:23 gunter Exp $
// GEANT4 tag $Name: geant4-09-02 $
//

#include "GPHelicalGenerator.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"


#include <cmath>
#include <iomanip>

#define AlignRight  std::setiosflags(std::ios_base::right)
#define AlignLeft  std::setiosflags(std::ios_base::left)

double HelicalPhotonEnergy();
double HelicalPhotonPolarization(double);
GPHelicalGenerator::GPHelicalGenerator()
{

  dPositionMean=0.0e-3;
  dPositionRMS=2.0e-3;
  dEnergyMean=30.0;
  dEnergyRMS=10.0;
  dMommentumMean=0.0;
  dMommentumRMS=0.0;
  dParticlePosZ=-3.0e-3;
  dBunchLength=6;
  dPolX=0;
  dPolY=0;
  dPolZ=1;
  vectMommentumDirection=G4ThreeVector(0,0,1);

  iNParticles = 1;

  randGauss = new CLHEP::RandGauss(&ranecuEngine,0.,2.);

  particleGun = new G4ParticleGun(iNParticles);

//default particle
  sParticleStyle = "gamma";
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* particle = particleTable->FindParticle(sParticleStyle);
  particleGun->SetParticleDefinition(particle);
  particleGun->SetParticlePosition(G4ThreeVector(0.0*m,0.0*m,dParticlePosZ*m));
  particleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
  particleGun->SetParticleEnergy(dEnergyMean*MeV);
  particleGun->SetParticleTime(0.0);
  particleGun->SetParticlePolarization(G4ThreeVector(dPolX,dPolY,dPolZ));

}

GPHelicalGenerator::~GPHelicalGenerator()
{
  if(particleGun) delete particleGun;
}

void GPHelicalGenerator::GeneratePrimaryVertex(G4Event* anEvent)
{
  G4double 	x0=randGauss->shoot(dPositionMean,dPositionRMS);
  G4double 	y0=randGauss->shoot(dPositionMean,dPositionRMS);
  G4double 	px0=randGauss->shoot(dMommentumMean,dMommentumRMS);
  G4double 	py0=randGauss->shoot(dMommentumMean,dMommentumRMS);
  G4double 	pz0=sqrt(1.0-(px0*px0+py0*py0));

  G4double 	energy=HelicalPhotonEnergy();
  dPolZ=HelicalPhotonPolarization(energy);
  G4double dTheta=rand()%360;
  dPolX=sqrt(1.0-dPolZ*dPolZ)*sin(dTheta);
  dPolY=sqrt(1.0-dPolZ*dPolZ)*cos(dTheta);

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
  particleGun->SetParticleTime(randGauss->shoot(0.0,dBunchLength)*picosecond);
  particleGun->SetParticlePolarization(G4ThreeVector(dPolX,dPolY,dPolZ));
    
  particleGun->GeneratePrimaryVertex(anEvent);

}

void GPHelicalGenerator::SetParticleStyle(G4String tmpParticleStyle)
{
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* particle = particleTable->FindParticle(tmpParticleStyle);
  if(particle)
  {
	  particleGun->SetParticleDefinition(particle);
	  sParticleStyle = tmpParticleStyle;
	  G4cout<<"\nSet primary partilce successed: "<<sParticleStyle<<"\n"<<G4endl;
  }
  else
  {
	  G4cout<<"\nSet primary partilce failed: "<<tmpParticleStyle<<"\n"<<G4endl;
  }
}

void GPHelicalGenerator::Print()
{ 
  G4cout
    <<"\n------------------Print primary status---------------------------\n"
    <<AlignRight<<std::setw(30)<<"GPHelicalGenerator:\n	"
    <<AlignRight<<std::setw(30)<<"Selected distribution is: "<<"CLHEP::RandGauss\n	"
    <<AlignRight<<std::setw(30)<<"Particle style: "<<sParticleStyle<<"\n"
    <<AlignRight<<std::setw(30)<<"Number per event: "<<iNParticles<<"\n"
    //<<AlignRight<<std::setw(30)<<"Energy mean: "<<dEnergyMean<<" MeV\n"
    //<<AlignRight<<std::setw(30)<<"Energy rms: "<<dEnergyRMS<<" MeV\n"
    //<<AlignRight<<std::setw(32)<<"Polarization(Sx,Sy,Sz): "<<"("<<dPolX<<","<<dPolY<<","<<dPolZ<<")\n"
    <<AlignRight<<std::setw(30)<<"Position mean(Transverse): "<<dPositionMean<<" m\n"
    <<AlignRight<<std::setw(30)<<"Position rms(Transverse): "<<dPositionRMS<<" m\n"
    <<AlignRight<<std::setw(30)<<"Position Z : "<<dParticlePosZ<<" m\n"
    <<AlignRight<<std::setw(30)<<"Momentum mean(Transverse): "<<dMommentumMean<<"\n"
    <<AlignRight<<std::setw(30)<<"Momentum rms(Transverse): "<<dMommentumRMS<<"\n"
    <<"-----------------------------------------------------------------\n"
    <<G4endl;

}

void GPHelicalGenerator::Print(std::ofstream& ofsOutput)
{ 
  ofsOutput
    <<"\nPrimary status:"
    <<"\nPrimary Generator type:,GPHelicalGenerator"
    <<"\nSelected distribution is, "<<"CLHEP::RandGauss"
    <<"\nParticle style, "<<sParticleStyle
    <<"\nNumber per event, "<<iNParticles
    //<<"\nEnergy mean, "<<dEnergyMean<<" MeV"
    //<<"\nEnergy rms, "<<dEnergyRMS<<" MeV"
    //<<"\nPolarization(Sx,Sy,Sz): "<<"("<<dPolX<<","<<dPolY<<","<<dPolZ<<")"
    <<"\nPosition mean(Transverse), "<<dPositionMean<<" m"
    <<"\nPosition rms(Transverse), "<<dPositionRMS<<" m"
    <<"\nPosition Z , "<<dParticlePosZ<<" m"
    <<"\nMomentum mean(Transverse), "<<dMommentumMean
    <<"\nMomentum rms(Transverse), "<<dMommentumRMS
    <<G4endl;

}

void GPHelicalGenerator::SetParameter(std::string sLocal, std::string sGlobal)
{
  std::stringstream ss(sLocal);
  std::string		  sUnit;
  std::string		  sKey;
  std::string		  sValueOrg;
  G4double   		  dValueNew;
  G4double   		  dValueOrg;

  ss>>sKey>>sValueOrg>>sUnit;
  ss.clear();
  ss.str(sValueOrg);
  ss>>dValueOrg;

  if(sUnit!="")
    dValueNew=(dValueOrg*G4UIcommand::ValueOf(sUnit.c_str()));
  else dValueNew=dValueOrg;

  if(sKey=="verbose")
    verbose = dValueNew;
  else if(sKey=="number")
    iNParticles=dValueNew;
  else if(sKey=="position.z")
    dParticlePosZ=dValueNew/m;
  else if(sKey=="position.tr.mean")
    dPositionMean=dValueNew/m;
  else if(sKey=="position.tr.rms")
    dPositionRMS=dValueNew/m;
  else if(sKey=="energy.mean")
    dEnergyMean=dValueNew/MeV;
  else if(sKey=="energy.rms")
    dEnergyRMS=dValueNew/MeV;
  else if(sKey=="particle.type")
  {
    SetParticleStyle(sValueOrg);
    return;
  }
  else if(sKey=="momentum.tr.mean")
    dMommentumMean=dValueNew;
  else if(sKey=="momentum.tr.rms")
    dMommentumRMS=dValueNew;

  else if(sKey=="bunch.rms")
    dBunchLength=dValueNew/picosecond;
  else if(sKey=="polarization")
  {
    std::string  sTmp;
    ss.clear();
    ss.str(sLocal);
    ss>>sTmp>>dPolX>>dPolY>>dPolZ;
    std::cout<<"Set polarization(Sx,Sy,Sz): "
      <<"("<<dPolX<<","<<dPolY<<","<<dPolZ<<")"
      <<std::endl;
	return;
  }

  else 
  {
    std::cout<<"The sKey: "<<sKey<<" is not exist."<<std::endl;
    return;
  }

  std::cout<<"Set: "<<sKey<<" to "<< sValueOrg<<" "<<sUnit<<std::endl;

}

G4double GPHelicalGenerator::GetParameter(std::string sKey,std::string sKeyGlobal)
{
    if(sKey=="verbose")
    return verbose;
    else if(sKey=="number")
    return iNParticles;
    else if(sKey=="position.z")
    return dParticlePosZ;
    else if(sKey=="position.tr.mean")
    return dPositionMean;
    else if(sKey=="position.tr.rms")
    return dPositionRMS;
    else if(sKey=="energy.mean")
    return dEnergyMean;
    else if(sKey=="energy.rms")
    return dEnergyRMS;
    
    else if(sKey=="momentum.tr.mean")
    return dMommentumMean;
    else if(sKey=="momentum.tr.rms")
    return dMommentumRMS;
    
    else if(sKey=="time.rms")
    return dBunchLength;

   else 
   {
     std::cout<<"The Key: "<<sKeyGlobal<<" is not exist."<<std::endl;
   }
   return 0;
}
double HelicalPhotonEnergy()
{
  static double daEnergySpectrum[40]={ 
    0.0, 0.0, 0.0, 0.0, 0.0, 1.93, 5.43, 12.3, 20.2, 26.6, 
    0.27, 0.69, 1.21, 1.86, 2.69, 3.8, 4.98, 4.96, 4.58, 3.64, 
    1.87, 1.08, 1.35, 1.61, 1.87, 1.85, 1.72, 1.53, 1.3, 1.05, 
    0.83, 0.77, 0.84, 0.85, 0.79, 0.72, 0.65, 0.58, 0.52, 0.47 };
  double dEnergy;
  double dEnergyY;
  size_t iIndex;
  do
  {
    dEnergy=rand()*40.0/RAND_MAX;
    dEnergyY=rand()*27.0/RAND_MAX;
    iIndex=(int)(dEnergy+0.5);
  }
  while(dEnergyY>daEnergySpectrum[iIndex]);
  return dEnergy;
}
double HelicalPhotonPolarization(double dEnergy)
{
  static double daPolarization[40]={ 
    0.0, 0.0, 0.0, 0.0, 0.0, 0.27, 0.62, 0.84, 0.95, 0.99, 
    0.71, 0.29, 0.49, 0.65, 0.77, 0.85, 0.89, 0.92, 0.93, 0.92, 
    0.84, 0.71, 0.76, 0.81, 0.85, 0.87, 0.88, 0.87, 0.86, 0.83, 
    0.80, 0.79, 0.81, 0.83, 0.84, 0.84, 0.83, 0.82, 0.81, 0.80 };
  size_t iIndex=(int)(dEnergy+0.5);
  if (iIndex>39)
    return 0.80;
  return daPolarization[iIndex]; 
}
