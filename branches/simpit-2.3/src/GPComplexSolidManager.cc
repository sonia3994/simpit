// $Id: GPComplexSolidManager.cc,v 1.19 2008/01/17 17:31:32 maire Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "GPComplexSolidManager.hh"
#include "GPComplexSolid.hh"
#include "GPHexagonalSolid.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GPComplexSolidManager* GPComplexSolidManager::instance=NULL;
GPComplexSolidManager* GPComplexSolidManager::GetInstance()
{
  if(instance==NULL) 
    instance = new GPComplexSolidManager();
  return instance;
}
void GPComplexSolidManager::Delete()
{
  if(instance)  delete instance;
}
GPComplexSolidManager::GPComplexSolidManager()
{
}


GPComplexSolidManager::~GPComplexSolidManager()
{
}
GPComplexSolid* GPComplexSolidManager::FindAndBuildComplexSolid(std::string sType,std::string sName,std::string sFatherName)
{
  GPComplexSolid* pComplexSolid=NULL;
  if(sType=="GPHexagonalSolid")
    pComplexSolid = new GPHexagonalSolid(sName,sFatherName);
      
  else if(sType=="GPHexagonalSolid")
    pComplexSolid = new GPComplexSolid(sName,sFatherName);
  
  else
  {
    std::cout<<"GPComplexSolidManager: "+sType+" does not exist."<<std::endl;
  }
  return pComplexSolid;
}
void GPComplexSolidManager::Print()
{
}
void GPComplexSolidManager::Print(std::ofstream& ofs)
{
}
void GPComplexSolidManager::SetParameter(std::string sGlobal)
{
}
double GPComplexSolidManager::GetParameter(std::string sGlobal)
{
  return 0;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
