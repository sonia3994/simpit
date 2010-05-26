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
// $Id: GPAcceleratorFieldManager.cc,v 1.5 2007/04/28 01:31:12 gum Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
//  
//   Field Setup class implementation.
//

#include "GPAcceleratorFieldManager.hh"
//#include "GPFieldMessenger.hh"   
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
  	tarL=detector->GetDetectorSize("target_z");
  	capL=detector->GetDetectorSize("capture_l");
  	accL=detector->GetDetectorSize("accelerator_l");

	
}


void GPAcceleratorField::GetFieldValue(const G4double Point[3], G4double *Bfield) const
{
	static G4double relativeZ;
	static G4ThreeVector local;
	local[0]=Point[0];local[1]=Point[1];local[2]=Point[2]-tarL/2-capL-accL/2;
	relativeZ=Point[2]-tarL/2-capL;

/*
	if(relativeZ>0&&relativeZ<=accL/20)
	{
	Bfield[0]=0;
	Bfield[1]=0;
	Bfield[2]=B0*20*relativeZ/accL;
	Bfield[3]=0;
	Bfield[4]=0;
	Bfield[5]=E0*20*relativeZ/accL;
	}
*/
	//else if(relativeZ<=accL&&relativeZ>accL/20)
	if(relativeZ>=0&&relativeZ<accL)
	{
	Bfield[0]=0;
	Bfield[1]=0;
	Bfield[2]=B0;
	Bfield[3]=0;
	Bfield[4]=0;
	Bfield[5]=E0;
	//G4cout<<"x: "<<Point[0]<<" y: "<<Point[1]<<" z: "<<Point[2]<<G4endl;
	//G4cout<<"Bx: "<<Bfield[0]<<" By: "<<Bfield[1]<<" Bz: "<<Bfield[2]<<G4endl;
	}
	else
	{
	Bfield[0]=0;
	Bfield[1]=0;
	Bfield[2]=0;
	Bfield[3]=0;
	Bfield[4]=0;
	Bfield[5]=0;
	}
}

//
//  Constructors:

GPAcceleratorFieldManager::GPAcceleratorFieldManager()
  :G4FieldManager(), fAcceleratorChordFinder(0), fAcceleratorStepper(0)  
{

  	fAcceleratorField = new GPAcceleratorField();
  	fAcceleratorEquation = new G4EqEMFieldWithSpin(fAcceleratorField); 
  	

  	//fFieldMessenger = new GPFieldMessenger(this) ;  
  	//fFieldMessenger->SetFieldPoint(fCaptureField) ;  

  	fMinStep     = 0.01*mm ; // minimal step of 1 mm is default
	G4cout<<"The minimal step is equal to "<<fMinStep/mm<<" mm"<<G4endl ;
  	fStepperType = 4 ;      // ClassicalRK4 is default stepper
  	acceleratorFieldFlag=true;
  	
	SetStepper();
	
	fAcceleratorIntegratorDriver = new G4MagInt_Driver(fMinStep,fAcceleratorStepper,fAcceleratorStepper->GetNumberOfVariables());
	fAcceleratorChordFinder = new G4ChordFinder(fAcceleratorIntegratorDriver);
	//fAcceleratorChordFinder = new G4ChordFinder((G4MagneticField*)fAcceleratorField,fMinStep,fAcceleratorStepper);
	
	SetChordFinder( fAcceleratorChordFinder );

  	UpdateField();
}

/////////////////////////////////////////////////////////////////////////////////

GPAcceleratorFieldManager::~GPAcceleratorFieldManager()
{
  	if(fAcceleratorField) 		delete fAcceleratorField;
  	if(fAcceleratorChordFinder)	delete fAcceleratorChordFinder;
  	if(fAcceleratorStepper)      	delete fAcceleratorStepper;
  	if(fAcceleratorEquation)		delete fAcceleratorEquation; 

  	//if(fAcceleratorIntegratorDriver)		delete fAcceleratorIntegratorDriver; 
     
  	//if(fFieldMessenger)     delete fFieldMessenger;
}

/////////////////////////////////////////////////////////////////////////////
//
void GPAcceleratorFieldManager::Init()
{
	UpdateField();
  	fAcceleratorField->Init();
	
}
/////////////////////////////////////////////////////////////////////////////
// Update field
void GPAcceleratorFieldManager::UpdateField()
{
	
	if(acceleratorFieldFlag)
	{
		SetDetectorField(fAcceleratorField );
		GetChordFinder()->SetDeltaChord(1e-9*m);
		SetDeltaIntersection(1e-9*m);
		SetDeltaOneStep(1e-9*m);
		//SetFieldChangesEnergy(true);
		
	}
	else
	{
		SetDetectorField(NULL );
	}
	
}

/////////////////////////////////////////////////////////////////////////////
// Set stepper according to the stepper type
//

void GPAcceleratorFieldManager::SetStepper()
{
	if(fAcceleratorStepper) delete fAcceleratorStepper;
	fAcceleratorStepper = new G4ClassicalRK4(fAcceleratorEquation, 12);       
}

/////////////////////////////////////////////////////////////////////////////
void GPAcceleratorFieldManager::SetAcceleratorFieldFlag(G4bool t)
{
	acceleratorFieldFlag=t; 
	if(acceleratorFieldFlag)
	{
	    SetDetectorField(fAcceleratorField );
	    G4cout<<"Active the Accelerator field!"<<G4endl;
	}
	else
	{
	    SetDetectorField(NULL );
	    G4cout<<"Inative the Accelerator field!"<<G4endl;
	}
}

