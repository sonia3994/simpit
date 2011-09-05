// $Id: GPHEPEvtInterface.hh,v 1.8 2006/06/29 18:08:36 gunter Exp $
// GEANT4 tag $Name: geant4-09-02 $
//

#ifndef GPHEPEvtInterface_h
#define GPHEPEvtInterface_h 1

#include <fstream>
#include <vector>
#include <sstream>
#include "globals.hh"
#include "G4VPrimaryGenerator.hh"
#include "G4HEPEvtParticle.hh"
#include "Randomize.hh"
#include "G4UIcommand.hh"
//extern CLHEP::RanecuEngine ranecuEngine;
class G4Event;
//class GPDetectorConstruction;
// class description:
//
//  This is a concrete class of G4VPrimaryGenerator.
//  This class object reads an ASCII file which contains particles generated
// by a physics generator which supports /HEPEVT/ common block.
// 
//  The format of ASCII file must be equivalent to the follwing sample fortran
// code.
//***********************************************************
//      SUBROUTINE HEP2G4
//*
//* Output /HEPEVT/ event structure to GPHEPEvtInterface
//*
//***********************************************************
//      PARAMETER (NMXHEP=2000)
//      COMMON/HEPEVT/NEVHEP,NHEP,ISTHEP(NMXHEP),IDHEP(NMXHEP),
//     >JMOHEP(2,NMXHEP),JDAHEP(2,NMXHEP),PHEP(5,NMXHEP),vectVHEP(4,NMXHEP)
//      DOUBLE PRECISION PHEP,vectVHEP
//*
//      WRITE(6,*) NHEP
//      DO IHEP=1,NHEP
//       WRITE(6,10) 
//     >  ISTHEP(IHEP),IDHEP(IHEP),JDAHEP(1,IHEP),JDAHEP(2,IHEP),
//     >  PHEP(1,IHEP),PHEP(2,IHEP),PHEP(3,IHEP),PHEP(5,IHEP)
//10    FORMAT(4I5,4(1X,D15.8))
//      ENDDO
//*
//      RETURN
//      END
//
//  The position and time of the primary interaction must be set by the corresponding
// set methods of G4VPrimaryGenerator base class, otherwise zero will be set.
//

class GPHEPEvtInterface:public G4VPrimaryGenerator
{
  public: 
    GPHEPEvtInterface();
    //GPHEPEvtInterface(char* evfile);
    GPHEPEvtInterface(G4String evfile);
    ~GPHEPEvtInterface();

    void SetParameter(std::string, std::string);
    G4double GetParameter(std::string, std::string);
    void SetInputFile(G4String );

    inline void SetParticlePosZ(G4double t)
    {dParticlePosZ=t;G4cout<<"The particle Z axis is set to: "<<t<<" m"<<G4endl;};

    inline void SetEnergyUnit(G4String t)
    {dUnitE=G4UIcommand::ValueOf(t);G4cout<<"The energy unit of input file is set to: "<<t<<G4endl;};

    inline void SetMomentumUnit(G4String t)
    {dUnitP=G4UIcommand::ValueOf(t);G4cout<<"The momentum unit of input file is set to: "<<t<<G4endl;};
	
    inline void SetLengthUnit(G4String t)
    {dUnitL=G4UIcommand::ValueOf(t);G4cout<<"The length unit of input file is set to: "<<t<<G4endl;};

    inline void SetInputFileRMSFactor(G4double t)
    {dRadiusRMSFactor=t;G4cout<<"The input file's dimension rms factor is set to: "<<t<<" m"<<G4endl;};

    inline void SetBunchLength(G4double t)
    {dBunchLength=t;G4cout<<"The bunch length is set to: "<<t<<" picosecond"<<G4endl;};
  
  public:
    void GeneratePrimaryVertex(G4Event* evt);
    void Print(std::ofstream&);
    void Print();

  private:
    void Init();

  private:
    G4String	sFileName;
    G4double	dUnitE;
    G4double	dUnitP;
    G4double	dUnitL;
    G4double	dTimeUnit;
    G4double	dParticlePosZ;
    G4double	dBunchLength;
    G4double	dRadiusRMSFactor;
    G4bool		bRadiusRMSFactorFlag;
    G4ThreeVector* 	vectVHEP;
    G4double 	dPolX;
    G4double 	dPolY;
    G4double 	dPolZ;

    std::fstream 	fsInputFile;
    std::fstream 	fsOutputFile;
	
    std::vector<G4HEPEvtParticle*> 	vectHPlist;
    std::vector<G4ThreeVector* >    vectPosList;
    CLHEP::RandGauss* 				randGauss;

};

#endif

