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
    void SetTargetCellXYZ(G4double x,G4double y,G4double z){ TargetCellX=x; TargetCellY=y; TargetCellZ=z;};
    void SetCaptureLength(G4double t)	{captureTubeLength=t;};
    void SetCaptureRadius(G4double t)	{captureTubeOuterRadius=t;};
    void SetWorldSizeXYZ(G4double ,G4double , G4double );
    void SetWorldSizeX(G4double);
    void SetWorldSizeY(G4double);
    void SetWorldSizeZ(G4double);
    inline G4double GetTargetThickness()	const	{return targetBoxZ;};
    inline G4double GetTargetXY()		const	{return targetBoxX;};
    inline G4double GetTargetX()		const	{return targetBoxX;};
    inline G4double GetTargetY()		const	{return targetBoxY;};
    inline G4double GetTargetLength()	const	{return targetBoxZ;};
    inline G4double GetCaptureLength()		const	{return captureTubeLength;};
    inline G4double GetCaptureRadius()		const	{return captureTubeOuterRadius;};
    inline G4double GetWorldSizeX() 		const	{return worldX;}; 
    inline G4double GetWorldSizeY()          	const	{return worldY;};
    inline G4double GetWorldSizeZ()           	const	{return worldZ;};
    inline std::vector<G4int> GetEddDim()           	const	{return eddDim;};

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
    G4double targetBoxY;
    G4double targetBoxX;
    G4double targetBoxZ;

    G4Material* targetMaterial;
    G4Box* targetBox;
    G4LogicalVolume* targetLog;
    G4VPhysicalVolume* targetPhys;

    //Capture
    //
    G4double captureTubeInnerRadius;
    G4double captureTubeOuterRadius;
    G4double captureTubeLength;
    G4double captureTubeStartAngle;
    G4double captureTubeSpanningAngle;

    G4Material* captureMaterial;
    G4Tubs* captureTube;
    G4LogicalVolume* captureLog;
    G4VPhysicalVolume* capturePhys;

    // transport
    G4double tranTubeInnerRadius;
    G4double tranTubeOuterRadius;
    G4double tranTubeLength;
    G4double tranTubeStartAngle;
    G4double tranTubeSpanningAngle;

    G4Material* tranMaterial;
    G4Tubs* tranTube;
    G4LogicalVolume* tranLog;
    G4VPhysicalVolume* tranPhys;

    // accelerator
    G4double acceleratorTubeInnerRadius;
    G4double acceleratorTubeOuterRadius;
    G4double acceleratorTubeLength;
    G4double acceleratorTubeStartAngle;
    G4double acceleratorTubeSpanningAngle;

    G4Material* acceleratorMaterial;
    G4Tubs* acceleratorTube;
    G4LogicalVolume* acceleratorLog;
    G4VPhysicalVolume* acceleratorPhys;

    // world  set
    G4double worldX;
    G4double worldY;
    G4double worldZ;

    G4Material* worldMaterial;
    G4Box* worldBox;
    G4LogicalVolume* worldLog;
    G4VPhysicalVolume* worldPhys;


    GPFieldSetup* fieldSetup;
    GPDetectorMessenger* detectorMessenger;

    //sensetive detector
    GPTargetSD* targetSD;
    GPTargetROGeometry* targetRO;

    G4double TargetCellX;
    G4double TargetCellY;
    G4double TargetCellZ;
    std::vector<G4int> eddDim;

    //material
    G4Material* Vacuum;
    G4Material* W;
};

#endif

