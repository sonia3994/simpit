// $Id: GPAcceleratorFieldManager.cc,v 1.5 2007/04/28 01:31:12 gum Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
//  
//   Field Setup class implementation.
//

#include "GPAcceleratorFieldManager.hh"
#include "GPAcceleratorFieldMessenger.hh"   
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
//#include "G4ExplicitEuler.hh"
//#include "G4ImplicitEuler.hh"
#include "G4SimpleRunge.hh"
#include "G4SimpleHeum.hh"
#include "G4ClassicalRK4.hh"
//#include "G4HelixExplicitEuler.hh"
//#include "G4HelixImplicitEuler.hh"
//#include "G4HelixSimpleRunge.hh"
#include "G4CashKarpRKF45.hh"
//#include "G4RKG3_Stepper.hh"
#include <fstream>
#include <sstream>
//////////////////////////////////////////////////////////////////////////
GPAcceleratorField::GPAcceleratorField():G4ElectroMagneticField()
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPAcceleratorField::GPAcceleratorField()"<<G4endl;
#endif
  	dB0=0.5;
  	dB1=0.5;
  	dE0=15e+6;
  	dE1=15e+6;
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPAcceleratorField::GPAcceleratorField()"<<G4endl;
#endif
}

GPAcceleratorField::~GPAcceleratorField()
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPAcceleratorField::~GPAcceleratorField()"<<G4endl;
#endif

#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPAcceleratorField::~GPAcceleratorField()"<<G4endl;
#endif
}

void GPAcceleratorField::Init()
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPAcceleratorField::Init()"<<G4endl;
#endif
  	GPDetectorConstruction * detector = (GPDetectorConstruction* )G4RunManager::GetRunManager()->GetUserDetectorConstruction() ;
  	dTarL=detector->GetParameter("target.gz");
  	dCapL=detector->GetParameter("capture.l");
  	dAccL=detector->GetParameter("accelerator.l");
  	dAccR=detector->GetParameter("accelerator.or");

	dDelta=dAccL/20;
	G4cout<<"\nAccelerator field:\n"
	      <<std::setw(8)<<"B0: "<<dB0<<" tesla\n"
	      <<std::setw(8)<<"E0: "<<dE0<<" volt/m"
	      <<G4endl;
	
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPAcceleratorField::Init()"<<G4endl;
#endif
}


void GPAcceleratorField::GetFieldValue(const G4double Point[3], G4double *Bfield) const
{
	static G4ThreeVector local;
	static G4ThreeVector global;
	static G4double		 localR2;
	//unit transform
	global[0]=Point[0]/m;global[1]=Point[1]/m;global[2]=Point[2]/m;

    //global to local transform
	local[0]=global[0];
	local[1]=global[1];
	local[2]=global[2]-dTarL/2-dCapL-dAccL/2;
    localR2=local[0]*local[0]+local[1]*local[1];

/*
	if(localR2<dAccR*dAccR&&local[2]>-dAccL/2&&local[2]<=(-dAccL/2+dDelta))
	{
	static G4double		 B;
	static G4double		 E;
	B=(local[2]+dAccL/2)*(dB1-dB0)/dDelta+dB0;
	E=(local[2]+dAccL/2)*(dE1-dE0)/dDelta+dE0;
	Bfield[0]=0;
	Bfield[1]=0;
	Bfield[2]=B*tesla;
	Bfield[3]=0;
	Bfield[4]=0;
	Bfield[5]=E*volt/m;
	}
*/
	//else if(localR2<dAccR*dAccR&&local[2]<dAccL/2&&local[2]>=(-dAccL/2+dDelta))
	if(localR2<dAccR*dAccR&&local[2]>-dAccL/2&&local[2]<=dAccL/2)
	{
	Bfield[0]=0;
	Bfield[1]=0;
	Bfield[2]=dB1*tesla;
	Bfield[3]=0;
	Bfield[4]=0;
	Bfield[5]=dE1*volt/m;
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

void GPAcceleratorField::Print(std::ofstream& ofsOutput)
{
  ofsOutput<<"\nAccelerator field:"
        <<"\nB0,"<<dB0<<" tesla"
        <<"\nE0,"<<dE0<<" volt/m"
        <<G4endl;
}
//
//  Constructors:

GPAcceleratorFieldManager::GPAcceleratorFieldManager()
  :G4FieldManager(), fAcceleratorChordFinder(0), fAcceleratorStepper(0)  
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPAcceleratorFieldManager::GPAcceleratorFieldManager()"<<G4endl;
#endif

  	fAcceleratorField = new GPAcceleratorField();
  	fAcceleratorEquation = new G4EqEMFieldWithSpin(fAcceleratorField); 
  	

  	fFieldMessenger = new GPAcceleratorFieldMessenger(this) ;  
  	fFieldMessenger->SetFieldPoint(fAcceleratorField) ;  

  	dMinStep     = 1e-3 ; // minimal step of 1 m is default
	G4cout<<"The Accelerator field minimal step: "<<dMinStep<<" m"<<G4endl ;
  	iStepperType = 2 ;      // ClassicalRK4 is default stepper
  	bAcceleratorFieldFlag=true;
  	
	SetStepper();
	
	fAcceleratorIntegratorDriver = new G4MagInt_Driver(dMinStep*m,fAcceleratorStepper,fAcceleratorStepper->GetNumberOfVariables());
	fAcceleratorChordFinder = new G4ChordFinder(fAcceleratorIntegratorDriver);
	//fAcceleratorChordFinder = new G4ChordFinder((G4MagneticField*)fAcceleratorField,dMinStep,fAcceleratorStepper);
	
	SetChordFinder( fAcceleratorChordFinder );

  	//UpdateField();
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPAcceleratorFieldManager::GPAcceleratorFieldManager()"<<G4endl;
#endif
}

/////////////////////////////////////////////////////////////////////////////////

GPAcceleratorFieldManager::~GPAcceleratorFieldManager()
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPAcceleratorFieldManager::~GPAcceleratorFieldManager()"<<G4endl;
#endif
  	if(fAcceleratorField) 		delete fAcceleratorField;
  	if(fAcceleratorChordFinder)	delete fAcceleratorChordFinder;
  	if(fAcceleratorStepper)      	delete fAcceleratorStepper;
  	if(fAcceleratorEquation)		delete fAcceleratorEquation; 

  	//if(fAcceleratorIntegratorDriver)		delete fAcceleratorIntegratorDriver; 
     
  	if(fFieldMessenger)     delete fFieldMessenger;
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPAcceleratorFieldManager::~GPAcceleratorFieldManager()"<<G4endl;
#endif
}

/////////////////////////////////////////////////////////////////////////////
//
void GPAcceleratorFieldManager::Init()
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPAcceleratorFieldManager::Init()"<<G4endl;
#endif
	UpdateField();
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPAcceleratorFieldManager::Init()"<<G4endl;
#endif
}
/////////////////////////////////////////////////////////////////////////////
// Update field
void GPAcceleratorFieldManager::UpdateField()
{
	
	if(bAcceleratorFieldFlag)
	{
		SetDetectorField(fAcceleratorField );
		///*
		GetChordFinder()->SetDeltaChord(1e-9*m);
		SetDeltaIntersection(1e-9*m);
		SetDeltaOneStep(1e-9*m);
	    	SetMaximumEpsilonStep(1e-6*m); 
		SetMinimumEpsilonStep(1e-9*m);
		//*/
  		fAcceleratorField->Init();
		
	}
	else
	{
		SetDetectorField(NULL );
  		G4cout<<"The Accelerator Field is inactive."<<G4endl;
	}
	
}

/////////////////////////////////////////////////////////////////////////////
// Set stepper according to the stepper type
//

void GPAcceleratorFieldManager::SetStepper()
{
	G4int nvar=12;
	if(fAcceleratorStepper) delete fAcceleratorStepper;
	switch ( iStepperType )
	{
		case 0:  
		  //  2nd  order, for less smooth fields
		  fAcceleratorStepper = new G4SimpleRunge( fAcceleratorEquation, nvar );    
		  G4cout<<"G4SimpleRunge is called"<<G4endl;    
		  break;
		case 1:  
		  //3rd  order, a good alternative to ClassicalRK
		  fAcceleratorStepper = new G4SimpleHeum( fAcceleratorEquation, nvar );    
		  G4cout<<"G4SimpleHeum is called"<<G4endl;    
		  break;
		case 2:  
		  //4th order, classical  Runge-Kutta stepper, which is general purpose and robust.
		  fAcceleratorStepper = new G4ClassicalRK4( fAcceleratorEquation, nvar );    
		  G4cout<<"G4ClassicalRK4 (default,nvar ) is called"<<G4endl;    
		  break;
		case 3:
		  //4/5th order for very smooth fields 
		  fAcceleratorStepper = new G4CashKarpRKF45( fAcceleratorEquation, nvar );
		  G4cout<<"G4CashKarpRKF45 is called"<<G4endl;
		  break;
		default: fAcceleratorStepper = new G4ClassicalRK4( fAcceleratorEquation, nvar );
	}
}

/////////////////////////////////////////////////////////////////////////////
void GPAcceleratorFieldManager::SetFieldFlag(G4bool t)
{
	bAcceleratorFieldFlag=t; 
	if(bAcceleratorFieldFlag)
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

void GPAcceleratorFieldManager::Print(std::ofstream& ofsOutput)
{
  fAcceleratorField->Print(ofsOutput);
}

