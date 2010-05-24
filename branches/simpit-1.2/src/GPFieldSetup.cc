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
// $Id: GPFieldSetup.cc,v 1.5 2007/04/28 01:31:12 gum Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
//  
//   Field Setup class implementation.
//

#include "GPFieldSetup.hh"
#include "GPFieldMessenger.hh"   
#include "GPDetectorConstruction.hh"     

#include "G4UniformMagField.hh"
#include "G4MagneticField.hh"
#include "G4FieldManager.hh"
#include "G4Mag_UsualEqRhs.hh"
#include "G4EqEMFieldWithSpin.hh"
#include "G4MagIntegratorStepper.hh"
#include "G4MagIntegratorDriver.hh"
#include "G4ChordFinder.hh"
#include "G4TransportationManager.hh"
#include "G4PropagatorInField.hh"

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
	fieldType=1;
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

  	const GPDetectorConstruction * detector =
         static_cast<const GPDetectorConstruction *>((G4RunManager::GetRunManager())->GetUserDetectorConstruction()) ;
  	tarL=detector->GetTargetThickness();
  	capL=detector->GetCaptureLength();
  	capR=detector->GetCaptureRadius();
   	sqrCapR=capR*capR;
	halfTarL=tarL/2;
	halfCapL=capL/2;
	relativeMagL=halfTarL+gapL;

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

	relativeZ=Point[2]-halfTarL;

  	if(relativeZ>0&&relativeZ<=capL)
  	{
		fz=B0/(1+amdAlpha*relativeZ/cm);
		fz2=fz*fz;
		//
		//Bz(z,r)=f(z);f(z)=B0/(1+amdAlpha*z)
		//Br(z,r)=-0.5*r*df(z)/dz;
  		Bfield[0]=0.5*Point[0]*fz2*amdAlpha/B0/cm;
  		Bfield[1]=Point[1]*Bfield[0]/Point[0];
		Bfield[2]=fz;

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

	relativeZ=Point[2]-halfTarL;
  	if(relativeZ>0&&relativeZ<=capL)
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

	relativeZ=Point[2]-halfTarL;
  	if(relativeZ>0&&relativeZ<=capL)
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

	relativeZ=Point[2]-halfTarL;
  	if(relativeZ>0&&relativeZ<=highQL)
	{
  		Bfield[0]=0;
  		Bfield[1]=0;
		Bfield[2]=B0;
	}
  	else if(relativeZ>highQL&&relativeZ<=capL)
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
	static  G4ThreeVector vectorI;
	static  G4ThreeVector vectorP;
	static  G4ThreeVector vectorB;
    vectorI=G4ThreeVector(0,0,currentI);;
    //vectorI[0]=0;vectorI[1]=0;vectorI[2]=currentI;
    vectorP=G4ThreeVector(Point[0]/m,Point[1]/m,0);
    //vectorP[0]=Point[0]/m;vectorP[1]=Point[1]/m;vectorP[2]=0;
	relativeZ=Point[2]-halfTarL;
	capR2=capR*capR/m/m;
  	if(relativeZ>0&&relativeZ<=highQL)
	{
		vectorB=vectorI.cross(vectorP)*tesla*mu0/(6.2832*capR2);
  		//Bfield[0]=tesla*mu0*currentI/(6.2832*capR2)*Point[0]/m;
  		//Bfield[1]=tesla*mu0*currentI/(6.2832*capR2)*Point[1]/m;

		Bfield[0]=vectorB.x();
		Bfield[1]=vectorB.y();
		Bfield[2]=vectorB.z();
		//Bfield[2]=0;
	}

  	else 
  	{ 
	  	Bfield[0]=Bfield[1]=Bfield[2]=0;
  	}

  	Bfield[3]=Bfield[4]=Bfield[5]=0;
}

//////////////////////////////////////////////////////////////////////////
GPAcceleratorField::GPAcceleratorField():G4ElectroMagneticField()
{
  	//B0=0;
  	B0=0.5*tesla;
  	E0=15e+6*volt/m;
  	//E0=0;
}

GPAcceleratorField::~GPAcceleratorField()
{
}

void GPAcceleratorField::Init()
{
  	GPDetectorConstruction * detector = (GPDetectorConstruction* )G4RunManager::GetRunManager()->GetUserDetectorConstruction() ;
  	tarL=detector->GetDetectorSize("target_x");
  	capL=detector->GetDetectorSize("capture_or");
  	accL=detector->GetDetectorSize("accelerator_or");

	
}


void GPAcceleratorField::GetFieldValue(const G4double Point[3], G4double *Bfield) const
{
	static G4double relativeZ;
	relativeZ=Point[2]-tarL/2-capL/2;

	if(relativeZ>=0)
	{
	Bfield[0]=0;
	Bfield[1]=0;
	Bfield[2]=B0;
	Bfield[3]=0;
	Bfield[4]=0;
	Bfield[5]=E0;
	}
}

//
//  Constructors:

GPFieldSetup::GPFieldSetup()
  :  fGlobalChordFinder(0), fCaptureChordFinder(0),fAcceleratorChordFinder(0),  fGlobalStepper(0),fCaptureStepper(0),fAcceleratorStepper(0)  
{

  	fGlobalMagnetic = new G4UniformMagField( G4ThreeVector(3.3*tesla, 0.0,  0.0));
  	fCaptureField = new GPCaptureField();
  	fAcceleratorField = new GPAcceleratorField();
  	
  	//fGlobalEquation = new G4Mag_UsualEqRhs(fGlobalMagnetic); 
  	//fCaptureEquation = new G4Mag_UsualEqRhs(fCaptureField); 
  	fGlobalEquation = new G4EqEMFieldWithSpin(fGlobalMagnetic); 
  	fCaptureEquation = new G4EqEMFieldWithSpin(fCaptureField); 
  	fAcceleratorEquation = new G4EqEMFieldWithSpin(fAcceleratorField); 
  	
  	fGlobalFieldManager = GetGlobalFieldManager();
  	fCaptureFieldManager = new G4FieldManager();
  	fAcceleratorFieldManager = new G4FieldManager();


  	fFieldMessenger = new GPFieldMessenger(this) ;  
  	fFieldMessenger->SetFieldPoint(fCaptureField) ;  

  	fMinStep     = 0.01*mm ; // minimal step of 1 mm is default
	G4cout<<"The minimal step is equal to "<<fMinStep/mm<<" mm"<<G4endl ;
  	fStepperType = 4 ;      // ClassicalRK4 is default stepper
  	
  	globalFieldFlag=false;
  	captureFieldFlag=true;
  	acceleratorFieldFlag=true;
  	
	SetStepper();
	
	fGlobalIntegratorDriver = new G4MagInt_Driver(fMinStep,fGlobalStepper,fGlobalStepper->GetNumberOfVariables());
	fCaptureIntegratorDriver = new G4MagInt_Driver(fMinStep,fCaptureStepper,fCaptureStepper->GetNumberOfVariables());
	fAcceleratorIntegratorDriver = new G4MagInt_Driver(fMinStep,fAcceleratorStepper,fAcceleratorStepper->GetNumberOfVariables());

	fGlobalChordFinder = new G4ChordFinder(fGlobalIntegratorDriver);
	fCaptureChordFinder = new G4ChordFinder(fCaptureIntegratorDriver);
	fAcceleratorChordFinder = new G4ChordFinder(fAcceleratorIntegratorDriver);
	//fAcceleratorChordFinder = new G4ChordFinder((G4MagneticField*)fAcceleratorField,fMinStep,fAcceleratorStepper);
	
	fGlobalFieldManager->SetChordFinder( fGlobalChordFinder );
	fCaptureFieldManager->SetChordFinder( fCaptureChordFinder );
	fAcceleratorFieldManager->SetChordFinder( fAcceleratorChordFinder );

  	UpdateField();
}

/////////////////////////////////////////////////////////////////////////////////

GPFieldSetup::GPFieldSetup(G4ThreeVector fieldVector)
{    
  	fGlobalMagnetic = new G4UniformMagField(fieldVector);
  	GetGlobalFieldManager()->CreateChordFinder(fGlobalMagnetic);
}

////////////////////////////////////////////////////////////////////////////////

GPFieldSetup::~GPFieldSetup()
{
 
  	//if(fGlobalMagnetic) 	delete fGlobalMagnetic;
  	if(fCaptureFieldManager) 	delete fCaptureFieldManager ;
  	if(fAcceleratorFieldManager) 	delete fAcceleratorFieldManager ;

  	if(fGlobalMagnetic) 		delete fGlobalMagnetic;
  	if(fCaptureField) 		delete fCaptureField;
  	if(fAcceleratorField) 		delete fAcceleratorField;

  	if(fGlobalChordFinder)   		delete fGlobalChordFinder;
  	if(fCaptureChordFinder)	delete fCaptureChordFinder;
  	if(fAcceleratorChordFinder)	delete fAcceleratorChordFinder;

  	if(fGlobalStepper)       		delete fGlobalStepper;
  	if(fCaptureStepper)      	delete fCaptureStepper;
  	if(fAcceleratorStepper)      	delete fAcceleratorStepper;

  	if(fGlobalEquation)			delete fGlobalEquation; 
  	if(fCaptureEquation)		delete fCaptureEquation; 
  	if(fAcceleratorEquation)		delete fAcceleratorEquation; 

  	//if(fGlobalIntegratorDriver)		delete fGlobalIntegratorDriver; 
  	//if(fCaptureIntegratorDriver)		delete fCaptureIntegratorDriver; 
  	//if(fAcceleratorIntegratorDriver)		delete fAcceleratorIntegratorDriver; 
     
  	if(fFieldMessenger)     delete fFieldMessenger;
}

/////////////////////////////////////////////////////////////////////////////
//
void GPFieldSetup::Init()
{
	UpdateField();
  	fCaptureField->Init();
  	fAcceleratorField->Init();
	
}
//
G4FieldManager* GPFieldSetup::GetLocalFieldManager(std::string name) 
{ 
	if(name=="capture")
		{return fCaptureFieldManager ;}
	else if(name=="accelerator")
		{return fAcceleratorFieldManager ;}
	else return NULL;
}

// Update field
void GPFieldSetup::UpdateField()
{
	propInField = G4TransportationManager::GetTransportationManager()->GetPropagatorInField();
	//propInField->SetMinimumEpsilonStep(1e-11);
	//propInField->SetMaximumEpsilonStep(1e-10);
	//propInField->SetVerboseLevel(1);
	propInField->SetMaxLoopCount(5000);
	

	if(globalFieldFlag)
	{
		fGlobalFieldManager->SetDetectorField(fGlobalMagnetic );
	}
	else
	{
		fGlobalFieldManager->SetDetectorField(NULL );
	}
	
	if(captureFieldFlag)
	{
		fCaptureFieldManager->SetDetectorField(fCaptureField );
		fCaptureFieldManager->GetChordFinder()->SetDeltaChord(1e-9*m);
		fCaptureFieldManager->SetDeltaIntersection(1e-9*m);
		fCaptureFieldManager->SetDeltaOneStep(1e-9*m);
		//fCaptureFieldManager->SetFieldChangesEnergy(false);
	}
	else
	{
		fCaptureFieldManager->SetDetectorField(NULL );
	}
	
	if(acceleratorFieldFlag)
	{
		fAcceleratorFieldManager->SetDetectorField(fAcceleratorField );
		fAcceleratorFieldManager->GetChordFinder()->SetDeltaChord(1e-9*m);
		fAcceleratorFieldManager->SetDeltaIntersection(1e-9*m);
		fAcceleratorFieldManager->SetDeltaOneStep(1e-9*m);
		//fAcceleratorFieldManager->SetFieldChangesEnergy(true);
		
	}
	else
	{
		fAcceleratorFieldManager->SetDetectorField(NULL );
	}
	
}

//
// Set stepper according to the stepper type
//

void GPFieldSetup::SetStepper()
{
	if(fGlobalStepper) delete fGlobalStepper;
	if(fCaptureStepper) delete fCaptureStepper;
	if(fAcceleratorStepper) delete fAcceleratorStepper;
	
	fGlobalStepper = new G4ClassicalRK4( fGlobalEquation,12);       
	fCaptureStepper = new G4ClassicalRK4( fCaptureEquation,12);       
	fAcceleratorStepper = new G4ClassicalRK4(fAcceleratorEquation, 12);       
}



void GPFieldSetup::SetCaptureFieldFlag(G4bool t)
{
	captureFieldFlag=t; 
	if(captureFieldFlag)
	{
	    fCaptureFieldManager->SetDetectorField(fCaptureField );
	    G4cout<<"Active the capture field!"<<G4endl;
	}
	else
	{
	    fCaptureFieldManager->SetDetectorField(NULL );
	    G4cout<<"Inative the capture field!"<<G4endl;
	}
}


void GPFieldSetup::SetAcceleratorFieldFlag(G4bool t)
{
	acceleratorFieldFlag=t; 
	if(acceleratorFieldFlag)
	{
	    fAcceleratorFieldManager->SetDetectorField(fAcceleratorField );
	    G4cout<<"Active the Accelerator field!"<<G4endl;
	}
	else
	{
	    fAcceleratorFieldManager->SetDetectorField(NULL );
	    G4cout<<"Inative the Accelerator field!"<<G4endl;
	}
}


G4FieldManager*  GPFieldSetup::GetGlobalFieldManager()
{
  return G4TransportationManager::GetTransportationManager()
	                        ->GetFieldManager();
}


// In place of G4UniformField::GetConstantFieldValue ...
// 
G4ThreeVector GPFieldSetup::GetConstantFieldValue()
{
  static G4double fieldValue[6],  position[4]; 
  position[0] = position[1] = position[2] = position[3] = 0.0; 

  fGlobalMagnetic->GetFieldValue( position, fieldValue);
  G4ThreeVector fieldVec(fieldValue[0], fieldValue[1], fieldValue[2]); 

  return fieldVec;
}

