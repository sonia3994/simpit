// $Id: GPMain.hh,v 1.11 2007/07/02 13:22:08 vnivanch Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef GPMain_H
#define GPMain_H 1
#include <string>
#include <vector>
#include <fstream>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class GPMainMessenger;
class GPMain 
{
  public:
  static GPMain* GetInstance();
  static void Delete();
  void Init(int ,char** );
  void Print();
  void Print(std::ofstream&);
  void Update();
  void SetParameter(std::string);
  double GetParameter(std::string);
  std::string GetValueInString(std::string);
  std::string GetArgv(size_t);
  std::string GetWorkDir();
  std::string GetDataDir();
  protected:
  GPMain();
  ~GPMain();
  protected:
  static GPMain* instance;
  std::string sProgramName;
  std::string sProgramVersion;
  std::vector<std::string> vsArgv;
  std::string sWorkDir;
  std::string sDataDir;
  GPMainMessenger* pMessenger;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

