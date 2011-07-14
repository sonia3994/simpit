// $Id: GPFieldManagerPool.cc,v 1.5 2007/04/28 01:31:12 gum Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
//  
//   Field Setup class implementation.
//

#include "GPFieldManagerPool.hh"
#include "GPField.hh"
#include "G4FieldManager.hh"

#include "G4EqEMFieldWithSpin.hh"
#include "G4MagIntegratorStepper.hh"
#include "G4MagIntegratorDriver.hh"
#include "G4ChordFinder.hh"
GPFieldManagerPool::GPFieldManagerPool(std::string sName, std::string sFatherName):
  pGPField(0),
  pChordFinder(0),
  pEquation(0), 
  pIntegratorStepper(0),
  pIntDriver(0)
{
  SetActive(1);
  SetName(sName);
  SetFatherName(sFatherName);

  pFieldManager = new G4FieldManager();
}
GPFieldManagerPool::~GPFieldManagerPool()
{
  if(pFieldManager)		delete pFieldManager; 
}

void GPFieldManagerPool::Update()
{
}
G4FieldManager*  GPFieldManagerPool::GetFieldManager()
{
  return pFieldManager;
}
void GPFieldManagerPool::Init()
{
  Update();
}
