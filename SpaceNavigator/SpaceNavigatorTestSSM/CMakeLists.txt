## Exe - SpaceNavigatorTestSSM ##

# Source files
set(SOURCES SpaceNavigatorTestSSMCity.cpp)

IF (VRED)
  include_directories(SYSTEM
    ${VRPN_INCLUDE_DIR} ${VRED_DIR}/include/OpenSG ${VRED_DIR}/include/glut-3.7)
  link_directories(${VRED_DIR}/bin/WIN32)
  add_executable(SpaceNavigatorTestSSMVRED ${SOURCES})
  add_dependencies(SpaceNavigatorTestSSMVRED SpaceNavigatorSSMVRED)
  target_link_libraries(SpaceNavigatorTestSSMVRED ${VRPN_LIBRARIES} ${QUAT_LIBRARIES} OSGBase OSGSystem)
ELSE (VRED)
  include_directories(SYSTEM ${SpaceNavigatorSSM_INCLUDE_DIR} ${OpenSG_INCLUDE_DIR} ${OpenSGSupportlibs_INCLUDE_DIR} ${VRPN_INCLUDE_DIR})
  add_executable(SpaceNavigatorTestSSM ${SOURCES})
  target_link_libraries(SpaceNavigatorTestSSM SpaceNavigatorSSM)
  target_link_libraries(SpaceNavigatorTestSSM ${VRPN_LIBRARIES} ${QUAT_LIBRARIES} OSGBase OSGSystem)
ENDIF(VRED)

