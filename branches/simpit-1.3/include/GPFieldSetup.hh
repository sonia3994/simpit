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
//#include "G4ElectroMagneticField.hh"
//#include "G4UniformMagField.hh"
#include <string>

class G4FieldManager;
class GPCaptureFieldManager;
class GPAcceleratorFieldManager;

class G4ChordFinder;
class G4Mag_UsualEqRhs;
class G4EqEMFieldWithSpin;
class G4MagIntegratorStepper;
class G4MagInt_Driver;
class G4PropagatorInField;

class GPFieldMessenger;

class GPFieldSetup
{

public:

  GPFieldSetup() ;
 ~GPFieldSetup() ;
      
  void Init();
  void SetStepperType( G4int i) { fStepperType = i ; }
  void SetMinStep(G4double s) { fMinStep = s ; }
  void UpdateField();
  void SetGlobalFieldFlag(G4bool) ;
  G4FieldManager*  GetLocalFieldManager(std::string name);

protected:
  void SetStepper();

protected:
  G4PropagatorInField*    	propInField;

  G4FieldManager*        	GetGlobalFieldManager() ;
  G4FieldManager*        	fGlobalFieldManager ;
  GPCaptureFieldManager*  	fCaptureFieldManager ;
  GPAcceleratorFieldManager*	fAcceleratorFieldManager ;

  G4MagneticField*       	fGlobalMagnetic ; 
  G4ChordFinder*         	fGlobalChordFinder ;
  G4EqEMFieldWithSpin*      fGlobalEquation; 
  G4MagIntegratorStepper*	fGlobalStepper ;
  G4MagInt_Driver*			fGlobalIntegratorDriver;
  G4int                  	fStepperType ;
  G4double               	fMinStep ;
 
  GPFieldMessenger*      	fFieldMessenger;
  G4bool					globalFieldFlag;
};

#endif
