find_package(xtensor REQUIRED)
include_directories(${xtensor_INCLUDE_DIRS})
set(LIBRARIES "${LIBRARIES};${xtensor_LIBRARIES}")


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