## SpaceNavigatorClient ##

# Source files
set(SOURCES
  SpaceNavigatorClient.h SpaceNavigatorClient.cpp
  SpaceNavigatorOSGClient.h SpaceNavigatorOSGClient.cpp
)


# Set include directories
include_directories(SYSTEM
  ${VRPN_INCLUDE_DIR} 
  ${QUAT_INCLUDE_DIR}
  ${VRED_DIR}/include/OpenSG
  ${VRED_DIR}/include/OpenSG/OpenSG
  ${VRED_DIR}/include/glut-3.7
)

# Create library
add_library( SpaceNavigatorClient STATIC ${SOURCES} )

# Linked libraries
target_link_libraries(SpaceNavigatorClient ${VRPN_LIBRARIES} ${QUAT_LIBRARIES} )