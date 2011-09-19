// $Id: GPMain.cc,v 1.19 2008/01/17 17:31:32 maire Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "GPMain.hh"
#include "GPMainMessenger.hh"
#include <sstream>
#include <string>
#include <algorithm>
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
  sProgramVersion = "2.8";
  sWorkDir=*getenv("PWD");
  sMacFile="";
  time_t time_m=time(0);
  G4String sTime;
  sTime=ctime(&time_m);
  sTime.resize(24);
  replace(sTime.begin(),sTime.end(),' ','-');
  replace(sTime.begin(),sTime.end(),':','-');
  sDataDir="./output/"+sTime;
  pMessenger=new GPMainMessenger();
}


GPMain::~GPMain()
{
  delete pMessenger;
}

void GPMain::Init(int argc,char** argv)
{
	for(int i=0;i<argc;i++)
		vsArgv.push_back(argv[i]);
        std::string sArgv;
        std::string sKey;
        std::string sValue;
        std::stringstream ss;
        for(size_t j=0;j!=vsArgv.size();j++)
        {
          sArgv=vsArgv[j];
          replace(sArgv.begin(),sArgv.end(),'=',' ');
          ss.clear();ss.str(sArgv);
          ss>>sKey>>sValue;
          if(sKey=="-o")
          {
            sDataDir=sValue;
          }
          else if(sKey=="-e")
          {
            sMacFile=sValue;
          }
        }
}
std::string GPMain::GetArgv(size_t i)
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
  if(sKey=="app.out_dir")
    sDataDir=sValue;
  else
  {
    std::cout<<"The key does not exist: "<<sKey<<std::endl;
  }
  
  std::cout<<"GPMain: Set "+sKey+": "+sValue<<std::endl;

}
double GPMain::GetParameter(std::string sGlobal)
{
	return -1;
}
std::string GPMain::GetValueInString(std::string sGlobal)
{
  std::string sLocal=sGlobal;
  std::stringstream ss(sLocal);
  std::string sKey;
  ss>>sKey;
  if(sKey=="app.out_dir")
    return sDataDir;
  else if (sKey=="app.version")
    return sProgramVersion;
  else if (sKey=="app.name")
    return sProgramName;
  else if (sKey=="app.work_dir")
    return sWorkDir;
  else if (sKey=="app.mac_file")
    return sMacFile;
  else
  {
    return "";
  }

  std::cout<<"The key does not exist: "<<sKey<<std::endl;
  return "";

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
