## Lib - TraingleElevationGrid ##

# Source files
SET(SOURCES
  ElevationGridBase.h ElevationGridBase.cpp
  TriangleElevationGrid.h TriangleElevationGrid.cpp
)

# Set include directories
INCLUDE_DIRECTORIES(
  ${OpenSG_INCLUDE_DIR}
)

# Create library
ADD_LIBRARY( TriangleElevationGrid STATIC ${SOURCES} )

# Linked libraries
TARGET_LINK_LIBRARIES(
  TriangleElevationGrid
  ${OpenSG_LIBRARIES}
)
