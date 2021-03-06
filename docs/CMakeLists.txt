#####
# Build doxygen documentation, if need be.
#####
IF(ENABLE_DOXYGEN)
  # The following is redundant but does not hurt anything.

  FILE(GLOB COPY_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.html ${CMAKE_CURRENT_SOURCE_DIR}/images ${CMAKE_CURRENT_SOURCE_DIR}/*.doc ${CMAKE_CURRENT_SOURCE_DIR}/*.xml ${CMAKE_CURRENT_SOURCE_DIR}/*.m4 ${CMAKE_CURRENT_SOURCE_DIR}/*.texi)
  FILE(COPY ${COPY_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)

  # Set abs_top_srcdir to work with the autotools
  # doxyfile template.
  SET(abs_top_srcdir ${CMAKE_SOURCE_DIR})
  SET(abs_builddir ${CMAKE_CURRENT_BINARY_DIR})

  # Create general and guide doxyfiles.
  CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
    ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)

  # Generate User Documentation
  ADD_CUSTOM_TARGET(doc_all ALL
    ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating API Documentation" VERBATIM)

  # If ENABLE_DOXYGEN_LATEX_OUTPUT is true, automatically build
  # the PDF files.
  IF(ENABLE_DOXYGEN_PDF_OUTPUT)
    # Process 'main' netcdf documentation.
    FIND_PROGRAM(NC_MAKE NAMES make)
    FIND_PROGRAM(NC_PDFLATEX NAMES pdflatex)
    IF(NOT NC_MAKE OR NOT NC_PDFLATEX)
      MESSAGE(WARNING "Unable to locate 'make' and/or 'pdflatex' program.  Unable to build pdf documentation.")
    ELSE()
      ADD_CUSTOM_TARGET(doc_all_pdf ALL
        make
        COMMAND mv refman.pdf ../netcdf.pdf
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/latex_main"
        COMMENT "Building NetCDF PDF Documentation" VERBATIM
        )

      INSTALL(FILES "${CMAKE_CURRENT_BINARY_DIR}/netcdf.pdf"
	"${CMAKE_CURRENT_BINARY_DIR}/netcdf-guide.pdf"
	"${CMAKE_CURRENT_BINARY_DIR}/netcdf-tutorial.pdf"
	DESTINATION "${CMAKE_INSTALL_DOCDIR}/pdf"
	COMPONENT documentation)

    ENDIF()

  ENDIF()


  # Copy the image files used by markdown documentation
  # manually.
  FILE(GLOB IMG_FILES ${CMAKE_CURRENT_BINARY_DIR}/images/*.jpg
    ${CMAKE_CURRENT_BINARY_DIR}/images/*.png)

  FILE(COPY ${IMG_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/html)

  INSTALL(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/html/"
    DESTINATION "${CMAKE_INSTALL_DOCDIR}/html"
    COMPONENT documentation)

ENDIF(ENABLE_DOXYGEN)

SET(CUR_EXTRA_DIST ${CUR_EXTRA_DIST} CMakeLists.txt Makefile.am netcdf.m4 DoxygenLayout.xml Doxyfile.in Doxyfile.guide.in footer.html mainpage.dox tutorial.dox dispatch.dox guide.dox types.dox notes.md cdl.dox architecture.dox internal.dox install-fortran.dox Doxyfile.in.cmake windows-binaries.md building-with-cmake.md install.md ocauth.html auth.html.in)

ADD_EXTRA_DIST("${CUR_EXTRA_DIST}")

# When auth.template is changed, you should generate ocauth.html
# using the following process. Note that we do not
# do this automatically because a windows machine may not have 'sed'
ADD_CUSTOM_TARGET(build_auth ALL
    rm -f ocauth.html
    cat ${CMAKE_SOURCE_DIR}/docs/auth.html.in | sed -e '/<NC/d' | sed -e 's|^<OC>||' | sed -e 's|zz|oc|g' -e 's|ZZ|OC|g' > ${CMAKE_SOURCE_DIR}/docs/ocauth.html
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    VERBATIM
    )

