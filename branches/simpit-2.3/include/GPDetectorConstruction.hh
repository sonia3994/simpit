// GEANT4 tag $Name: geant4-09-02 $
//

#ifndef GPDetectorConstruction_H
#define GPDetectorConstruction_H 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class GPDetectorMessenger;
//class GPFieldSetup;
class GPTargetSD;
class GPTargetROGeometry;
class GPTargetROGeometryTubs;
class G4Box;
class G4Tubs;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class GPGranularHexagonGeometry;
class GPCaptureGeometry;
class GPAcceleratorGeometry;
class GPSweeperGeometry;
class GPCrystalGeometry;

class GPDetectorConstruction : public G4VUserDetectorConstruction
{
  public:

    GPDetectorConstruction();
    ~GPDetectorConstruction();
    G4VPhysicalVolume* Construct();
  public:
    const G4VPhysicalVolume* GetPhysicalVolume(std::string name) const;
    G4double GetParameter(std::string name) const;
    void SetParameter(std::string str);

  public:
    inline const G4VPhysicalVolume* GetVacuumPhysical() const        {return worldPhys;};
    inline const G4VPhysicalVolume* GetTargetPhysical()  const       {return targetPhys;};
    inline const G4VPhysicalVolume* GetCapturePhysical()  const       {return capturePhys;};
    inline const G4VPhysicalVolume* GetTransferPhysical()  const       {return sweeperPhys;};

    std::vector<G4int> GetEddDim();

    void SetWorldMaterial (G4String);
    void UpdateGeometry();
    void PrintDetectorParameters(); 
    void Print(std::ofstream&);

  private:
    G4VPhysicalVolume* ConstructPositronResource();
    void DefineMaterials();
    
  private:
    
    // Subsytem Objects
    //
    GPGranularHexagonGeometry* targetGeometry;
    GPAcceleratorGeometry* acceleratorGeometry;
    GPSweeperGeometry* sweeperGeometry;
    GPCrystalGeometry* crystalGeometry;
    GPCaptureGeometry* captureGeometry;

    G4VPhysicalVolume* targetPhys;
    G4VPhysicalVolume* capturePhys;
    G4VPhysicalVolume* acceleratorPhys;
    G4VPhysicalVolume* sweeperPhys;


    // world  set
    G4double dWorldX;
    G4double dWorldY;
    G4double dWorldZ;

    G4Material* worldMaterial;
    G4Box* worldBox;
    G4LogicalVolume* worldLog;
    G4VPhysicalVolume* worldPhys;

    GPDetectorMessenger* detectorMessenger;

    G4Material* Vacuum;
    G4Material* W;
};

#endif

