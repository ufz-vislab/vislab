## SkyBackgroundPlugin ##

# Source files
SET( SRCS
	SkyBackgroundPluginForm.cpp
	SkyBackgroundPlugin.cpp
	LightSettingsHandler.cpp
	PythonBindings.cpp
)

# Moc Header files
SET( MOC_HDRS
	SkyBackgroundPluginForm.h
)

# Header files
SET( HDRS
	SkyBackgroundPlugin.h
	LightSettingsHandler.h
)

# UI files
SET( UIS
  SkyBackgroundPluginFormBase.ui
)

INCLUDE_DIRECTORIES(
	.
	${CMAKE_BINARY_DIR}/VRED/SkyBackgroundPlugin
	)

# this will run uic on .ui files:
QT_WRAP_UI( SkyBackgroundPlugin UI_H UI_CPP ${UIS} )

# and finally this will run moc:
QT_WRAP_CPP( SkyBackgroundPlugin UI_CPP ${UI_H} ${MOC_HDRS} )

# Put moc files in visual studio project folder
SOURCE_GROUP("UI Files" REGULAR_EXPRESSION "\\w*\\.ui")
SOURCE_GROUP("Moc Files" REGULAR_EXPRESSION "moc_*")

ADD_LIBRARY( SkyBackgroundPlugin SHARED ${SRCS} ${HDRS} ${MOC_HDRS} ${UI_CPP} )

# Link additional libraries
TARGET_LINK_LIBRARIES( SkyBackgroundPlugin ${QT_LIBRARIES} ${VRED_LIBRARIES} )

INSTALL_PLUGIN (SkyBackgroundPlugin)

