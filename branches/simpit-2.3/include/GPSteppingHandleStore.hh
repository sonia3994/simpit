// $Id: GPSteppingHandleStore.hh,v 1.10 2007/07/02 13:22:08 vnivanch Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef GPSTEPPINGHANDLESTORE_H
#define GPSTEPPINGHANDLESTORE_H 1

#include <map>
#include <string>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class GPSteppingHandle;
typedef std::map<std::string, GPSteppingHandle* > GPSteppingHandleMap;
class GPSteppingHandleStore
{
  protected:
    GPSteppingHandleStore();
    ~GPSteppingHandleStore();
  public:
    static GPSteppingHandleStore* GetInstance();
    static void Delete();
    GPSteppingHandleMap*  GetSteppingHandleMap(){return &mStrSteppingHandle;};
    GPSteppingHandle* FindSteppingHandle(std::string);
    void AddSteppingHandle(std::string, GPSteppingHandle*);
    void DelSteppingHandle(std::string);
    void Clear();

  protected:
    static GPSteppingHandleStore* instance;
    GPSteppingHandleMap mStrSteppingHandle;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
