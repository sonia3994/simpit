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
#include "G4TransportationManager.hh"
#include "G4Mag_UsualEqRhs.hh"
#include "G4MagIntegratorStepper.hh"
#include "G4ChordFinder.hh"

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
GPAMDField::GPAMDField()
{
  	B0=6*tesla;
  	B0=0.5*tesla;
  	alpha=22/m;
	fieldType=1;
	highQL=8*cm;
	lowQL=1*m;
//  G4String file="test";
//  fs.open(file,std::fstream::app);
}

GPAMDField::~GPAMDField()
{
//fs.close();
}
/*
void GPAMDField::WriteToFile(double x, double y, double z)
{

  fs<<x<<" "<<y<<" "<<z<<std::endl;

}*/

void GPAMDField::GetDetectorParameter() 
{
  	const GPDetectorConstruction * detector =
         dynamic_cast<const GPDetectorConstruction *>((G4RunManager::GetRunManager())->GetUserDetectorConstruction()) ;
  	G4double	tarL=detector->GetTargetThickness();
  	G4double	capL=detector->GetCaptureLength();
  	G4double	capR=detector->GetCaptureRadius();
}

void GPAMDField::GetFieldValue(const G4double Point[3], G4double *Bfield) const
{
	//GetDetectorParameter();
	switch(fieldType)
	{
		case 0:
			GetFieldValueAMD(Point, Bfield);
			break;
		case 1:
			GetFieldValueQWT(Point, Bfield);
			break;
		default:
			GetFieldValueAMD(Point, Bfield);
	}

}
void GPAMDField::GetFieldValueAMD(const G4double Point[3], G4double *Bfield) const
{
  	const GPDetectorConstruction * detector =
         dynamic_cast<const GPDetectorConstruction *>((G4RunManager::GetRunManager())->GetUserDetectorConstruction()) ;
  	G4double	tarL=detector->GetTargetThickness();
  	G4double	capL=detector->GetCaptureLength();
  	G4double	capR=detector->GetCaptureRadius();
  //G4cout<<"z: "<<Point[2]<<G4endl;
  	static G4double r2;
  	static G4double fz;
  	static G4double fz2;
	r2=sqr(Point[0])+sqr(Point[1]);
  if(Point[2]>tarL/2&&Point[2]<=(tarL/2+capL)&&r2<capR*capR)
  {
	fz=B0/(1+alpha*(Point[2]-tarL/2));
	fz2=sqr(fz);
	//
	//Bz(z,r)=f(z);f(z)=B0/(1+alpha*z)
	//Br(z,r)=-0.5*r*df(z)/dz;
  	Bfield[0]=0.5*Point[0]*fz2*alpha/B0;
  	Bfield[1]=0.5*Point[1]*fz2*alpha/B0;
	Bfield[2]=fz;

  	//Bfield[2]=0.5*tesla;
  }
  else 
  { 
	  Bfield[0]=Bfield[1]=Bfield[2]=0;
  }

  	Bfield[3]=Bfield[4]=Bfield[5]=0;
}

void GPAMDField::GetFieldValueQWT(const G4double Point[3], G4double *Bfield) const
{
  	const GPDetectorConstruction * detector =
         dynamic_cast<const GPDetectorConstruction *>((G4RunManager::GetRunManager())->GetUserDetectorConstruction()) ;
  	G4double	tarL=detector->GetTargetThickness();
  	G4double	capL=detector->GetCaptureLength();
  	G4double	capR=detector->GetCaptureRadius();
  	static	G4double	feiMi;
  	static	G4double	feiMiOne;

	G4double 	r2=sqr(Point[0])+sqr(Point[1]);
	if(r2<capR*capR)
	{
		//if(Point[2]>tarL/2&&Point[2]<=(tarL/2+highQL))
		if(Point[2]>tarL/2&&Point[2]<=(tarL/2+highQL+lowQL))
		{
			feiMi=exp((Point[2]-tarL/2-highQL)/mm);
			feiMiOne=1/(1+feiMi);
  			Bfield[0]=Point[0]*(B0-B1)*feiMi*sqr(feiMiOne)/2;
  			Bfield[1]=Point[1]*Bfield[0]/Point[0];
			Bfield[2]=(B0-B1)*feiMiOne+B1;

			//feiMi=1/(1+1720*sqr(Point[2]-tarL/2)/m/m);
			//feiMiOne=1/(1+feiMi);
  			//Bfield[0]=Point[0]*B0*1720*Point[2]*sqr(feiMi);
  			//Bfield[1]=Point[1]*Bfield[0]/Point[0];
			//Bfield[2]=B0*feiMi;

  			//Bfield[0]=0;
  			//Bfield[1]=0;
			//Bfield[2]=6*tesla;
			
		}
		/*
		else if(Point[2]>(tarL/2+highQL)&&Point[2]<=(tarL/2+highQL+lowQL))
		{
  			Bfield[0]=0;
  			Bfield[1]=0;
			Bfield[2]=0.5*tesla;
			
		}
		*/

	}

  	else 
  	{ 
	  	Bfield[0]=Bfield[1]=Bfield[2]=0;
  	}

  	Bfield[3]=Bfield[4]=Bfield[5]=0;
}


//////////////////////////////////////////////////////////////////////////
//
//  Constructors:

GPFieldSetup::GPFieldSetup()
  :  fChordFinder(0), fLocalChordFinder(0), fStepper(0),fLocalStepper(0)
{

  fMagneticField = new G4UniformMagField(
		       G4ThreeVector(3.3*tesla,
                                     0.0,              // 0.5*tesla,
                                     0.0       ));
  fAMDField = new GPAMDField();

  fFieldMessenger = new GPFieldMessenger(this) ;  
 
  fEquation = new G4Mag_UsualEqRhs(fMagneticField); 
  fLocalEquation = new G4Mag_UsualEqRhs(fAMDField); 
 
  fMinStep     = 1*mm ; // minimal step of 1 mm is default
  fStepperType = 4 ;      // ClassicalRK4 is default stepper

  fFieldManager = GetGlobalFieldManager();
  fLocalFieldManager = new G4FieldManager();
  captureFieldFlag=TRUE;
  globalFieldFlag=FALSE;

  UpdateField();
}

/////////////////////////////////////////////////////////////////////////////////

GPFieldSetup::GPFieldSetup(G4ThreeVector fieldVector)
{    
  fMagneticField = new G4UniformMagField(fieldVector);
  GetGlobalFieldManager()->CreateChordFinder(fMagneticField);
}

////////////////////////////////////////////////////////////////////////////////

GPFieldSetup::~GPFieldSetup()
{
 
  //if(fMagneticField) 		delete fMagneticField;
  if(fLocalFieldManager) 	delete fLocalFieldManager ;
  if(fMagneticField) 		delete fMagneticField;
  if(fAMDField) 	delete fAMDField;
  if(fChordFinder)   		delete fChordFinder;
  if(fLocalChordFinder)		delete fLocalChordFinder;
  if(fStepper)       		delete fStepper;
  if(fLocalStepper)      	delete fLocalStepper;
  if(fEquation)			delete fEquation; 
  if(fLocalEquation)		delete fLocalEquation; 
  if(fFieldMessenger)       	delete fFieldMessenger;
}

/////////////////////////////////////////////////////////////////////////////
//
// Update field
//

void GPFieldSetup::UpdateField()
{
  SetStepper();
  G4cout<<"The minimal step is equal to "<<fMinStep/mm<<" mm"<<G4endl ;

  if(globalFieldFlag)
  {
  	  fFieldManager->SetDetectorField(fMagneticField );
  }
  else
  {
  	  fFieldManager->SetDetectorField(NULL );
  }

  if(captureFieldFlag)
  {
	  fLocalFieldManager->SetDetectorField(fAMDField );
  }
  else
  {
	  fLocalFieldManager->SetDetectorField(NULL );
  }

  if(fChordFinder) delete fChordFinder;
  if(fLocalChordFinder) delete fLocalChordFinder;

  fChordFinder = new G4ChordFinder( fMagneticField, fMinStep,fStepper);
  fLocalChordFinder = new G4ChordFinder( fAMDField,fMinStep,fLocalStepper);

  fFieldManager->SetChordFinder( fChordFinder );
  fLocalFieldManager->SetChordFinder( fLocalChordFinder );
}

/////////////////////////////////////////////////////////////////////////////
//
// Set stepper according to the stepper type
//

void GPFieldSetup::SetStepper()
{
  if(fStepper) delete fStepper;
  if(fLocalStepper) delete fLocalStepper;

  switch ( fStepperType ) 
  {
    case 0:  
      fStepper = new G4ExplicitEuler( fEquation ); 
      fLocalStepper = new G4ExplicitEuler( fLocalEquation ); 
      G4cout<<"G4ExplicitEuler is calledS"<<G4endl;     
      break;
    case 1:  
      fStepper = new G4ImplicitEuler( fEquation );      
      fLocalStepper = new G4ImplicitEuler( fLocalEquation );      
      G4cout<<"G4ImplicitEuler is called"<<G4endl;     
      break;
    case 2:  
      fStepper = new G4SimpleRunge( fEquation );        
      fLocalStepper = new G4SimpleRunge( fLocalEquation );        
      G4cout<<"G4SimpleRunge is called"<<G4endl;     
      break;
    case 3:  
      fStepper = new G4SimpleHeum( fEquation );         
      fLocalStepper = new G4SimpleHeum( fLocalEquation );         
      G4cout<<"G4SimpleHeum is called"<<G4endl;     
      break;
    case 4:  
      fStepper = new G4ClassicalRK4( fEquation );       
      fLocalStepper = new G4ClassicalRK4( fLocalEquation );       
      G4cout<<"G4ClassicalRK4 (default) is called"<<G4endl;     
      break;
    case 5:  
      fStepper = new G4HelixExplicitEuler( fEquation ); 
      fLocalStepper = new G4HelixExplicitEuler( fLocalEquation ); 
      G4cout<<"G4HelixExplicitEuler is called"<<G4endl;     
      break;
    case 6:  
      fStepper = new G4HelixImplicitEuler( fEquation ); 
      fLocalStepper = new G4HelixImplicitEuler( fLocalEquation ); 
      G4cout<<"G4HelixImplicitEuler is called"<<G4endl;     
      break;
    case 7:  
      fStepper = new G4HelixSimpleRunge( fEquation );   
      fLocalStepper = new G4HelixSimpleRunge( fLocalEquation );   
      G4cout<<"G4HelixSimpleRunge is called"<<G4endl;     
      break;
    case 8:  
      fStepper = new G4CashKarpRKF45( fEquation );      
      fLocalStepper = new G4CashKarpRKF45( fLocalEquation );      
      G4cout<<"G4CashKarpRKF45 is called"<<G4endl;     
      break;
    case 9:  
      fStepper = new G4RKG3_Stepper( fEquation );       
      fLocalStepper = new G4RKG3_Stepper( fLocalEquation );       
      G4cout<<"G4RKG3_Stepper is called"<<G4endl;     
      break;
    default: fStepper = 0;
  }
}

/////////////////////////////////////////////////////////////////////////////
//
// Set the value of the Global Field to fieldValue along Z
//

void GPFieldSetup::SetCaptureType(G4int t) 
{
	fAMDField->SetCaptureType(t);
}


void GPFieldSetup::SetCaptureFieldFlag(G4bool t)
{
  captureFieldFlag=t; 
  if(captureFieldFlag)
  {
	  fLocalFieldManager->SetDetectorField(fAMDField );
	  G4cout<<"Active the capture field!"<<G4endl;
  }
  else
  {
	  fLocalFieldManager->SetDetectorField(NULL );
	  G4cout<<"Inative the capture field!"<<G4endl;
  }
}
void GPFieldSetup::SetFieldValue(G4double fieldStrength)
{
//  fAMDField->SetFieldValueB0(fieldStrength); 
  //G4ThreeVector fieldSetVec(0.0, 0.0, fieldStrength);
  //G4ThreeVector fieldSetVec(0.0, 0.0, fieldStrength);
  //this->SetFieldValue( fieldSetVec ); 
  //    *************

}
///////////////////////////////////////////////////////////////////////////////
//
// Set the value of the Global Field
//

void GPFieldSetup::SetFieldValueB0(G4double      fieldValue) 
{
	fAMDField->SetFieldValueB0(fieldValue);
}

void GPFieldSetup::SetFieldValue(G4ThreeVector fieldVector)
{
  if(fMagneticField) delete fMagneticField;

  if(fieldVector != G4ThreeVector(0.,0.,0.))
  { 
    fMagneticField = new  G4UniformMagField(fieldVector);
  }
  else 
  {
    // If the new field's value is Zero, then 
    //  setting the pointer to zero ensures 
    //  that it is not used for propagation.
    fMagneticField = 0; 
  }

  // Either  
  //   - UpdateField() to reset all (ChordFinder, Equation);
     // UpdateField();
  //   or simply update the field manager & equation of motion 
  //      with pointer to new field
  //GetGlobalFieldManager()->SetDetectorField(fMagneticField);
  GetGlobalFieldManager()->SetDetectorField(NULL);
  fEquation->SetFieldObj( fMagneticField ); 

}

////////////////////////////////////////////////////////////////////////////////
//
//  Utility method

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

  fMagneticField->GetFieldValue( position, fieldValue);
  G4ThreeVector fieldVec(fieldValue[0], fieldValue[1], fieldValue[2]); 

  return fieldVec;
}

