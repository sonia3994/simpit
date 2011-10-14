// $Id: GPSimpitGenerator.cc,v 1.6 2006/06/29 17:47:23 gunter Exp $
// GEANT4 tag $Name: geant4-09-02 $
//

#include "GPSimpitGenerator.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"


#include <cmath>
#include <iomanip>

#define AlignRight  std::setiosflags(std::ios_base::right)
#define AlignLeft  std::setiosflags(std::ios_base::left)

GPSimpitGenerator::GPSimpitGenerator()
{

  dParticlePosZ=-3.0e-3;
  particleGun = new G4ParticleGun(1);

}

GPSimpitGenerator::~GPSimpitGenerator()
{
  if(particleGun) delete particleGun;
  if(fsInputFile.is_open()) fsInputFile.close();
}

void GPSimpitGenerator::GeneratePrimaryVertex(G4Event* anEvent)
{
  if(!fsInputFile.is_open()) 
  {
    G4cout<<"GPSimpitGenerator: did not set the input file: "<<G4endl;
    return;
  }

  if(fsInputFile.eof()) 
  {
    G4Exception("GPSimpitGenerator: End-Of-File : HEPEvt input file");
    return;
  }

  G4int 	iPdg;  
  G4int 	iEventID;   
  G4int 	iTrackID;   
  G4double 	dX; 
  G4double 	dY; 
  G4double 	dPx; 
  G4double 	dPy; 
  G4double 	dPz; 
  G4double 	dEnergy; 
  G4double 	dTime; 
  G4double 	dSx; 
  G4double 	dSy; 
  G4double 	dSz; 
  G4double      dPxD;
  G4double      dPyD;
  G4double      dPzD;
  G4double      dP;
  std::string sLine;
  std::stringstream ss;
  if(!getline(fsInputFile,sLine))
  {
    G4Exception("GPSimpitGenerator: End-Of-File : HEPEvt input file");
    return;
  }
  ss.clear();
  ss.str(sLine);
  ss>>iPdg
    >>iEventID>>iTrackID
    >>dX>>dY
    >>dPx>>dPy>>dPz
    >>dEnergy>>dTime
    >>dSx>>dSy>>dSz;

  dP=sqrt(dPx*dPx+dPy*dPy+dPz*dPz);
  dPxD=dPx/dP;
  dPyD=dPy/dP;
  dPzD=dPz/dP;

  G4ParticleDefinition* pParticle =
    G4ParticleTable::GetParticleTable()->FindParticle(iPdg);

  particleGun->SetParticleDefinition(pParticle);
  particleGun->SetParticlePosition(G4ThreeVector(dX*mm,dY*mm,dParticlePosZ*m));
  particleGun->SetParticleMomentumDirection(G4ThreeVector(dPxD,dPyD,dPzD));
  particleGun->SetParticleEnergy(dEnergy*MeV);
  particleGun->SetParticleTime(dTime*picosecond);
  particleGun->SetParticlePolarization(G4ThreeVector(dSx,dSy,dSz));

  particleGun->GeneratePrimaryVertex(anEvent);

}


void GPSimpitGenerator::Print()
{ 
  G4cout
    <<"\n------------------Print primary status---------------------------\n"
    <<AlignRight<<std::setw(30)<<"GPSimpitGenerator:\n	"
    <<AlignRight<<std::setw(30)<<"Input file:,"<<sInputFile+"\n"
    <<AlignRight<<std::setw(30)<<"Position Z : "<<dParticlePosZ<<" m\n"
    <<"-----------------------------------------------------------------\n"
    <<G4endl;

}

void GPSimpitGenerator::Print(std::ofstream& ofsOutput)
{ 
  ofsOutput
    <<"\nPrimary status:"
    <<"\nPrimary Generator type:,GPSimpitGenerator"
    <<"\nInput file:,"<<sInputFile
    <<"\nPosition Z , "<<dParticlePosZ<<" m"
    <<G4endl;

}

void GPSimpitGenerator::SetParameter(std::string sLocal, std::string sGlobal)
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
  else if(sKey=="position.z")
    dParticlePosZ=dValueNew/m;
  else if(sKey=="input_file")
    return SetInputFile(sValueOrg);
  else 
  {
    std::cout<<"The sKey: "<<sKey<<" is not exist."<<std::endl;
    return;
  }

  std::cout<<"Set: "<<sKey<<" to "<< sValueOrg<<" "<<sUnit<<std::endl;

}

G4double GPSimpitGenerator::GetParameter(std::string sKey,std::string sKeyGlobal)
{
  if(sKey=="verbose")
    return verbose;
  else if(sKey=="position.z")
    return dParticlePosZ;
  else 
  {
    std::cout<<"The Key: "<<sKeyGlobal<<" is not exist."<<std::endl;
  }
  return 0;
}

void GPSimpitGenerator::SetInputFile(G4String sFile)
{
  if(fsInputFile.is_open())
  {
    fsInputFile.close();
    G4cout<<"GPSimpitGenerator: Close input file: "<<sInputFile<<G4endl;
  }

  fsInputFile.open(sFile,std::fstream::in);

  if(fsInputFile.is_open()) 
  {
    sInputFile=sFile;
    G4cout<<"GPSimpitGenerator: Set input file successed: "<<sFile<<G4endl;
    fsInputFile.clear();
  }
  else 
  { 
    G4cout<<"GPSimpitGenerator: Set input file failed: "<<sFile<<G4endl;
  }

}
