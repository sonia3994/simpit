// $Id: GPAcceleratorFieldManagerPool.hh,v 1.2 2006/06/29 17:18:51 gunter Exp $
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
//#include "G4ElectroMagneticField.hh"
#include "GPFieldManagerPool.hh"
#include "GPField.hh"
#include "globals.hh"
//#include "G4UniformMagField.hh"
//#include <string>

class G4ChordFinder;
class G4Mag_UsualEqRhs;
class G4EqEMFieldWithSpin;
class G4MagIntegratorStepper;
class G4MagInt_Driver;


//class GPAcceleratorField : public G4ElectroMagneticField
class GPAcceleratorField : public GPField
{
  public:
    GPAcceleratorField();
    ~GPAcceleratorField();
    void Init();
    void Print(std::ofstream&);
    void Print();
    void SetParameter(std::string, std::string);
    double GetParameter(std::string, std::string)const;
    void GetFieldValue(const G4double Point[3], G4double *Bfield) const;
    G4bool	DoesFieldChangeEnergy() const {return true;};

  protected:
    G4double 		dB0;
    G4double 		dB1;
    G4double 		dE0;
    G4double 		dE1;
    G4int		iFieldType;

};

class GPAcceleratorFieldManagerPool : public GPFieldManagerPool
{

  public:

    GPAcceleratorFieldManagerPool(std::string, std::string) ;
    ~GPAcceleratorFieldManagerPool() ;

    void Init();
    void Print(std::ofstream&);
    void Print();
    void SetParameter(std::string, std::string);
    double GetParameter(std::string, std::string) const;
    void Update();

  protected:
    void SetStepper();

  protected:
    //G4PropagatorInField*    	propInField;

};

#endif
