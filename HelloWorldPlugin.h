#ifndef HELLO_WORLD_PLUGIN_H
#define HELLO_WORLD_PLUGIN_H

#include "rtPluginInterface.h"
#include "CompatibilityWithQt.h"

class HelloWorldPlugin : public QObject, public DataInterface 
{
  Q_OBJECT
  Q_INTERFACES(DataInterface)
  V_PLUGIN_METADATA(IID "HelloWorld" FILE "HelloWorld.json")

 public:
  void objectModified(int objID);
  bool init();
  void cleanup();
  void update();
  void point3DSelected(double px, double py, double pz, int intensity);

 private:

};

#endif
