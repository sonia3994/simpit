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
#include "G4ThreeVector.hh"
//#include "G4ElectroMagneticField.hh"
#include "GPFieldManagerPool.hh"
#include "GPField.hh"
//#include "G4UniformMagField.hh"
//#include <string>

class G4ChordFinder;
class G4Mag_UsualEqRhs;
class G4EqEMFieldWithSpin;
class G4MagIntegratorStepper;
class G4MagInt_Driver;


class GPCaptureField : public GPField
{
  public:
    GPCaptureField();
    ~GPCaptureField();
    void Init();
    void SetParameter(std::string, std::string);
    double GetParameter(std::string, std::string)const;
    void Print(std::ofstream&);
    void Print();
    G4bool	DoesFieldChangeEnergy() const {return false;};
    void GetFieldValue(const G4double Point[3], G4double *Bfield) const;

  private:
    void GetFieldValueAMD(G4ThreeVector, G4double *Bfield) const;
    void GetFieldValueQWTFermi(G4ThreeVector, G4double *Bfield) const;
    void GetFieldValueQWTNegativeSqr(G4ThreeVector, G4double *Bfield) const;
    void GetFieldValueQWTAbrupt(G4ThreeVector, G4double *Bfield) const;
    void GetFieldValueLithium(G4ThreeVector, G4double *Bfield) const;

  private:
    G4double 	dB0;
    G4double 	dB1;
    G4double 	dAmdAlpha;
    G4double 	dQwtNegaSqrAlpha;
    G4double 	dQwtFermiCoef0;
    G4double 	dQwtFermiCoef1;
    G4double 	dQwtFermiAlpha;
    G4int		iFieldType;
  private:
    G4double 	dSolidHalfLength;
    G4double 	dSolidRadius;
    G4double 	dSqrCapR;
    G4double 	dQwtExtend;

  private:
    G4double 	dMu0;
    G4double 	dCurrentI;
    G4double 	dMagneticRigidity;
    G4double 	dFocalLength;
};

//Field manager
class GPCaptureFieldManagerPool : public GPFieldManagerPool
{

  public:

    GPCaptureFieldManagerPool(std::string, std::string) ;               //  A zero field
    ~GPCaptureFieldManagerPool() ;

    void Init();
    void Print(std::ofstream&);
    void Print();
    void SetParameter(std::string, std::string);
    double GetParameter(std::string, std::string)const;
    void Update();

  protected:
    void SetStepper();

  protected:
};

#endif
