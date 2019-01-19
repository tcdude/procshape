SET (SEARCH_PATHS 
  /usr/local/
  /usr/
  "${CMAKE_SOURCE_DIR}/OpenMesh/src/OpenMesh"
  "${CMAKE_SOURCE_DIR}/libs_required/OpenMesh/src/OpenMesh"
  "${CMAKE_SOURCE_DIR}/../OpenMesh/src/OpenMesh"
  "C:/Program Files/OpenMesh 8.0"
  "C:/Program Files/OpenMesh 7.2"
  "C:/Program Files/OpenMesh 7.1"
  "C:/Program Files/OpenMesh 7.0"
  "C:/Program Files/OpenMesh 6.3"
  "C:/Program Files/OpenMesh 6.2"
  "C:/Program Files/OpenMesh 6.1"
  "C:/Program Files/OpenMesh 6.0"
  "C:/Program Files/OpenMesh 5.2"
  "C:/Program Files/OpenMesh 5.1"
  "C:/Program Files/OpenMesh 5.0"
  "C:/Program Files/OpenMesh 4.2"
  "C:/Program Files/OpenMesh 4.1"
  "C:/Program Files/OpenMesh 4.0"
  "C:/Program Files/OpenMesh 3.4"
  "C:/Program Files/OpenMesh 3.3"
  "C:/Program Files/OpenMesh 3.2"
  "C:/Program Files/OpenMesh 3.1"
  "C:/Program Files/OpenMesh 3.0"
  "C:/Program Files/OpenMesh 2.4.1"
  "C:/Program Files/OpenMesh 2.4"
  "C:/Program Files/OpenMesh 2.0/include"
  "C:/libs/OpenMesh 8.0"
  "C:/libs/OpenMesh 7.1"
  "C:/libs/OpenMesh 7.0"
  "C:/libs/OpenMesh 6.3"
  "C:/libs/OpenMesh 6.2"
  "C:/libs/OpenMesh 6.1"
  "C:/libs/OpenMesh 6.0"
  "C:/libs/OpenMesh 5.2"
  "C:/libs/OpenMesh 5.1"
  "C:/libs/OpenMesh 5.0"
  "C:/libs/OpenMesh 4.2"
  "C:/libs/OpenMesh 4.1"
  "C:/libs/OpenMesh 4.0"
  "C:/libs/OpenMesh 3.4"
  "C:/libs/OpenMesh 3.3"
  "C:/libs/OpenMesh 3.2"
  "C:/libs/OpenMesh 3.1"
  "C:/libs/OpenMesh 3.0"
  "C:/libs/OpenMesh 2.4.1"
  "C:/libs/OpenMesh 2.4"
  "${OPENMESH_LIBRARY_DIR}"
)

FIND_PATH (OPENMESH_INCLUDE_DIR OpenMesh/Core/Mesh/PolyMeshT.hh
  PATHS ${SEARCH_PATHS}
  PATH_SUFFIXES include)

FIND_LIBRARY(OPENMESH_CORE_LIBRARY_RELEASE NAMES OpenMeshCore
  PATHS ${SEARCH_PATHS}
  PATH_SUFFIXES lib lib64)

FIND_LIBRARY(OPENMESH_CORE_LIBRARY_DEBUG NAMES OpenMeshCored 
  PATHS ${SEARCH_PATHS}
  PATH_SUFFIXES lib lib64)

FIND_LIBRARY(OPENMESH_TOOLS_LIBRARY_RELEASE NAMES OpenMeshTools
  PATHS ${SEARCH_PATHS}
  PATH_SUFFIXES lib lib64)

FIND_LIBRARY(OPENMESH_TOOLS_LIBRARY_DEBUG NAMES OpenMeshToolsd
  PATHS ${SEARCH_PATHS}
  PATH_SUFFIXES lib lib64)

#select configuration depending on platform (optimized... on windows)
include(SelectLibraryConfigurations)
select_library_configurations( OPENMESH_TOOLS )
select_library_configurations( OPENMESH_CORE )

set(OPENMESH_LIBRARIES ${OPENMESH_CORE_LIBRARY} ${OPENMESH_TOOLS_LIBRARY} )
set(OPENMESH_INCLUDE_DIRS ${OPENMESH_INCLUDE_DIR} )

#checks, if OPENMESH was found and sets OPENMESH_FOUND if so
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(OpenMesh  DEFAULT_MSG
                                  OPENMESH_CORE_LIBRARY OPENMESH_TOOLS_LIBRARY OPENMESH_INCLUDE_DIR)

#sets the library dir 
if ( OPENMESH_CORE_LIBRARY_RELEASE )
  get_filename_component(_OPENMESH_LIBRARY_DIR ${OPENMESH_CORE_LIBRARY_RELEASE} PATH)
else( OPENMESH_CORE_LIBRARY_RELEASE )
  get_filename_component(_OPENMESH_LIBRARY_DIR ${OPENMESH_CORE_LIBRARY_DEBUG} PATH)
endif( OPENMESH_CORE_LIBRARY_RELEASE )
set (OPENMESH_LIBRARY_DIR "${_OPENMESH_LIBRARY_DIR}" CACHE PATH "The directory where the OpenMesh libraries can be found.")


mark_as_advanced(OPENMESH_INCLUDE_DIR OPENMESH_CORE_LIBRARY_RELEASE OPENMESH_CORE_LIBRARY_DEBUG OPENMESH_TOOLS_LIBRARY_RELEASE OPENMESH_TOOLS_LIBRARY_DEBUG OPENMESH_LIBRARY_DIR)
set(LIBRARIES "${LIBRARIES};${OPENMESH_LIBRARIES}")


# find_package(xtensor REQUIRED)
# include_directories(${xtensor_INCLUDE_DIRS})
# set(LIBRARIES "${LIBRARIES};${xtensor_LIBRARIES}")


# Find numpy
# if( NOT PYTHONINTERP_FOUND )
#   find_package(PythonInterp 3.5 REQUIRED)
#  endif( NOT PYTHONINTERP_FOUND )
# 
#  exec_program(${PYTHON_EXECUTABLE}
#              ARGS "-c \"import numpy; print(numpy.get_include())\""
#              OUTPUT_VARIABLE NUMPY_INCLUDE_DIR
#              RETURN_VALUE NUMPY_NOT_FOUND
#             )

# if (NUMPY_NOT_FOUND)
#   message (FATAL_ERROR "Numpy package/header is missing")
# else (NUMPY_NOT_FOUND)
#     message (STATUS "Numpy headers found: ${NUMPY_INCLUDE_DIR}")
# endif (NUMPY_NOT_FOUND)
# include_directories("${NUMPY_INCLUDE_DIR}")