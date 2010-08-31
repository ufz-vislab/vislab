## Lib - SpaceNavigatorSSM ##

# Source files
set(SOURCES
  SpaceNavigatorSSM.h SpaceNavigatorSSM.cpp
)

IF (USE_VRED)
  include_directories(SYSTEM ${TriangleElevationGrid_INCLUDE_DIR}
    ${VRPN_INCLUDE_DIR} ${VRED_DIR}/include/OpenSG ${VRED_DIR}/include/glut-3.7)
  link_directories(${VRED_DIR}/bin/WIN32)
  add_library( SpaceNavigatorSSMVRED STATIC ${SOURCES} )
  add_dependencies(SpaceNavigatorSSMVRED SpaceNavigatorClient)
  target_link_libraries(SpaceNavigatorSSMVRED SpaceNavigatorSSM OSGBase OSGSystem)
ELSE (USE_VRED)
  include_directories(SYSTEM ${SpaceNavigatorClient_INCLUDE_DIR} ${TriangleElevationGrid_INCLUDE_DIR}
    ${VRPN_INCLUDE_DIR} ${OpenSG_INCLUDE_DIR} ${OpenSGSupportlibs_INCLUDE_DIR})
  add_library( SpaceNavigatorSSM STATIC ${SOURCES} )
  target_link_libraries(SpaceNavigatorSSM SpaceNavigatorClient
    TriangleElevationGrid ${OpenSG_LIBRARIES} ${OpenSGSupportlibs_LIBRARIES})
ENDIF(USE_VRED)