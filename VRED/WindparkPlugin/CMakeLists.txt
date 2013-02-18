## Windpark Plugin ##

# Source files
SET( SRCS
	ElevationGridBase.cpp
	ProjectControllerLoader.cpp
	TriangleElevationGrid.cpp
	VredWindparkPlugin.cpp
	WPE3DModel.cpp
	WPEDiagramModel.cpp
	WPEDiagramView.cpp
	WPEProjectController.cpp
	WPEState.cpp
	WPETypeInfo.cpp
)

# Moc header files
SET( MOC_HDRS
	ProjectControllerLoader.h
	WindparkPluginPanel.h
	WPEAttrPanel.h
	WPEMainFrame.h
)

# Header files
#SET( HDRS SwitchPlugin.h )

# UI files
SET( UIS
	ProjectControllerLoader.ui
	WindparkPluginPanel.ui
	WPEAttrPanel.ui
	WPEMainFrame.ui
)

# this will run uic on .ui files:
QT_WRAP_UI( WindparkPlugin UI_H UI_CPP ${UIS} )

# and finally this will run moc:
QT_WRAP_CPP( WindparkPlugin UI_CPP ${UI_H} ${MOC_HDRS} )

INCLUDE_DIRECTORIES(
	.
	${CMAKE_CURRENT_BINARY_DIR}
)

# Put moc files in visual studio project folder
SOURCE_GROUP("UI Files" REGULAR_EXPRESSION "\\w*\\.ui")
SOURCE_GROUP("Moc Files" REGULAR_EXPRESSION "moc_*")

# Create the plugin dll
ADD_LIBRARY( WindparkPlugin SHARED ${SRCS} ${MOC_HDRS} ${UI_CPP} )

#SET_TARGET_PROPERTIES(SwitchPlugin PROPERTIES LINK_FLAGS "/NODEFAULTLIB:libcmt.lib /NODEFAULTLIB:libcmtd.lib /INCREMENTAL:NO")

# Link additional libraries
TARGET_LINK_LIBRARIES( WindparkPlugin
	${QT_LIBRARIES} ${VRED_LIBRARIES}
)

INSTALL_PLUGIN (WindparkPlugin)

