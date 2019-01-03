find_package(xtensor REQUIRED)
include_directories(${xtensor_INCLUDE_DIRS})
set(LIBRARIES "${LIBRARIES};${xtensor_LIBRARIES}")
