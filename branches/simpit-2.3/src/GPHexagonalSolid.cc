// $Id: GPHexagonalSolid.cc,v 1.9 2006/06/29 17:47:19 gunter Granular $
// GEANT4 tag $Name: geant4-09-02 $
//

#include "GPHexagonalSolid.hh"
#include "GPSolidManager.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVReplica.hh"
#include "G4VisAttributes.hh"
#include "G4Orb.hh"
#include "G4Box.hh"
#include "G4UIcommand.hh"

#include "globals.hh"

#include <sstream>
#include <algorithm>
GPHexagonalSolid::GPHexagonalSolid(std::string sName, std::string sFatherName)
  :pSolid(0),pLogicalVolume(0),pPhysicalVolume(0)
{
  SetActive(1);
  SetName(sName);
  SetFatherName(sFatherName);
  sBaseName = sName;
  replace(sBaseName.begin(),sBaseName.end(),'/','_');

  pMaterial = G4NistManager::Instance()->FindOrBuildMaterial("G4_W");
  pMaterialSpace = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");

  dSphereRadius=0.02;
  iCellNumberX = 5;
  iCellNumberY = 5;
  iCellNumberZ = 5;
  Init();

}

GPHexagonalSolid::~GPHexagonalSolid()
{
}

void GPHexagonalSolid::Construct(G4LogicalVolume* pMotherLog,G4ThreeVector vPoint)
{

  vPosition = vPoint;
  pSolid = new G4Box(sBaseName+"solid",
	m*dGlobalSolidX/2,
	m*dGlobalSolidY/2,
	m*dGlobalSolidZ/2);

  pLogicalVolume = new G4LogicalVolume(pSolid,pMaterial,sBaseName+"logicalVolume");
  pPhysicalVolume = new G4PVPlacement(0,
             vPosition*m,
             pLogicalVolume,sBaseName+"physicalVolume",pMotherLog,false,0);
  /*
  dGlobalSolidX = pSolidManager->GetParameter("width","width");
  dGlobalSolidY = pSolidManager->GetParameter("height","height");
  dGlobalSolidZ = pSolidManager->GetParameter("length","length");
  */

  GranularHexagonal(pLogicalVolume);
  //GranularHexagonal(pMotherLog);
}

void GPHexagonalSolid::Update()
{
  Init();
}
void GPHexagonalSolid::Init()
{
  /*dSphereRadius = dGlobalSolidZ/(2+(iCellNumberZ-0.5)*4*sqrt(6)/3);
  dCellWidthX = 4*dSphereRadius;
  dCellWidthY = 2*sqrt(3.0)*dSphereRadius;
  dCellWidthZ = 4*sqrt(6.0)*dSphereRadius/3;

  iCellNumberX = (dGlobalSolidX-2*dSphereRadius+0.5*dCellWidthX)/dCellWidthX;
  dGlobalSolidX = iCellNumberX*dCellWidthX+2*dSphereRadius-0.5*dCellWidthX;
  iCellNumberY = iCellNumberX;
  dGlobalSolidY = iCellNumberY*dCellWidthY+2*dSphereRadius-0.5*dCellWidthY;
  */
  dCellWidthX = 4*dSphereRadius;
  dCellWidthY = 2*sqrt(3.0)*dSphereRadius;
  dCellWidthZ = 4*sqrt(6.0)*dSphereRadius/3;

  dGlobalSolidX = iCellNumberX*dCellWidthX+4*dSphereRadius-0.5*dCellWidthX;
  dGlobalSolidY = iCellNumberY*dCellWidthY+2*dSphereRadius-0.5*dCellWidthY;
  dGlobalSolidZ = iCellNumberZ*dCellWidthZ+2*dSphereRadius-0.5*dCellWidthZ;
}

void GPHexagonalSolid::Print()
{
  G4cout
    <<"\n[Begin Hexagonal Solid: "+GetName()+"]"
    <<"\nRadius of sphere: "<<dSphereRadius*m/mm<<" mm"
    <<"\nX axis Cell width: "<<dCellWidthX*m/mm<< " mm"
    <<"\nY axis Cell width: "<<dCellWidthY*m/mm<< " mm"
    <<"\nZ axis Cell width: "<<dCellWidthZ*m/mm<< " mm"
    <<"\nNumber of X cell: "<<iCellNumberX
    <<"\nNumber of Y cell: "<<iCellNumberY
    <<"\nNumber of Z cell: "<<iCellNumberZ
    <<"\n[End Hexagonal Solid: "+GetName()+"]"
    <<G4endl;
}

void GPHexagonalSolid::GranularHexagonal(G4LogicalVolume* pMotherLog)
{

  vsPhysicalVector.clear();
  for(G4int k=0;k<iCellNumberZ;k++)
  {
    for(G4int j=0;j<iCellNumberY;j++)
    {
      for(G4int i=0;i<iCellNumberX;i++)
      {
	G4ThreeVector vCenter
	  =G4ThreeVector(i*dCellWidthX+0.25*dCellWidthX+2*dSphereRadius-0.5*dGlobalSolidX,
	      j*dCellWidthY+0.25*dCellWidthY+dSphereRadius-0.5*dGlobalSolidY,
	      k*dCellWidthZ+0.25*dCellWidthZ+dSphereRadius-0.5*dGlobalSolidZ);
	GranularHexagonalCell(pMotherLog,
	    vCenter,
	    i,j,k);
      }
    }

  }

}

void GPHexagonalSolid::GranularHexagonalCell(G4LogicalVolume* pMotherLog,
    G4ThreeVector vCenter,
    int iX,int iY,int iZ)
{
  G4ThreeVector vSphPoint;
  std::stringstream ssInCell;
  std::string sSphSuffix;
  std::string sSphSolid;
  std::string sSphLog;
  std::string sSphPhy;
  std::string sX;
  std::string sY;
  std::string sZ;

  ssInCell.clear();
  ssInCell.width(6);
  ssInCell.fill('0');
  ssInCell<<iX;
  ssInCell>>sX;
  ssInCell.clear();
  ssInCell.width(6);
  ssInCell.fill('0');
  ssInCell<<iY;
  ssInCell>>sY;
  ssInCell.clear();
  ssInCell.width(6);
  ssInCell.fill('0');
  ssInCell<<iZ;
  ssInCell>>sZ;

  sSphSuffix = sX+"-"+sY+"-"+sZ;
  sSphSolid=sSphSuffix+"-1-"+"sphOrbA"+sBaseName;
  sSphLog=sSphSuffix+"-1-"+"sphLogA"+sBaseName;
  sSphPhy=sSphSuffix+"-1-"+"sphPhyA"+sBaseName;
  vSphPoint= G4ThreeVector(-2*dSphereRadius*m,-2*sqrt(3.0)*dSphereRadius/3*m,-sqrt(6.0)*dSphereRadius/3*m);
  vSphPoint=vSphPoint+vCenter*m;
  G4Orb* sphOrbA = new G4Orb(sSphSolid,dSphereRadius*m);
  G4LogicalVolume* sphLogA = new G4LogicalVolume(sphOrbA,
      pMaterial,
      sSphLog,
      0,0,0);
  G4VPhysicalVolume* sphPhyA = new G4PVPlacement(0,
      vSphPoint,
      sphLogA,
      sSphPhy,
      pMotherLog,
      false,
      0);

  sSphSolid=sSphSuffix+"-2-"+"sphOrbB"+sBaseName;
  sSphLog=sSphSuffix+"-2-"+"sphLogB"+sBaseName;
  sSphPhy=sSphSuffix+"-2-"+"sphPhyB"+sBaseName;
  vSphPoint= G4ThreeVector(0,-2.0*sqrt(3.0)*dSphereRadius/3*m,-sqrt(6.0)*dSphereRadius/3*m); 
  vSphPoint=vSphPoint+vCenter*m;
  G4Orb* sphOrbB = new G4Orb(sSphSolid,dSphereRadius*m);
  G4LogicalVolume* sphLogB = new G4LogicalVolume(sphOrbB,
      pMaterial,
      sSphLog,
      0,0,0);
  G4VPhysicalVolume* sphPhyB = new G4PVPlacement(0,
      vSphPoint,
      sphLogB,
      sSphPhy,
      pMotherLog,
      false,
      0);

  sSphSolid=sSphSuffix+"-3-"+"sphOrbC"+sBaseName;
  sSphLog=sSphSuffix+"-3-"+"sphLogC"+sBaseName;
  sSphPhy=sSphSuffix+"-3-"+"sphPhyC"+sBaseName;
  vSphPoint= G4ThreeVector(dSphereRadius*m,-sqrt(3.0)*dSphereRadius/3*m,sqrt(6.0)*dSphereRadius/3*m); 
  vSphPoint=vSphPoint+vCenter*m;
  G4Orb* sphOrbC = new G4Orb(sSphSolid,dSphereRadius*m);
  G4LogicalVolume* sphLogC = new G4LogicalVolume(sphOrbC,
      pMaterial,
      sSphLog,
      0,0,0);
  G4VPhysicalVolume* sphPhyC = new G4PVPlacement(0,
      vSphPoint,
      sphLogC,
      sSphPhy,
      pMotherLog,
      false,
      0);

  sSphSolid=sSphSuffix+"-4-"+"sphOrbD"+sBaseName;
  sSphLog=sSphSuffix+"-4-"+"sphLogD"+sBaseName;
  sSphPhy=sSphSuffix+"-4-"+"sphPhyD"+sBaseName;
  vSphPoint= G4ThreeVector(-dSphereRadius*m,-sqrt(3.0)*dSphereRadius/3*m,sqrt(6.0)*dSphereRadius/3*m); 
  vSphPoint=vSphPoint+vCenter*m;
  G4Orb* sphOrbD = new G4Orb(sSphSolid,dSphereRadius*m);
  G4LogicalVolume* sphLogD = new G4LogicalVolume(sphOrbD,
      pMaterial,
      sSphLog,
      0,0,0);
  G4VPhysicalVolume* sphPhyD = new G4PVPlacement(0,
      vSphPoint,
      sphLogD,
      sSphPhy,
      pMotherLog,
      false,
      0);

  sSphSolid=sSphSuffix+"-5-"+"sphOrbE"+sBaseName;
  sSphLog=sSphSuffix+"-5-"+"sphLogE"+sBaseName;
  sSphPhy=sSphSuffix+"-5-"+"sphPhyE"+sBaseName;
  vSphPoint= G4ThreeVector(-dSphereRadius*m,sqrt(3.0)*dSphereRadius/3*m,-sqrt(6.0)*dSphereRadius/3*m); 
  vSphPoint=vSphPoint+vCenter*m;
  G4Orb* sphOrbE = new G4Orb(sSphSolid,dSphereRadius*m);
  G4LogicalVolume* sphLogE = new G4LogicalVolume(sphOrbE,
      pMaterial,
      sSphLog,
      0,0,0);
  G4VPhysicalVolume* sphPhyE = new G4PVPlacement(0,
      vSphPoint,
      sphLogE,
      sSphPhy,
      pMotherLog,
      false,
      0);

  sSphSolid=sSphSuffix+"-6-"+"sphOrbF"+sBaseName;
  sSphLog=sSphSuffix+"-6-"+"sphLogF"+sBaseName;
  sSphPhy=sSphSuffix+"-6-"+"sphPhyF"+sBaseName;
  vSphPoint= G4ThreeVector(dSphereRadius*m,sqrt(3.0)*dSphereRadius*m/3,-sqrt(6.0)*dSphereRadius*m/3); 
  vSphPoint=vSphPoint+vCenter*m;
  G4Orb* sphOrbF = new G4Orb(sSphSolid,dSphereRadius*m);
  G4LogicalVolume* sphLogF = new G4LogicalVolume(sphOrbF,
      pMaterial,
      sSphLog,
      0,0,0);
  G4VPhysicalVolume* sphPhyF = new G4PVPlacement(0,
      vSphPoint,
      sphLogF,
      sSphPhy,
      pMotherLog,
      false,
      0);

  sSphSolid=sSphSuffix+"-7-"+"sphOrbG"+sBaseName;
  sSphLog=sSphSuffix+"-7-"+"sphLogG"+sBaseName;
  sSphPhy=sSphSuffix+"-7-"+"sphPhyG"+sBaseName;
  vSphPoint= G4ThreeVector(2*dSphereRadius*m,2*sqrt(3.0)*dSphereRadius*m/3,sqrt(6.0)*dSphereRadius*m/3); 
  vSphPoint=vSphPoint+vCenter*m;
  G4Orb* sphOrbG = new G4Orb(sSphSolid,dSphereRadius*m);
  G4LogicalVolume* sphLogG = new G4LogicalVolume(sphOrbG,
      pMaterial,
      sSphLog,
      0,0,0);
  G4VPhysicalVolume* sphPhyG = new G4PVPlacement(0,
      vSphPoint,
      sphLogG,
      sSphPhy,
      pMotherLog,
      false,
      0);

  sSphSolid=sSphSuffix+"-8-"+"sphOrbH"+sBaseName;
  sSphLog=sSphSuffix+"-8-"+"sphLogH"+sBaseName;
  sSphPhy=sSphSuffix+"-8-"+"sphPhyH"+sBaseName;
  vSphPoint= G4ThreeVector(0,2*sqrt(3.0)*dSphereRadius*m/3,sqrt(6.0)*dSphereRadius*m/3); 
  vSphPoint=vSphPoint+vCenter*m;
  G4Orb* sphOrbH = new G4Orb(sSphSolid,dSphereRadius*m);
  G4LogicalVolume* sphLogH = new G4LogicalVolume(sphOrbH,
      pMaterial,
      sSphLog,
      0,0,0);
  G4VPhysicalVolume* sphPhyH = new G4PVPlacement(0,
      vSphPoint,
      sphLogH,
      sSphPhy,
      pMotherLog,
      false,
      0);


  G4VisAttributes* sphereLogVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0,0.3));
  sphereLogVisAtt->SetVisibility(true);
  sphereLogVisAtt->SetForceSolid(true);
  sphLogA->SetVisAttributes(sphereLogVisAtt);
  sphLogB->SetVisAttributes(sphereLogVisAtt);
  sphLogC->SetVisAttributes(sphereLogVisAtt);
  sphLogD->SetVisAttributes(sphereLogVisAtt);
  sphLogE->SetVisAttributes(sphereLogVisAtt);
  sphLogF->SetVisAttributes(sphereLogVisAtt);
  sphLogG->SetVisAttributes(sphereLogVisAtt);
  sphLogH->SetVisAttributes(sphereLogVisAtt);
  vsPhysicalVector.push_back(sphPhyA);
  vsPhysicalVector.push_back(sphPhyB);
  vsPhysicalVector.push_back(sphPhyC);
  vsPhysicalVector.push_back(sphPhyD);
  vsPhysicalVector.push_back(sphPhyE);
  vsPhysicalVector.push_back(sphPhyF);
  vsPhysicalVector.push_back(sphPhyG);
  vsPhysicalVector.push_back(sphPhyH);


}

void GPHexagonalSolid::SetParameter(std::string str,std::string strGlobal)
{
  std::stringstream ss(str);
  std::string		  sUnit;
  std::string		  sKey;
  std::string		  sValueOrg;
  G4double   		  dValueNew;
  G4double   		  dValueOrg;

  ss>>sKey>>sValueOrg>>sUnit;
  ss.clear();
  ss.str(sValueOrg);
  ss>>dValueOrg;
  if(sUnit!="")
    dValueNew=(dValueOrg*G4UIcommand::ValueOf(sUnit.c_str()))/m;
  else dValueNew=dValueOrg;

  if(sKey=="sphere.radius")
    dSphereRadius = dValueNew;
  else if(sKey=="cell.number.z")
    iCellNumberZ = dValueNew;
  else if(sKey=="cell.number.y")
    iCellNumberY = dValueNew;
  else if(sKey=="cell.number.x")
    iCellNumberX = dValueNew;
  else if(sKey=="material")
  {
    SetMaterial(sValueOrg);
    return;
  }


  else 
  {
    std::cout<<GetName()+": "+sKey+": Key does not exist."<<std::endl;
    return;
  }

  //Init();
  std::cout<<GetName()<<": Set "<<sKey<<": "<< sValueOrg<<" "<<sUnit<<std::endl;
}

G4double GPHexagonalSolid::GetParameter(std::string sKey,std::string sGlobal) const
{
  if(sKey=="cell.number.z")
    return iCellNumberZ;
  else if(sKey=="cell.number.y")
    return iCellNumberY;
  else if(sKey=="cell.number.x")
    return iCellNumberX;
  else if(sKey=="length")
    return dGlobalSolidZ;
  else if(sKey=="height")
    return dGlobalSolidY;
  else if(sKey=="width")
    return dGlobalSolidX;

  else
  {
    std::cout<<GetName()+": "+sKey+": Key does not exist."<<std::endl;
    return -1;
  }
}


void GPHexagonalSolid::Print(std::ofstream& fstOutput)
{
  fstOutput
    <<"\n[Begin Hexagonal Solid: "+GetName()+"]"
    <<"\nRadius of sphere, "<<dSphereRadius*m/mm<<" mm"
    <<"\nX axis Cell width: "<<dCellWidthX*m/mm<< " mm"
    <<"\nY axis Cell width: "<<dCellWidthY*m/mm<< " mm"
    <<"\nZ axis Cell width, "<<dCellWidthZ*m/mm<< " mm"
    <<"\nNumber of X cell, "<<iCellNumberX
    <<"\nNumber of Y cell, "<<iCellNumberY
    <<"\nNumber of Z cell, "<<iCellNumberZ
    <<"\n[End Hexagonal Solid: "+GetName()+"]"
    <<G4endl;
}
void GPHexagonalSolid::SetMaterial(std::string sValue)
{
  pMaterial = G4NistManager::Instance()->FindOrBuildMaterial(sValue);
  if(pMaterial)
  {
    sMaterial=sValue;
    std::cout<<GetName()+": Set Material: "+sMaterial<<std::endl;
  }
  else
  {
    std::cout<<"This Material does not exist: "<<sValue<<std::endl;
  }

}
