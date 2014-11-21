#Minimum required CMake Version
cmake_minimum_required(VERSION 2.8.8)
 
#Project Name
project(OC)
set(PACKAGE "OC" CACHE STRING "")
#Project Version
SET(OC_VERSION_MAJOR 2)
SET(OC_VERSION_MINOR 0)
SET(OC_VERSION_PATCH 0)
SET(OC_VERSION ${OC_VERSION_MAJOR}.${OC_VERSION_MINOR}.${OC_VERSION_PATCH})
SET(VERSION ${OC_VERSION})
#Library .so version.
SET(OC_SO_VERSION 7.2.0)

SET(TLL_LIBS "")

# Set some default C Flags if we are using GCC
IF(CMAKE_COMPILER_IS_GNUCC)
    SET(CMAKE_C_FLAGS "-g -O2")
ENDIF()

# Get system configuration
find_program(UNAME NAMES uname)
IF(UNAME)
macro(getuname name flag)
    exec_program("${UNAME}" ARGS "${flag}" OUTPUT_VARIABLE "${name}")
endmacro(getuname)
getuname(osname -s)
getuname(osrel  -r)
getuname(cpu    -m)
set(BUILDNAME "${osname}-${osrel}-${cpu}" CACHE STRING "Build name variable for CDash")
ENDIF()

# For CMAKE_INSTALL_LIBDIR
INCLUDE(GNUInstallDirs)
INCLUDE(InstallRequiredSystemLibraries)

IF(MSVC)
    SET(GLOBAL PROPERTY USE_FOLDERS ON)
ENDIF()

FIND_PACKAGE(CURL)
IF(NOT CURL_LIBRARY)
    MESSAGE(FATAL_ERROR "DAP Support specified, CURL libraries are not found.")
ENDIF()
ADD_DEFINITIONS(-DCURL_STATICLIB=1)
INCLUDE_DIRECTORIES(${CURL_INCLUDE_DIRS})

# Add libcurl as linking dependency
SET(TLL_LIBS ${TLL_LIBS} ${CURL_LIBRARY})

# Default to shared libs on
OPTION(BUILD_SHARED_LIBS "Configure oc as a shared library." ON)
# Set variable to define the build type.
INCLUDE(GenerateExportHeader)
SET (LIB_TYPE STATIC)
IF (BUILD_SHARED_LIBS)
    SET(LIB_TYPE SHARED)
#    IF(CMAKE_COMPILER_IS_GNUCC)
#        SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
#    ENDIF()
ENDIF()
ADD_DEFINITIONS()

### Verbose make, for debugging.
#SET(CMAKE_VERBOSE_MAKEFILE ON)

SET(CMAKE_INCLUDE_CURRENT_DIR ON)

#Add custom CMake Module
SET (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules/"
     CACHE INTERNAL "Location of our custom CMake modules.")

# Configure-type checks
INCLUDE (${CMAKE_ROOT}/Modules/CheckLibraryExists.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/CheckIncludeFile.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/CheckTypeSize.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/CheckFunctionExists.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/CheckCXXSourceCompiles.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/TestBigEndian.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/CheckSymbolExists.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/GetPrerequisites.cmake)
FIND_PACKAGE(PkgConfig QUIET)

## Enable 'dist and distcheck'.
## File taken from http://ensc.de/cmake/FindMakeDist.cmake
FIND_PACKAGE(MakeDist)
## End 'enable dist and distcheck'

# Only necessary for Windows
IF(MSVC)
    # Supress some warnings on Windows
    ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)
ENDIF()

IF (NOT CMAKE_BUILD_TYPE)
    SET (CMAKE_BUILD_TYPE DEBUG CACHE STRING "Choose the type of build, options are: None, Debug, Release."
         FORCE)
ENDIF()

#####
# Some utility macros/scripts.
#####
# Macro to append files to the EXTRA_DIST files.
#####
SET(EXTRA_DIST "")
MACRO(ADD_EXTRA_DIST files)
  FOREACH(F ${files})
    SET(EXTRA_DIST ${EXTRA_DIST} ${CMAKE_CURRENT_SOURCE_DIR}/${F})
  ENDFOREACH()
ENDMACRO()

# A basic script used to convert m4 files
MACRO(GEN_m4 filename)
    IF(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${filename}.c)
    ADD_CUSTOM_COMMAND(
        OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/${filename}.c
        COMMAND m4 
        ARGS ${CMAKE_CURRENT_SOURCE_DIR}/${filename}.m4 > ${CMAKE_CURRENT_SOURCE_DIR}/${filename}.c
        VERBATIM
    )
    ENDIF()
ENDMACRO(GEN_m4)

# A script to print out information at the end of the configuration step.
MACRO(print_conf_summary)
    MESSAGE("")
    MESSAGE("")
    MESSAGE(STATUS "CMake Summary:")
    MESSAGE("")
    MESSAGE(STATUS "\tBuild Type:\t\t\t" ${CMAKE_BUILD_TYPE})
    MESSAGE(STATUS "\tBuilding Shared Libraries:\t" ${BUILD_SHARED_LIBS})
    IF(CMAKE_PREFIX_PATH)
        MESSAGE(STATUS "\tCMake Prefix Path:\t\t" "${CMAKE_PREFIX_PATH}")
    ENDIF()
    MESSAGE(STATUS "\tLinking against:\t\t" "${TLL_LIBS}") 
    MESSAGE("")
ENDMACRO()

##
# Configuration for post-install RPath
# Adapted from http://www.cmake.org/Wiki/CMake_RPATH_handling
##
IF(NOT MSVC)
    # use, i.e. don't skip the full RPATH for the build tree
    SET(CMAKE_SKIP_BUILD_RPATH  FALSE)

    # when building, don't use the install RPATH already
    # (but later on when installing)
    SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE) 

    SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

    # add the automatically determined parts of the RPATH
    # which point to directories outside the build tree to the install RPATH
    SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

    # the RPATH to be used when installing, but only if it's not a system directory
    LIST(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
    IF("${isSystemDir}" STREQUAL "-1")
        SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
    ENDIF("${isSystemDir}" STREQUAL "-1")
ENDIF()

##
# End configuration for post-install RPath
##


#####
# Option checks
#####

# Option to use a local or remote CDash instance.
# instances Defined in CTestConfig.cmake
OPTION (USE_REMOTE_CDASH "Use CDash instance at CDash.org." OFF)

# Set the appropriate compiler/architecture for universal OSX binaries.
IF(${CMAKE_SYSTEM_NAME} EQUAL "Darwin")
    SET(CMAKE_OSX_ARCHITECTURES i386;x86_64)
ENDIF(${CMAKE_SYSTEM_NAME} EQUAL "Darwin")

IF(ENABLE_COVERAGE_TESTS)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0 -Wall -W -fprofile-arcs -ftest-coverage")
    SET(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")
ENDIF()

# Option to Build DLL
IF(WIN32)
    OPTION (ENABLE_DLL "Build a Windows DLL." ${BUILD_SHARED_LIBS})
    IF (ENABLE_DLL)
        SET(BUILD_DLL ON CACHE BOOL "")
        ADD_DEFINITIONS(-DDLL_OC)
        ADD_DEFINITIONS(-DDLL_EXPORT)
    ENDIF ()
ENDIF()

# Option to use bundled XGetopt in place of getopt(). This is mostly useful
# for MSVC builds. If not building utilities, getopt() isn't required at all.
IF(MSVC)
    OPTION(ENABLE_XGETOPT "Enable bundled XGetOpt instead of external getopt()." ON)
    IF(ENABLE_XGETOPT)
        SET(USE_X_GETOPT ON CACHE BOOL "")
    ENDIF()
ENDIF()

SET(MATH "")

# Enable Tests

# Option to Enable remote tests.
OPTION(ENABLE_REMOTE_TESTS "Enable remote tests." ON)

IF(NOT MSVC)
  OPTION (ENABLE_TESTS "Enable basic tests, run with 'make test'." ON)
  IF(ENABLE_TESTS) 
   SET(BUILD_TESTSETS ON CACHE BOOL "")
  ENDIF()
ENDIF()

OPTION (ENABLE_INTERNAL_DOCS "Enable documentation of library internals. This is of interest only to those developing the netCDF library." OFF)
IF(ENABLE_INTERNAL_DOCS)
    SET(BUILD_INTERNAL_DOCS ON)
ENDIF()

# Linux specific large file support flags.
# Modelled after check in CMakeLists.txt for hdf5.
IF (NOT WINDOWS)
  OPTION(ENABLE_LFS "Enable large (64-bit) files on linux." ON)
  # TODO: There needs to be a check here to ensure
  # that large file support is actually supported.
  
  IF(ENABLE_LFS)
  SET(_FILE_OFFSET_BITS 64)
  SET(_LARGEFILE64_SOURCE TRUE)
  SET(_LARGEFILE_SOURCE TRUE)
  ENDIF()
ENDIF()

# Provide the option to perform coverage tests.
OPTION (ENABLE_COVERAGE_TESTS "Enable compiler flags needed to perform coverage tests." OFF)
IF(ENABLE_COVERAGE_TESTS)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -Wall -W -profile-arcs -ftest-coverage")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0 -Wall -W -fprofile-arcs -ftest-coverage")
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")
    MESSAGE(STATUS "Coverage Tests: On.")
ENDIF()

OPTION (ENABLE_DOXYGEN "Enable generation of doxygen." OFF)
IF(ENABLE_DOXYGEN)
    SET(BUILD_DOCS ON CACHE BOOL "")
    OPTION(ENABLE_INTERNAL_DOCS "Build internal documentation. This is of interest to developers only." OFF)
    IF(ENABLE_INTERNAL_DOCS)
        SET(BUILD_INTERNAL_DOCS YES CACHE STRING "")
    ELSE()
        SET(BUILD_INTERNAL_DOCS NO CACHE STRING "")
    ENDIF()
ENDIF()

# Set some of the options as advanced.
MARK_AS_ADVANCED(ENABLE_INTERNAL_DOCS VALGRIND_TESTS ENABLE_COVERAGE_TESTS)
MARK_AS_ADVANCED(ENABLE_REMOTE_TESTS USE_REMOTE_CDASH)
#####
# End option checks.
#####

#####
# System inspection checks
#####
INCLUDE_DIRECTORIES(".")
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/tests)
SET(CMAKE_REQUIRED_INCLUDES ${CMAKE_SOURCE_DIR}/tests)

#
# Library include checks
CHECK_INCLUDE_FILE("math.h"    HAVE_MATH_H)
CHECK_INCLUDE_FILE("unistd.h"  HAVE_UNISTD_H)

# Taken from netcdf; this list needs updating for oc
CHECK_INCLUDE_FILE("alloca.h"  HAVE_ALLOCA_H)
CHECK_INCLUDE_FILE("malloc.h"  HAVE_MALLOC_H)
CHECK_INCLUDE_FILE("ctype.h"   HAVE_CTYPE_H)
CHECK_INCLUDE_FILE("dirent.h"  HAVE_DIRENT_H)
CHECK_INCLUDE_FILE("dlfcn.h"   HAVE_DLFCN_H)
CHECK_INCLUDE_FILE("errno.h"   HAVE_ERRNO_H)
CHECK_INCLUDE_FILE("fcntl.h"   HAVE_FCNTL_H)
CHECK_INCLUDE_FILE("getopt.h"  HAVE_GETOPT_H)
CHECK_INCLUDE_FILE("stdbool.h"   HAVE_STDBOOL_H)
CHECK_INCLUDE_FILE("locale.h"  HAVE_LOCAL_H)
CHECK_INCLUDE_FILE("stdint.h"  HAVE_STDINT_H)
CHECK_INCLUDE_FILE("stdio.h"   HAVE_STDIO_H)
CHECK_INCLUDE_FILE("stdlib.h"  HAVE_STDLIB_H)
CHECK_INCLUDE_FILE("stdarg.h"  HAVE_STDARG_H)
CHECK_INCLUDE_FILE("strings.h"   HAVE_STRINGS_H)
CHECK_INCLUDE_FILE("signal.h"  HAVE_SIGNAL_H)
CHECK_INCLUDE_FILE("sys/dir.h"   HAVE_SYS_DIR_H)
CHECK_INCLUDE_FILE("sys/ndir.h"  HAVE_SYS_NDIR_H)
CHECK_INCLUDE_FILE("sys/param.h" HAVE_SYS_PARAM_H)
CHECK_INCLUDE_FILE("sys/stat.h"  HAVE_SYS_STAT_H)
CHECK_INCLUDE_FILE("sys/time.h"  HAVE_SYS_TIME_H)
CHECK_INCLUDE_FILE("sys/types.h" HAVE_SYS_TYPES_H)
CHECK_INCLUDE_FILE("sys/wait.h"  HAVE_SYS_WAIT_H)
CHECK_INCLUDE_FILE("sys/resource.h" HAVE_SYS_RESOURCE_H)
CHECK_INCLUDE_FILE("fcntl.h"  HAVE_FCNTL_H)
CHECK_INCLUDE_FILE("inttypes.h"  HAVE_INTTYPES_H)
CHECK_INCLUDE_FILE("pstdint.h"  HAVE_PSTDINT_H)
CHECK_INCLUDE_FILE("endian.h"   HAVE_ENDIAN_H)
CHECK_INCLUDE_FILE("BaseTsd.h"  HAVE_BASETSD_H)

# Type checks
CHECK_TYPE_SIZE("double"     SIZEOF_DOUBLE)
CHECK_TYPE_SIZE("float"      SIZEOF_FLOAT)
CHECK_TYPE_SIZE("int"      SIZEOF_INT)
CHECK_TYPE_SIZE("long"       SIZEOF_LONG)
CHECK_TYPE_SIZE("long long"    SIZEOF_LONG_LONG)
CHECK_TYPE_SIZE("off_t"      SIZEOF_OFF_T)
CHECK_TYPE_SIZE("off64_t"    SIZEOF_OFF64_T)
CHECK_TYPE_SIZE("short"      SIZEOF_SHORT)
CHECK_TYPE_SIZE("size_t"     SIZEOF_SIZE_T)
CHECK_TYPE_SIZE("ssize_t"    SIZEOF_SSIZE_T)
CHECK_TYPE_SIZE("uchar"      SIZEOF_UCHAR)
CHECK_TYPE_SIZE("int64_t"    SIZEOF_INT64_T)
CHECK_TYPE_SIZE("uint64_t"     SIZEOF_UINT64_T)

IF(SIZEOF_SSIZE_T)
    SET(HAVE_SSIZE_T 1)
ELSE()
    CHECK_TYPE_SIZE("SSIZE_T"     SIZEOF_SSIZE_T)
    IF(SIZEOF_SSIZE_T)
        SET(HAVE_SSIZE_T 1)
    ENDIF()
ENDIF()

# Check for various functions. 
CHECK_FUNCTION_EXISTS(fsync   HAVE_FSYNC)
CHECK_FUNCTION_EXISTS(strlcat   HAVE_STRLCAT)
CHECK_FUNCTION_EXISTS(strerror  HAVE_STRERROR)
CHECK_FUNCTION_EXISTS(snprintf  HAVE_SNPRINTF)
CHECK_FUNCTION_EXISTS(strchr  HAVE_STRCHR)
CHECK_FUNCTION_EXISTS(strrchr   HAVE_STRRCHR)
CHECK_FUNCTION_EXISTS(strcat  HAVE_STRCAT)
CHECK_FUNCTION_EXISTS(strcpy  HAVE_STRCPY)
CHECK_FUNCTION_EXISTS(strdup  HAVE_STRDUP)
CHECK_FUNCTION_EXISTS(strcasecmp    HAVE_STRCASECMP)
CHECK_FUNCTION_EXISTS(strtod  HAVE_STRTOD)
CHECK_FUNCTION_EXISTS(strtoll   HAVE_STRTOLL)
CHECK_FUNCTION_EXISTS(strtoull  HAVE_STROULL)
CHECK_FUNCTION_EXISTS(strstr  HAVE_STRSTR)
CHECK_FUNCTION_EXISTS(mkstemp   HAVE_MKSTEMP)
CHECK_FUNCTION_EXISTS(rand    HAVE_RAND)
CHECK_FUNCTION_EXISTS(gettimeofday    HAVE_GETTIMEOFDAY)
CHECK_FUNCTION_EXISTS(fsync   HAVE_FSYNC)
CHECK_FUNCTION_EXISTS(MPI_Comm_f2C    HAVE_MPI_COMM_F2C)
CHECK_FUNCTION_EXISTS(memmove   HAVE_MEMMOVE)
CHECK_FUNCTION_EXISTS(getpagesize     HAVE_GETPAGESIZE)
CHECK_FUNCTION_EXISTS(sysconf   HAVE_SYSCONF)
CHECK_FUNCTION_EXISTS(mremap  HAVE_MREMAP)
CHECK_FUNCTION_EXISTS(getrlimit HAVE_GETRLIMIT)

#####
# End system inspection checks.
#####

# Check for the math library so it can be explicitely linked.
IF(NOT WIN32)
     FIND_LIBRARY(HAVE_LIBM NAMES math m libm)
     MESSAGE(STATUS "Found Math library: ${HAVE_LIBM}")
     IF(NOT HAVE_LIBM)
        MESSAGE(FATAL_ERROR "Unable to find the math library.")
     ENDIF()
ENDIF()

# Create config.h file
configure_file("${OC_SOURCE_DIR}/config.h.in.cmake"
               "${OC_BINARY_DIR}/config.h")

INCLUDE_DIRECTORIES(${OC_BINARY_DIR})
# End autotools-style checs for config.h

#####
# Set core names of the libraries.
#####
SET (OC_LIB_CORENAME    "oc")

#####
# Set the true names of all the libraries, if customized by external project
#####
# Recurse into other subdirectories.
#add_subdirectory("include")
#add_subdirectory(libdispatch)
#add_subdirectory(libsrc)

# For tests and utilities, we are no longer
# exporting symbols but rather importing them.
IF(BUILD_DLL)
    REMOVE_DEFINITIONS(-DDLL_EXPORT)
ENDIF()

#####
# Build doxygen documentation, if need be.
#####
add_subdirectory(docs)

CONFIGURE_FILE(
  ${CMAKE_CURRENT_SOURCE_DIR}/FixBundle.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/FixBundle.cmake
  @ONLY
)
#INSTALL(SCRIPT ${CMAKE_CURRENT_BINARY_DIR}/FixBundle.cmake)

configure_file (
    ${OC_SOURCE_DIR}/oc-config.cmake.in
    ${OC_BINARY_DIR}/oc-config.cmake @ONLY)

configure_file (
    ${OC_SOURCE_DIR}/oc-config-version.cmake.in
    ${OC_BINARY_DIR}/oc-config-version.cmake @ONLY)

# End CMake package configuration files.

#####
# Various options for CPACK
#####

##
# Declare exclusions list used when building a source file.
# NOTE!! This list uses regular expressions, NOT wildcards!! 
## 
SET (CPACK_SOURCE_IGNORE_FILES "${CPACK_SOURCE_IGNORE_FILES}"
    "/tests/"
    "${CMAKE_BINARY_DIR}/*"
    "/myhtml/*"
    "/.svn/"
    "my.*\\\\.sh"
    "/.deps/"
    "/.libs"
    "/html/"
    ".*\\\\.jar"
    ".*\\\\.jdl"
    ".*\\\\.sed"
    ".*\\\\.proto"
    ".*\\\\.texi"
    ".*\\\\.example"
    "Make0"
    ".*~" 
)
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/COPYRIGHT
    ${CMAKE_CURRENT_BINARY_DIR}/COPYRIGHT.txt
    @ONLY
)

SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_BINARY_DIR}/COPYRIGHT.txt")
IF(NOT CPACK_PACK_VERSION)
    SET(CPACK_PACKAGE_VERSION ${VERSION})
ENDIF()


IF(APPLE)
  SET(CPACK_SOURCE_GENERATOR "TGZ")
  SET(CPACK_GENERATOR "PackageMaker" "STGZ" "TBZ2" "TGZ" "ZIP")
ENDIF()

# Create an 'uninstall' target.
CONFIGURE_FILE(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

ADD_CUSTOM_TARGET(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)

## Customize some of the package component descriptions
set (CPACK_COMPONENT_UTILITIES_DISPLAY_NAME "oc Utilities")
set (CPACK_COMPONENT_LIBRARIES_DISPLAY_NAME "oc Libraries")
set (CPACK_COMPONENT_HEADERS_DISPLAY_NAME "oc Headers")
set (CPACK_COMPONENT_DEPENDENCIES_DISPLAY_NAME "oc Dependencies")
set (CPACK_COMPONENT_DOCUMENTATION_DISPLAY_NAME "oc Documentation")

set (CPACK_COMPONENT_UTILITIES_DESCRIPTION
    "The oc Utilities")
set (CPACK_COMPONENT_LIBRARIES_DESCRIPTION
    "The oc Libraries")
set (CPACK_COMPONENT_HEADERS_DESCRIPTION
    "C header files for use with oc")
set (CPACK_COMPONENT_DEPENDENCIES_DESCRIPTION
    "Dependencies for this build of oc")
set (CPACK_COMPONENT_DOCUMENTATION_DESCRIPTION
    "The oc user documentation.")

# Create oc-config
FILE(MAKE_DIRECTORY ${oc_BINARY_DIR}/tmp)
configure_file("${OC_SOURCE_DIR}/oc-config.in.cmake"
  "${OC_BINARY_DIR}/tmp/oc-config" @ONLY)
FILE(COPY "${OC_BINARY_DIR}/tmp/oc-config"
  DESTINATION ${OC_BINARY_DIR}/
  FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
#EXECUTE_PROCESS(COMMAND "chmod 755 ${OC_BINARY_DIR}/oc-config")

# Enable Makedist files.
ADD_MAKEDIST()
ENABLE_MAKEDIST(
    COPYING
    COPYRIGHT
    RELEASE_NOTES
    README
    INSTALL
    INSTALL.cmake
    dap.y
    oc.spec
    ocprint.1
    cmake
    CMakeLists.txt
    COMPILE.cmake.txt
    config.h.in.cmake
    cmake_uninstall.cmake.in
    oc-config-version.cmake.in
    oc-config.cmake.in
    FixBundle.cmake.in
    oc-config.in.cmake
    configure
    configure.ac
    install-sh
    config.h.in
    config.sub)
    


##################################################
##################################################

SET(liboc_SOURCES oc.c daplex.c dapparse.c daptab.c ocbytes.c occlientparams.c occompile.c occurlfunctions.c ocdata.c ocdebug.c ocdump.c ochttp.c ocinternal.c oclist.c oclog.c ocnode.c ocrc.c ocread.c ocuri.c ocutil.c xxdr.c)

add_library(oc ${LIB_TYPE} ${liboc_SOURCES}) 
IF(MOD_OC_NAME)
  SET_TARGET_PROPERTIES(oc PROPERTIES LIBRARY_OUTPUT_NAME ${OC_LIB_NAME})
  SET_TARGET_PROPERTIES(oc PROPERTIES ARCHIVE_OUTPUT_NAME ${OC_LIB_NAME})
  SET_TARGET_PROPERTIES(oc PROPERTIES RUNTIME_OUTPUT_NAME ${OC_LIB_NAME})
ENDIF()

FILE(GLOB CUR_EXTRA_DIST RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*.h ${CMAKE_CURRENT_SOURCE_DIR}/*.c )
SET(CUR_EXTRA_DIST ${CUR_EXTRA_DIST} dap.y)
SET(CUR_EXTRA_DIST ${CUR_EXTRA_DIST} CMakeLists.txt Makefile.am)
ADD_EXTRA_DIST("${CUR_EXTRA_DIST}")

IF(MOD_OC_NAME)
    SET_TARGET_PROPERTIES(oc PROPERTIES LIBRARY_OUTPUT_NAME ${OC_LIB_NAME})
    SET_TARGET_PROPERTIES(oc PROPERTIES ARCHIVE_OUTPUT_NAME ${OC_LIB_NAME})
    SET_TARGET_PROPERTIES(oc PROPERTIES RUNTIME_OUTPUT_NAME ${OC_LIB_NAME})
ENDIF()

SET_TARGET_PROPERTIES(oc PROPERTIES 
	VERSION ${oc_VERSION}
	SOVERSION ${oc_SO_VERSION})

#####
# Build doxygen documentation, if need be.
#####
IF(BUILD_DOCS)
    FIND_PACKAGE(Doxygen)
    IF(DOXYGEN_FOUND)
        FILE(GLOB COPY_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.html ${CMAKE_CURRENT_SOURCE_DIR}/images ${CMAKE_CURRENT_SOURCE_DIR}/*.doc ${CMAKE_CURRENT_SOURCE_DIR}/*.xml ${CMAKE_CURRENT_SOURCE_DIR}/*.m4 ${CMAKE_CURRENT_SOURCE_DIR}/*.texi)
        FILE(COPY ${COPY_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
        # Copy the image files used by markdown documentation 
        # manually.
        FILE(GLOB IMG_FILES ${CMAKE_CURRENT_BINARY_DIR}/images/*.jpg)
        CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
        ADD_CUSTOM_TARGET(doc ALL
            ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API Documentation with Doxygen" VERBATIM
        )
        FILE(COPY ${IMG_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/html/)
    ENDIF()
ENDIF()

SET(ocprint_SOURCES ocprint.c)

IF(USE_X_GETOPT)
	SET(ocprint_SOURCES ${ocprint_SOURCES} XGetopt.c)
ENDIF()

ADD_EXECUTABLE(ocprint ${ocprint_SOURCES} ${liboc_SOURCES})

TARGET_LINK_LIBRARIES(ocprint ${TLL_LIBS})
TARGET_LINK_LIBRARIES(oc ${TLL_LIBS})

print_conf_summary()

################################
# Install packages.
################################

# Install oc-config
INSTALL(PROGRAMS ${OC_BINARY_DIR}/oc-config DESTINATION bin COMPONENT utilities)

# Install liboc
INSTALL(
        TARGETS oc
        EXPORT oc-targets
        RUNTIME
        DESTINATION bin
        COMPONENT libraries 
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT libraries 
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT libraries 
    )

# Install ocprint
INSTALL(PROGRAMS ${OC_BINARY_DIR}/ocprint.exe DESTINATION bin COMPONENT utilities)

# Create CMake package configuration files. With these, other packages using
# cmake should be able to find oc using find_package and find_library.
install (EXPORT oc-targets
         DESTINATION share/cmake
         COMPONENT documentation)

INSTALL(FILES ${OC_SOURCE_DIR}/oc.h
    DESTINATION include COMPONENT headers)

# Install the dependencies.
IF(MSVC)
    INSTALL(DIRECTORY ${CMAKE_PREFIX_PATH} DESTINATION "deps" COMPONENT dependencies)
ENDIF()

IF(ENABLE_DOXYGEN)    
    INSTALL(DIRECTORY docs DESTINATION "." COMPONENT documentation)
ENDIF()

install (FILES ${OC_BINARY_DIR}/oc-config.cmake
        ${OC_BINARY_DIR}/oc-config-version.cmake
        DESTINATION share/cmake)

################################
# Set CTest Properties
################################

ENABLE_TESTING()
INCLUDE(CTest)

# Copy the CTest customization file into binary directory, as required.
FILE(COPY ${CMAKE_SOURCE_DIR}/CTestCustom.cmake DESTINATION ${CMAKE_BINARY_DIR})

# Set Memory test program for non-MSVC based builds.
# Assume valgrind for now.
IF(NOT MSVC)
  SET (CTEST_MEMORYCHECK_COMMAND valgrind CACHE STRING "")
ENDIF()

# Enable tests
IF(ENABLE_TESTS)
    ADD_SUBDIRECTORY(tests)
ENDIF()

# Set variable to define the build type.
INCLUDE(GenerateExportHeader)

################################
# End CTest Properties
################################

##################################################
##################################################

# CPack inclusion must come last.
INCLUDE(CPack)
