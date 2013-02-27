## Vtk Plugin ##

# Source files
SET( SRCS

	VredPanel/DataVisualizerVREDPanel.cpp
	VredPanel/DataVisualizerVREDPanel.ui.h
	VredPlugin/OpenSGDataVisualizerVREDPlugin.cpp
	VtkOsgActor/vtkOsgActor.cpp
	VtkScalarGrid/ScalarCellPropGrid.cpp
	VtkScalarGrid/ScalarCellPropGridState.cpp
	VtkScalarGrid/ScalarGridState.cpp
	VtkScalarGrid/ScalarUnstructuredGrid.cpp
	VtkScalarGrid/TensorGrid.cpp
	VtkScalarGrid/TensorGridState.cpp
	VtkScalarGrid/VectorGrid.cpp
	VtkScalarGrid/VectorGridState.cpp
	VtkScalarGrid/VtkDataSetLoader.cpp
	VtkScalarGrid/VtkDataSetLoaderState.cpp
	VtkScalarGrid/vtkLookupTable2D.cpp
)

# Moc header files
SET( MOC_HDRS
	VredPanel/DataVisualizerVREDPanel.h
)

# UI files
SET( UIS VredPanel/DataVisualizerVREDPanel.ui )

# this will run uic on .ui files:
QT_WRAP_UI( VtkPlugin UI_H UI_CPP ${UIS} )

# and finally this will run moc:
QT_WRAP_CPP( VtkPlugin UI_CPP ${UI_H} ${MOC_HDRS} )

INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/VredPanel
	${CMAKE_CURRENT_SOURCE_DIR}/OpenSGDataVisualizerVREDPlugin
	${CMAKE_CURRENT_SOURCE_DIR}/VtkOsgActor
	${CMAKE_CURRENT_BINARY_DIR}
)

# Put moc files in visual studio project folder
SOURCE_GROUP("UI Files" REGULAR_EXPRESSION "\\w*\\.ui")
SOURCE_GROUP("Moc Files" REGULAR_EXPRESSION "moc_*")

# Create the plugin dll
ADD_LIBRARY( VtkPlugin SHARED ${SRCS} ${MOC_HDRS} ${UI_CPP} )

#SET_TARGET_PROPERTIES(VtkPlugin PROPERTIES LINK_FLAGS "/NODEFAULTLIB:libcmt.lib /NODEFAULTLIB:libcmtd.lib /INCREMENTAL:NO")

# Link additional libraries
TARGET_LINK_LIBRARIES( VtkPlugin
	${QT_LIBRARIES} ${VRED_LIBRARIES}
)

INSTALL_PLUGIN (VtkPlugin)

