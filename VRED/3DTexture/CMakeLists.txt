## 3DTexture Plugin ##

# Source files
SET( SRCS
	C3DTexture.cpp
	DataVolume.cpp
	VRED3DTexturePanel.ui.h
	VRED3DTexturePlugin.cpp
)

# Moc header files
SET( MOC_HDRS
	""
)

# UI files
SET( UIS
	VRED3DTexturePanel.ui
)

# this will run uic on .ui files:
QT_WRAP_UI( 3DTexturePlugin UI_H UI_CPP ${UIS} )

# and finally this will run moc:
QT_WRAP_CPP( 3DTexturePlugin UI_CPP ${UI_H} ${MOC_HDRS} )

INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
)

# Put moc files in visual studio project folder
SOURCE_GROUP("UI Files" REGULAR_EXPRESSION "\\w*\\.ui")
SOURCE_GROUP("Moc Files" REGULAR_EXPRESSION "moc_*")
SOURCE_GROUP("Generated Files" REGULAR_EXPRESSION "\\w*\\.cxx")

# Create the plugin dll
ADD_LIBRARY( 3DTexturePlugin SHARED ${SRCS} ${MOC_HDRS} ${UI_CPP} )

#SET_TARGET_PROPERTIES(SwitchPlugin PROPERTIES LINK_FLAGS "/NODEFAULTLIB:libcmt.lib /NODEFAULTLIB:libcmtd.lib /INCREMENTAL:NO")

# Link additional libraries
TARGET_LINK_LIBRARIES( 3DTexturePlugin
	${QT_LIBRARIES} ${VRED_LIBRARIES}
)

INSTALL_PLUGIN (3DTexturePlugin)

