find_package(PythonExtensions REQUIRED)

execute_process(COMMAND "interrogate" "-module geomstore" "-oc geomstore.cxx" 
  "-od geomstore.in" "-python-native" "-I/usr/include/panda3d/" 
  "-S/usr/include/panda3d/parser-inc/" "-DCPPPARSER" 
  "-D__STDC__=1 -D__cplusplus=201103L" "-do-module" geomstore.cpp
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})

add_library(geomstore_lib OBJECT triangle.cpp vertex.cpp common.cpp)
add_library(_geomstore MODULE $<TARGET_OBJECTS:geomstore_lib> geomstore.cxx)
python_extension_module(_geomstore)
install(TARGETS _geomstore LIBRARY DESTINATION geomstore)