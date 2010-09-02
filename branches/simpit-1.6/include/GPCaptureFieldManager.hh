// $Id: GPCaptureManagerField.hh,v 1.2 2006/06/29 17:18:51 gunter Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
//  A class for setting up the Magnetic Field of the setup, and 
//   creating the necessary classes to control accuracy of propagation.
//  In this example
//    - There is a global field for most of the setup;
//    - A local  field overides it for some volume(s) and it assumed to be uniform.
// 

#ifndef GPCaptureManagerField_H
#define GPCaptureManagerField_H 1

//#include "G4MagneticField.hh"
#include "G4FieldManager.hh"
#include "G4ThreeVector.hh"
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

class GPCaptureFieldMessenger;
class GPCaptureField : public G4ElectroMagneticField
{
public:
  	GPCaptureField();
  	~GPCaptureField();
	void Init();
	G4bool	DoesFieldChangeEnergy() const {return false;};
  	void GetFieldValue(const G4double Point[3], G4double *Bfield) const;
  	inline void SetFieldValueB0(G4double      t){dB0=t;G4cout<<"Set capture field dB0 value: "<<dB0<<" tesla."<<G4endl;};
  	inline void SetFieldValueAlpha(G4double   t){dAmdAlpha=t;G4cout<<"Set AMD field alpha value: "<<dAmdAlpha<<G4endl;};
  	inline void SetAMDFieldAlpha(G4double     t){dAmdAlpha=t;G4cout<<"Set AMD field alpha value: "<<dAmdAlpha<<G4endl;} ;
  	inline void SetFermiApproximateAlpha(G4double     t){dQwtFermiAlpha=t;G4cout<<"Set QWT fermi approximate field alpha: "<<dQwtFermiAlpha<<G4endl;} ;
  	inline void SetCaptureType(G4int t) {iFieldType=t;G4cout<<"Set field type to: "<<t<<G4endl;};
  	inline void SetLithiumFocalLength(G4double t) {dFocalLength=t;G4cout<<"Set lithium focal length to: "<<t<<" m"<<G4endl;};
  	inline void SetMagneticRigidity(G4double t) {dMagneticRigidity=t;G4cout<<"Set Magnetic Rigidity to: "<<t<<" T*m"<<G4endl;};
  	inline void SetLithiumCurrent(G4double t) {dCurrentI=t;G4cout<<"Set Lithium current to: "<<t<<" A"<<G4endl;};

private:
  	void GetFieldValueAMD(const G4double Point[3], G4double *Bfield) const;
  	void GetFieldValueQWTFermi(const G4double Point[3], G4double *Bfield) const;
  	void GetFieldValueQWTNegativeSqr(const G4double Point[3], G4double *Bfield) const;
  	void GetFieldValueQWTAbrupt(const G4double Point[3], G4double *Bfield) const;
  	void GetFieldValueLithium(const G4double Point[3], G4double *Bfield) const;

private:
  	G4double 	dB0;
  	G4double 	dB1;
  	G4double 	dAmdAlpha;
  	G4double 	dQwtNegaSqrAlpha;
  	G4double 	dQwtFermiCoef0;
  	G4double 	dQwtFermiCoef1;
  	G4double 	dQwtFermiAlpha;
  	G4double 	dCaptureLithiumR;
  	G4double 	dCaptureLithiumL;
  	G4int		iFieldType;
private:
  	G4double 	dTargetL;
  	G4double 	dCaptureL;
  	G4double 	dCaptureR;
   	G4double 	dSqrCapR;
	G4double 	dHalfTarL;
	G4double 	dHalfCapL;

private:
  	G4double 	dMu0;
  	G4double 	dCurrentI;
  	G4double 	dMagneticRigidity;
  	G4double 	dFocalLength;
};

//Field manager
class GPCaptureFieldManager : public G4FieldManager
{

public:

  GPCaptureFieldManager() ;               //  A zero field
 ~GPCaptureFieldManager() ;
      
  void Init();
  inline void SetStepperType( G4int i) { iStepperType = i ;G4cout<<"Set capture field stepper type: "<<i<<G4endl; };
  inline void SetMinStep(G4double s) { dMinStep = s ;G4cout<<"Set  capture field minmum step: "<<s<<" m"<<G4endl; };
  void UpdateField();
  void SetFieldFlag(G4bool) ;

protected:
  void SetStepper();

protected:
  //G4PropagatorInField*    	propInField;
  GPCaptureField*          	fCaptureField ; 
  G4ChordFinder*         	fCaptureChordFinder ;
  G4EqEMFieldWithSpin*      fCaptureEquation; 
  G4MagIntegratorStepper*	fCaptureStepper ;
  G4MagInt_Driver*			fCaptureIntegratorDriver;

  G4int                  	iStepperType ;
  G4double               	dMinStep ;

  GPCaptureFieldMessenger*      	fFieldMessenger;
  G4bool					bCaptureFieldFlag;

};

#endif
