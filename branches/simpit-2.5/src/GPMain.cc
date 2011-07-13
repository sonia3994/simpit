// $Id: GPMain.cc,v 1.19 2008/01/17 17:31:32 maire Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "GPMain.hh"
#include <sstream>
#include <iostream>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GPMain* GPMain::instance=NULL;
GPMain* GPMain::GetInstance()
{
  if(instance==NULL) 
    instance = new GPMain();
  return instance;
}
void GPMain::Delete()
{
  if(instance)  delete instance;
}
GPMain::GPMain()
{
  sProgramName = "simpit";
  sProgramVersion = "2.4";
}


GPMain::~GPMain()
{
}

void GPMain::Init(int argc,char** argv)
{
	for(int i=0;i<argc;i++)
		vsArgv.push_back(argv[i]);
}
std::string GPMain::GetArgv(int i)
{
	if(i<vsArgv.size())
		return vsArgv[i];
	else
		return "";
}
std::string GPMain::GetWorkDir()
{
	return sWorkDir;
}
std::string GPMain::GetDataDir()
{
	return sDataDir;
}
void GPMain::Print()
{
}
void GPMain::Print(std::ofstream& ofs)
{
}
void GPMain::Update()
{
}
void GPMain::SetParameter(std::string sGlobal)
{
    std::string sLocal=sGlobal;
    std::stringstream ss(sLocal);
    std::string sKey;
    std::string sValue;
    std::string sUnit;
    ss>>sKey>>sValue>>sUnit;
	if(sKey=="data.dir")
		sDataDir=sValue;
	else if(sKey=="work.dir")
		sWorkDir=sValue;
    else
    {
      std::cout<<"The key does not exist: "<<sKey<<std::endl;
    }
}
double GPMain::GetParameter(std::string sGlobal)
{
	return -1;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
