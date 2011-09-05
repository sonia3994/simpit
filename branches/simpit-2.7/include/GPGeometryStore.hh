//
// $Id: GPGeometryStore.hh,v 1.6 
// GEANT4 tag $Name: geant4-09-02 $
//

#ifndef GPGEOMETRYSTORE_H
#define GPGEOMETRYSTORE_H 1

#include <map>
#include <string>

class GPGeometry;
typedef std::map<std::string, GPGeometry* > GPGeometryMap;
class GPGeometryStore 
{

  protected:
    GPGeometryStore();
    ~GPGeometryStore();
  public:
    static GPGeometryStore* GetInstance();
    static void Delete();
    GPGeometryMap*  GetGeometryMap(){return &mStrGeometry;};
    GPGeometry* FindGeometry(std::string);
    void AddGeometry(std::string, GPGeometry*);
    void DelGeometry(std::string);
    void EraseItem(std::string);
    void Clear();

  protected:
    static GPGeometryStore* instance;
    GPGeometryMap mStrGeometry;
};

#endif

