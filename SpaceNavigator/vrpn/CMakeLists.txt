## VRPN_SpaceNavigator ##

# Source files
set(SOURCES
  vrpn.C
  vrpn_3DConnexion.h vrpn_3DConnexion.C
  vrpn_Generic_server_object.h vrpn_Generic_server_object.C
)

# Set include directories
INCLUDE_DIRECTORIES(SYSTEM ${VRPN_INCLUDE_DIR})
INCLUDE_DIRECTORIES(SYSTEM ${QUAT_INCLUDE_DIR})

# Create library
ADD_LIBRARY( VRPN_SpaceNavigator STATIC ${SOURCES} )

TARGET_LINK_LIBRARIES(VRPN_SpaceNavigator ${VRPN_LIBRARIES})