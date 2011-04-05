//
// $Id: GPCrystalManager.hh,v 1.6 
// GEANT4 tag $Name: geant4-09-02 $
//

#ifndef GPCRYSTALMANAGER_H
#define GPCRYSTALMANAGER_H 1
#include "G4ThreeVector.hh"
#include <string>
#include <vector>

class Crystal;
class RunParameters;

//#include "G4VUserDetectorConstruction.hh"

class GPCrystalManager
{
  protected:
    GPCrystalManager();
    ~GPCrystalManager();

  public:

    static GPCrystalManager* GetInstance();
    static void Delete();
    void Clear();
    void SetCrystal(Crystal* );
    void SetRunPara(RunParameters* );
    Crystal* GetCrystal(){return crysW;};
    RunParameters* GetRunPara(){return runPara;};
    void SetParameter(std::string, std::string);
    G4double GetParameter(std::string) const;
    void Print();
    void Print(std::ofstream& );

  protected:
    static GPCrystalManager* instance;
  protected:
    
    Crystal*	crysW;
    RunParameters* runPara;


};

#endif

