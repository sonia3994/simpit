// $Id: GPSteppingAction.cc,v 1.15 2006/06/29 17:49:13 gunter Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "GPSteppingAction.hh"
#include "GPSteppingMessenger.hh"
#include "GPSteppingHandleManager.hh"

#include "G4ThreeVector.hh"
#include "G4RunManager.hh"
#include "G4Step.hh"

//#include <fstream>
#include <sstream>
using namespace std;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GPSteppingAction::GPSteppingAction()
{ 
  steppingMessenger = new GPSteppingMessenger(this);
  verbose=0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GPSteppingAction::~GPSteppingAction()
{ 
  delete 	steppingMessenger;
  //delete	targetSteppingAction;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GPSteppingAction::Prepare()
{
	GPSteppingHandleManager::GetInstance()->Prepare();
  //targetSteppingAction->Prepare();
}

void GPSteppingAction::CleanUp()
{
	GPSteppingHandleManager::GetInstance()->CleanUp();
  //targetSteppingAction->CleanUp();
}
void GPSteppingAction::UserSteppingAction(const G4Step* aStep)
{
  GPSteppingHandleManager::GetInstance()->UserSteppingAction(aStep);
}

void GPSteppingAction::SetSelectedParticle(G4String tmpParticle)
{
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
