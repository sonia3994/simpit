// $Id: GPCaptureFieldManagerPool.cc,v 1.5 2007/04/28 01:31:12 gum Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
//  
//   Field Setup class implementation.
//

#include "GPCaptureFieldManagerPool.hh"
//#include "GPCaptureFieldMessenger.hh"   
#include "GPModuleManager.hh"     
#include "GPGeometryStore.hh"     
#include "GPGeometry.hh"     

//#include "G4UniformMagField.hh"
//#include "G4MagneticField.hh"
#include "G4FieldManager.hh"
#include "G4Mag_UsualEqRhs.hh"
#include "G4EqEMFieldWithSpin.hh"
#include "G4MagIntegratorStepper.hh"
#include "G4MagIntegratorDriver.hh"
#include "G4ChordFinder.hh"
//#include "G4TransportationManager.hh"
//#include "G4PropagatorInField.hh"

#include "G4UIcommand.hh"
//#include "G4TransportationManager.hh"
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
//GPCaptureField::GPCaptureField():G4ElectroMagneticField()
GPCaptureField::GPCaptureField():GPField()
{
  //Change to kg, m, s, tesla units now.
  dB0=6;
  dB1=0.5;
  iFieldType=0;
  dMu0=12.5664e-7;
  dCurrentI=150*1000;
  dQwtFermiAlpha=300;
  dMagneticRigidity=3.3e-2;
  dQwtExtend=0.0;
  iVerbose = 1;

}

GPCaptureField::~GPCaptureField()
{
}

void GPCaptureField::SetParameter(std::string sLocal, std::string sGlobal)
{
    std::stringstream ss(sLocal);
    std::string		  sUnit;
    std::string		  sKey;
    std::string		  sValue;
    G4double   		  dValueNew;
    G4double   		  dValueOrg;
    
    ss>>sKey>>sValue>>sUnit;
    ss.clear();
    ss.str(sValue);
    ss>>dValueOrg;

    if(sUnit!="")
      dValueNew=(dValueOrg*G4UIcommand::ValueOf(sUnit.c_str()))/m;
    else dValueNew=dValueOrg;

    if(sKey=="focal.length")
      dFocalLength = dValueNew;
    else if(sKey=="B0")
      dB0 = dValueNew;
    else if(sKey=="Bs")
      dB1 = dValueNew;
    else if(sKey=="current")
      dCurrentI = dValueNew;
    else if(sKey=="type")
      iFieldType = dValueNew;
    else if(sKey=="fermi.alpha")
      dQwtFermiAlpha = dValueNew;
    else if(sKey=="magnetic.rigidity")
      dMagneticRigidity = dValueNew;
    else if(sKey=="verbose")
      iVerbose = dValueNew;
    else 
    {
      std::cout<<sGeometryName+"field_mananger/field/: "+sKey+": Key does not exist."<<std::endl;
      return;
    }
    std::cout<<sGeometryName+"field_mananger/field/: Set "+sKey+": "+sValue+" "+sUnit<<std::endl;
}
double GPCaptureField::GetParameter(std::string sLocal, std::string sGlobal) const
{
  return -1;
}
void GPCaptureField::Init()
{

  GPGeometry* geometry =NULL;
  if(geometry==NULL)
    geometry = GPGeometryStore::GetInstance()->FindGeometry(sGeometryName);
  dSolidRadius = geometry->GetParameter("width","width")/2;
  dSolidHalfLength = geometry->GetParameter("length","length")/2;
  dSqrCapR=dSolidRadius*dSolidRadius;
  dAmdAlpha=(dB0/dB1-1)/(2*dSolidHalfLength); //unit m^(-1)
  dQwtNegaSqrAlpha=(dB0/dB1-1)/(2*dSolidHalfLength*2*dSolidHalfLength);//unit m^(-2)
  dQwtFermiCoef1=(dB0-dB1)/(-0.5+1/(1+exp(-dQwtFermiAlpha*2*dSolidHalfLength)));
  dQwtFermiCoef0=dB1-0.5*dQwtFermiCoef1;

  Print();

}


void GPCaptureField::GetFieldValue(const G4double Point[3], G4double *Bfield) const
{
  static G4ThreeVector vGlobal;
  static G4ThreeVector vLocal;
  static GPGeometry* geometry =NULL;

  vGlobal[0]=Point[0]/m;vGlobal[1]=Point[1]/m;vGlobal[2]=Point[2]/m;
  if(geometry==NULL)
    geometry = GPGeometryStore::GetInstance()->FindGeometry(sGeometryName);
  if(geometry->IsInThisGeometry(vGlobal)==false)
  {
    Bfield[0]=0;
    Bfield[1]=0;
    Bfield[2]=0;
    Bfield[3]=0;
    Bfield[4]=0;
    Bfield[5]=0;
    return;
  }
  //local position in this geometry
  vLocal=geometry->TransferToLocalFrame(vGlobal);

  switch(iFieldType)
  {
    case 0:
      GetFieldValueAMD(vLocal, Bfield);
      break;
    case 1:
      GetFieldValueQWTFermi(vLocal, Bfield);
      break;
    case 2:
      GetFieldValueQWTNegativeSqr(vLocal, Bfield);
      break;
    case 3:
      GetFieldValueQWTAbrupt(vLocal, Bfield);
      break;
    case 4:
      GetFieldValueLithium(vLocal, Bfield);
      break;
    default:
      GetFieldValueAMD(vLocal, Bfield);
  }
  if(iVerbose>1)
  {
    G4ThreeVector vB( Bfield[0]/tesla, Bfield[1]/tesla, Bfield[2]/tesla);
    G4ThreeVector vE( Bfield[3]*m/volt, Bfield[5]*m/volt, Bfield[5]*m/volt);
    std::cout
      <<"Global position (m): "<<vGlobal<<"; Local position (m): "<<vLocal
      <<"\nB (tesla): "<<vB<<";  E (volt/m): "<<vE
      <<std::endl;
  }

}
void GPCaptureField::GetFieldValueAMD(G4ThreeVector vLocal, G4double *Bfield) const
{
  static 	G4double 	fz;
  static 	G4double 	fz2;

  fz=1/(1+dAmdAlpha*(vLocal[2]+dSolidHalfLength));
  fz2=fz*fz;
  //
  //Bz(z,r)=f(z);f(z)=dB0/(1+dAmdAlpha*(z+z0))
  //Br(z,r)=-0.5*r*df(z)/dz;
  //Add the magnetic unit "tesla" when transfer to kernel.
  Bfield[0]=0.5*vLocal[0]*fz2*dAmdAlpha*dB0*tesla;
  Bfield[1]=Bfield[0]*vLocal[1]/vLocal[0];
  Bfield[2]=fz*dB0*tesla;
  Bfield[3]=Bfield[4]=Bfield[5]=0;
  //G4cout<<"x: "<<vLocal[0]<<" y: "<<vLocal[1]<<" z: "<<vLocal[2]<<G4endl;
  //G4cout<<"Bx: "<<Bfield[0]<<" By: "<<Bfield[1]<<" Bz: "<<Bfield[2]<<G4endl;

}

void GPCaptureField::GetFieldValueQWTFermi(G4ThreeVector vLocal, G4double *Bfield) const
{
  static	G4double	dPublic0;
  static	G4double	dPublic1;

  dPublic0=exp(dQwtFermiAlpha*(vLocal[2]-dSolidHalfLength));
  dPublic1=dQwtFermiCoef0+dQwtFermiCoef1/(1+dPublic0);

  //Add the magnetic unit "tesla" when transfer to kernel.
  Bfield[0]=tesla*0.5*vLocal[0]*dQwtFermiCoef1*dQwtFermiAlpha*dPublic0/((1+dPublic0)*(1+dPublic0));
  Bfield[1]=Bfield[0]*vLocal[1]/vLocal[0];
  Bfield[2]=tesla*dPublic1;
  Bfield[3]=Bfield[4]=Bfield[5]=0;
}

void GPCaptureField::GetFieldValueQWTNegativeSqr(G4ThreeVector vLocal, G4double *Bfield) const
{
  static 	G4double	feiMi;

  feiMi=1/(1+dQwtNegaSqrAlpha*vLocal[2]*vLocal[2]);
  Bfield[0]=vLocal[0]*dQwtNegaSqrAlpha*(vLocal[2]+dSolidHalfLength)*feiMi*feiMi*dB0*tesla;
  Bfield[1]=Bfield[0]*vLocal[1]/vLocal[0];
  Bfield[2]=feiMi*dB0*tesla;
  Bfield[3]=Bfield[4]=Bfield[5]=0;
}

void GPCaptureField::GetFieldValueQWTAbrupt(G4ThreeVector vLocal, G4double *Bfield) const
{
  Bfield[0]=0;
  Bfield[1]=0;
  Bfield[2]=dB0*tesla;
  Bfield[3]=Bfield[4]=Bfield[5]=0;
}


void GPCaptureField::GetFieldValueLithium(G4ThreeVector vLocal, G4double *Bfield) const
{
  static 	G4double 	magTP2;
  static 	G4double 	magTP;
  static 	G4double 	magI;
  static 	G4double 	dLocalR;

  static  G4ThreeVector vectCurrent;
  static  G4ThreeVector vectCurrentUnit;
  static  G4ThreeVector vectHorizonPos;
  static  G4ThreeVector vectHorizonPosUnit;
  static  G4ThreeVector vectorB;

  dLocalR=sqrt(vLocal[0]*vLocal[0]+vLocal[1]*vLocal[1]);
  vectCurrent=G4ThreeVector(0,0,dCurrentI);
  vectCurrentUnit=vectCurrent.unit();

  vectHorizonPos=G4ThreeVector(vLocal[0],vLocal[1],0);
  vectHorizonPosUnit=vectHorizonPos.unit();
  magTP2=vectHorizonPos.mag2();
  magTP=vectHorizonPos.mag();
  magI=vectCurrent.mag();

  //*/
  vectorB=vectCurrentUnit.cross(vectHorizonPosUnit)*magI*magTP*dMu0*tesla/(6.2832*dSolidRadius*dSolidRadius);
  Bfield[0]=vectorB.x();
  Bfield[1]=vectorB.y();
  Bfield[2]=vectorB.z();
  //G4cout<<"x: "<<vLocal[0]<<" y: "<<vLocal[1]<<" z: "<<vLocal[2]<<G4endl;
  //G4cout<<"Bx: "<<Bfield[0]<<" By: "<<Bfield[1]<<" Bz: "<<Bfield[2]<<" B: "<<vectorB.mag()<<G4endl;
  //G4cout<<"r: "<<magTP/m<<" m"<<" z: "<<vLocal[2]<<" mm"<<G4endl;
  //G4cout<<"Bx="<<Bfield[0]<<"By="<<Bfield[1]<<"Bz"<<Bfield[2]<<G4endl;

  Bfield[3]=Bfield[4]=Bfield[5]=0;
}

void GPCaptureField::Print()
{

  if(iFieldType==0)
  {
    std::cout<<"\nCapture field type, AMD"
      <<"\nFormula, B=B0/(1+Alpha*(Z+Z0))"
      <<"\nAlpha, "<<dAmdAlpha<<" m^(-1)"
      <<"\nB0, "<<dB0<<" tesla"
      <<"\nB1, "<<dB1<<" tesla"
      <<"\nZ0, "<<dSolidHalfLength<<" m"
      <<G4endl;
  }
  else if(iFieldType==1)
  {
    std::cout<<"\nCapture field type, QWT(Fermi Distribution approximation)"
      <<"\nFormula, B=A1+A2/(1+exp(A3*(Z-Z0)))"
      <<"\nA1, "<<dQwtFermiCoef0
      <<"\nA2, "<<dQwtFermiCoef1
      <<"\nA3, "<<dQwtFermiAlpha
      <<"\nB0, "<<dB0<<" tesla"
      <<"\nB1, "<<dB1<<" tesla"
      <<"\nZ0, "<<dSolidHalfLength<<" m"
      <<G4endl;
  }
  else if(iFieldType==2)
  {
    std::cout<<"\nCapture field tyep, QWT(Negative Quadratic approximation)"
      <<"\nFormula, B=B0/(1+z*z*Alpha)"
      <<"\nAlpha, "<<dQwtNegaSqrAlpha<<" m^(-2)"
      <<"\nB0, "<<dB0<<" tesla"
      <<"\nB1, "<<dB1<<" tesla"
      <<G4endl;
  }
  else if(iFieldType==3)
  {
    std::cout<<"\nCapture field, QWT(Abrupt field)"
      <<"\nB0, "<<dB0<<" tesla"
      <<"\nB1, "<<dB1<<" tesla"
      <<G4endl;
  }
  else if(iFieldType==4)
  {
    std::cout<<"\nCapture field type,Lithium"
      <<"\nCapture length, "<<2*dSolidHalfLength<<" m"
      <<"\nCapture radius, "<<dSolidRadius<<" m"
      <<"\nLithium lens length, "<<2*dSolidHalfLength<<" m"
      <<"\nLithium lens radius, "<<dSolidRadius<<" m"
      <<"\nFocal length, "<<dFocalLength<<" m"
      <<"\nCurrent, "<<dCurrentI<<" A"
      <<G4endl;
  }

}
void GPCaptureField::Print(std::ofstream& ofsOutput)
{

  if(iFieldType==0)
  {
    ofsOutput<<"\nCapture field type, AMD"
      <<"\nFormula, B=B0/(1+Alpha*(Z+Z0))"
      <<"\nAlpha, "<<dAmdAlpha<<" m^(-1)"
      <<"\nB0, "<<dB0<<" tesla"
      <<"\nB1, "<<dB1<<" tesla"
      <<"\nZ0, "<<dSolidHalfLength<<" m"
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
      <<"\nCapture length, "<<2*dSolidHalfLength<<" m"
      <<"\nCapture radius, "<<dSolidRadius<<" m"
      <<"\nLithium lens length, "<<2*dSolidHalfLength<<" m"
      <<"\nLithium lens radius, "<<dSolidRadius<<" m"
      <<"\nFocal length, "<<dFocalLength<<" m"
      <<"\nCurrent, "<<dCurrentI<<" A"
      <<G4endl;
  }

}
//////////////////////////////////////////////////////////////////////////
//  Constructors:

GPCaptureFieldManagerPool::GPCaptureFieldManagerPool(std::string sName, std::string sFatherName):
GPFieldManagerPool(sName,sFatherName)
{

  pGPField = new GPCaptureField();
  pEquation = new G4EqEMFieldWithSpin(pGPField); 
  pGPField->SetGeometryName(sFatherName);

  dMinStep     = 1e-3 ; // minimal step of 1 mm is default
  G4cout<<"The capture field minimal step: "<<dMinStep<<" m"<<G4endl ;
  iStepperType = 2 ;      // ClassicalRK4 is default stepper

  SetStepper();

  pIntDriver = new G4MagInt_Driver(dMinStep*m,pIntegratorStepper,pIntegratorStepper->GetNumberOfVariables());
  pChordFinder = new G4ChordFinder(pIntDriver);
  pFieldManager->SetChordFinder( pChordFinder );


  //UpdateField();
}


////////////////////////////////////////////////////////////////////////////////

GPCaptureFieldManagerPool::~GPCaptureFieldManagerPool()
{

  if(pGPField) 		delete pGPField;
  if(pChordFinder)	delete pChordFinder;
  if(pIntegratorStepper)      	delete pIntegratorStepper;
  if(pEquation)		delete pEquation; 

}

/////////////////////////////////////////////////////////////////////////////
//
void GPCaptureFieldManagerPool::Init()
{
    Update();
}
// Update field
void GPCaptureFieldManagerPool::Update()
{

  if(IsActive())
  {
    pFieldManager->SetDetectorField(pGPField );
    //It seems hard to simulate when setting following parameters too small.
    ///*
    pFieldManager->GetChordFinder()->SetDeltaChord(1e-9*m);
    pFieldManager->SetDeltaIntersection(1e-9*m);
    pFieldManager->SetDeltaOneStep(1e-9*m);
    pFieldManager->SetMaximumEpsilonStep(1e-6*m); 
    pFieldManager->SetMinimumEpsilonStep(1e-9*m);
    //*/
    pGPField->Init();
  }
  else
  {
    pFieldManager->SetDetectorField(NULL );
    G4cout<<"The Capture Field is inactive."<<G4endl;
  }


}
//
// Set stepper according to the stepper type
//

void GPCaptureFieldManagerPool::SetStepper()
{
  G4int nvar=12;
  if(pIntegratorStepper) delete pIntegratorStepper;
  switch ( iStepperType )
  {
    case 0:  
      //  2nd  order, for less smooth fields
      pIntegratorStepper = new G4SimpleRunge( pEquation, nvar );    
      G4cout<<"G4SimpleRunge is called"<<G4endl;    
      break;
    case 1:  
      //3rd  order, a good alternative to ClassicalRK
      pIntegratorStepper = new G4SimpleHeum( pEquation, nvar );    
      G4cout<<"G4SimpleHeum is called"<<G4endl;    
      break;
    case 2:  
      //4th order, classical  Runge-Kutta stepper, which is general purpose and robust.
      pIntegratorStepper = new G4ClassicalRK4( pEquation, nvar );    
      G4cout<<"G4ClassicalRK4 (default,nvar ) is called"<<G4endl;    
      break;
    case 3:
      //4/5th order for very smooth fields 
      pIntegratorStepper = new G4CashKarpRKF45( pEquation, nvar );
      G4cout<<"G4CashKarpRKF45 is called"<<G4endl;
      break;
    default: pIntegratorStepper = new G4ClassicalRK4( pEquation, nvar );
  }
}






void GPCaptureFieldManagerPool::Print(std::ofstream& ofsOutput)
{
  ofsOutput
    <<"\n[Begin Field: "+GetName()+"]"
    <<"\nState:"<<IsActive() ;
  if(IsActive()) pGPField->Print(ofsOutput);
  ofsOutput
    <<"\n[End Field: "+GetName()+"]" ;
}
void GPCaptureFieldManagerPool::Print()
{
  std::cout
    <<"\n[Begin Field: "+GetName()+"]"
    <<"\nState:"<<IsActive() ;
  if(IsActive()) pGPField->Print();
  std::cout
    <<"\n[End Field: "+GetName()+"]" ;
}
void GPCaptureFieldManagerPool::SetParameter(std::string sLocal, std::string sGlobal)
{
    std::stringstream ss(sLocal);
    std::string		  sUnit;
    std::string		  sKey;
    std::string		  sValue;
    G4double   		  dValueNew;
    G4double   		  dValueOrg;
    
    ss>>sKey>>sValue>>sUnit;
    ss.clear();
    ss.str(sValue);
    ss>>dValueOrg;

    if(sUnit!="")
      dValueNew=(dValueOrg*G4UIcommand::ValueOf(sUnit.c_str()))/m;
    else dValueNew=dValueOrg;

    std::string sTotalKey=sKey;
    std::string sFirstKey;
    std::string sLeftKey;
    size_t iFirstDot;
    iFirstDot = sTotalKey.find(".");
    if(iFirstDot!=std::string::npos)
    {
      sFirstKey=sTotalKey.substr(0,iFirstDot);
      sLeftKey=sTotalKey.substr(iFirstDot+1);
    }
    if(sFirstKey=="field")
    {
      pGPField->SetParameter(sLeftKey+" "+sValue+" "+sUnit,sGlobal);
      return;
    }
    else if(sKey=="active")
      SetActive(1);
    else if(sKey=="inactive")
      SetActive(0);
    else 
    {
      std::cout<<GetName()<<": "+sKey+": Key does not exist."<<std::endl;
      return;
    }
    //Init();

    std::cout<<GetName()<<": Set "<<sKey<<": "<< sValue<<" "<<sUnit<<std::endl;
}
double GPCaptureFieldManagerPool::GetParameter(std::string sLocal, std::string sGlobal) const
{
  return -1;
}
