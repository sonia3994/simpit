//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
// $Id: GPAcceleratorFieldMessenger.hh,v 1.4 2006/06/29 17:18:49 gunter Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#ifndef GPAcceleratorFieldMessenger_h
#define GPAcceleratorFieldMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class GPAcceleratorFieldManager;
class GPAcceleratorField;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADouble;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;
class G4UIcmdWithABool;



class GPAcceleratorFieldMessenger: public G4UImessenger
{
  public:
    GPAcceleratorFieldMessenger(GPAcceleratorFieldManager* );
   ~GPAcceleratorFieldMessenger();
    
    void SetNewValue(G4UIcommand*, G4String);
    void SetNewValue(G4UIcommand*, G4int);
    void SetFieldPoint(GPAcceleratorField* t){fieldPoint=t;};
    
  private:

    GPAcceleratorFieldManager*             	 fEMfieldManager;
    GPAcceleratorField*           	 fieldPoint;
    
    G4UIdirectory*            	 GPdetDir;

    G4UIcmdWithAnInteger*     	 StepperCmd;
    G4UIcmdWithAnInteger*     	 FieldType;
    G4UIcmdWithADoubleAndUnit*	 MagFieldB0Cmd;
    G4UIcmdWithADoubleAndUnit*	 EleFieldE0Cmd;
    G4UIcmdWithADoubleAndUnit*	 MinStepCmd;
    G4UIcmdWithoutParameter*  	 UpdateCmd;

    G4UIcmdWithABool*         	 FieldFlag;


};

#endif

