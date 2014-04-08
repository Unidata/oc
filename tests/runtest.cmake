#!cmake

SET(DEBUG "")

# For some reason, cmake is not distinguishing SOURCE and BINARY
SET(SRCDIR ${CMAKE_CURRENT_SOURCE_DIR})
SET(BINDIR ${CMAKE_CURRENT_BINARY_DIR})
SET(TOPBINDIR ${CMAKE_BINARY_DIR}/..)

SET(STDOUT ${BINDIR}/stdout)
SET(BASELINE ${SRCDIR}/baselines)
SET(TESTDATA ${SRCDIR}/testdata)
SET(WORKING ${SRCDIR})

# cygwin => fixup path
#if(CYGWIN)
  execute_process(COMMAND cygpath -m -l "${TESTDATA}" OUTPUT_VARIABLE TMP1)
#  execute_process(COMMAND cygpath -m -l "${BASELINE}" OUTPUT_VARIABLE TMP2)
  STRING(REGEX REPLACE "[\r\n]" "" TMP1 "${TMP1}")
#  STRING(REGEX REPLACE "[\r\n]" "" TMP2 "${TMP2}")
  SET(TESTDATA "${TMP1}")
#  SET(BASELINE "${TMP2}")
#endif()

SET(OCPRINT ${TOPBINDIR}/ocprint)

if("${LOCAL}" STREQUAL "1")
  SET(URL "file://${TESTDATA}")
else()
  SET(URL "http://thredds-test.ucar.edu/dts")
endif()

file(MAKE_DIRECTORY ${STDOUT})

# Translate WHAT
SET(EXT ${WHAT})
if("${EXT}" STREQUAL "datadds")
  SET(EXT "dods")
endif()

if(DEFINED DEBUG)
  message(STATUS "testdata=|${TESTDATA}|")
  message(STATUS "baseline=|${BASELINE}|")
  message(STATUS "local=${LOCAL}")
  message(STATUS "url=|${URL}|")
  message(STATUS "cmd: ${OCPRINT} -T -p ${WHAT} ${URL}/${TEST}.${EXT}")
endif()

execute_process(COMMAND ${OCPRINT} -T -p ${WHAT} ${URL}/${TEST}.${EXT}
                OUTPUT_FILE ${STDOUT}/${TEST}.${EXT}
                RESULT_VARIABLE HAD_ERROR)
if(HAD_ERROR)
    message(STATUS "ocprint failed")
endif()

if(NOT MSVC OR CYGWIN)
    if(DEFINED DEBUG)
      message(STATUS "diff -wb ${BASELINE}/${TEST}.${EXT} ${STDOUT}/${TEST}.${EXT}")
    endif()
    execute_process(COMMAND diff "-wb" ${BASELINE}/${TEST}.${EXT} ${STDOUT}/${TEST}.${EXT}
                    RESULT_VARIABLE DIFFERENT)
    if(DIFFERENT)
      message(FATAL_ERROR "***FAIL: files differ")
    else()
      MESSAGE(STATUS "***PASSED")
    endif()
endif()
