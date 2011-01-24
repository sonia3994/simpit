// $Id: GPDetectorConstruction.hh,v 1.6 2006/06/29 17:47:13 gunter Exp $
// GEANT4 tag $Name: geant4-09-02 $
//

#ifndef GPDetectorConstruction_H
#define GPDetectorConstruction_H 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class GPDetectorMessenger;
class GPFieldSetup;
class GPTargetSD;
class GPTargetROGeometry;
class GPTargetROGeometryTubs;
class G4Box;
class G4Tubs;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class GPTargetGeometry;

class GPDetectorConstruction : public G4VUserDetectorConstruction
{
  public:

    GPDetectorConstruction();
    ~GPDetectorConstruction();
    G4VPhysicalVolume* Construct();
  public:
    const G4VPhysicalVolume* GetPhysicalVolume(std::string name) const;
    G4double GetDetectorSize(std::string name) const;
    void SetDetectorSize(std::string str);
    void SetUserLimits(std::string str);

  public:
    inline const G4VPhysicalVolume* GetVacuumPhysical() const        {return worldPhys;};
    inline const G4VPhysicalVolume* GetTargetPhysical()  const       {return targetPhys;};
    inline const G4VPhysicalVolume* GetCapturePhysical()  const       {return capturePhys;};
    inline const G4VPhysicalVolume* GetTransferPhysical()  const       {return tranPhys;};
    inline const G4LogicalVolume* 	GetCaptureLogical()		const		{return captureLog;};
    inline GPFieldSetup* GetFieldSetup	()					const		{return fieldSetup;};

    void SetTargetCellXYZ(G4double x,G4double y,G4double z){ dTargetCellZ=x; dTargetCellR=y; dTargetCellPhi=z;};
    void SetCaptureType(G4int);
    
    inline std::vector<G4int> GetEddDim()           	const	{return vectEddDim;};
    inline G4int GetCaptureType()           	const	{return iCaptureType;};

    void SetTargetMaterial (G4String);
    void SetWorldMaterial (G4String);
    void SetCaptureMaterial (G4String);
    void SetAcceleratorMaterial (G4String);
    void SetTranTubeMaterial (G4String);

    void UpdateGeometry();
    void PrintDetectorParameters(); 

  private:
    G4VPhysicalVolume* ConstructPositronResource();
    void DefineMaterials();
    void SetLithiumLens(G4double dLength=1e-2,G4double dOuterRadius=1e-2,G4double dInnerRadius=0.0, G4double dStartAngle=0.0, G4double dSpanningAngle=360.0 );
    
    void ConstructTarget();
    void ConstructCapture();
    void ConstructAccelerator();
    void ConstructTranTubs();
  private:
    
    // target
    //
    GPTargetGeometry* targetGeometry;
    G4double dTargetTubeInnerRadius;
    G4double dTargetTubeOuterRadius;
    G4double dTargetTubeLength;
    G4double dTargetTubeStartAngle;
    G4double dTargetTubeSpanningAngle;
    G4double dTargetGranularRadius;
    G4int iTargetGranularFlag;
    G4int iTargetGranularZNumber;

    G4Material* targetMaterial;
    G4Tubs* targetTubs;
    G4LogicalVolume* targetLog;
    G4VPhysicalVolume* targetPhys;

    //Capture
    //
    G4double dCaptureTubeInnerRadius;
    G4double dCaptureTubeOuterRadius;
    G4double dCaptureTubeLength;
    G4double dCaptureTubeStartAngle;
    G4double dCaptureTubeSpanningAngle;
    G4double dCaptureStepMax;
    G4double dLithiumTubeLength;
    G4double dLithiumTubeOuterRadius;
    G4int iCaptureType;

    G4Material* captureMaterial;
    G4Tubs* captureTube;
    G4LogicalVolume* captureLog;
    G4VPhysicalVolume* capturePhys;

    // transport
    G4double dTranTubeInnerRadius;
    G4double dTranTubeOuterRadius;
    G4double dTranTubeLength;
    G4double dTranTubeStartAngle;
    G4double dTranTubeSpanningAngle;

    G4Material* tranMaterial;
    G4Tubs* tranTube;
    G4LogicalVolume* tranLog;
    G4VPhysicalVolume* tranPhys;

    // accelerator
    G4double dAcceleratorTubeInnerRadius;
    G4double dAcceleratorTubeOuterRadius;
    G4double dAcceleratorTubeLength;
    G4double dAcceleratorTubeStartAngle;
    G4double dAcceleratorTubeSpanningAngle;
    G4double dAcceleratorStepMax;

    G4Material* acceleratorMaterial;
    G4Tubs* acceleratorTube;
    G4LogicalVolume* acceleratorLog;
    G4VPhysicalVolume* acceleratorPhys;

    // world  set
    G4double dWorldX;
    G4double dWorldY;
    G4double dWorldZ;

    G4Material* worldMaterial;
    G4Box* worldBox;
    G4LogicalVolume* worldLog;
    G4VPhysicalVolume* worldPhys;


    GPFieldSetup* fieldSetup;
    GPDetectorMessenger* detectorMessenger;

    //sensetive detector
    GPTargetSD* targetSD;
    GPTargetROGeometry* targetRO;
    GPTargetROGeometryTubs* targetROTubs;

    G4double dTargetCellZ;
    G4double dTargetCellR;
    G4double dTargetCellPhi;
    std::vector<G4int> vectEddDim;

    //material
    G4Material* Vacuum;
    G4Material* W;
};

#endif

