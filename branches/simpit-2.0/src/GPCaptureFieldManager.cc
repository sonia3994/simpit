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
#define MacRightAlign  std::setiosflags(std::ios_base::right)
#define MacLeftAlign  std::setiosflags(std::ios_base::left)
GPCaptureField::GPCaptureField():G4ElectroMagneticField()
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPCaptureField::GPCaptureField()"<<G4endl;
#endif
	//Change to kg, m, s, tesla units now.
  	dB0=6;
  	dB1=0.5;
	iFieldType=0;
	dMu0=12.5664e-7;
	dCurrentI=150*1000;
	dQwtFermiAlpha=300;
  	dMagneticRigidity=3.3e-2;
  	dQwtExtend=0.0;
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPCaptureField::GPCaptureField()"<<G4endl;
#endif
}

GPCaptureField::~GPCaptureField()
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPCaptureField::~GPCaptureField()"<<G4endl;
#endif
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPCaptureField::~GPCaptureField()"<<G4endl;
#endif
}

void GPCaptureField::Init()
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPCaptureField::Init()"<<G4endl;
#endif
  	GPDetectorConstruction * detector = (GPDetectorConstruction* )G4RunManager::GetRunManager()->GetUserDetectorConstruction() ;

	//transfer to international units
  	dTargetL=detector->GetDetectorSize("target.gz");
  	dCaptureL=detector->GetDetectorSize("capture.l");
  	dCaptureR=detector->GetDetectorSize("capture.or");
	dCaptureLithiumL=detector->GetDetectorSize("capture.lithium.l");
	dCaptureLithiumR=detector->GetDetectorSize("capture.lithium.or");
	iFieldType=detector->GetCaptureType();

   	dSqrCapR=dCaptureR*dCaptureR;
	dHalfTarL=dTargetL/2;
	dHalfCapL=dCaptureL/2;
  	dAmdAlpha=(dB0/dB1-1)/(dCaptureL); //unit m^(-1)
	dQwtNegaSqrAlpha=(dB0/dB1-1)/(dCaptureL*dCaptureL);//unit m^(-2)
  	dQwtFermiCoef1=(dB0-dB1)/(-0.5+1/(1+exp(-dQwtFermiAlpha*(dCaptureL-dQwtExtend))));
  	dQwtFermiCoef0=dB1-0.5*dQwtFermiCoef1;
	dFocalLength=dCaptureL-dCaptureLithiumL;

	if(iFieldType==0)
	{
		G4cout<<"\nCapture field Active AMD\n"
		      <<MacRightAlign<<std::setw(16)<<"Alpha: "<<dAmdAlpha<<" m^(-1)\n"
	      	      <<MacRightAlign<<std::setw(16)<<"B0: "<<dB0<<" tesla\n"
	      	      <<MacRightAlign<<std::setw(16)<<"B1: "<<dB1<<" tesla"
		      <<G4endl;
	}
	else if(iFieldType==1)
	{
		G4cout<<"\nCapture field Active QWT, Fermi Distribution approximation:\n"
		      <<MacRightAlign<<std::setw(16)<<"Formula: "<<"B=A1+A2/(1+exp(A3*(Z-Z1)))\n"
		      <<MacRightAlign<<std::setw(16)<<"A1: "<<dQwtFermiCoef0<<" \n"
		      <<MacRightAlign<<std::setw(16)<<"A2: "<<dQwtFermiCoef1<<" \n"
		      <<MacRightAlign<<std::setw(16)<<"A3: "<<dQwtFermiAlpha<<" \n"
		      <<MacRightAlign<<std::setw(16)<<"B0: "<<dB0<<" tesla\n"
		      <<MacRightAlign<<std::setw(16)<<"B1: "<<dB1<<" tesla"
		      <<G4endl;
	}
	else if(iFieldType==2)
	{
		G4cout<<"\nCapture field Active QWT, Negative Quadratic approximation:\n"
		      <<MacRightAlign<<std::setw(16)<<"Alpha: "<<dQwtNegaSqrAlpha<<" m^(-2)\n"
		      <<MacRightAlign<<std::setw(16)<<"B0: "<<dB0<<" tesla\n"
		      <<MacRightAlign<<std::setw(16)<<"B1: "<<dB1<<" tesla"
		      <<G4endl;
	}
	else if(iFieldType==3)
	{
		G4cout<<"\nCapture field Active QWT,Abrupt field.\n"
		      <<MacRightAlign<<std::setw(16)<<"B0: "<<dB0<<" tesla\n"
		      <<MacRightAlign<<std::setw(16)<<"B1: "<<dB1<<" tesla"
		      <<G4endl;
	}
	else if(iFieldType==4)
	{
		G4cout<<"\nCapture field Active Lithium.\n"
		      <<MacRightAlign<<std::setw(24)<<"Capture length: "<<dCaptureL<<" m\n"
		      <<MacRightAlign<<std::setw(24)<<"Capture radius: "<<dCaptureR<<" m\n"
		      <<MacRightAlign<<std::setw(24)<<"Lithium lens length: "<<dCaptureLithiumL<<" m\n"
		      <<MacRightAlign<<std::setw(24)<<"Lithium lens radius: "<<dCaptureLithiumR<<" m\n"
		      <<MacRightAlign<<std::setw(24)<<"Focal length: "<<dFocalLength<<" m\n"
		      <<MacRightAlign<<std::setw(24)<<"Current: "<<dCurrentI<<" A\n"
		      <<G4endl;
	}

#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPCaptureField::Init()"<<G4endl;
#endif
}


void GPCaptureField::GetFieldValue(const G4double Point[3], G4double *Bfield) const
{
	switch(iFieldType)
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
	localZ=Point[2]/m-dHalfTarL;

    localR2=localX*localX+localY*localY;

  	if(localZ>0&&localZ<=dCaptureL&&localR2<=dSqrCapR)
  	{
		fz=1/(1+dAmdAlpha*localZ);
		fz2=fz*fz;
		//
		//Bz(z,r)=f(z);f(z)=dB0/(1+dAmdAlpha*z)
		//Br(z,r)=-0.5*r*df(z)/dz;
		//Add the magnetic unit "tesla" when transfer to kernel.
  		Bfield[0]=0.5*localX*fz2*dAmdAlpha*dB0*tesla;
  		Bfield[1]=Bfield[0]*localY/localX;
		Bfield[2]=fz*dB0*tesla;
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
	localZ=Point[2]/m-dHalfTarL;
    localR2=localX*localX+localY*localY;

  	if(localZ>0&&localZ<=(dCaptureL-dQwtExtend)&&localR2<=dSqrCapR)
	{
		dPublic0=exp(dQwtFermiAlpha*(localZ-dCaptureL));
		dPublic1=dQwtFermiCoef0+dQwtFermiCoef1/(1+dPublic0);

		//Add the magnetic unit "tesla" when transfer to kernel.
  		Bfield[0]=tesla*0.5*localX*dQwtFermiCoef1*dQwtFermiAlpha*dPublic0/((1+dPublic0)*(1+dPublic0));
  		Bfield[1]=Bfield[0]*localY/localX;
		Bfield[2]=tesla*dPublic1;
	}
	else if(localZ>(dCaptureL-dQwtExtend)&&localZ<=dCaptureL&&localR2<=dSqrCapR)
	{

		//Add the magnetic unit "tesla" when transfer to kernel.
  		Bfield[0]=Bfield[1]=0;
		Bfield[2]=tesla*dB1;
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
	localZ=Point[2]/m-dHalfTarL;

    localR2=localX*localX+localY*localY;


  	if(localZ>0&&localZ<=dCaptureL&&localR2<=dSqrCapR)
	{
		feiMi=1/(1+dQwtNegaSqrAlpha*localZ*localZ);
  		Bfield[0]=localX*dQwtNegaSqrAlpha*localZ*feiMi*feiMi*dB0*tesla;
  		Bfield[1]=Bfield[0]*localY/localX;
		Bfield[2]=feiMi*dB0*tesla;
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
	localZ=Point[2]/m-dHalfTarL;

    localR2=localX*localX+localY*localY;
  	if(localZ>0&&localZ<=dCaptureL&&localR2<=dSqrCapR)
	{
  		Bfield[0]=0;
  		Bfield[1]=0;
		Bfield[2]=dB0*tesla;
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
  	//static 	G4double 	currentShape;
	
///*
	static  G4ThreeVector vectCurrent;
	static  G4ThreeVector vectCurrentUnit;
	static  G4ThreeVector vectHorizonPos;
	static  G4ThreeVector vectHorizonPosUnit;
	static  G4ThreeVector vectorB;

	//transfer to international units
	localX=Point[0]/m;
	localY=Point[1]/m;
	localZ=Point[2]/m-dHalfTarL;

    localR2=localX*localX+localY*localY;

    vectCurrent=G4ThreeVector(0,0,dCurrentI);
    vectCurrentUnit=vectCurrent.unit();

    vectHorizonPos=G4ThreeVector(localX,localY,0);
    vectHorizonPosUnit=vectHorizonPos.unit();
	magTP2=vectHorizonPos.mag2();
	magTP=vectHorizonPos.mag();
	magI=vectCurrent.mag();
	
//*/
  	if(localZ>dFocalLength&&localZ<=dCaptureL&&magTP2<=dCaptureLithiumR*dCaptureLithiumR)
	{

		vectorB=vectCurrentUnit.cross(vectHorizonPosUnit)*magI*magTP*dMu0*tesla/(6.2832*dCaptureLithiumR*dCaptureLithiumR);
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

void GPCaptureField::Print(std::ofstream& ofsOutput)
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPCaptureField::Print()"<<G4endl;
#endif

	if(iFieldType==0)
	{
		ofsOutput<<"\nCapture field type, AMD"
		      <<"\nFormula, B=B0/(1+z*Alpha)"
		      <<"\nAlpha, "<<dAmdAlpha<<" m^(-1)"
	      	      <<"\nB0, "<<dB0<<" tesla"
	      	      <<"\nB1, "<<dB1<<" tesla"
		      <<G4endl;
	}
	else if(iFieldType==1)
	{
		ofsOutput<<"\nCapture field type, QWT(Fermi Distribution approximation)"
		      <<"\nFormula, B=A1+A2/(1+exp(A3*(Z-Z1)))"
		      <<"\nA1, "<<dQwtFermiCoef0
		      <<"\nA2, "<<dQwtFermiCoef1
		      <<"\nA3, "<<dQwtFermiAlpha
		      <<"\nB0, "<<dB0<<" tesla"
		      <<"\nB1, "<<dB1<<" tesla"
		      <<G4endl;
	}
	else if(iFieldType==2)
	{
		ofsOutput<<"\nCapture field tyep, QWT(Negative Quadratic approximation)"
		      <<"\nFormula, B=B0/(1+z*z*Alpha)"
		      <<"\nAlpha, "<<dQwtNegaSqrAlpha<<" m^(-2)"
		      <<"\nB0, "<<dB0<<" tesla"
		      <<"\nB1, "<<dB1<<" tesla"
		      <<G4endl;
	}
	else if(iFieldType==3)
	{
		ofsOutput<<"\nCapture field, QWT(Abrupt field)"
		      <<"\nB0, "<<dB0<<" tesla"
		      <<"\nB1, "<<dB1<<" tesla"
		      <<G4endl;
	}
	else if(iFieldType==4)
	{
		ofsOutput<<"\nCapture field type,Lithium"
		      <<"\nCapture length, "<<dCaptureL<<" m"
		      <<"\nCapture radius, "<<dCaptureR<<" m"
		      <<"\nLithium lens length, "<<dCaptureLithiumL<<" m"
		      <<"\nLithium lens radius, "<<dCaptureLithiumR<<" m"
		      <<"\nFocal length, "<<dFocalLength<<" m"
		      <<"\nCurrent, "<<dCurrentI<<" A"
		      <<G4endl;
	}

#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPCaptureField::Print()"<<G4endl;
#endif
}
//////////////////////////////////////////////////////////////////////////
//  Constructors:

GPCaptureFieldManager::GPCaptureFieldManager()
:G4FieldManager(), fCaptureChordFinder(0),fCaptureStepper(0)  
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPCaptureFieldManager::GPCaptureFieldManager()"<<G4endl;
#endif

  	fCaptureField = new GPCaptureField();
  	fCaptureEquation = new G4EqEMFieldWithSpin(fCaptureField); 

  	fFieldMessenger = new GPCaptureFieldMessenger(this) ;  
  	fFieldMessenger->SetFieldPoint(fCaptureField) ;  

  	dMinStep     = 1e-3 ; // minimal step of 1 mm is default
	G4cout<<"The capture field minimal step: "<<dMinStep<<" m"<<G4endl ;
  	iStepperType = 2 ;      // ClassicalRK4 is default stepper
  	bCaptureFieldFlag=true;
  	
	SetStepper();

	fCaptureIntegratorDriver = new G4MagInt_Driver(dMinStep*m,fCaptureStepper,fCaptureStepper->GetNumberOfVariables());
	fCaptureChordFinder = new G4ChordFinder(fCaptureIntegratorDriver);
	SetChordFinder( fCaptureChordFinder );

  	//UpdateField();
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPCaptureFieldManager::GPCaptureFieldManager()"<<G4endl;
#endif
}


////////////////////////////////////////////////////////////////////////////////

GPCaptureFieldManager::~GPCaptureFieldManager()
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPCaptureFieldManager::~GPCaptureFieldManager()"<<G4endl;
#endif
 
  	if(fCaptureField) 		delete fCaptureField;
  	if(fCaptureChordFinder)	delete fCaptureChordFinder;
  	if(fCaptureStepper)      	delete fCaptureStepper;
  	if(fCaptureEquation)		delete fCaptureEquation; 

  	//if(fCaptureIntegratorDriver)		delete fCaptureIntegratorDriver; 
     
  	if(fFieldMessenger)     delete fFieldMessenger;
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPCaptureFieldManager::~GPCaptureFieldManager()"<<G4endl;
#endif
}

/////////////////////////////////////////////////////////////////////////////
//
void GPCaptureFieldManager::Init()
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPCaptureFieldManager::Init()"<<G4endl;
#endif
	UpdateField();
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPCaptureFieldManager::Init()"<<G4endl;
#endif
}
// Update field
void GPCaptureFieldManager::UpdateField()
{
	
	if(bCaptureFieldFlag)
	{
		SetDetectorField(fCaptureField );
		//It seems hard to simulate when setting following parameters too small.
		///*
		GetChordFinder()->SetDeltaChord(1e-9*m);
		SetDeltaIntersection(1e-9*m);
		SetDeltaOneStep(1e-9*m);
	    	SetMaximumEpsilonStep(1e-6*m); 
		SetMinimumEpsilonStep(1e-9*m);
		//*/
  		fCaptureField->Init();
	}
	else
	{
		SetDetectorField(NULL );
  		G4cout<<"The Capture Field is inactive."<<G4endl;
	}
	
	
}

//
// Set stepper according to the stepper type
//

void GPCaptureFieldManager::SetStepper()
{
	G4int nvar=12;
	if(fCaptureStepper) delete fCaptureStepper;
	switch ( iStepperType )
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
	bCaptureFieldFlag=t; 
	if(bCaptureFieldFlag)
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


void GPCaptureFieldManager::Print(std::ofstream& ofsOutput)
{
  fCaptureField->Print(ofsOutput);
}
