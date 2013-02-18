## Interaction Plugin ##

# Source files
SET( SRCS
	2DView/ART2DImageViewer.cpp
	2DView/IART2DView.cpp
	2DView/IQtART2DViewWidget.cpp
	2DView/QtVRED2DViewMainWindow.cpp

	NavigationView/QtNavCanvasView.cpp
	NavigationView/QtUserMarker.cpp
	NavigationView/QtVRED2DNavMainWindow.cpp

	Tools/QtVREDToolsPanel.ui.h
	Tools/VREDToolsPlugin.cpp
	Tools/VREDToolsSceneManager.cpp
	Tools/WPEAttrPanel.ui.h
)

# Moc header files
SET( MOC_HDRS
	""
)

# Header files
#SET( HDRS SwitchPlugin.h )

# UI files
SET( UIS
	Tools/QtVREDToolsPanel.ui
	Tools/WPEAttrPanel.ui
)

# this will run uic on .ui files:
QT_WRAP_UI( InteractionPlugin UI_H UI_CPP ${UIS} )

# and finally this will run moc:
QT_WRAP_CPP( InteractionPlugin UI_CPP ${UI_H} ${MOC_HDRS} )

INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}/2DView
	${CMAKE_CURRENT_SOURCE_DIR}/NavigationView
	${CMAKE_CURRENT_SOURCE_DIR}/Tools
	${CMAKE_CURRENT_BINARY_DIR}/2DView
	${CMAKE_CURRENT_BINARY_DIR}/NavigationView
	${CMAKE_CURRENT_BINARY_DIR}/Tools
)

# Put moc files in visual studio project folder
SOURCE_GROUP("UI Files" REGULAR_EXPRESSION "\\w*\\.ui")
SOURCE_GROUP("Moc Files" REGULAR_EXPRESSION "moc_*")

# Create the plugin dll
ADD_LIBRARY( InteractionPlugin SHARED ${SRCS} ${MOC_HDRS} ${UI_CPP} )

#SET_TARGET_PROPERTIES(SwitchPlugin PROPERTIES LINK_FLAGS "/NODEFAULTLIB:libcmt.lib /NODEFAULTLIB:libcmtd.lib /INCREMENTAL:NO")

# Link additional libraries
TARGET_LINK_LIBRARIES( InteractionPlugin
	${QT_LIBRARIES} ${VRED_LIBRARIES}
)

INSTALL_PLUGIN (InteractionPlugin)

