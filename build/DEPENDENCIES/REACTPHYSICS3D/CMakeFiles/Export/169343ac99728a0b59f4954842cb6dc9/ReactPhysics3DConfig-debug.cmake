#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "ReactPhysics3D::ReactPhysics3D" for configuration "Debug"
set_property(TARGET ReactPhysics3D::ReactPhysics3D APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(ReactPhysics3D::ReactPhysics3D PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/reactphysics3d.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/reactphysics3d.dll"
  )

list(APPEND _cmake_import_check_targets ReactPhysics3D::ReactPhysics3D )
list(APPEND _cmake_import_check_files_for_ReactPhysics3D::ReactPhysics3D "${_IMPORT_PREFIX}/lib/reactphysics3d.lib" "${_IMPORT_PREFIX}/bin/reactphysics3d.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
