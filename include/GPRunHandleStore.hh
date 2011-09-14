// $Id: GPRunHandleStore.hh,v 1.11 2007/07/02 13:22:08 vnivanch Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef GPRUNHANDLESTORE_H
#define GPRUNHANDLESTORE_H 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include <map>
#include <string>
class GPRunHandle;
typedef std::map<std::string, GPRunHandle* > GPRunHandleMap;
class GPRunHandleStore
{
  protected:
    GPRunHandleStore();
    ~GPRunHandleStore();
  public:
    static GPRunHandleStore* GetInstance();
    static void Delete();
    GPRunHandleMap*  GetRunHandleMap(){return &mStrRunHandle;};
    GPRunHandle* FindRunHandle(std::string);
    void AddRunHandle(std::string, GPRunHandle*);
    void DelRunHandle(std::string);
    void EraseItem(std::string);
    void Clear();

  protected:
    static GPRunHandleStore* instance;
    GPRunHandleMap mStrRunHandle;
};

#endif
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
