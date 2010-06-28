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
#define GPCaptureManagerField_H

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
  	inline void SetFieldValueB0(G4double      t){B0=t/tesla;G4cout<<"Set capture field B0 value to: "<<B0<<" tesla."<<G4endl;};
  	inline void SetFieldValueAlpha(G4double   t){amdAlpha=t;G4cout<<"Set field AMD alpha value to: "<<amdAlpha<<G4endl;};
  	inline void SetAMDFieldAlpha(G4double     t){amdAlpha=t;G4cout<<"Set field AMD alpha value to: "<<amdAlpha<<G4endl;} ;
  	inline void SetCaptureType(G4int t) {fieldType=t;G4cout<<"Set field type to: "<<t<<G4endl;};

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
  	G4int		fieldType;
private:
  	G4double 	tarL;
  	G4double 	capL;
  	G4double 	capR;
   	G4double 	sqrCapR;
	G4double 	halfTarL;
	G4double 	halfCapL;

private:
  	G4double 	mu0;
  	G4double 	currentI;
};

//Field manager
class GPCaptureFieldManager : public G4FieldManager
{

public:

  GPCaptureFieldManager() ;               //  A zero field
 ~GPCaptureFieldManager() ;
      
  void Init();
  void SetStepperType( G4int i) { fStepperType = i ; }
  void SetMinStep(G4double s) { fMinStep = s ; }
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

  G4int                  	fStepperType ;
  G4double               	fMinStep ;

  GPCaptureFieldMessenger*      	fFieldMessenger;
  G4bool					captureFieldFlag;

};

#endif
