## SpaceNavigatorServer ##

# Source files
set(SOURCES
  SpaceNavigatorServer.cpp
)

# Set include directories
include_directories(${VRPN_SpaceNavigator_INCLUDE_DIR})
include_directories(SYSTEM ${VRPN_INCLUDE_DIR})
include_directories(SYSTEM ${QUAT_INCLUDE_DIR})

# Create executable
add_executable(SpaceNavigatorServer ${SOURCES})

# Add additional link directories

# Linked libraries
target_link_libraries(SpaceNavigatorServer VRPN_SpaceNavigator)