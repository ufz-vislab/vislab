cmake_minimum_required(VERSION 2.6)

# set project's name
set( PROJECTNAME SkyBackgroundPlugin )
project( ${PROJECTNAME} )

# Source files
set( ${PROJECTNAME}_SRCS
	SkyBackgroundPluginForm.cpp
	SkyBackgroundPlugin.cpp
	LightSettingsHandler.cpp
	PythonBindings.cpp
)

# Moc Header files
set( ${PROJECTNAME}_MOC_HDRS
	SkyBackgroundPluginForm.h
)

# Header files
set( ${PROJECTNAME}_HDRS
	SkyBackgroundPlugin.h
	LightSettingsHandler.h
)

# UI files
set( ${PROJECTNAME}_UIS
  SkyBackgroundPluginFormBase.ui
)

STRING(REGEX REPLACE "\\\\" "/" VRED_DIR $ENV{VRED_DIR}) 


set (CMAKE_CONFIGURATION_TYPES "RelWithDebInfo;Release" CACHE TYPE INTERNAL FORCE)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${VRED_DIR}/dev")
find_package( Qt3VRED REQUIRED )

#set(LIBRARY_OUTPUT_PATH $ENV{VRED_DIR}/lib/plugins/WIN32/Module)

# OpenSG Compiler flags
# Suppress OpenSG warnings, Set warning Level to 1
IF(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
     STRING(REGEX REPLACE "/W[0-4]" "/W1" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
ELSE(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
     SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W1")
ENDIF(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")

add_definitions(
  -DBOOST_PYTHON_DYNAMIC_LIB
  -DBOOST_IOSTREAMS_DYN_LINK
  -DQT_DLL
  -DQT_THREAD_SUPPORT
  -DQT_ACCESSIBILITY_SUPPORT
  -DQT_NO_DEBUG
  -DQT_CLEAN_NAMESPACE

  -D_OSG_HAVE_CONFIGURED_H_
  -DOSG_BUILD_DLL
  -DOSG_WITH_GLUT
  -DOSG_WITH_GIF
  -DOSG_WITH_TIF
  -DOSG_WITH_JPG
  -D/MP
)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DOSG_DEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO  "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /ZI /Od /Ob0")

# this will run uic on .ui files:
qt_wrap_ui( ${PROJECTNAME} ${PROJECTNAME}_UI_H ${PROJECTNAME}_UI_CPP ${${PROJECTNAME}_UIS} )

# and finally this will run moc:
qt_wrap_cpp( ${PROJECTNAME} ${PROJECTNAME}_UI_CPP ${${PROJECTNAME}_UI_H} ${${PROJECTNAME}_MOC_HDRS} )

include_directories( ${QT_INCLUDE_DIR} ${QT_INCLUDE_PATH}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${VRED_DIR}/include/boost-1.34-vc8.0
  ${VRED_DIR}/include/vred
  ${VRED_DIR}/include/glew
  ${VRED_DIR}/include/glut-3.7
  ${VRED_DIR}/include/OpenSG
  ${VRED_DIR}/include/OpenSG/OpenSG
  ${VRED_DIR}/include/qt-3.38-vc8.0
  ${VRED_DIR}/include/python-2.52-vc8.0
)

link_directories ( ${VRED_DIR}/bin/WIN64 )

# Put moc files in visual studio project folder
source_group("Moc Files" REGULAR_EXPRESSION moc_*)

add_library( ${PROJECTNAME} SHARED ${${PROJECTNAME}_SRCS} ${${PROJECTNAME}_HDRS} ${${PROJECTNAME}_MOC_HDRS} ${${PROJECTNAME}_UI_CPP} )

# Link additional libraries
target_link_libraries( ${PROJECTNAME} ${QT_LIBRARIES}
  user32 kernel32 MSVCPRT MSVCRT winmm gdi32 wsock32 oldnames python25
  boost_python-vc80-mt-1_34_1 boost_iostreams-vc80-mt-1_34_1
  vrUtils vrQtExt Cg CgGL OSGBase OSGSystem OSGWindowWIN32 OSGContrib opengl32
  glu32 vrOpenSGExt vrOpenSGNav vrOpenSGUtils vrMicroKernel glew32 vrOpenSGRender
  vrMessageRW vrIconManager als vrQScintilla vrCBShader vrKernel
)

message(STATUS "${VRED_DIR}/lib/plugins/WIN64/Module/${PROJECTNAME}.dll")
message(STATUS "${CMAKE_CFG_INTDIR}")

if (${CMAKE_GENERATOR} STREQUAL "Visual Studio 8 2005 Win64")
ADD_CUSTOM_COMMAND( TARGET ${PROJECTNAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_BINARY_DIR}/RelWithDebInfo/${PROJECTNAME}.dll" ${VRED_DIR}/lib/plugins/WIN64/Module/${PROJECTNAME}.dll
)
else (${CMAKE_GENERATOR} STREQUAL "Visual Studio 8 2005 Win64")
ADD_CUSTOM_COMMAND( TARGET ${PROJECTNAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_BINARY_DIR}/RelWithDebInfo/${PROJECTNAME}.dll" ${VRED_DIR}/lib/plugins/WIN32/Module/${PROJECTNAME}.dll
)
endif (${CMAKE_GENERATOR} STREQUAL "Visual Studio 8 2005 Win64")