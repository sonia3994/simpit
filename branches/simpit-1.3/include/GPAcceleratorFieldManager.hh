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

#include "G4MagneticField.hh"
#include "G4FieldManager.hh"
#include "G4ElectroMagneticField.hh"
#include "G4UniformMagField.hh"
#include <string>

//class G4FieldManager;
class G4ChordFinder;
class G4Mag_UsualEqRhs;
class G4EqEMFieldWithSpin;
class G4MagIntegratorStepper;
class G4MagInt_Driver;
//class G4PropagatorInField;

//class GPFieldMessenger;
/*
class GPCaptureField : public G4ElectroMagneticField
{
public:
  	GPCaptureField();
  	~GPCaptureField();
	void Init();
	G4bool	DoesFieldChangeEnergy() const {return false;};
  	void SetFieldValueB0(G4double      t){B0=t;G4cout<<"Set field B0 value to: "<<B0<<G4endl;} ;
  	void SetFieldValueAlpha(G4double      t) {amdAlpha=t;G4cout<<"Set field AMD alpha value to: "<<amdAlpha<<G4endl;};
  	void SetAMDFieldAlpha(G4double      t){amdAlpha=t;G4cout<<"Set field AMD alpha value to: "<<amdAlpha<<G4endl;} ;
  	void GetFieldValue(const G4double Point[3], G4double *Bfield) const;
  	void SetCaptureType(G4int t) {fieldType=t;G4cout<<"Set field type to: "<<t<<G4endl;};

private:
  	void GetFieldValueAMD(const G4double Point[3], G4double *Bfield) const;
  	void GetFieldValueQWTFermi(const G4double Point[3], G4double *Bfield) const;
  	void GetFieldValueQWTNegativeSqr(const G4double Point[3], G4double *Bfield) const;
  	void GetFieldValueQWTAbrupt(const G4double Point[3], G4double *Bfield) const;
  	void GetFieldValueLithium(const G4double Point[3], G4double *Bfield) const;

private:
	G4ThreeVector	TransformToLocal(G4ThreeVector ) const;
private:
  	G4double 	B0;
  	G4double 	B1;
  	G4double 	amdAlpha;
  	G4double 	qwtAlpha;
	G4double	highQL;
	G4double	lowQL;
  	G4int		fieldType;
private:
  	G4double 	tarL;
  	G4double 	capL;
  	G4double 	capR;
   	G4double 	sqrCapR;
	G4double 	halfTarL;
	G4double 	halfCapL;
	G4double 	gapL;
	G4double 	relativeMagL;

private:
  	G4double 	mu0;
  	G4double 	currentI;
};

class GPAcceleratorField : public G4ElectroMagneticField
{
public:
  	GPAcceleratorField();
  	~GPAcceleratorField();
	void Init();
  	void GetFieldValue(const G4double Point[3], G4double *Bfield) const;
	G4bool	DoesFieldChangeEnergy() const {return true;};

private:
  	G4double 		B0;
  	G4double 		E0;
   	G4double 	 	tarL;
   	G4double 	 	capL;
   	G4double 	 	accL;

};
*/
class GPAcceleratorFieldManager public : G4FieldManager
{

public:

  GPAcceleratorFieldManager() ;               //  A zero field
  //GPAcceleratorFieldManager(G4ThreeVector) ;  //  The value of the field
 ~GPAcceleratorFieldManager() ;
      
  void Init();
  void SetStepperType( G4int i) { fStepperType = i ; }
  void SetMinStep(G4double s) { fMinStep = s ; }
  void UpdateField();
  //void SetCaptureFieldFlag(G4bool) ;
  void SetAcceleratorFieldFlag(G4bool) ;
  //G4ThreeVector GetConstantFieldValue();
  G4FieldManager*  GetLocalFieldManager(std::string name);

protected:
  void SetStepper();

protected:
  //G4PropagatorInField*    	propInField;

  //G4FieldManager*        	GetGlobalFieldManager() ;
  //G4FieldManager*        	fGlobalFieldManager ;
  //G4FieldManager*        	fCaptureFieldManager ;
  //G4FieldManager*        	fAcceleratorFieldManager ;

  //G4MagneticField*       	fGlobalMagnetic ; 
  //GPCaptureField*          	fCaptureField ; 
  GPAcceleratorField*       fAcceleratorField ; 

  //G4ChordFinder*         	fGlobalChordFinder ;
  //G4ChordFinder*         	fCaptureChordFinder ;
  G4ChordFinder*         	fAcceleratorChordFinder ;

  //G4Mag_UsualEqRhs*      	fGlobalEquation ; 
  //G4Mag_UsualEqRhs*      	fCaptureEquation ; 
  //G4EqEMFieldWithSpin*         fGlobalEquation; 
  //G4EqEMFieldWithSpin*         fCaptureEquation; 
  G4EqEMFieldWithSpin*      	fAcceleratorEquation ; 

  //G4MagIntegratorStepper*	fGlobalStepper ;
  //G4MagIntegratorStepper*	fCaptureStepper ;
  G4MagIntegratorStepper*	fAcceleratorStepper ;

  //G4MagInt_Driver*			fGlobalIntegratorDriver;
  //G4MagInt_Driver*			fCaptureIntegratorDriver;
  G4MagInt_Driver*			fAcceleratorIntegratorDriver;

  G4int                  	fStepperType ;
  G4double               	fMinStep ;

  //GPFieldMessenger*      	fFieldMessenger;
  //G4bool					globalFieldFlag;
  //G4bool					captureFieldFlag;
  G4bool					acceleratorFieldFlag;

};

#endif
