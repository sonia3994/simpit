// $Id: GPFieldManagerPool.hh,v 1.2 2006/06/29 17:18:51 gunter Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
//  A class for setting up the Magnetic Field of the setup, and 
//   creating the necessary classes to control accuracy of propagation.
//  In this example
//    - There is a global field for most of the setup;
//    - A local  field overides it for some volume(s) and it assumed to be uniform.
// 

#ifndef GPFIELDMANAGERPOOL_H
#define GPFIELDMANAGERPOOL_H 1

#include "GPObject.hh"
#include "G4ThreeVector.hh"
#include <string>


class GPField;
class G4ChordFinder;
class G4EqEMFieldWithSpin; 
class G4MagIntegratorStepper;
class G4MagInt_Driver;
class G4FieldManager;

class GPFieldManagerPool : public GPObject
{

public:

  GPFieldManagerPool(std::string sName, std::string sFatherName);
 ~GPFieldManagerPool() ;
      
  virtual void Update();
  virtual G4FieldManager* GetFieldManager();
  virtual void Init();

protected:
  GPField* pGPField;
  G4FieldManager* pFieldManager;
  G4ChordFinder*         	pChordFinder ;
  G4EqEMFieldWithSpin*      pEquation; 
  G4MagIntegratorStepper*	pIntegratorStepper ;
  G4MagInt_Driver*			pIntDriver;
  G4int                  	iStepperType ;
  G4double               	dMinStep ;

};

#endif
