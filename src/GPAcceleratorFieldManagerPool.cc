// $Id: GPAcceleratorFieldManagerPool.cc,v 1.5 2007/04/28 01:31:12 gum Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
//  
//   Field Setup class implementation.
//

#include "GPAcceleratorFieldManagerPool.hh"
#include "GPGeometry.hh"
#include "GPGeometryStore.hh"
#include "GPModuleManager.hh"     

#include "G4FieldManager.hh"
#include "G4Mag_UsualEqRhs.hh"
#include "G4EqEMFieldWithSpin.hh"
#include "G4MagIntegratorStepper.hh"
#include "G4MagIntegratorDriver.hh"
#include "G4ChordFinder.hh"

#include "G4UIcommand.hh"
#include "G4RunManager.hh"
#include "G4SimpleRunge.hh"
#include "G4SimpleHeum.hh"
#include "G4ClassicalRK4.hh"
#include "G4CashKarpRKF45.hh"
#include <fstream>
#include <sstream>
//////////////////////////////////////////////////////////////////////////
//GPAcceleratorField::GPAcceleratorField():G4ElectroMagneticField()
GPAcceleratorField::GPAcceleratorField():GPField()
{
  dB0=0.5;
  dB1=0.5;
  dE0=15e+6;
  dE1=15e+6;
  iVerbose=1;
}

GPAcceleratorField::~GPAcceleratorField()
{
}

void GPAcceleratorField::SetParameter(std::string sLocal, std::string sGlobal)
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

    if(sKey=="B0")
    {
      dValueNew=(dValueOrg*G4UIcommand::ValueOf(sUnit.c_str()))/tesla;
      dB0 = dValueNew;
    }
    else if(sKey=="E0")
    {
      dValueNew=(dValueOrg*G4UIcommand::ValueOf(sUnit.c_str()))/(volt/m);
      dE0 = dValueNew;
    }
    else if(sKey=="verbose")
      iVerbose = dValueOrg;
    else 
    {
      std::cout<<sGeometryName+"field_mananger/field/: "+sKey+": Key does not exist."<<std::endl;
      return;
    }
    std::cout<<sGeometryName+"field_mananger/field/: Set "+sKey+": "+sValue+" "+sUnit<<std::endl;
}
double GPAcceleratorField::GetParameter(std::string sLocal, std::string sGlobal)const
{
  return -1;
}
void GPAcceleratorField::Init()
{
  G4cout<<"\nAccelerator field:\n"
    <<std::setw(8)<<"B0: "<<dB0<<" tesla\n"
    <<std::setw(8)<<"E0: "<<dE0<<" volt/m"
    <<G4endl;

}


void GPAcceleratorField::GetFieldValue(const G4double Point[3], G4double *Bfield) const
{
  static G4ThreeVector vLocal;
  static G4ThreeVector vGlobal;
  //unit transform
  vGlobal[0]=Point[0]/m;vGlobal[1]=Point[1]/m;vGlobal[2]=Point[2]/m;

  static GPGeometry* geometry =NULL;
  if(geometry==NULL)
    geometry = GPGeometryStore::GetInstance()->FindGeometry(sGeometryName);
  //if it's in the right geometry
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

  Bfield[0]=0;
  Bfield[1]=0;
  Bfield[2]=dB1*tesla;
  Bfield[3]=0;
  Bfield[4]=0;
  Bfield[5]=dE1*volt/m;
  if(iVerbose>1)
  {
    G4ThreeVector vB( Bfield[0]/tesla, Bfield[1]/tesla, Bfield[2]/tesla);
    G4ThreeVector vE( Bfield[3]*m/volt, Bfield[4]*m/volt, Bfield[5]*m/volt);
    std::cout
      <<"; Local position (m): "<<vLocal
      <<"; B (tesla): "<<vB<<";  E (volt/m): "<<vE
      <<std::endl;
  }
}

void GPAcceleratorField::Print(std::ofstream& ofsOutput)
{
  ofsOutput
    <<"\nAccelerator field:"
    <<"\nB0,"<<dB0<<" tesla"
    <<"\nE0,"<<dE0<<" volt/m"
    <<G4endl;
}
void GPAcceleratorField::Print()
{
  std::cout
    <<"\nAccelerator field:"
    <<"\nB0,"<<dB0<<" tesla"
    <<"\nE0,"<<dE0<<" volt/m"
    <<G4endl;
}
//
//  Constructors:

GPAcceleratorFieldManagerPool::GPAcceleratorFieldManagerPool(std::string sName, std::string sFatherName):
  GPFieldManagerPool(sName,sFatherName)
{
  pGPField = new GPAcceleratorField();
  pEquation = new G4EqEMFieldWithSpin(pGPField); 

  pGPField->SetGeometryName(sFatherName);


  dMinStep     = 1e-3 ; // minimal step of 1 m is default
  G4cout<<"The Accelerator field minimal step: "<<dMinStep<<" m"<<G4endl ;
  iStepperType = 2 ;      // ClassicalRK4 is default stepper

  SetStepper();

  pIntDriver = new G4MagInt_Driver(dMinStep*m,pIntegratorStepper,pIntegratorStepper->GetNumberOfVariables());
  pChordFinder = new G4ChordFinder(pIntDriver);
  //pChordFinder = new G4ChordFinder((G4MagneticField*)pGPField,dMinStep,pIntegratorStepper);

  pFieldManager->SetChordFinder( pChordFinder );

  //UpdateField();
}

/////////////////////////////////////////////////////////////////////////////////

GPAcceleratorFieldManagerPool::~GPAcceleratorFieldManagerPool()
{
  if(pGPField) 		delete pGPField;
  if(pChordFinder)	delete pChordFinder;
  if(pIntegratorStepper)      	delete pIntegratorStepper;
  if(pEquation)		delete pEquation; 

}

/////////////////////////////////////////////////////////////////////////////
//
void GPAcceleratorFieldManagerPool::Init()
{
    Update();
}
/////////////////////////////////////////////////////////////////////////////
// Update field
void GPAcceleratorFieldManagerPool::Update()
{

  if(IsActive())
  {
    pFieldManager->SetDetectorField(pGPField );
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
    G4cout<<"The Accelerator Field is inactive."<<G4endl;
  }

}

/////////////////////////////////////////////////////////////////////////////
// Set stepper according to the stepper type
//

void GPAcceleratorFieldManagerPool::SetStepper()
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

/////////////////////////////////////////////////////////////////////////////

void GPAcceleratorFieldManagerPool::Print(std::ofstream& ofsOutput)
{
  ofsOutput
    <<"\n[Begin Field: "+GetName()+"]"
    <<"\nState:"<<IsActive() ;
  if(IsActive()) pGPField->Print(ofsOutput);
  ofsOutput
    <<"\n[End Field: "+GetName()+"]" ;
}

void GPAcceleratorFieldManagerPool::Print()
{
  std::cout
    <<"\n[Begin Field: "+GetName()+"]"
    <<"\nState:"<<IsActive() ;
  if(IsActive()) pGPField->Print();
  std::cout
    <<"\n[End Field: "+GetName()+"]" ;
}
void GPAcceleratorFieldManagerPool::SetParameter(std::string sLocal, std::string sGlobal)
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

    std::cout<<GetName()<<": Set "<<sKey<<": "<< sValue<<" "<<sUnit<<std::endl;
}
double GPAcceleratorFieldManagerPool::GetParameter(std::string sLocal, std::string sGlobal) const
{
  return -1;
}
