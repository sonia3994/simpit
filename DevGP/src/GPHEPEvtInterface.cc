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
#include "GPDetectorConstruction.hh"

//#include "G4UIcommand.hh"

#include <sys/types.h>
#include <unistd.h>

#include <cmath>

#include "GPHEPEvtInterface.hh"

extern CLHEP::RanecuEngine ranecuEngine;
GPHEPEvtInterface::GPHEPEvtInterface():inputFile(NULL)
{
	Init();
}

GPHEPEvtInterface::GPHEPEvtInterface(G4String evfile)
{
  Init();
  SetInputFile(evfile);
}

void GPHEPEvtInterface::Init()
{
  UnitE=MeV;
  UnitP=MeV;
  UnitL=cm;
  eRMSR=2.5*mm;
  eRMSRflag=TRUE;
  G4ThreeVector zero;
  particle_position = zero;
  particle_time = 0.0;
  particlePosZ=-3.*mm;
  CLHEP:: HepRandom::setTheSeed(time(0),time(0));
  randGauss = new CLHEP::RandGauss(&ranecuEngine,0.,2.5);

  /*
  outputFile.open("photonInit.dat",std::fstream::out);
   if(outputFile.is_open()) 
   {
     G4cout<<"Set output file successed: "<<G4endl;
     outputFile.clear();
   }
   else 
   { 
     G4cout<<"Set output file failed: "<<G4endl;
   }
   */

}

GPHEPEvtInterface::~GPHEPEvtInterface()
{  
       	if(inputFile.is_open()) inputFile.close();
        if(outputFile.is_open()) outputFile.close();
}

void GPHEPEvtInterface::SetInputFile(G4String tmp)
{
   if(inputFile.is_open())
   {
     inputFile.close();
     G4cout<<"Close input file: "<<fileName<<G4endl;
   }
   
   inputFile.open(tmp,std::fstream::in);
//   inputFile=inf;
   if(inputFile.is_open()) 
   {
     fileName=tmp;
     G4cout<<"Set input file successed: "<<tmp<<G4endl;
     inputFile.clear();
   }
   else 
   { 
     G4cout<<"Set input file failed: "<<tmp<<G4endl;
   }

}

void GPHEPEvtInterface::GeneratePrimaryVertex(G4Event* evt)
{
  if(!inputFile.is_open()) 
  {
      G4cout<<"did not set the input file: "<<G4endl;
      return;
   }

  if(inputFile.eof()) 
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

	G4double 	polX;
	G4double 	polY;
	G4double 	polZ;

    G4double 	x1=0;
	G4double	y1=0;

   	const GPDetectorConstruction * detector =  
     	dynamic_cast<const GPDetectorConstruction *>((G4RunManager::GetRunManager())->GetUserDetectorConstruction()) ;
    G4double tarThick=detector->GetTargetThickness();

	G4PrimaryParticle* 	particle; 
	G4PrimaryParticle* 	mother;
	G4PrimaryParticle* 	daughter;
	G4HEPEvtParticle* 	hepParticle;
    G4PrimaryVertex* 	vertex;

    VHEP3=-particlePosZ-tarThick/2-1;
  
    inputFile>>NHEP;
  //  G4cout<<"The entries of this event: "<<NHEP<<G4endl;

  for(G4int i=0; i<NHEP; i++ )
  {
    inputFile >> ISTHEP >> IDHEP >> JDAHEP1 >> JDAHEP2>> PHEP1 >> PHEP2 >> PHEP3 >> PHEP5 >>VHEP1>>VHEP2;//>>
   // G4cout<<","<<ISTHEP<<","<<IDHEP<<","<<JDAHEP1<<","<<JDAHEP2<<","<<PHEP1<<","<<PHEP2<<","<<PHEP3<<","<<PHEP5<<","<<VHEP1<<","<<VHEP2<<","<<G4endl;

    if(eRMSRflag==TRUE)
    {
    x1=randGauss->shoot(0.0,eRMSR);
    y1=randGauss->shoot(0.0,eRMSR);
    }
    else 
	{x1=0;y1=0;} 

	//double	rr=sqrt(x1*x1+y1*y1);
    //outputFile<<rr<<"\n"<<-rr<<"\n";
    //outputFile<<x1<<" "<<y1<<"\n";

	VHEP1=VHEP1*UnitL+x1;
	VHEP2=VHEP2*UnitL+y1;

	PHEP1=PHEP1*UnitP;
	PHEP2=PHEP2*UnitP;
	PHEP3=PHEP3*UnitP;
	PHEP5=PHEP5*UnitE;

	polX=G4UniformRand();
	polY=G4UniformRand();
	polZ=std::sqrt(1.0-polX*polX-polY*polY);

    VHEP= new G4ThreeVector(VHEP1,VHEP2,VHEP3);

	//rr=sqrt(VHEP1*VHEP1+VHEP2*VHEP2);

    // create G4PrimaryParticle object
    particle = new G4PrimaryParticle( IDHEP, PHEP1, PHEP2, PHEP3);
    particle->SetMass( PHEP5 );
	particle->SetPolarization(polX,polY,polZ);

    // create G4HEPEvtParticle object
    hepParticle = new G4HEPEvtParticle( particle, ISTHEP, JDAHEP1, JDAHEP2 );

    // Store
    HPlist.push_back( hepParticle );
    PosList.push_back(VHEP);
  }

  // check if there is at least one particle
  if( HPlist.size() == 0 ) return; 

  // make connection between daughter particles decayed from 
  // the same mother
  for( size_t i=0; i<HPlist.size(); i++ )
  {
    if( HPlist[i]->GetJDAHEP1() > 0 ) //  it has daughters
    {
      G4int jda1 = HPlist[i]->GetJDAHEP1()-1; // FORTRAN index starts from 1
      G4int jda2 = HPlist[i]->GetJDAHEP2()-1; // but C++ starts from 0.
      mother = HPlist[i]->GetTheParticle();
      for( G4int j=jda1; j<=jda2; j++ )
      {
        daughter = HPlist[j]->GetTheParticle();
        if(HPlist[j]->GetISTHEP()>0)
        {
          mother->SetDaughter( daughter );
          HPlist[j]->Done();
        }
		
      }

    }

  }

  particle_position=*PosList[0];

  // put initial particles to the vertex
  for( size_t ii=0; ii<HPlist.size(); ii++ )
  {
    if( HPlist[ii]->GetISTHEP() > 0 ) // ISTHEP of daughters had been 
                                       // set to negative
    {
      particle= HPlist[ii]->GetTheParticle();
      vertex= new G4PrimaryVertex(particle_position,particle_time);
	//create G4PrimaryVertex object
      vertex->SetPrimary( particle);
    // Put the vertex to G4Event object
      evt->AddPrimaryVertex( vertex );
    }

  }

  // clear G4HEPEvtParticles
  //HPlist.clearAndDestroy();
  for(size_t iii=0;iii<HPlist.size();iii++)
  { 
   delete HPlist[iii];
   delete PosList[iii];
  }
  HPlist.clear();
  PosList.clear();

}

