## SpaceNavigatorClient ##

# Source files
SET(SOURCES
  SpaceNavigatorClient.h SpaceNavigatorClient.cpp
  SpaceNavigatorOSGClient.h SpaceNavigatorOSGClient.cpp
)


# Set include directories
INCLUDE_DIRECTORIES(
  ${VRPN_INCLUDE_DIR} 
  ${QUAT_INCLUDE_DIR}
)

# Create library
ADD_LIBRARY( SpaceNavigatorClient STATIC ${SOURCES} )

# Linked libraries
TARGET_LINK_LIBRARIES(SpaceNavigatorClient ${VRPN_LIBRARIES} ${QUAT_LIBRARIES} )