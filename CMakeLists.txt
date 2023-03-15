cmake_minimum_required (VERSION 2.6)
if(UNIX AND NOT APPLE)
set(CMAKE_INSTALL_PREFIX "/usr" CACHE PATH "Directory to install under: should contain lib/Vurtigo-Plugins" )
endif(UNIX AND NOT APPLE)

project (HelloWorldPlugin)

add_definitions ( -Wall -g )

################
# Find Qt
################

find_package(Qt4 REQUIRED)
if (QT4_FOUND)
  # Qt Core and Qt Gui are included by default.
  set(QT_USE_QTNETWORK 1)
  set(QT_USE_QTXML 1)
  set(QT_USE_QTHELP 1)
  include(${QT_USE_FILE})
endif(QT4_FOUND)

################
# Find VTK
################

find_package(VTK 6.2 REQUIRED)
if (VTK_FOUND)
  message(STATUS "Found VTK: " ${VTK_MAJOR_VERSION} "." ${VTK_MINOR_VERSION} "." ${VTK_BUILD_VERSION})
  include(${VTK_USE_FILE})
endif(VTK_FOUND)


###############
# Find Vurtigo core libs
###############

find_path(VURTIGO_INCLUDE_DIR rtApplication.h /usr/include/vurtigoIncludes)

include_directories ( ${VURTIGO_INCLUDE_DIR} )

find_path(VURTIGO_GUI_INCLUDE_DIR rtMainWindow.h /usr/include/vurtigoIncludes)

include_directories ( ${VURTIGO_GUI_INCLUDE_DIR} )

set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/Vurtigo-Plugins")

find_library(VURTIGO_LIB VurtigoBase /usr/lib/VurtigoLibs)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

include_directories( "/opt/vurt/ctk/build/CMakeExternals/Install/include" )

###################
# Setup Plugin files
###################

# List of source files.
set(HELLO_SRC_FILES
  HelloWorldPlugin.cpp
  HelloWorldPluginUI.cpp
  HelloWorldDataObject.cpp
  HelloWorldRenderObject.cpp
  HelloWorldRenderObjectMaker.cpp
)

# MOC files are the ones the use Q_OBJECT
set (HELLO_MOC_FILES
  HelloWorldPlugin.h
  HelloWorldPluginUI.h
  HelloWorldDataObject.h
)

set (UI_FILES_HELLO
  HelloWorldUI.ui
  HelloWorldPluginUI.ui
)

qt4_wrap_ui(UI_HELLO_OUT ${UI_FILES_HELLO} )
qt4_wrap_cpp(HELLO_MOC_FILES_OUT ${HELLO_MOC_FILES} )

include_directories( 
  ${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR}
)

add_library(HelloWorld SHARED ${HELLO_SRC_FILES} ${UI_HELLO_OUT} ${HELLO_MOC_FILES_OUT})
target_link_libraries(HelloWorld ${QT_LIBRARIES} ${VTK_LIBRARIES} ${VURTIGO_LIB})

if(UNIX AND NOT APPLE)
install(TARGETS HelloWorld 
DESTINATION lib/Vurtigo-Plugins
)
install(FILES HelloWorldConfig.xml
  DESTINATION lib/Vurtigo-Plugins
)
endif(UNIX AND NOT APPLE)

if(WIN32)
install(TARGETS HelloWorld
DESTINATION ./VurtigoPlugins
)
endif(WIN32)
