#### SpaceNavigator-Project ####

# Set minimum CMake version
cmake_minimum_required(VERSION 2.6)

# Set development for VRED or not
option (USE_VRED "Do you want to compile for VRED?" ON)

# Replace backslashes in environment variables here
STRING(REGEX REPLACE "\\\\" "/" VRED_DIR $ENV{VRED_DIR}) 
STRING(REGEX REPLACE "\\\\" "/" LIBRARIES_DIR $ENV{LIBRARIES_DIR})

# Set additional CMake modules path
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "$ENV{LIBRARIES_DIR}/CMakeFindModules/")

# Set VRED project name, include dirs and libraries
IF (USE_VRED)
  set (CMAKE_CONFIGURATION_TYPES "Release;RelWithDebInfo" CACHE TYPE INTERNAL FORCE)
  project(SpaceNavigatorVRED)
  SET (VRED_INCLUDE_DIRS 
    "${VRED_DIR}/include/qt-3.38-vc8.0"
    "${VRED_DIR}/include/vred"
    "${VRED_DIR}/include/boost-1.34-vc8.0"
    "${VRED_DIR}/include/cg-1.41"
    "${VRED_DIR}/include/ffmpeg-0.49"
    "${VRED_DIR}/include/glew"
    "${VRED_DIR}/include/glut-3.7"
    "${VRED_DIR}/include/OpenAL"
    "${VRED_DIR}/include/python-2.52-vc8.0"
    "${VRED_DIR}/include/zlib-1.23"
    "${VRED_DIR}/include/OpenSG"
    "${VRED_DIR}/include/OpenSG/OpenSG"
  )
  SET (VRED_LIBRARIES
    user32 kernel32 MSVCPRT MSVCRT winmm gdi32 wsock32 oldnames python25 qt-mt338
    boost_python-vc80-mt-1_34_1 boost_iostreams-vc80-mt-1_34_1
    vrUtils vrQtExt Cg CgGL OSGBase OSGSystem OSGWindowWIN32 OSGContrib opengl32
    glu32 vrOpenSGExt vrOpenSGNav vrOpenSGUtils vrMicroKernel glew32 vrOpenSGRender
    vrMessageRW vrIconManager als vrQScintilla vrCBShader vrKernel
  )
ELSE (USE_VRED)
  set (CMAKE_CONFIGURATION_TYPES "Release;Debug" CACHE TYPE INTERNAL FORCE)
  project(SpaceNavigator)
  set(OPENSG_ROOT "${LIBRARIES_DIR}/OpenSG/Build/win32-msvc80/installed/")
  find_package(OpenSG REQUIRED COMPONENTS OSGBase OSGSystem OSGWindowGLUT)
  find_package(OpenSGSupportlibs REQUIRED)
ENDIF (USE_VRED)

find_package(VRPN REQUIRED)

#Compiler flags
# Suppress OpenSG warnings, Set warning Level to 1
IF(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
     STRING(REGEX REPLACE "/W[0-4]" "/W1" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
ELSE(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
     SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W1")
ENDIF(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")

add_definitions( 
  -D_OSG_HAVE_CONFIGURED_H_
  -DOSG_BUILD_DLL
  -DOSG_WITH_GLUT
  -DOSG_WITH_GIF
  -DOSG_WITH_TIF
  -DOSG_WITH_JPG
  -D/MP
)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DOSG_DEBUG")

# Set build directories
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

# Add subprojects
add_subdirectory(SpaceNavigator)
set(SpaceNavigatorClient_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/SpaceNavigator)

add_subdirectory(TriangleElevationGrid)
set(TriangleElevationGrid_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/TriangleElevationGrid)

#add_subdirectory(vrpn)
#set(vrpn_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vrpn)

IF (USE_VRED)
  add_subdirectory(SpaceNavigatorPlugin)
ELSE (USE_VRED)
  add_subdirectory(vrpn)
  set(VRPN_SpaceNavigator_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vrpn)
  add_subdirectory(SpaceNavigatorServer)

  add_subdirectory(SpaceNavigatorSSM)
  set(SpaceNavigatorSSM_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/SpaceNavigatorSSM)

  add_subdirectory(SpaceNavigatorTestSSM)

  add_subdirectory(TriangleElevationGrid)
  set(TriangleElevationGrid_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/TriangleElevationGrid)

  add_subdirectory(SpaceNavigatorTestSSMGrid)
ENDIF (USE_VRED)