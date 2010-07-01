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
GPCaptureField::GPCaptureField():G4ElectroMagneticField()
{
	//Change to kg, m, s units now.
  	//B0=6*tesla;
  	B0=6;
  	//B1=0.5*tesla;
  	B1=0.5;
	fieldType=0;
	mu0=12.5664e-7;
	currentI=150*1000;
	qwtFermiAlpha=300;
}

GPCaptureField::~GPCaptureField()
{
}

void GPCaptureField::Init()
{
  	GPDetectorConstruction * detector = (GPDetectorConstruction* )G4RunManager::GetRunManager()->GetUserDetectorConstruction() ;

	//transfer to international units
  	tarL=detector->GetDetectorSize("target.z")/m;
  	capL=detector->GetDetectorSize("capture.l")/m;
  	capR=detector->GetDetectorSize("capture.or")/m;

   	sqrCapR=capR*capR;
	halfTarL=tarL/2;
	halfCapL=capL/2;
  	amdAlpha=(B0/B1-1)/(capL); //unit m^(-1)
	qwtNegaSqrAlpha=(B0/B1-1)/(capL*capL);//unit m^(-2)
  	qwtFermiCoef1=(B0-B1)/(-0.5+1/(1+exp(-qwtFermiAlpha*capL)));
  	qwtFermiCoef0=B1-0.5*qwtFermiCoef1;

	if(fieldType==0)
	{
		G4cout<<"Active AMD, AMDAlpha: "<<amdAlpha<<" m^(-1)"<<G4endl;
	}
	else if(fieldType==1)
	{
	G4cout<<"Active QWT, qwtFermiAlpha: "<<qwtFermiAlpha<<" qwtFermiCoef0: "<<qwtFermiCoef0<<" qwtFermiCoef1: "<<qwtFermiCoef1<<G4endl;
	}
	else if(fieldType==2)
	{
	G4cout<<"Active QWT, QWTNegaSqrAlpha: "<<qwtNegaSqrAlpha<<" m^(-2)"<<G4endl;
	}
	else if(fieldType==3)
	{
	G4cout<<"Active QWT,Abrupt field. "<<G4endl;
	}
	else if(fieldType==4)
	{
	G4cout<<"Active Lithium. "<<G4endl;
	}

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
  	static 	G4double 	localX;
  	static 	G4double 	localY;
  	static 	G4double 	localZ;
  	static 	G4double 	localR2;
  	static 	G4double 	fz;
  	static 	G4double 	fz2;
	
	//transfer to international units
	localX=Point[0]/m;
	localY=Point[1]/m;
	localZ=Point[2]/m-halfTarL;

    localR2=localX*localX+localY*localY;

  	if(localZ>0&&localZ<=capL&&localR2<=sqrCapR)
  	{
		fz=1/(1+amdAlpha*localZ);
		fz2=fz*fz;
		//
		//Bz(z,r)=f(z);f(z)=B0/(1+amdAlpha*z)
		//Br(z,r)=-0.5*r*df(z)/dz;
		//Add the magnetic unit "tesla" when transfer to kernel.
  		Bfield[0]=0.5*localX*fz2*amdAlpha*B0*tesla;
  		Bfield[1]=Bfield[0]*localY/localX;
		Bfield[2]=fz*B0*tesla;
		//G4cout<<"x: "<<localX<<" y: "<<localY<<" z: "<<Point[2]<<G4endl;
		//G4cout<<"Bx: "<<Bfield[0]<<" By: "<<Bfield[1]<<" Bz: "<<Bfield[2]<<G4endl;

  	}
  	else 
  	{ 
	  	Bfield[0]=Bfield[1]=Bfield[2]=0;
  	}

  	Bfield[3]=Bfield[4]=Bfield[5]=0;
}

void GPCaptureField::GetFieldValueQWTFermi(const G4double Point[3], G4double *Bfield) const
{
  	static 	G4double 	localX;
  	static 	G4double 	localY;
  	static 	G4double 	localZ;
  	static 	G4double 	localR2;
  	static	G4double	dPublic0;
  	static	G4double	dPublic1;

	localX=Point[0]/m;
	localY=Point[1]/m;
	localZ=Point[2]/m-halfTarL;
    localR2=localX*localX+localY*localY;

  	if(localZ>0&&localZ<=capL&&localR2<=sqrCapR)
	{
		dPublic0=exp(qwtFermiAlpha*(localZ-capL));
		dPublic1=qwtFermiCoef0+qwtFermiCoef1/(1+dPublic0);

		//Add the magnetic unit "tesla" when transfer to kernel.
  		Bfield[0]=tesla*0.5*localX*qwtFermiCoef1*qwtFermiAlpha*dPublic0/((1+dPublic0)*(1+dPublic0));
  		Bfield[1]=Bfield[0]*localY/localX;
		Bfield[2]=tesla*dPublic1;
	}

  	else 
  	{ 
	  	Bfield[0]=Bfield[1]=Bfield[2]=0;
  	}

  	Bfield[3]=Bfield[4]=Bfield[5]=0;
}

void GPCaptureField::GetFieldValueQWTNegativeSqr(const G4double Point[3], G4double *Bfield) const
{
  	static 	G4double 	localX;
  	static 	G4double 	localY;
  	static 	G4double 	localZ;
  	static 	G4double	feiMi;
  	static 	G4double 	localR2;

	//transfer to international units
	localX=Point[0]/m;
	localY=Point[1]/m;
	localZ=Point[2]/m-halfTarL;

    localR2=localX*localX+localY*localY;


  	if(localZ>0&&localZ<=capL&&localR2<=sqrCapR)
	{
		feiMi=1/(1+qwtNegaSqrAlpha*localZ*localZ);
  		Bfield[0]=localX*qwtNegaSqrAlpha*localZ*feiMi*feiMi*B0*tesla;
  		Bfield[1]=Bfield[0]*localY/localX;
		Bfield[2]=feiMi*B0*tesla;
	}

  	else 
  	{ 
	  	Bfield[0]=Bfield[1]=Bfield[2]=0;
  	}

    Bfield[3]=Bfield[4]=Bfield[5]=0;
}

void GPCaptureField::GetFieldValueQWTAbrupt(const G4double Point[3], G4double *Bfield) const
{
  	static 	G4double 	localX;
  	static 	G4double 	localY;
  	static 	G4double 	localZ;
  	static 	G4double 	localR2;

	//transfer to international units
	localX=Point[0]/m;
	localY=Point[1]/m;
	localZ=Point[2]/m-halfTarL;

    localR2=localX*localX+localY*localY;
  	if(localZ>0&&localZ<=capL&&localR2<=sqrCapR)
	{
  		Bfield[0]=0;
  		Bfield[1]=0;
		Bfield[2]=B0*tesla;
	}
  	else 
  	{ 
	  	Bfield[0]=Bfield[1]=Bfield[2]=0;
  	}

  	Bfield[3]=Bfield[4]=Bfield[5]=0;
}


void GPCaptureField::GetFieldValueLithium(const G4double Point[3], G4double *Bfield) const
{
  	static 	G4double 	localX;
  	static 	G4double 	localY;
  	static 	G4double 	localZ;
  	static 	G4double 	localR2;
  	static 	G4double 	magTP2;
  	static 	G4double 	magTP;
  	static 	G4double 	magI;
  	static 	G4double 	currentShape;
	
///*
	static  G4ThreeVector vectCurrent;
	static  G4ThreeVector vectCurrentUnit;
	static  G4ThreeVector vectHorizonPos;
	static  G4ThreeVector vectHorizonPosUnit;
	static  G4ThreeVector vectorB;

	//transfer to international units
	localX=Point[0]/m;
	localY=Point[1]/m;
	localZ=Point[2]/m-halfTarL-halfCapL;

    localR2=localX*localX+localY*localY;

	//currentShape=currentI/(1+exp(10*abs(localZ-(capL-4)/2)));
    vectCurrent=G4ThreeVector(0,0,currentI);
    vectCurrentUnit=vectCurrent.unit();

    vectHorizonPos=G4ThreeVector(localX,localY,0);
    vectHorizonPosUnit=vectHorizonPos.unit();
	magTP2=vectHorizonPos.mag2();
	magTP=vectHorizonPos.mag();
	magI=vectCurrent.mag();
	
//*/
  	if(localZ>-halfCapL&&localZ<=halfCapL&&magTP2<=sqrCapR)
	{

		vectorB=vectCurrentUnit.cross(vectHorizonPosUnit)*magI*magTP*mu0*tesla/(6.2832*sqrCapR);
		Bfield[0]=vectorB.x();
		Bfield[1]=vectorB.y();
		Bfield[2]=vectorB.z();
		//G4cout<<"x: "<<localX<<" y: "<<localY<<" z: "<<localZ<<G4endl;
		//G4cout<<"Bx: "<<Bfield[0]<<" By: "<<Bfield[1]<<" Bz: "<<Bfield[2]<<" B: "<<vectorB.mag()<<G4endl;
		//G4cout<<"r: "<<magTP/m<<" m"<<" z: "<<localZ<<" mm"<<G4endl;
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

  	fMinStep     = 1*mm ; // minimal step of 1 mm is default
	G4cout<<"The capture field minimal step: "<<fMinStep/mm<<" mm"<<G4endl ;
  	fStepperType = 2 ;      // ClassicalRK4 is default stepper
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
		//It seems hard to simulate when setting following parameters too small.
		GetChordFinder()->SetDeltaChord(1e-10*m);
		SetDeltaIntersection(1e-10*m);
		SetDeltaOneStep(1e-10*m);
	    SetMaximumEpsilonStep(1e-6*m); 
		SetMinimumEpsilonStep(1e-9*m);
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
	G4int nvar=12;
	if(fCaptureStepper) delete fCaptureStepper;
	switch ( fStepperType )
	{
		case 0:  
		  //  2nd  order, for less smooth fields
		  fCaptureStepper = new G4SimpleRunge( fCaptureEquation, nvar );    
		  G4cout<<"G4SimpleRunge is called"<<G4endl;    
		  break;
		case 1:  
		  //3rd  order, a good alternative to ClassicalRK
		  fCaptureStepper = new G4SimpleHeum( fCaptureEquation, nvar );    
		  G4cout<<"G4SimpleHeum is called"<<G4endl;    
		  break;
		case 2:  
		  //4th order, classical  Runge-Kutta stepper, which is general purpose and robust.
		  fCaptureStepper = new G4ClassicalRK4( fCaptureEquation, nvar );    
		  G4cout<<"G4ClassicalRK4 (default,nvar ) is called"<<G4endl;    
		  break;
		case 3:
		  //4/5th order for very smooth fields 
		  fCaptureStepper = new G4CashKarpRKF45( fCaptureEquation, nvar );
		  G4cout<<"G4CashKarpRKF45 is called"<<G4endl;
		  break;
		default: fCaptureStepper = new G4ClassicalRK4( fCaptureEquation, nvar );
	}
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


