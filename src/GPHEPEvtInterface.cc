// $Id: GPHEPEvtInterface.cc,v 1.11 2006/06/29 18:09:48 gunter Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
// 
// --------------------------------------------------------------------

#include "G4Types.hh"

#include "G4RunManager.hh"
#include "G4ios.hh"
#include "G4PrimaryVertex.hh"
#include "G4PrimaryParticle.hh"
#include "G4HEPEvtParticle.hh"
#include "G4Event.hh"

//#include "G4UIcommand.hh"

//#include <sys/types.h>
//#include <unistd.h>

#include <cmath>

#include "GPHEPEvtInterface.hh"

extern CLHEP::RanecuEngine ranecuEngine;
GPHEPEvtInterface::GPHEPEvtInterface():fsInputFile(NULL)
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPHEPEvtInterface::GPHEPEvtInterface()"<<G4endl;
#endif
	Init();
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPHEPEvtInterface::GPHEPEvtInterface()"<<G4endl;
#endif
}

GPHEPEvtInterface::GPHEPEvtInterface(G4String evfile)
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPHEPEvtInterface::GPHEPEvtInterface(G4String)"<<G4endl;
#endif
	Init();
	SetInputFile(evfile);
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPHEPEvtInterface::GPHEPEvtInterface(G4String)"<<G4endl;
#endif
}

void GPHEPEvtInterface::Init()
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPHEPEvtInterface::Init()"<<G4endl;
#endif
	dUnitE=MeV;
	dUnitP=MeV;
	dUnitL=cm;
	dRadiusRMSFactor=2.5e-3;
	dBunchLength=10;
	bRadiusRMSFactorFlag=TRUE;
	G4ThreeVector zero;
	particle_position = zero;
	particle_time = 0.0;
	dTimeUnit=picosecond;
	dParticlePosZ=-4e-3;
	CLHEP:: HepRandom::setTheSeed(time(0),time(0));
	randGauss = new CLHEP::RandGauss(&ranecuEngine,0.,2.5);
	
	/*
	fsOutputFile.open("photonInit.dat",std::fstream::out);
	 if(fsOutputFile.is_open()) 
	 {
	   G4cout<<"Set output file successed: "<<G4endl;
	   fsOutputFile.clear();
	 }
	 else 
	 { 
	   G4cout<<"Set output file failed: "<<G4endl;
	 }
	 */
	
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPHEPEvtInterface::Init()"<<G4endl;
#endif
}

GPHEPEvtInterface::~GPHEPEvtInterface()
{  
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPHEPEvtInterface::~GPHEPEvtInterface()"<<G4endl;
#endif
	if(fsInputFile.is_open()) fsInputFile.close();
	if(fsOutputFile.is_open()) fsOutputFile.close();
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPHEPEvtInterface::~GPHEPEvtInterface()"<<G4endl;
#endif
}

void GPHEPEvtInterface::SetInputFile(G4String tmp)
{
	if(fsInputFile.is_open())
	{
	  fsInputFile.close();
	  G4cout<<"Close input file: "<<sFileName<<G4endl;
	}
	
	fsInputFile.open(tmp,std::fstream::in);
	
	if(fsInputFile.is_open()) 
	{
	  sFileName=tmp;
	  G4cout<<"Set input file successed: "<<tmp<<G4endl;
	  fsInputFile.clear();
	}
	else 
	{ 
	  G4cout<<"Set input file failed: "<<tmp<<G4endl;
	}
	
}

void GPHEPEvtInterface::GeneratePrimaryVertex(G4Event* evt)
{
	if(!fsInputFile.is_open()) 
	{
	    G4cout<<"did not set the input file: "<<G4endl;
	    return;
	 }
	
	if(fsInputFile.eof()) 
	{
	  G4Exception("End-Of-File : HEPEvt input file");
	  return;
	}

    G4int 	NHEP;  // number of entries
    G4int 	ISTHEP;   // status code
    G4int 	IDHEP;    // PDG code
    G4int 	JDAHEP1;  // first daughter
    G4int 	JDAHEP2;  // last daughter
    G4double 	PHEP1; // px in MeV
    G4double 	PHEP2; // py in MeV
    G4double 	PHEP3; // pz in MeV
//    G4double 	PHEP4; // energy in MeV
    G4double 	PHEP5; // mass in MeV
    G4double 	VHEP1; //production vertex x;
    G4double 	VHEP2; //production vertex y
    G4double 	VHEP3; //production vertex z
//    G4double 	VHEP4; production time


        G4double 	x1=0;
	G4double	y1=0;

	G4PrimaryParticle* 	particle; 
	G4PrimaryParticle* 	mother;
	G4PrimaryParticle* 	daughter;
	G4HEPEvtParticle* 	hepParticle;
    G4PrimaryVertex* 	vertex;

    VHEP3=dParticlePosZ;
  
    fsInputFile>>NHEP;
  //  G4cout<<"The entries of this event: "<<NHEP<<G4endl;

  	for(G4int i=0; i<NHEP; i++ )
  	{
		fsInputFile >> ISTHEP >> IDHEP >> JDAHEP1 >> JDAHEP2>> PHEP1 >> PHEP2 >> PHEP3 >> PHEP5 >>VHEP1>>VHEP2;//>>
		
		if(bRadiusRMSFactorFlag==TRUE)
		{
			x1=randGauss->shoot(0.0,dRadiusRMSFactor);
			y1=randGauss->shoot(0.0,dRadiusRMSFactor);
		}
		else 
		{x1=0;y1=0;} 
		
		//double	rr=sqrt(x1*x1+y1*y1);
		//fsOutputFile<<rr<<"\n"<<-rr<<"\n";
		//fsOutputFile<<x1<<" "<<y1<<"\n";
		
		
		vectVHEP= new G4ThreeVector(VHEP1*dUnitL+x1*m,VHEP2*dUnitL+y1*m,VHEP3*m);
		
		//rr=sqrt(VHEP1*VHEP1+VHEP2*VHEP2);
		
		// create G4PrimaryParticle object
		particle = new G4PrimaryParticle( IDHEP, PHEP1*dUnitP, PHEP2*dUnitP, PHEP3*dUnitP);
		particle->SetMass( PHEP5*dUnitE);
		particle->SetPolarization(dPolX,dPolY,dPolZ);
		
		// create G4HEPEvtParticle object
		hepParticle = new G4HEPEvtParticle( particle, ISTHEP, JDAHEP1, JDAHEP2 );
		
		// Store
		vectHPlist.push_back( hepParticle );
		vectPosList.push_back(vectVHEP);
  	}

	// check if there is at least one particle
	if( vectHPlist.size() == 0 ) return; 
	
	// make connection between daughter particles decayed from 
	// the same mother
	for( size_t i=0; i<vectHPlist.size(); i++ )
	{
    	if( vectHPlist[i]->GetJDAHEP1() > 0 ) //  it has daughters
    	{
			G4int jda1 = vectHPlist[i]->GetJDAHEP1()-1; // FORTRAN index starts from 1
			G4int jda2 = vectHPlist[i]->GetJDAHEP2()-1; // but C++ starts from 0.
			mother = vectHPlist[i]->GetTheParticle();
			for( G4int j=jda1; j<=jda2; j++ )
			{
				daughter = vectHPlist[j]->GetTheParticle();
				if(vectHPlist[j]->GetISTHEP()>0)
				{
				  mother->SetDaughter( daughter );
				  vectHPlist[j]->Done();
				}
			  
      		}

    	}

  	}

	particle_position=*vectPosList[0];
	particle_time=randGauss->shoot(0.0,dBunchLength)*dTimeUnit;
	
	// put initial particles to the vertex
	for( size_t ii=0; ii<vectHPlist.size(); ii++ )
	{
		if( vectHPlist[ii]->GetISTHEP() > 0 ) // ISTHEP of daughters had been 
		                                   // set to negative
		{
			particle= vectHPlist[ii]->GetTheParticle();
			vertex= new G4PrimaryVertex(particle_position,particle_time);
			//create G4PrimaryVertex object
			vertex->SetPrimary( particle);
			// Put the vertex to G4Event object
			evt->AddPrimaryVertex( vertex );
		}
		
	}
	
	// clear G4HEPEvtParticles
	//vectHPlist.clearAndDestroy();
	for(size_t iii=0;iii<vectHPlist.size();iii++)
	{ 
	 	delete vectHPlist[iii];
	 	delete vectPosList[iii];
	}
	vectHPlist.clear();
	vectPosList.clear();
	
}

void GPHEPEvtInterface::Print()
{
    G4cout
    <<"\n------------------Print primary status---------------------------\n"
    <<"\nPrimary Generator type: HEPEvt"
    <<"\nEvents is implemented from: "<<sFileName
    <<"\nBeam length: "<<dBunchLength<<" ps"
    <<"\nBeam transverse dimension: "<<dRadiusRMSFactor<<" m"
    <<"\nPolarization(Sx,Sy,Sz): "<<"("<<dPolX<<","<<dPolY<<","<<dPolZ<<",)"
    <<"\nDistance From crystal to amorphous target: "<<dParticlePosZ<<" m"
    <<"\n-----------------------------------------------------------------\n"
    <<G4endl;

}

void GPHEPEvtInterface::Print(std::ofstream& ofsOutput)
{

  ofsOutput
    <<"\nPrimary status:"
    <<"\nPrimary Generator type:, HEPEvt"
    <<"\nEvents is implemented from, "<<sFileName
    <<"\nBeam length, "<<dBunchLength<<" ps"
    <<"\nPolarization(Sx,Sy,Sz): "<<"("<<dPolX<<","<<dPolY<<","<<dPolZ<<",)"
    <<"\nBeam transverse dimension, "<<dRadiusRMSFactor<<" m"
    <<"\nDistance From crystal to amorphous target, "<<dParticlePosZ<<" m"
    <<"\n-----------------------------------------------------------------\n"
    <<G4endl;

}
void GPHEPEvtInterface::SetParameter(std::string sLocal, std::string sGlobal)
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

    if(sValueOrg!="")
    {
      if(sLocalKey=="inputFile")
      {
	SetInputFile(sValueOrg);
        std::cout<<"Set: "<<sGlobalKey<<" to "<< sValueOrg<<std::endl;
	return;
      }
      if(sLocalKey=="unit.length")
      {
        dUnitL = G4UIcommand::ValueOf(sValueOrg.c_str());
        std::cout<<"Set: "<<sGlobalKey<<" to "<< sValueOrg<<std::endl;
	return;
      }
      else if(sLocalKey=="unit.energy")
      {
        dUnitE = G4UIcommand::ValueOf(sValueOrg.c_str());
        std::cout<<"Set: "<<sGlobalKey<<" to "<< sValueOrg<<std::endl;
	return;
      }
      else if(sLocalKey=="unit.momentum")
      {
        dUnitP = G4UIcommand::ValueOf(sValueOrg.c_str());
        std::cout<<"Set: "<<sGlobalKey<<" to "<< sValueOrg<<std::endl;
	return;
      }
      else if(sLocalKey=="unit.time")
      {
        dTimeUnit = G4UIcommand::ValueOf(sValueOrg.c_str());
        std::cout<<"Set: "<<sGlobalKey<<" to "<< sValueOrg<<std::endl;
	return;
      }
    }

    if(sLocalKey=="position.z")
      dParticlePosZ = dValueNew/m;
    else if(sLocalKey=="position.transverse.rms")
      dRadiusRMSFactor = dValueNew/m;
    else if(sLocalKey=="time.rms")
      dBunchLength = dValueNew/picosecond;
    else if(sLocalKey=="polarization")
    {
      std::string  sTmp;
      ss.clear();
      ss.str(sLocal);
      ss>>sTmp>>dPolX>>dPolY>>dPolZ;
      std::cout<<"Set polarization(Sx,Sy,Sz): "
	<<"("<<dPolX<<","<<dPolY<<","<<dPolZ<<",)"
	<<std::endl;
    }
   else 
   {
     std::cout<<"The Key: "<<sGlobalKey<<" is not exist."<<std::endl;
     return;
   }

   std::cout<<"Set: "<<sGlobalKey<<" to "<< dValueOrg<<" "<<sUnit<<std::endl;

}
G4double GPHEPEvtInterface::GetParameter(std::string sKey,std::string sKeyGlobal)
{
  return 0;
}

