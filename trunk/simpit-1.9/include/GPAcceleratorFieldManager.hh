// $Id: GPAcceleratorFieldManager.hh,v 1.2 2006/06/29 17:18:51 gunter Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
//  A class for setting up the Magnetic Field of the setup, and 
//   creating the necessary classes to control accuracy of propagation.
//  In this example
//    - There is a global field for most of the setup;
//    - A local  field overides it for some volume(s) and it assumed to be uniform.
// 

#ifndef GPAcceleratorFieldManager_H
#define GPAcceleratorFieldManager_H

//#include "G4MagneticField.hh"
#include "G4FieldManager.hh"
#include "G4ElectroMagneticField.hh"
//#include "G4UniformMagField.hh"
//#include <string>

//class G4FieldManager;
class G4ChordFinder;
class G4Mag_UsualEqRhs;
class G4EqEMFieldWithSpin;
class G4MagIntegratorStepper;
class G4MagInt_Driver;
//class G4PropagatorInField;

class GPAcceleratorFieldMessenger;

class GPAcceleratorField : public G4ElectroMagneticField
{
public:
  	GPAcceleratorField();
  	~GPAcceleratorField();
	void Init();
  	void GetFieldValue(const G4double Point[3], G4double *Bfield) const;
  	inline void SetFieldValueB0(G4double t){dB0=t;G4cout<<"Set accelerator field  B0: "<<t<<" tesla"<<G4endl;};
  	inline void SetFieldValueE0(G4double t){dE0=t;G4cout<<"Set accelerator field  E0: "<<t<<" volt/m"<<G4endl;};
  	inline void SetFieldType(G4int t){iFieldType=t;G4cout<<"Set accelerator field  type: "<<t<<G4endl;};
	G4bool	DoesFieldChangeEnergy() const {return true;};

protected:
  	G4double 		dB0;
  	G4double 		dB1;
  	G4double 		dE0;
  	G4double 		dE1;
   	G4double 	 	dTarL;
   	G4double 	 	dCapL;
   	G4double 	 	dAccL;
   	G4double 	 	dAccR;
  	G4double 		dDelta;
	G4int			iFieldType;

};

class GPAcceleratorFieldManager : public G4FieldManager
{

public:

  GPAcceleratorFieldManager() ;
 ~GPAcceleratorFieldManager() ;
      
  void Init();
  inline void SetStepperType( G4int i) { iStepperType = i ;G4cout<<"Set accelerator field stepper type: "<<i<<G4endl; };
  inline void SetMinStep(G4double s) { dMinStep = s ;G4cout<<"Set accelerator field  minmum step: "<<s<<" m"<<G4endl; };
  void UpdateField();
  void SetFieldFlag(G4bool) ;

protected:
  void SetStepper();

protected:
  //G4PropagatorInField*    	propInField;

  GPAcceleratorField*       fAcceleratorField ; 
  G4ChordFinder*         	fAcceleratorChordFinder ;
  G4EqEMFieldWithSpin*      	fAcceleratorEquation ; 
  G4MagIntegratorStepper*	fAcceleratorStepper ;
  G4MagInt_Driver*			fAcceleratorIntegratorDriver;

  G4int                  	iStepperType ;
  G4double               	dMinStep ;

  GPAcceleratorFieldMessenger*      	fFieldMessenger;
  G4bool					bAcceleratorFieldFlag;

};

#endif
