// $Id: GPComplexSolidManager.hh,v 1.11 2007/07/02 13:22:08 vnivanch Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef GPCOMPLEXSOLIDMANAGER_H
#define GPCOMPLEXSOLIDMANAGER_H 1
#include <string>
#include <fstream>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
class GPComplexSolid;

class GPComplexSolidManager 
{
  public:
    static GPComplexSolidManager* GetInstance();
    static void Delete();
    GPComplexSolid* FindAndBuildComplexSolid(std::string,std::string ,std::string );
    void Print();
    void Print(std::ofstream&);
    void SetParameter(std::string);
    double GetParameter(std::string);
  protected:
    GPComplexSolidManager();
    ~GPComplexSolidManager();
  protected:
    static GPComplexSolidManager* instance;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

