// $Id: GPPrimaryGeneratorAction.cc,v 1.6 2006/06/29 17:47:23 gunter Exp $
// GEANT4 tag $Name: geant4-09-02 $
//

#include "GPPrimaryGeneratorAction.hh"
#include "GPPrimaryGeneratorMessenger.hh"
#include "GPHEPEvtInterface.hh"
#include "GPCrystalPrimaryGA.hh"
#include "GPHelicalGenerator.hh"
#include "GPSimpitGenerator.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"

//#include "globals.hh"
//#include "Randomize.hh"

//#include <sys/types.h>
//#include <unistd.h>

#include <cmath>
#include <iomanip>

#define AlignRight  std::setiosflags(std::ios_base::right)
#define AlignLeft  std::setiosflags(std::ios_base::left)

GPPrimaryGeneratorAction::GPPrimaryGeneratorAction()
{

  dPositionMean=0.0e-3;
  dPositionRMS=2.0e-3;
  dEnergyMean=30.0;
  dEnergyRMS=10.0;
  dAngleMeanTheta=0.0;
  dAngleRMSTheta=0.0;
  dParticlePosZ=-3.0e-3;
  dBunchLength=6;
  bFixedParticleGun=true;
  dPolX=0;
  dPolY=0;
  dPolZ=1;
  vectMommentumDirection=G4ThreeVector(0,0,1);

  srand(time(0));
  verbose=0;
  iNParticles = 1;

  CLHEP:: HepRandom::setTheSeed(rand(),rand());
  randGauss = new CLHEP::RandGauss(&ranecuEngine,0.,2.);
  randFlat=new CLHEP::RandFlat(&ranecuEngine,-1.0,1.0);

  particleGun = new G4ParticleGun(iNParticles);
  primaryMessenger = new GPPrimaryGeneratorMessenger(this);

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

  HEPEvt = new GPHEPEvtInterface();
  crystalGenerator = new GPCrystalPrimaryGA();
  helicalGenerator = new GPHelicalGenerator();
  simpitGenerator = new GPSimpitGenerator();
  sGeneratorType = "HEPEvt";
}

GPPrimaryGeneratorAction::~GPPrimaryGeneratorAction()
{
  if(particleGun) delete particleGun;
//  if(randGauss) delete randGauss;// we should not free randGausss, but why????
//  if(randFlat) delete randFlat;
  if(primaryMessenger) delete primaryMessenger;
  if(HEPEvt) delete HEPEvt;
  if(crystalGenerator) delete crystalGenerator;
  if(helicalGenerator) delete helicalGenerator;
  if(simpitGenerator) delete simpitGenerator;
}

void GPPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  if(sGeneratorType=="HEPEvt")
  { 
    HEPEvt->GeneratePrimaryVertex(anEvent);
    return;
  }
  else if(sGeneratorType=="crystal")
  { 
    crystalGenerator->GeneratePrimaryVertex(anEvent);
    return;
  }
  else if(sGeneratorType=="helical")
  { 
    helicalGenerator->GeneratePrimaryVertex(anEvent);
    return;
  }
  else if(sGeneratorType=="simpit")
  { 
    simpitGenerator->GeneratePrimaryVertex(anEvent);
    return;
  }


  if(bFixedParticleGun)
  {
    GeneratePrimariesFixedParticleGun(anEvent);
    return;
  }

  G4double 	energy=randGauss->shoot(dEnergyMean,dEnergyRMS);
  energy=std::abs(energy);
  G4double 	x0=randGauss->shoot(dPositionMean,dPositionRMS);
  G4double 	y0=randGauss->shoot(dPositionMean,dPositionRMS);

  G4double      dAngleTheta=randGauss->shoot(dAngleMeanTheta,dAngleRMSTheta);
  G4double      dAngleAlpha=rand()%360;
  G4double      pz0=cos(dAngleTheta);
  G4double 	px0=sin(dAngleTheta)*cos(dAngleAlpha);
  G4double 	py0=sin(dAngleTheta)*sin(dAngleAlpha);
  G4double      dPolZ=randFlat->shoot(-1.0,1.0);
  //G4double      dTheta=rand()%360;
  //dPolX=sqrt(1.0-dPolZ*dPolZ)*cos(dTheta);
  dPolX=0;
  //dPolY=sqrt(1.0-dPolZ*dPolZ)*sin(dTheta);
  dPolY=0;

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

void GPPrimaryGeneratorAction::GeneratePrimariesFixedParticleGun(G4Event* anEvent)
{
  G4double 	x0=dPositionMean;
  G4double 	y0=dPositionMean;

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
  particleGun->SetParticleTime(0);
  particleGun->SetParticlePolarization(G4ThreeVector(dPolX,dPolY,dPolZ));
    
  particleGun->GeneratePrimaryVertex(anEvent);

}

void GPPrimaryGeneratorAction::SetParticleStyle(G4String tmpParticleStyle)
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

void GPPrimaryGeneratorAction::Print()
{ 
  if(sGeneratorType=="HEPEvt")
  {
    HEPEvt->Print();
    return;
  }
  if(sGeneratorType=="crystal")
  {
    crystalGenerator->Print();
    return;
  }
  if(sGeneratorType=="helical")
  {
    helicalGenerator->Print();
    return;
  }
  if(sGeneratorType=="simpit")
  {
    simpitGenerator->Print();
    return;
  }
  else if(bFixedParticleGun== true)
  {
    G4cout
      <<"\n------------------Print primary status---------------------------\n"
      <<AlignRight<<std::setw(32)<<"Fixed primary condition:\n"
      <<AlignRight<<std::setw(32)<<"Particle style: "<<sParticleStyle<<"\n"
      <<AlignRight<<std::setw(32)<<"Number per event: "<<iNParticles<<"\n"
      <<AlignRight<<std::setw(32)<<"Energy : "<<dEnergyMean<<"MeV\n"
      <<AlignRight<<std::setw(32)<<"Polarization(Sx,Sy,Sz): "<<"("<<dPolX<<","<<dPolY<<","<<dPolZ<<")\n"
      <<AlignRight<<std::setw(32)<<"Position radius(Transverse): "<<dPositionMean<<" m\n"
      <<AlignRight<<std::setw(32)<<"Position Z : "<<dParticlePosZ<<" m\n"
      <<AlignRight<<std::setw(32)<<"Momentum direction: "<<"\n" 
      <<AlignRight<<std::setw(32)<<"px0: "<<vectMommentumDirection.x()<<"\n"
      <<AlignRight<<std::setw(32)<<"py0: "<<vectMommentumDirection.y()<<"\n"
      <<AlignRight<<std::setw(32)<<"pz0: "<<vectMommentumDirection.z()<<"\n"
      <<"-----------------------------------------------------------------\n"
      <<G4endl;	
    return;
  }

  G4cout
    <<"\n------------------Print primary status---------------------------\n"
    <<AlignRight<<std::setw(30)<<"Selected distribution is: "<<"CLHEP::RandGauss\n	"
    <<AlignRight<<std::setw(30)<<"Particle style: "<<sParticleStyle<<"\n"
    <<AlignRight<<std::setw(30)<<"Number per event: "<<iNParticles<<"\n"
    <<AlignRight<<std::setw(30)<<"Energy mean: "<<dEnergyMean<<" MeV\n"
    <<AlignRight<<std::setw(30)<<"Energy rms: "<<dEnergyRMS<<" MeV\n"
    <<AlignRight<<std::setw(32)<<"Polarization(Sx,Sy,Sz): "<<"("<<dPolX<<","<<dPolY<<","<<dPolZ<<")\n"
    <<AlignRight<<std::setw(32)<<"Sz: Distributin: Flat. Range: [-1,1]\n"
    <<AlignRight<<std::setw(30)<<"Position mean(Transverse): "<<dPositionMean<<" m\n"
    <<AlignRight<<std::setw(30)<<"Position rms(Transverse): "<<dPositionRMS<<" m\n"
    <<AlignRight<<std::setw(30)<<"Position Z : "<<dParticlePosZ<<" m\n"
    <<AlignRight<<std::setw(30)<<"Momentum Angle mean(To Z axis): "<<dAngleMeanTheta<<" deg.\n"
    <<AlignRight<<std::setw(30)<<"Momentum Angle rms(To Z axis): "<<dAngleRMSTheta<<" deg.\n"
    <<"-----------------------------------------------------------------\n"
    <<G4endl;

}

void GPPrimaryGeneratorAction::Print(std::ofstream& ofsOutput)
{ 
  if(sGeneratorType=="HEPEvt")
  {           
    HEPEvt->Print(ofsOutput);
    return;
  }
  if(sGeneratorType=="crystal")
  {           
    crystalGenerator->Print(ofsOutput);
    return;
  }
  if(sGeneratorType=="helical")
  {
    helicalGenerator->Print(ofsOutput);
    return;
  }
  if(sGeneratorType=="simpit")
  {
    simpitGenerator->Print(ofsOutput);
    return;
  }
  if(bFixedParticleGun== true)
  {
    ofsOutput
      <<"\nPrimary status:"
      <<"\nPrimary Generator type:,Particle Gun"
      <<"\nFixed primary condition:"
      <<"\nParticle style, "<<sParticleStyle
      <<"\nNumber per event, "<<iNParticles
      <<"\nEnergy , "<<dEnergyMean<<" MeV"
      <<"\nPolarization(Sx,Sy,Sz): "<<"("<<dPolX<<","<<dPolY<<","<<dPolZ<<")"
      <<"\nPosition radius(Transverse), "<<dPositionMean<<" m"
      <<"\nPosition Z , "<<dParticlePosZ<<" m"
      <<"\nMomentum direction: " 
      <<"\npx0, "<<vectMommentumDirection.x()
      <<"\npy0, "<<vectMommentumDirection.y()
      <<"\npz0, "<<vectMommentumDirection.z()
      <<G4endl;	
    return;
  }

  ofsOutput
    <<"\nPrimary status:"
    <<"\nPrimary Generator type:,Particle Gun"
    <<"\nSelected distribution is, "<<"CLHEP::RandGauss"
    <<"\nParticle style, "<<sParticleStyle
    <<"\nNumber per event, "<<iNParticles
    <<"\nEnergy mean, "<<dEnergyMean<<" MeV"
    <<"\nEnergy rms, "<<dEnergyRMS<<" MeV"
    <<"\nPolarization(Sx,Sy,Sz): "<<"("<<dPolX<<","<<dPolY<<","<<dPolZ<<")"
    <<"\nSz: Distributin: Flat. Range: [-1,1]"
    <<"\nPosition mean(Transverse), "<<dPositionMean<<" m"
    <<"\nPosition rms(Transverse), "<<dPositionRMS<<" m"
    <<"\nPosition Z , "<<dParticlePosZ<<" m"
    <<"\nMomentum Angle mean(To Z axis): "<<dAngleMeanTheta<<" deg."
    <<"\nMomentum Angle rms(To Z axis): "<<dAngleRMSTheta<<" deg."
    <<G4endl;

}

void GPPrimaryGeneratorAction::SetParameter(std::string sLocal)
{
    std::stringstream ss(sLocal);
    std::string		  sGlobal=sLocal;
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

    std::string strInput=sLocal;
    std::string strFirstLevel;
    std::string strLeft;
    size_t iFirstDot;
    iFirstDot = strInput.find(".");
    if(iFirstDot!=std::string::npos)
    {
      strFirstLevel=strInput.substr(0,iFirstDot);
      strLeft=strInput.substr(iFirstDot+1);
    }

    if(strFirstLevel=="HEPEvt")
    {
      HEPEvt->SetParameter(strLeft,sGlobal);
      return;
    }
    if(strFirstLevel=="crystal")
    {
      crystalGenerator->SetParameter(strLeft,sGlobal);
      return;
    }
    if(strFirstLevel=="helical")
    {
      helicalGenerator->SetParameter(strLeft,sGlobal);
      return;
    }
    if(strFirstLevel=="simpit")
    {
      simpitGenerator->SetParameter(strLeft,sGlobal);
      return;
    }
    if(strFirstLevel=="particleGun")
    {
      SetParameter(strLeft,sGlobal);
      return;
    }

    if(sKey=="type")
    {
      sGeneratorType=sValueOrg;
      std::cout<<"Set: "<<sKey<<" to "<< sValueOrg<<" "<<sUnit<<std::endl;
      return;
    }
    
    std::cout<<"The Key: "<<sKey<<" is not exist."<<std::endl;
}

G4double GPPrimaryGeneratorAction::GetParameter(std::string sKey)
{
    std::string strInput=sKey;
    std::string strFirstLevel;
    std::string strLeft;
    size_t iFirstDot;
    iFirstDot = strInput.find(".");
    if(iFirstDot!=std::string::npos)
    {
    strFirstLevel=strInput.substr(0,iFirstDot);
    strLeft=strInput.substr(iFirstDot+1);
    }

    if(strFirstLevel=="HEPEvt")
    {
      return HEPEvt->GetParameter(strLeft,sKey);
    }
    
    if(strFirstLevel=="crystal")
    {
      return crystalGenerator->GetParameter(strLeft,sKey);
    }
    if(strFirstLevel=="helical")
    {
      return helicalGenerator->GetParameter(strLeft,sKey);
    }
    if(strFirstLevel=="simpit")
    {
      return simpitGenerator->GetParameter(strLeft,sKey);
    }
    if(strFirstLevel=="particleGun")
    {
      return GetParameter(strLeft,sKey);
    }
    else 
    {
     std::cout<<"The Key: "<<sKey<<" is not exist."<<std::endl;
    }
    return 0;
}

void GPPrimaryGeneratorAction::SetParameter(std::string sLocal, std::string sGlobal)
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
  else if(sKey=="angle.theta.mean")
    dAngleMeanTheta=dValueNew;
  else if(sKey=="angle.theta.rms")
    dAngleRMSTheta=dValueNew;

  else if(sKey=="bunch.rms")
    dBunchLength=dValueNew/picosecond;
  else if(sKey=="fixed.flag")
    bFixedParticleGun=dValueNew;
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

G4double GPPrimaryGeneratorAction::GetParameter(std::string sKey,std::string sKeyGlobal)
{
    if(sKey=="verbose")
    return verbose;
    else if(sKey=="number")
    return iNParticles;
    //else if(sKey=="particle.type")
    //return sParticleStyle;
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
    
    else if(sKey=="angle.theta.mean")
    return dAngleMeanTheta;
    else if(sKey=="angle.theta.rms")
    return dAngleRMSTheta;
    
    else if(sKey=="time.rms")
    return dBunchLength;
    //else if(sKey=="verbose")
    //vectMommentumDirection;
    else if(sKey=="random.flag")
    return bFixedParticleGun;

   else 
   {
     std::cout<<"The Key: "<<sKeyGlobal<<" is not exist."<<std::endl;
   }
   return 0;
}
