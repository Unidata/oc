INCLUDE(BundleUtilities)

# Set bundle to the full path name of the executable

SET (BUNDLE "${CMAKE_INSTALL_PREFIX}/ncdump.exe")

SET (OTHER_LIBS "")

SET (DIRS "")

fixup_bundle("${BUNDLE}" "${OTHER_LIBS}" "${DIRS}")
