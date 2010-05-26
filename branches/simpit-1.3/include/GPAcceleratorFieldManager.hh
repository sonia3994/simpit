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
  	void SetFieldValueB0(G4double t){B0=t;};
  	void SetFieldValueE0(G4double t){E0=t;};
  	void SetFieldType(G4int t){fieldType=t;};
	G4bool	DoesFieldChangeEnergy() const {return true;};

private:
  	G4double 		B0;
  	G4double 		E0;
   	G4double 	 	tarL;
   	G4double 	 	capL;
   	G4double 	 	accL;
	G4int			fieldType;

};

class GPAcceleratorFieldManager : public G4FieldManager
{

public:

  GPAcceleratorFieldManager() ;
 ~GPAcceleratorFieldManager() ;
      
  void Init();
  void SetStepperType( G4int i) { fStepperType = i ; }
  void SetMinStep(G4double s) { fMinStep = s ; }
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

  G4int                  	fStepperType ;
  G4double               	fMinStep ;

  GPAcceleratorFieldMessenger*      	fFieldMessenger;
  G4bool					acceleratorFieldFlag;

};

#endif
