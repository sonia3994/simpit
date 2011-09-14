// $Id: GPField.hh,v 1.2 2006/06/29 17:18:51 gunter Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
//  A class for setting up the Magnetic Field of the setup, and 
//   creating the necessary classes to control accuracy of propagation.
//  In this example
//    - There is a global field for most of the setup;
//    - A local  field overides it for some volume(s) and it assumed to be uniform.
// 

#ifndef GPField_H
#define GPField_H

#include "G4ElectroMagneticField.hh"
#include "G4ThreeVector.hh"
#include <string>

class GPField : public G4ElectroMagneticField
{
public:
  	GPField();
  	~GPField();
	virtual void Init();
	virtual void Print(std::ofstream&);
	virtual void Print();
	virtual void SetParameter(std::string, std::string);
	virtual double GetParameter(std::string, std::string) const;
	void SetGeometryName(std::string);

protected:
	std::string sGeometryName;
	int iVerbose;

};

#endif
