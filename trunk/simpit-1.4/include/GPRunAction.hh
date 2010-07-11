// $Id: GPRunAction.hh,v 1.11 2007/07/02 13:22:08 vnivanch Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef GPRunAction_h
#define GPRunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"
#include <fstream>
#include <string>
#include <utility>
#include <vector>
#include <map>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4Run;
class GPPrimaryGeneratorAction;
class GPDetectorConstruction;  
class GPRunAction : public G4UserRunAction
{
public:
//  GPRunAction();
  GPRunAction(GPPrimaryGeneratorAction*,GPDetectorConstruction*);
  virtual ~GPRunAction();

  void  BeginOfRunAction(const G4Run*);
  void  EndOfRunAction(const G4Run*);
    
  void OutPutData(std::string,std::vector<G4double>); 
  void FillPerEvent(G4double,G4double,G4int); 
  void AddEddHit(G4int, G4int, G4int, G4double);
  void AddActualG(G4int t){iActualG+=t;};

private:
  void OutPut(std::map<std::string, std::ofstream* >::iterator,std::vector<G4double>); 

private:
  G4String sFilePath;
  G4double dSumETar, dSum2ETar;
  G4double dSumLTrack, dSum2LTrack;
  G4int  iPositronPerRun;
  G4int  iActualG;
  G4bool bTargetSDFlag; 

  std::ofstream* ofsDataFileDT;
  std::ofstream* ofsDataFileDC;
  std::ofstream* ofsDataFileAC;

  std::ofstream ofsParaFile;
  std::ofstream ofsEddHandle;

  std::vector<G4double > vecDouEdd;
  std::vector<G4int> vecIntEddDim;
  std::map<std::string, std::ofstream* > mapStrOfsOutputHandler;

  GPPrimaryGeneratorAction* primaryGenerator;
  GPDetectorConstruction*   mydetector;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

