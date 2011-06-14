// $Id: GPFieldSetup.hh,v 1.2 2006/06/29 17:18:51 gunter Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
//  A class for setting up the Magnetic Field of the setup, and 
//   creating the necessary classes to control accuracy of propagation.
//  In this example
//    - There is a global field for most of the setup;
//    - A local  field overides it for some volume(s) and it assumed to be uniform.
// 

#ifndef GPFieldSetup_H
#define GPFieldSetup_H

#include "G4MagneticField.hh"
//#include "G4ElectroMagneticField.hh"
#include "globals.hh"
#include <string>

class G4FieldManager;
//class GPCaptureFieldManagerPool;
//class GPAcceleratorFieldManagerPool;
class GPFieldManagerPool;

class G4ChordFinder;
class G4Mag_UsualEqRhs;
class G4EqEMFieldWithSpin;
class G4MagIntegratorStepper;
class G4MagInt_Driver;
class G4PropagatorInField;

class GPFieldMessenger;

class GPFieldSetup
{

  protected:
    GPFieldSetup() ;
    ~GPFieldSetup() ;
public:

 static GPFieldSetup* GetGPFieldSetup();
 static void DestroyGPFieldSetup();
      
  void Init();
  void Print(std::ofstream&);
  inline void SetStepperType( G4int i) { iStepperType = i ;G4cout<<"Set global field stepper: "<<i<<G4endl; };  
  inline void SetMinStep(G4double s) { dMinStep = s ;G4cout<<"Set global field  minmum step: "<<s/mm<<" mm"<<G4endl; };
  void SetFieldFlag(G4bool) ;
  void UpdateField();
  GPFieldManagerPool*  FindFieldManagerPool(std::string name);

protected:
  void SetStepper();

protected:
  static GPFieldSetup* instance;
protected:
  G4PropagatorInField*    	propInField;

  G4FieldManager*        	GetGlobalFieldManager() ;
  G4FieldManager*        	fGlobalFieldManager ;
  GPFieldManagerPool*  	fCaptureFieldManagerPool ;
  GPFieldManagerPool*	fAcceleratorFieldManagerPool ;

  G4MagneticField*       	fGlobalMagnetic ; 
  G4ChordFinder*         	fGlobalChordFinder ;
  G4EqEMFieldWithSpin*      fGlobalEquation; 
  G4MagIntegratorStepper*	fGlobalStepper ;
  G4MagInt_Driver*			fGlobalIntegratorDriver;
  G4int                  	iStepperType ;
  G4double               	dMinStep ;
 
  GPFieldMessenger*      	fFieldMessenger;
  G4bool					bGlobalFieldFlag;
};

#endif
