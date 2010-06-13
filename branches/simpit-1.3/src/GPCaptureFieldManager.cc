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
// $Id: GPCaptureFieldManager.cc,v 1.5 2007/04/28 01:31:12 gum Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
//  
//   Field Setup class implementation.
//

#include "GPCaptureFieldManager.hh"
#include "GPCaptureFieldMessenger.hh"   
#include "GPDetectorConstruction.hh"     

//#include "G4UniformMagField.hh"
//#include "G4MagneticField.hh"
//#include "G4FieldManager.hh"
#include "G4Mag_UsualEqRhs.hh"
#include "G4EqEMFieldWithSpin.hh"
#include "G4MagIntegratorStepper.hh"
#include "G4MagIntegratorDriver.hh"
#include "G4ChordFinder.hh"
//#include "G4TransportationManager.hh"
//#include "G4PropagatorInField.hh"

#include "G4RunManager.hh"
#include "G4ExplicitEuler.hh"
#include "G4ImplicitEuler.hh"
#include "G4SimpleRunge.hh"
#include "G4SimpleHeum.hh"
#include "G4ClassicalRK4.hh"
#include "G4HelixExplicitEuler.hh"
#include "G4HelixImplicitEuler.hh"
#include "G4HelixSimpleRunge.hh"
#include "G4CashKarpRKF45.hh"
#include "G4RKG3_Stepper.hh"
#include <fstream>
#include <sstream>
//////////////////////////////////////////////////////////////////////////
GPCaptureField::GPCaptureField():G4ElectroMagneticField()
{
  	B0=6*tesla;
  	B1=0.5*tesla;
  	amdAlpha=0.22; //unit cm^(-1)
  	qwtAlpha=0.172; //unit cm^(-2)
	fieldType=0;
	highQL=8*cm;
	lowQL=100*cm;
	gapL=1*cm;
	mu0=12.5664e-7;
	currentI=150*1000;
//  G4String file="test";
//  fs.open(file,std::fstream::app);
}

GPCaptureField::~GPCaptureField()
{
//fs.close();
}

void GPCaptureField::Init()
{

  	//const GPDetectorConstruction * detector =
         //static_cast<const GPDetectorConstruction *>((G4RunManager::GetRunManager())->GetUserDetectorConstruction()) ;
  	GPDetectorConstruction * detector = (GPDetectorConstruction* )G4RunManager::GetRunManager()->GetUserDetectorConstruction() ;
  	//tarL=detector->GetTargetThickness();
  	//capL=detector->GetCaptureLength();
  	//capR=detector->GetCaptureRadius();
  	tarL=detector->GetDetectorSize("target.z");
  	capL=detector->GetDetectorSize("capture.l");
  	capR=detector->GetDetectorSize("capture.or");

   	sqrCapR=capR*capR;
	halfTarL=tarL/2;
	halfCapL=capL/2;
	relativeMagL=halfTarL+gapL;
  	amdAlpha=(B0/B1-1)/(capL/cm); //unit cm^(-1)
	G4cout<<"AMD alpha: "<<amdAlpha<<" cm^(-1)"<<G4endl;
	qwtAlpha=(B0/B1-1)/(capL*capL/cm/cm);//unit cm^(-2)
	G4cout<<"QWT alpha: "<<qwtAlpha<<" cm^(-2)"<<G4endl;

}


void GPCaptureField::GetFieldValue(const G4double Point[3], G4double *Bfield) const
{
	switch(fieldType)
	{
		case 0:
			GetFieldValueAMD(Point, Bfield);
			break;
		case 1:
			GetFieldValueQWTFermi(Point, Bfield);
			break;
		case 2:
			GetFieldValueQWTNegativeSqr(Point, Bfield);
			break;
		case 3:
			GetFieldValueQWTAbrupt(Point, Bfield);
			break;
		case 4:
			GetFieldValueLithium(Point, Bfield);
			break;
		default:
			GetFieldValueAMD(Point, Bfield);
	}

}
void GPCaptureField::GetFieldValueAMD(const G4double Point[3], G4double *Bfield) const
{
  	static G4double 	fz;
  	static G4double 	fz2;
  	static G4double 	relativeZ;
  	static G4double 	localR2;
	
    localR2=Point[0]*Point[0]+Point[1]*Point[1];
	relativeZ=Point[2]-halfTarL;

  	if(relativeZ>0&&relativeZ<=capL&&localR2<=sqrCapR)
  	{
		fz=B0/(1+amdAlpha*relativeZ/cm);
		fz2=fz*fz;
		//
		//Bz(z,r)=f(z);f(z)=B0/(1+amdAlpha*z)
		//Br(z,r)=-0.5*r*df(z)/dz;
  		Bfield[0]=0.5*Point[0]*fz2*amdAlpha/B0/cm;
  		Bfield[1]=Point[1]*Bfield[0]/Point[0];
		Bfield[2]=fz;
		//G4cout<<"x: "<<Point[0]<<" y: "<<Point[1]<<" z: "<<Point[2]<<G4endl;
		//G4cout<<"Bx: "<<Bfield[0]<<" By: "<<Bfield[1]<<" Bz: "<<Bfield[2]<<G4endl;

  		//Bfield[2]=0.5*tesla;
  	}
  	else 
  	{ 
	  	Bfield[0]=Bfield[1]=Bfield[2]=0;
  	}

  	Bfield[3]=Bfield[4]=Bfield[5]=0;
}

void GPCaptureField::GetFieldValueQWTFermi(const G4double Point[3], G4double *Bfield) const
{
  	static	G4double	feiMi;
  	static	G4double	feiMiOne;
  	static 	G4double 	relativeZ;
  	static 	G4double 	localR2;

	relativeZ=Point[2]-halfTarL;
    localR2=Point[0]*Point[0]+Point[1]*Point[1];

  	if(relativeZ>0&&relativeZ<=capL&&localR2<=sqrCapR)
	{
     	feiMi=exp((relativeZ-highQL)/cm);
		feiMiOne=1/(1+feiMi);
  		Bfield[0]=0.5*Point[0]*(B0-B1)*feiMi*feiMiOne*feiMiOne/2/cm;
  		Bfield[1]=Point[1]*Bfield[0]/Point[0];
		Bfield[2]=(B0-B1)*feiMiOne+B1;
	}

  	else 
  	{ 
	  	Bfield[0]=Bfield[1]=Bfield[2]=0;
  	}

  	Bfield[3]=Bfield[4]=Bfield[5]=0;
}

void GPCaptureField::GetFieldValueQWTNegativeSqr(const G4double Point[3], G4double *Bfield) const
{
  	static	G4double	feiMi;
  	static 	G4double 	relativeZ;
	//std::cout<<Point[0]<<" "<<Point[1]<<" "<<Point[2]<<std::endl;
  	static G4double 	localR2;

	relativeZ=Point[2]-halfTarL;
    localR2=Point[0]*Point[0]+Point[1]*Point[1];

  	if(relativeZ>0&&relativeZ<=capL&&localR2<=sqrCapR)
	{
		feiMi=1/(1+qwtAlpha*relativeZ*relativeZ/cm/cm);
  		Bfield[0]=Point[0]*B0*qwtAlpha*relativeZ*feiMi*feiMi/cm/cm;
  		Bfield[1]=Point[1]*Bfield[0]/Point[0];
		Bfield[2]=B0*feiMi;
	}

  	else 
  	{ 
	  	Bfield[0]=Bfield[1]=Bfield[2]=0;
  	}

  	Bfield[3]=Bfield[4]=Bfield[5]=0;
}

void GPCaptureField::GetFieldValueQWTAbrupt(const G4double Point[3], G4double *Bfield) const
{
  	static 	G4double 	relativeZ;
  	static 	G4double 	localR2;

	relativeZ=Point[2]-halfTarL;
    localR2=Point[0]*Point[0]+Point[1]*Point[1];
  	if(relativeZ>0&&relativeZ<=highQL&&localR2<=sqrCapR)
	{
  		Bfield[0]=0;
  		Bfield[1]=0;
		Bfield[2]=B0;
	}
  	else if(relativeZ>highQL&&relativeZ<=capL&&localR2<=sqrCapR)
	{
  		Bfield[0]=0;
  		Bfield[1]=0;
		Bfield[2]=B1;
	}

  	else 
  	{ 
	  	Bfield[0]=Bfield[1]=Bfield[2]=0;
  	}

  	Bfield[3]=Bfield[4]=Bfield[5]=0;
}


void GPCaptureField::GetFieldValueLithium(const G4double Point[3], G4double *Bfield) const
{
  	static 	G4double 	relativeZ;
  	static 	G4double 	capR2;
  	static 	G4double 	magTP2;
  	static 	G4double 	magTP;
  	static 	G4double 	magI;
  	static 	G4double 	currentShape;
	
///*
	static  G4ThreeVector vectorI;
	static  G4ThreeVector vectorIUnit;
	static  G4ThreeVector vectorTP;
	static  G4ThreeVector vectorTPUnit;
	static  G4ThreeVector vectorB;
	static  G4ThreeVector local;
	//static  G4ThreeVector vectorBUnit;

	//local[0]=Point[0];local[1]=Point[1];local[2]=Point[2]-tarL/2-capL/2;
	local=TransformToLocal(G4ThreeVector(Point[0],Point[1],Point[2]));
	relativeZ=Point[2]-halfTarL-halfCapL;
	capR2=capR*capR/m/m;

	currentShape=currentI/(1+exp(10*abs(relativeZ-(capL-4)/2)));
    vectorI=G4ThreeVector(0,0,currentI);
    vectorIUnit=vectorI.unit();

    vectorTP=G4ThreeVector(Point[0]/m,Point[1]/m,0);
    vectorTPUnit=vectorTP.unit();
	magTP2=vectorTP.mag2();
	magTP=vectorTP.mag();
	magI=vectorI.mag();
	
//*/
  	if(relativeZ>-halfCapL&&relativeZ<=halfCapL&&magTP2<=capR2)
	{
/*
  		Bfield[0]=-tesla*mu0*currentI/(6.2832*capR2)*Point[1]/m;
  		Bfield[1]=tesla*mu0*currentI/(6.2832*capR2)*Point[0]/m;
		Bfield[2]=0;
*/

		vectorB=vectorIUnit.cross(vectorTPUnit)*magI*magTP*mu0*tesla/(6.2832*capR2);
		Bfield[0]=vectorB.x();
		Bfield[1]=vectorB.y();
		Bfield[2]=vectorB.z();
		//G4cout<<"x: "<<Point[0]<<" y: "<<Point[1]<<" z: "<<Point[2]<<G4endl;
		//G4cout<<"Bx: "<<Bfield[0]<<" By: "<<Bfield[1]<<" Bz: "<<Bfield[2]<<" B: "<<vectorB.mag()<<G4endl;
		//G4cout<<"r: "<<magTP/m<<" m"<<" z: "<<Point[2]<<" mm"<<G4endl;
		//G4cout<<"Bx="<<Bfield[0]<<"By="<<Bfield[1]<<"Bz"<<Bfield[2]<<G4endl;
	}

  	else 
  	{ 
	  	Bfield[0]=Bfield[1]=Bfield[2]=0;
  	}

  	Bfield[3]=Bfield[4]=Bfield[5]=0;
}

G4ThreeVector	GPCaptureField::TransformToLocal(G4ThreeVector global ) const
{
	G4ThreeVector local=G4ThreeVector(global[0],global[1],global[2]-halfTarL-halfCapL); 
	return local;
}

//////////////////////////////////////////////////////////////////////////
//  Constructors:

GPCaptureFieldManager::GPCaptureFieldManager()
:G4FieldManager(), fCaptureChordFinder(0),fCaptureStepper(0)  
{

  	fCaptureField = new GPCaptureField();
  	fCaptureEquation = new G4EqEMFieldWithSpin(fCaptureField); 

  	fFieldMessenger = new GPCaptureFieldMessenger(this) ;  
  	fFieldMessenger->SetFieldPoint(fCaptureField) ;  

  	fMinStep     = 0.01*mm ; // minimal step of 1 mm is default
	G4cout<<"The minimal step is equal to "<<fMinStep/mm<<" mm"<<G4endl ;
  	fStepperType = 4 ;      // ClassicalRK4 is default stepper
  	captureFieldFlag=true;
  	
	SetStepper();

	fCaptureIntegratorDriver = new G4MagInt_Driver(fMinStep,fCaptureStepper,fCaptureStepper->GetNumberOfVariables());
	fCaptureChordFinder = new G4ChordFinder(fCaptureIntegratorDriver);
	SetChordFinder( fCaptureChordFinder );

  	UpdateField();
}


////////////////////////////////////////////////////////////////////////////////

GPCaptureFieldManager::~GPCaptureFieldManager()
{
 
  	if(fCaptureField) 		delete fCaptureField;
  	if(fCaptureChordFinder)	delete fCaptureChordFinder;
  	if(fCaptureStepper)      	delete fCaptureStepper;
  	if(fCaptureEquation)		delete fCaptureEquation; 

  	//if(fCaptureIntegratorDriver)		delete fCaptureIntegratorDriver; 
     
  	if(fFieldMessenger)     delete fFieldMessenger;
}

/////////////////////////////////////////////////////////////////////////////
//
void GPCaptureFieldManager::Init()
{
	UpdateField();
  	fCaptureField->Init();
	
}
// Update field
void GPCaptureFieldManager::UpdateField()
{
	
	if(captureFieldFlag)
	{
		SetDetectorField(fCaptureField );
		GetChordFinder()->SetDeltaChord(1e-9*m);
		SetDeltaIntersection(1e-9*m);
		SetDeltaOneStep(1e-9*m);
	}
	else
	{
		SetDetectorField(NULL );
	}
	
	
}

//
// Set stepper according to the stepper type
//

void GPCaptureFieldManager::SetStepper()
{
	if(fCaptureStepper) delete fCaptureStepper;
	
	fCaptureStepper = new G4ClassicalRK4( fCaptureEquation,12);       
}



void GPCaptureFieldManager::SetFieldFlag(G4bool t)
{
	captureFieldFlag=t; 
	if(captureFieldFlag)
	{
	    SetDetectorField(fCaptureField );
	    G4cout<<"Active the capture field!"<<G4endl;
	}
	else
	{
	    SetDetectorField(NULL );
	    G4cout<<"Inative the capture field!"<<G4endl;
	}
}


