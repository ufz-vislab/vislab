## VRPN_SpaceNavigator ##

# Source files
set(SOURCES
  vrpn.C
  vrpn_3DConnexion.h vrpn_3DConnexion.C
  vrpn_Generic_server_object.h vrpn_Generic_server_object.C
)

# Set include directories
include_directories(SYSTEM ${VRPN_INCLUDE_DIR})
include_directories(SYSTEM ${QUAT_INCLUDE_DIR})

# Create library
add_library( VRPN_SpaceNavigator STATIC ${SOURCES} )

target_link_libraries(VRPN_SpaceNavigator ${VRPN_LIBRARIES})