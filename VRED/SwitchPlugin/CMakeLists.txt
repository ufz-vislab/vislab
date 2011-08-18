## SpaceNavigatorPlugin ##

# Source files
SET( SRCS
	# PythonBindings.cpp
	SwitchPluginForm.cpp
	SwitchPlugin.cpp
)

# Moc header files
SET( MOC_HDRS SwitchPluginForm.h )

# Header files
SET( HDRS SwitchPlugin.h )

# UI files
SET( UIS SwitchPluginFormBase.ui )

# this will run uic on .ui files:
QT_WRAP_UI( SwitchPlugin UI_H UI_CPP ${UIS} )

# and finally this will run moc:
QT_WRAP_CPP( SwitchPlugin UI_CPP ${UI_H} ${MOC_HDRS} )

INCLUDE_DIRECTORIES(
	.
	${CMAKE_CURRENT_BINARY_DIR}
)

# Put moc files in visual studio project folder
SOURCE_GROUP("UI Files" REGULAR_EXPRESSION "\\w*\\.ui")
SOURCE_GROUP("Moc Files" REGULAR_EXPRESSION "moc_*")

# Create the plugin dll
ADD_LIBRARY( SwitchPlugin SHARED ${SRCS} ${HDRS} ${MOC_HDRS} ${UI_CPP} )

#SET_TARGET_PROPERTIES(SwitchPlugin PROPERTIES LINK_FLAGS "/NODEFAULTLIB:libcmt.lib /NODEFAULTLIB:libcmtd.lib /INCREMENTAL:NO")

# Link additional libraries
TARGET_LINK_LIBRARIES( SwitchPlugin
	${QT_LIBRARIES} ${VRED_LIBRARIES}
)

INSTALL_PLUGIN (SwitchPlugin)

