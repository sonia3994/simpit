// $Id: GPEventHandleStore.hh,v 1.12 2007/07/02 13:22:08 vnivanch Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef GPEVENTHANDLESTORE_H
#define GPEVENTHANDLESTORE_H 1
#include <map>
#include <string>

class GPEventHandle;
typedef std::map<std::string, GPEventHandle* > GPEventHandleMap;
class GPEventHandleStore 
{
  protected:
    GPEventHandleStore();
    ~GPEventHandleStore();
  public:
    static GPEventHandleStore* GetInstance();
    static void Delete();
    GPEventHandleMap*  GetEventHandleMap(){return &mStrEvtHangle;};
    GPEventHandle* FindEventHandle(std::string);
    void AddEventHandle(std::string, GPEventHandle*);
    void DelEventHandle(std::string);
    void EraseItem(std::string);
    void Clear();
    
  protected:
    static GPEventHandleStore* instance;
    GPEventHandleMap mStrEvtHangle;
    
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
