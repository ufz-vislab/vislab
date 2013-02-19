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

	Tools/ART2DWindowInterface.cpp
	Tools/ART2DWindowManager.cpp
	Tools/ARTNavWindowInterface.cpp
	Tools/ARTToolsManager.cpp
	Tools/CageFilter.cpp
	Tools/DefaultTool.cpp
	Tools/FlystickCageFilterBBoxTool.cpp
	Tools/FlystickMixedModePickTool.cpp
	Tools/FlystickNavigationTool.cpp
	Tools/FlystickPickTool.cpp
	Tools/FlystickTranslateTool.cpp
	Tools/FlystickTranslationResetTool.cpp
	Tools/FlystickWalkCageFilterTool.cpp
	Tools/FlystickWalkNavigationPickTool.cpp
	Tools/FlystickWalkNavigationTool.cpp
	Tools/FlystickWalkNavigationTranslateTool.cpp
	Tools/FlystickWalkNavigationTranslationResetTool.cpp
	Tools/MouseNavigationTool.cpp
	Tools/MouseToolBase.cpp
	Tools/MouseTrackballFlystickWalkNavigationTool.cpp
	Tools/MouseTrackballNavigationTool.cpp
	Tools/NodeNamesSet.cpp
	Tools/OpenSGToolsSceneManager.cpp
	Tools/ScreenIntersector.cpp
	Tools/ToolBase.cpp
	Tools/ToolFactory.cpp
	Tools/ToolHolder.cpp
	Tools/Vr3DAnalogToolBase.cpp
	Tools/Vr3DButtonToolBase.cpp
	Tools/VrpnArtTrackingClient.cpp
	Tools/XmlToolLoader.cpp
)

# Moc header files
SET( MOC_HDRS
	2DView/ART2DImageViewer.h
	2DView/IART2DView.h
	2DView/IQtART2DViewWidget.h
	2DView/QtVRED2DViewMainWindow.h
	NavigationView/QtNavCanvasView.h
	NavigationView/QtVRED2DNavMainWindow.h
)

# Header files
#SET( HDRS SwitchPlugin.h )

# UI files
SET( UIS
	Tools/QtVREDToolsPanel.ui
)

# this will run uic on .ui files:
QT_WRAP_UI( InteractionPlugin UI_H UI_CPP ${UIS} )

# and finally this will run moc:
QT_WRAP_CPP( InteractionPlugin UI_CPP ${UI_H} ${MOC_HDRS} )

INCLUDE(ExternalProject)
ExternalProject_Add( TinyXML
	PREFIX ${CMAKE_CURRENT_BINARY_DIR}/TinyXML
	URL http://libtinyxml.googlecode.com/files/libtinyxml-src.zip
	BUILD_IN_SOURCE 1
	INSTALL_COMMAND ""
)

INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}/2DView
	${CMAKE_CURRENT_SOURCE_DIR}/NavigationView
	${CMAKE_CURRENT_SOURCE_DIR}/Tools
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_BINARY_DIR}/2DView
	${CMAKE_CURRENT_BINARY_DIR}/NavigationView
	${CMAKE_CURRENT_BINARY_DIR}/Tools
	${VRPN_INCLUDE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}/TinyXML/src/TinyXML
)

# Put moc files in visual studio project folder
SOURCE_GROUP("UI Files" REGULAR_EXPRESSION "\\w*\\.ui")
SOURCE_GROUP("Moc Files" REGULAR_EXPRESSION "moc_*")
SOURCE_GROUP("Generated Files" REGULAR_EXPRESSION "\\w*\\.cxx")

# Create the plugin dll
ADD_LIBRARY( InteractionPlugin SHARED ${SRCS} ${MOC_HDRS} ${UI_CPP} )
ADD_DEPENDENCIES( InteractionPlugin VRPN TinyXML )

#SET_TARGET_PROPERTIES(SwitchPlugin PROPERTIES LINK_FLAGS "/NODEFAULTLIB:libcmt.lib /NODEFAULTLIB:libcmtd.lib /INCREMENTAL:NO")

#LINK_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR}/TinyXML/src/TinyXML/lib/${CMAKE_CFG_INTDIR})

# Link additional libraries
TARGET_LINK_LIBRARIES( InteractionPlugin
	${QT_LIBRARIES} ${VRED_LIBRARIES} ${VRPN_LIBRARIES}
	${CMAKE_CURRENT_BINARY_DIR}/TinyXML/src/TinyXML/lib/${CMAKE_CFG_INTDIR}/tinyxml.lib
)

INSTALL_PLUGIN (InteractionPlugin)

IF (INSTALL_ON_POSTBUILD)
	ADD_CUSTOM_COMMAND( TARGET InteractionPlugin POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/TinyXML/src/TinyXML/lib/${CMAKE_CFG_INTDIR}/tinyxml.dll"
										  ${VRED_BIN_DIR}/tinyxml.dll
	)
ENDIF ()