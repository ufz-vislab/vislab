cmake_minimum_required(VERSION 2.6)
SET(CMAKE_USE_RELATIVE_PATHS TRUE)

# set project's name
SET( PROJECTNAME SpaceNavigatorPlugin )
PROJECT( ${PROJECTNAME} )

# Source files
SET( ${PROJECTNAME}_SRCS
	PythonBindings.cpp
	SpaceNavigatorPluginForm.cpp
	SpaceNavigatorPlugin.cpp
)

# Moc header files
SET( ${PROJECTNAME}_MOC_HDRS
	SpaceNavigatorPluginForm.h
)

# Header files
SET( ${PROJECTNAME}_HDRS
	SpaceNavigatorPlugin.h
)


# UI files
SET( ${PROJECTNAME}_UIS
	SpaceNavigatorPluginFormBase.ui
)

# Set directory where tge FindQt3VRED.cmake is located and finds the qt package
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${LIBRARIES_DIR}/CMakeFindModules/")
FIND_PACKAGE( Qt3VRED REQUIRED )

# Adds preprocessor definitions
ADD_DEFINITIONS(
  -D_WIN32_WINNT=0x0400
  -DWINVER=0x0400
  -D_WIN32_WINDOWS=0x0410
  -DOPTIMIZED_COMPILE
  -DVR_VERSION_STRING=\"1.2\"
  -DBOOST_PYTHON_DYNAMIC_LIB
  -DBOOST_IOSTREAMS_DYN_LINK
  -DQT_DLL
  -DQT_THREAD_SUPPORT
  -DQT_ACCESSIBILITY_SUPPORT
  -DQT_NO_DEBUG
  -DQT_CLEAN_NAMESPACE
  -DUSE_QSCINTILLA
  -D_CRT_SECURE_NO_DEPRECATE
  -D/NODEFAULTLIB
)

# this will run uic on .ui files:
QT_WRAP_UI( ${PROJECTNAME} ${PROJECTNAME}_UI_H ${PROJECTNAME}_UI_CPP ${${PROJECTNAME}_UIS} )

# and finally this will run moc:
QT_WRAP_CPP( ${PROJECTNAME} ${PROJECTNAME}_UI_CPP ${${PROJECTNAME}_UI_H} ${${PROJECTNAME}_MOC_HDRS} )

INCLUDE_DIRECTORIES(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${VRED_INCLUDE_DIRS}
  ${SpaceNavigatorClient_INCLUDE_DIR}
  ${TriangleElevationGrid_INCLUDE_DIR}
  ${VRPN_INCLUDE_DIR} 
  ${QUAT_INCLUDE_DIR}
)

# Additional library directories
LINK_DIRECTORIES( "${VRED_DIR}/bin/WIN64" )

# Put moc files in visual studio project folder
source_group("Moc Files" REGULAR_EXPRESSION moc_*)

# Create the plugin dll
ADD_LIBRARY( ${PROJECTNAME} SHARED ${${PROJECTNAME}_SRCS} ${${PROJECTNAME}_HDRS} ${${PROJECTNAME}_MOC_HDRS} ${${PROJECTNAME}_UI_CPP} )

SET_TARGET_PROPERTIES(${PROJECTNAME} PROPERTIES LINK_FLAGS "/NODEFAULTLIB:libcmt.lib /NODEFAULTLIB:libcmtd.lib /INCREMENTAL:NO")

add_dependencies(${PROJECTNAME} SpaceNavigatorClient TriangleElevationGridVRED)

# Link additional libraries
TARGET_LINK_LIBRARIES( ${PROJECTNAME} 
  ${VRED_LIBRARIES} ${VRPN_LIBRARIES} ${QUAT_LIBRARIES} SpaceNavigatorClient TriangleElevationGridVRED
)

if (${CMAKE_GENERATOR} STREQUAL "Visual Studio 8 2005 Win64")
ADD_CUSTOM_COMMAND( TARGET ${PROJECTNAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy "${LIBRARY_OUTPUT_PATH}/RelWithDebInfo/${PROJECTNAME}.dll" ${VRED_DIR}/lib/plugins/WIN64/Module/${PROJECTNAME}.dll
)
else (${CMAKE_GENERATOR} STREQUAL "Visual Studio 8 2005 Win64")
ADD_CUSTOM_COMMAND( TARGET ${PROJECTNAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy "${LIBRARY_OUTPUT_PATH}/RelWithDebInfo/${PROJECTNAME}.dll" ${VRED_DIR}/lib/plugins/WIN32/Module/${PROJECTNAME}.dll
)
endif (${CMAKE_GENERATOR} STREQUAL "Visual Studio 8 2005 Win64")
