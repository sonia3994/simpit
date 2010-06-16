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

  	fMinStep     = 0.01*mm ; // minimal step of 1 mm is default
	G4cout<<"The global field minimal step: "<<fMinStep/mm<<" mm"<<G4endl ;
  	fStepperType = 4 ;      // ClassicalRK4 is default stepper
  	
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
}

//
// Set stepper according to the stepper type
//

void GPFieldSetup::SetStepper()
{
	if(fGlobalStepper) delete fGlobalStepper;
	fGlobalStepper = new G4ClassicalRK4( fGlobalEquation,12);       
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


