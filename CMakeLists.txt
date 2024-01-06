cmake_minimum_required(VERSION 3.1.0)

project(spaiot-simulator)
set (PROJECT_DESCRIPTION "Spa 2840 simulator")
set (PROJECT_DESCRIPTION_TEXT "\
  Simulator for test C28403 control panel.\n\
  Runs on all pi boards supported by piduino.."
)

string(TOLOWER ${CMAKE_PROJECT_NAME} PROJECT_NAME)

# Set a default build type if none was specified
set(default_build_type "Release")
 
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
      STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

set(INSTALL_BIN_DIR bin CACHE PATH "Installation directory for executables")

set (LIB_SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set (LIB_INC_DIR ${PROJECT_SOURCE_DIR}/include)

find_package(Piduino REQUIRED)
find_package(Curses REQUIRED)


include (GitVersion)
GetGitVersion(SPAIOT_SIMULATOR_VERSION)
set(SPAIOT_SIMULATOR_VERSION
  ${SPAIOT_SIMULATOR_VERSION_MAJOR}.${SPAIOT_SIMULATOR_VERSION_MINOR}.${SPAIOT_SIMULATOR_VERSION_PATCH})

# Make relative paths absolute (needed later on)
foreach(p BIN)
  set(var INSTALL_${p}_DIR)
  if(NOT IS_ABSOLUTE "${${var}}")
    set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
  endif()
endforeach()

message (STATUS "Building for ${CMAKE_SYSTEM_PROCESSOR} architecture.")
if ((${CMAKE_SYSTEM_PROCESSOR} MATCHES "^arm.*") OR (${CMAKE_SYSTEM_PROCESSOR} MATCHES "aarch64"))
else()
  message (WARNING "${CMAKE_SYSTEM_PROCESSOR} is not a supported architecture !")
endif()

# uninstall target
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)
add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)

add_custom_target(setuid
  COMMAND piduino-setuid  ${PROJECT_NAME}
  WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
  COMMENT "Setting set-uid root bit for executable")

set(PROGRAM_PERMISSIONS_DEFAULT
    OWNER_WRITE OWNER_READ OWNER_EXECUTE
    GROUP_READ GROUP_EXECUTE
    WORLD_READ WORLD_EXECUTE)
    
# ------------------------------------------------------------------------------
set (SOURCES
  ${LIB_SRC_DIR}/main.cpp
  ${LIB_SRC_DIR}/bus.cpp
  ${LIB_SRC_DIR}/engine.cpp
)

if (CMAKE_BUILD_TYPE STREQUAL "Release")
  add_definitions (-DQT_NO_DEBUG_OUTPUT=1)
  message(STATUS "-- Debug output disabled")
endif()

include_directories( 
  . 
  ${LIB_INC_DIR}
  ${CMAKE_BINARY_DIR} 
  ${PIDUINO_INCLUDE_DIRS} 
  ${CURSES_INCLUDE_DIR}
)

add_executable(spaiot-simulator "${SOURCES}")
target_link_libraries(spaiot-simulator ${PIDUINO_LIBRARIES} ${CURSES_LIBRARIES})

install(TARGETS ${PROJECT_NAME} DESTINATION "${INSTALL_BIN_DIR}" 
        PERMISSIONS ${PROGRAM_PERMISSIONS_DEFAULT} SETUID COMPONENT utils)

### Packages generation --------------------------------------------------------
set(CPACK_PACKAGE_VERSION "${SPAIOT_SIMULATOR_VERSION_MAJOR}.${SPAIOT_SIMULATOR_VERSION_MINOR}.${SPAIOT_SIMULATOR_VERSION_PATCH}")
set(CPACK_PACKAGE_VERSION_MAJOR "${SPAIOT_SIMULATOR_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${SPAIOT_SIMULATOR_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${SPAIOT_SIMULATOR_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT "Pascal JEAN <pascal.jean@piduino.org>")
set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/COPYING.LESSER")

set(CPACK_GENERATOR "DEB")

if (CPACK_GENERATOR STREQUAL "DEB")

  set(CPACK_DEB_PACKAGE_COMPONENT 1)
  set(CPACK_DEB_COMPONENT_INSTALL 1)
  set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://github.com/epsilonrt/spaiot-simulator")

  find_program(dpkg_query NAMES dpkg-query)
  find_program(dpkg_cmd NAMES dpkg)

  if (NOT CPACK_DEBIAN_PACKAGE_ARCHITECTURE)
    if (dpkg_cmd)
      execute_process(COMMAND ${dpkg_cmd} --print-architecture RESULT_VARIABLE ret OUTPUT_VARIABLE arch OUTPUT_STRIP_TRAILING_WHITESPACE)
      if (ret EQUAL 0)
        set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "${arch}")
      else (ret EQUAL 0)
        message(WARNING "Unable to get debian architecture !")
      endif (ret EQUAL 0)
    endif (dpkg_cmd)
  endif (NOT CPACK_DEBIAN_PACKAGE_ARCHITECTURE)
  
  message(STATUS "Debian architecture ${CPACK_DEBIAN_PACKAGE_ARCHITECTURE}")
  
  
  set(CPACK_DEBIAN_PACKAGE_NAME ${PROJECT_NAME})
  set(CPACK_COMPONENT_DESCRIPTION "${PROJECT_NAME} - ${PROJECT_DESCRIPTION}\n${PROJECT_DESCRIPTION_TEXT}\n  This package provides the command-line utilities.")
  set(CPACK_DEBIAN_FILE_NAME "${PROJECT_NAME}_${CPACK_PACKAGE_VERSION}_${CPACK_DEBIAN_PACKAGE_ARCHITECTURE}.deb")
  set(CPACK_DEBIAN_PACKAGE_DEPENDS "libpiduino")
  set(CPACK_DEBIAN_PACKAGE_SECTION "utils")
  
endif (CPACK_GENERATOR STREQUAL "DEB")

include(CPack)


