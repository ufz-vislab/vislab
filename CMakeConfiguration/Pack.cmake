#### Packaging setup for Vislab ####

SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Vislab")
SET(CPACK_PACKAGE_VENDOR "Lars Bilke")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/README.md")
SET(CPACK_PACKAGE_FILE_NAME "vislab-${CMAKE_SYSTEM}-x${BITS}")

IF (VISLAB_PACKAGING_ZIP)
  SET (CPACK_GENERATOR ZIP)
  MESSAGE (STATUS "Packaging set to ZIP")
ELSE ()
  SET(CPACK_GENERATOR NSIS)
  # There is a bug in NSI that does not handle full unix paths properly. Make
  # sure there is at least one set of four (4) backlasshes.
  SET(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_DESCRIPTION_SUMMARY}")
  SET(CPACK_NSIS_CONTACT "lars.bilke@ufz.de")
  MESSAGE (STATUS "Packaging set to NSIS")
ENDIF ()

#SET(CPACK_PACKAGE_EXECUTABLES "ogs" "OGS Command Line" "ogs-gui" "OGS User Interface")

INCLUDE (CPack)
