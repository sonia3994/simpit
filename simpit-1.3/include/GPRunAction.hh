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
  void AddActualG(G4int t){actualG+=t;};

private:
  void OutPut(std::map<std::string, std::ofstream* >::iterator,std::vector<G4double>); 

private:
  G4double sumETar, sum2ETar;
  G4double sumLTrack, sum2LTrack;
  G4int  positronPerRun;
  G4int  actualG;

  GPPrimaryGeneratorAction* primaryGenerator;
  GPDetectorConstruction*   mydetector;

  std::map<std::string, std::ofstream* > outputHandlerMap;
  std::ofstream* dataFileDT;
  std::ofstream* dataFileDC;
  std::ofstream* dataFileAC;

  std::ofstream paraFile;
  std::ofstream eddHandle;
  G4String filePath;

  std::vector<G4double > edd;
  std::vector<G4int> eddDim;
  G4bool targetSDFlag; 
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

