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
#include "G4UniformMagField.hh"
//#include <fstream>
class G4FieldManager;
class G4ChordFinder;
class G4Mag_UsualEqRhs;
class G4MagIntegratorStepper;
class GPFieldMessenger;
//class GPDetectorConstruction;

class GPMagneticField : public G4MagneticField
{
public:
  	GPMagneticField();
  	~GPMagneticField();
  	void SetFieldValueB0(G4double      fieldValue){B0=fieldValue;} ;
  	void SetFieldValueAlpha(G4double      fieldValue) {alpha=fieldValue;};
  	void GetFieldValue(const G4double Point[3], G4double *Bfield) const;
  	void WriteToFile(double x, double y, double z);

private:
  	void GetFieldValueAMD(const G4double Point[3], G4double *Bfield) const;
  	void GetFieldValueQWT(const G4double Point[3], G4double *Bfield) const;
	void GetDetectorParameter(); 

private:
  	G4double 	B0;
  	G4double 	alpha;
	G4double	highQL;
	G4double	lowQL;
  	G4int		fieldType;
private:
  	//G4double 	tarL;
  	//G4double 	capL;
  	//G4double 	capR;
  //std::ofstream fs;
  //GPDetectorConstruction* detector;
};


class GPFieldSetup
{

public:

  GPFieldSetup() ;               //  A zero field
  GPFieldSetup(G4ThreeVector) ;  //  The value of the field
 ~GPFieldSetup() ;
      
  void SetStepperType( G4int i) { fStepperType = i ; }

  void SetStepper();

  void SetMinStep(G4double s) { fMinStep = s ; }

  void UpdateField();

  void SetCaptureFieldFlag(G4bool) ;
  void SetFieldValue(G4ThreeVector fieldVector) ;
  void SetFieldValue(G4double      fieldValue) ;
  void SetFieldValueB0(G4double      fieldValue) ;
  void SetFieldValueAlpha(G4double      fieldValue) ;
  G4ThreeVector GetConstantFieldValue();
  G4FieldManager*  GetLocalFieldManager() { return fLocalFieldManager ;}

protected:

  G4FieldManager*         GetGlobalFieldManager() ;
    // Returns the global Field Manager

  G4FieldManager*         fFieldManager ;
  G4FieldManager*         fLocalFieldManager ;

  G4ChordFinder*          fChordFinder ;
  G4ChordFinder*          fLocalChordFinder ;

  G4Mag_UsualEqRhs*       fEquation ; 
  G4Mag_UsualEqRhs*       fLocalEquation ; 

  G4MagneticField*        fMagneticField ; 
  GPMagneticField*        fLocalMagneticField ; 

  G4MagIntegratorStepper* fStepper ;
  G4MagIntegratorStepper* fLocalStepper ;
  G4int                   fStepperType ;
  G4double                fMinStep ;

  GPFieldMessenger*      fFieldMessenger;
  G4bool		 globalFieldFlag;
  G4bool		 captureFieldFlag;

};

#endif
