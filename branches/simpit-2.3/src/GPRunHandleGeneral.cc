// $Id: GPRunHandleGeneral.cc,v 1.19 2008/01/17 17:31:32 maire Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "GPRunHandleGeneral.hh"
#include "GPRunHandleStore.hh"
#include "G4Run.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GPRunHandleGeneral::GPRunHandleGeneral(std::string sName, std::string sFatherName)
{
  SetActive(1);
  SetName(sName);
  SetFatherName(sFatherName);
  GPRunHandleStore::GetInstance()->AddRunHandle(GetName(),this);
}


GPRunHandleGeneral::~GPRunHandleGeneral()
{
  GPRunHandleStore::GetInstance()->EraseItem(GetName());
}
void  GPRunHandleGeneral::BeginOfRunAction(const G4Run* run)
{
  iRunID=run->GetRunID();
}
void  GPRunHandleGeneral::EndOfRunAction(const G4Run* run)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
