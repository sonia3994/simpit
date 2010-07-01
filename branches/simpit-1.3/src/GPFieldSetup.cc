// $Id: GPFieldSetup.cc,v 1.5 2007/04/28 01:31:12 gum Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
//  
//   Field Setup class implementation.
//

#include "GPFieldSetup.hh"
#include "GPFieldMessenger.hh"   
#include "GPAcceleratorFieldManager.hh"
#include "GPCaptureFieldManager.hh"
//#include "GPDetectorConstruction.hh"     

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
//  Constructors:

GPFieldSetup::GPFieldSetup():
propInField(0),
fGlobalFieldManager(0),
fCaptureFieldManager(0),
fAcceleratorFieldManager(0),
fGlobalMagnetic(0), 
fGlobalChordFinder(0),
fGlobalEquation(0), 
fGlobalStepper(0),
fGlobalIntegratorDriver(0),
fFieldMessenger(0)
{

  	fGlobalMagnetic = new G4UniformMagField( G4ThreeVector(3.3*tesla, 0.0,  0.0));
  	fGlobalEquation = new G4EqEMFieldWithSpin(fGlobalMagnetic); 
  	
  	fGlobalFieldManager = GetGlobalFieldManager();
  	fCaptureFieldManager = new GPCaptureFieldManager();
  	fAcceleratorFieldManager = new GPAcceleratorFieldManager();


  	fFieldMessenger = new GPFieldMessenger(this) ;  
  	//fFieldMessenger->SetFieldPoint(fCaptureField) ;  

  	fMinStep     = 1*mm ; // minimal step of 1 mm is default
	G4cout<<"The global field minimal step: "<<fMinStep/mm<<" mm"<<G4endl ;
  	fStepperType = 2 ;      // ClassicalRK4 is default stepper
  	
  	globalFieldFlag=false;
	SetStepper();
	
	fGlobalIntegratorDriver = new G4MagInt_Driver(fMinStep,fGlobalStepper,fGlobalStepper->GetNumberOfVariables());
	fGlobalChordFinder = new G4ChordFinder(fGlobalIntegratorDriver);
	fGlobalFieldManager->SetChordFinder( fGlobalChordFinder );
	//G4cout<<"This is ok: mark 1 "<<G4endl ;

  	UpdateField();
}

/////////////////////////////////////////////////////////////////////////////////


GPFieldSetup::~GPFieldSetup()
{
 
  	if(fGlobalMagnetic) 		delete fGlobalMagnetic;
  	if(fGlobalChordFinder)   		delete fGlobalChordFinder;
  	if(fGlobalStepper)       		delete fGlobalStepper;
  	if(fGlobalEquation)			delete fGlobalEquation; 
  	//if(fGlobalIntegratorDriver)		delete fGlobalIntegratorDriver; 
  	if(fFieldMessenger)     delete fFieldMessenger;
}

/////////////////////////////////////////////////////////////////////////////
//
void GPFieldSetup::Init()
{
	UpdateField();
    fCaptureFieldManager->Init();
    fAcceleratorFieldManager->Init();
	
}
//
G4FieldManager* GPFieldSetup::GetLocalFieldManager(std::string name) 
{ 
	if(name=="capture")
		{return (G4FieldManager*) fCaptureFieldManager ;}
	else if(name=="accelerator")
		{return (G4FieldManager*) fAcceleratorFieldManager ;}
	else return NULL;
}

// Update field
void GPFieldSetup::UpdateField()
{
	propInField = G4TransportationManager::GetTransportationManager()->GetPropagatorInField();
	propInField->SetVerboseLevel(0);
	propInField->SetMaxLoopCount(50);

	if(globalFieldFlag)
	{
		fGlobalFieldManager->SetDetectorField(fGlobalMagnetic );
	}
	else
	{
		fGlobalFieldManager->SetDetectorField(NULL );
	}
}

//
// Set stepper according to the stepper type
//

void GPFieldSetup::SetStepper()
{
	G4int nvar=12;
	if(fGlobalStepper) delete fGlobalStepper;
	switch ( fStepperType )
	{
		case 0:  
		  //  2nd  order, for less smooth fields
		  fGlobalStepper = new G4SimpleRunge( fGlobalEquation, nvar );    
		  G4cout<<"G4SimpleRunge is called"<<G4endl;    
		  break;
		case 1:  
		  //3rd  order, a good alternative to ClassicalRK
		  fGlobalStepper = new G4SimpleHeum( fGlobalEquation, nvar );    
		  G4cout<<"G4SimpleHeum is called"<<G4endl;    
		  break;
		case 2:  
		  //4th order, classical  Runge-Kutta stepper, which is general purpose and robust.
		  fGlobalStepper = new G4ClassicalRK4( fGlobalEquation, nvar );    
		  G4cout<<"G4ClassicalRK4 (default,nvar ) is called"<<G4endl;    
		  break;
		case 3:
		  //4/5th order for very smooth fields 
		  fGlobalStepper = new G4CashKarpRKF45( fGlobalEquation, nvar );
		  G4cout<<"G4CashKarpRKF45 is called"<<G4endl;
		  break;
		default: fGlobalStepper = new G4ClassicalRK4( fGlobalEquation, nvar );
	}
}



void GPFieldSetup::SetFieldFlag(G4bool t)
{
	globalFieldFlag=t; 
	if(globalFieldFlag)
	{
	    fGlobalFieldManager->SetDetectorField(fGlobalMagnetic);
	    G4cout<<"Active the global field!"<<G4endl;
	}
	else
	{
	    fGlobalFieldManager->SetDetectorField(NULL);
	    G4cout<<"Inative the global field!"<<G4endl;
	}
}

G4FieldManager*  GPFieldSetup::GetGlobalFieldManager()
{
  return G4TransportationManager::GetTransportationManager()
	                        ->GetFieldManager();
}


