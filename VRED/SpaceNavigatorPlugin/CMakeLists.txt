## SpaceNavigatorPlugin ##

# Source files
SET( SRCS
	PythonBindings.cpp
	SpaceNavigatorPluginForm.cpp
	SpaceNavigatorPlugin.cpp
)

# Moc header files
SET( MOC_HDRS
	SpaceNavigatorPluginForm.h
)

# Header files
SET( HDRS
	SpaceNavigatorPlugin.h
)


# UI files
SET( UIS
	SpaceNavigatorPluginFormBase.ui
)


# Adds preprocessor definitions
ADD_DEFINITIONS(
  #-D_WIN32_WINNT=0x0400
  #-DWINVER=0x0400
  #-D_WIN32_WINDOWS=0x0410
  #-DOPTIMIZED_COMPILE
  #-DVR_VERSION_STRING=\"1.2\"
  #-DUSE_QSCINTILLA
  #-D_CRT_SECURE_NO_DEPRECATE
  #-D/NODEFAULTLIB
)

# this will run uic on .ui files:
QT_WRAP_UI( SpaceNavigatorPlugin UI_H UI_CPP ${UIS} )

# and finally this will run moc:
QT_WRAP_CPP( SpaceNavigatorPlugin UI_CPP ${UI_H} ${MOC_HDRS} )

INCLUDE_DIRECTORIES(
	.
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_SOURCE_DIR}/SpaceNavigator/SpaceNavigatorClient
	${CMAKE_SOURCE_DIR}/SpaceNavigator/TriangleElevationGrid
	${VRPN_INCLUDE_DIR}
	${QUAT_INCLUDE_DIR}
)

# Put moc files in visual studio project folder
SOURCE_GROUP("UI Files" REGULAR_EXPRESSION "\\w*\\.ui")
SOURCE_GROUP("Moc Files" REGULAR_EXPRESSION "moc_*")

# Create the plugin dll
ADD_LIBRARY( SpaceNavigatorPlugin SHARED ${SRCS} ${HDRS} ${MOC_HDRS} ${UI_CPP} )

#SET_TARGET_PROPERTIES(SpaceNavigatorPlugin PROPERTIES LINK_FLAGS "/NODEFAULTLIB:libcmt.lib /NODEFAULTLIB:libcmtd.lib /INCREMENTAL:NO")

# Link additional libraries
TARGET_LINK_LIBRARIES( SpaceNavigatorPlugin
	${QT_LIBRARIES} ${VRED_LIBRARIES} ${VRPN_LIBRARIES} ${QUAT_LIBRARIES}
	SpaceNavigatorClient TriangleElevationGrid
)

INSTALL_PLUGIN (SpaceNavigatorPlugin)

