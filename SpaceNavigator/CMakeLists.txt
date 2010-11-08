## SpaceNavigator-Project ##

# Set development for VRED or not
OPTION (USE_VRED "Do you want to compile the SpaceNavigator libraries for VRED?" ON)

IF (NOT USE_VRED)
  SET (CMAKE_CONFIGURATION_TYPES "Release;Debug" CACHE TYPE INTERNAL FORCE)
  FIND_PACKAGE(OpenSG REQUIRED COMPONENTS OSGBase OSGSystem OSGWindowGLUT)
  FIND_PACKAGE(OpenSGSupportlibs REQUIRED)
ENDIF ()

FIND_PACKAGE(VRPN REQUIRED)

# Add subprojects
ADD_SUBDIRECTORY(SpaceNavigatorClient)
ADD_SUBDIRECTORY(TriangleElevationGrid)

IF (USE_VRED)
	#add_subdirectory(vrpn)
	#set(vrpn_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vrpn)
	#ADD_SUBDIRECTORY(${CMAKE_SOURCE_DIR}/VRED/SpaceNavigatorPlugin)
ELSE (USE_VRED)
	ADD_SUBDIRECTORY(vrpn)
	ADD_SUBDIRECTORY(SpaceNavigatorServer)
	ADD_SUBDIRECTORY(SpaceNavigatorSSM)
	ADD_SUBDIRECTORY(SpaceNavigatorTestSSM)
	ADD_SUBDIRECTORY(TriangleElevationGrid)
	ADD_SUBDIRECTORY(SpaceNavigatorTestSSMGrid)
ENDIF (USE_VRED)