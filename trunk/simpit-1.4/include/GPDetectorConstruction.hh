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
class G4Box;
class G4Tubs;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;

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

  public:
    inline const G4VPhysicalVolume* GetVacuumPhysical() const        {return worldPhys;};
    inline const G4VPhysicalVolume* GetTargetPhysical()  const       {return targetPhys;};
    inline const G4VPhysicalVolume* GetCapturePhysical()  const       {return capturePhys;};
    inline const G4VPhysicalVolume* GetTransferPhysical()  const       {return tranPhys;};
    inline const G4LogicalVolume* 	GetCaptureLogical()		const		{return captureLog;};
    inline GPFieldSetup* GetFieldSetup	()					const		{return fieldSetup;};

    void SetTargetThickness(G4double);
    void SetTargetXY(G4double);
    void SetTargetCellXYZ(G4double x,G4double y,G4double z){ dTargetCellX=x; dTargetCellY=y; dTargetCellZ=z;};
    void SetCaptureLength(G4double t)	{dCaptureTubeLength=t;};
    void SetCaptureRadius(G4double t)	{dCaptureTubeOuterRadius=t;};
    void SetWorldSizeXYZ(G4double ,G4double , G4double );
    void SetWorldSizeX(G4double);
    void SetWorldSizeY(G4double);
    void SetWorldSizeZ(G4double);
    inline G4double GetTargetThickness()	const	{return dTargetBoxZ;};
    inline G4double GetTargetXY()		const	{return dTargetBoxX;};
    inline G4double GetTargetX()		const	{return dTargetBoxX;};
    inline G4double GetTargetY()		const	{return dTargetBoxY;};
    inline G4double GetTargetLength()	const	{return dTargetBoxZ;};
    inline G4double GetCaptureLength()		const	{return dCaptureTubeLength;};
    inline G4double GetCaptureRadius()		const	{return dCaptureTubeOuterRadius;};
    inline G4double GetWorldSizeX() 		const	{return dWorldX;}; 
    inline G4double GetWorldSizeY()          	const	{return dWorldY;};
    inline G4double GetWorldSizeZ()           	const	{return dWorldZ;};
    inline std::vector<G4int> GetEddDim()           	const	{return vectEddDim;};

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
    
  private:
    
    // target
    //
    G4double dTargetBoxY;
    G4double dTargetBoxX;
    G4double dTargetBoxZ;

    G4Material* targetMaterial;
    G4Box* targetBox;
    G4LogicalVolume* targetLog;
    G4VPhysicalVolume* targetPhys;

    //Capture
    //
    G4double dCaptureTubeInnerRadius;
    G4double dCaptureTubeOuterRadius;
    G4double dCaptureTubeLength;
    G4double dCaptureTubeStartAngle;
    G4double dCaptureTubeSpanningAngle;

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

    G4double dTargetCellX;
    G4double dTargetCellY;
    G4double dTargetCellZ;
    std::vector<G4int> vectEddDim;

    //material
    G4Material* Vacuum;
    G4Material* W;
};

#endif

