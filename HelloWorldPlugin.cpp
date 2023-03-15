#include "HelloWorldPlugin.h"
#include "HelloWorldPluginUI.h"
#include <iostream>

void HelloWorldPlugin::objectModified(int ) {

}

bool HelloWorldPlugin::init() {
    m_mainWidget = new HelloWorldPluginUI();
    return true;
}

void HelloWorldPlugin::cleanup() {
  std::cout << "ByeBye World! " << std::endl;
}

void HelloWorldPlugin::update(){

}

void HelloWorldPlugin::point3DSelected(double, double, double, int) {

}

// This last line will export the plugin.
V_EXPORT_PLUGIN(HelloWorld, HelloWorldPlugin)
