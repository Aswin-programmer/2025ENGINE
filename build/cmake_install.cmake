# Install script for directory: C:/Aswin_Game_DEV/OPENGL_TUTORIAL/GetIntoGameDevOPENGL/2025ENGINE/2025ENGINE

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/2025ENGINE_ROOT")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set path to fallback-tool for dependency-resolution.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "C:/Program Files/LLVM-21/bin/llvm-objdump.exe")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Aswin_Game_DEV/OPENGL_TUTORIAL/GetIntoGameDevOPENGL/2025ENGINE/2025ENGINE/build/DEPENDENCIES/FLECS/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Aswin_Game_DEV/OPENGL_TUTORIAL/GetIntoGameDevOPENGL/2025ENGINE/2025ENGINE/build/DEPENDENCIES/GLFW/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Aswin_Game_DEV/OPENGL_TUTORIAL/GetIntoGameDevOPENGL/2025ENGINE/2025ENGINE/build/DEPENDENCIES/GLAD/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Aswin_Game_DEV/OPENGL_TUTORIAL/GetIntoGameDevOPENGL/2025ENGINE/2025ENGINE/build/DEPENDENCIES/GLM/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Aswin_Game_DEV/OPENGL_TUTORIAL/GetIntoGameDevOPENGL/2025ENGINE/2025ENGINE/build/DEPENDENCIES/KTX/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Aswin_Game_DEV/OPENGL_TUTORIAL/GetIntoGameDevOPENGL/2025ENGINE/2025ENGINE/build/DEPENDENCIES/TINYGLTF/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Aswin_Game_DEV/OPENGL_TUTORIAL/GetIntoGameDevOPENGL/2025ENGINE/2025ENGINE/build/DEPENDENCIES/REACTPHYSICS3D/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Aswin_Game_DEV/OPENGL_TUTORIAL/GetIntoGameDevOPENGL/2025ENGINE/2025ENGINE/build/DEPENDENCIES/GLYPHGL/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Aswin_Game_DEV/OPENGL_TUTORIAL/GetIntoGameDevOPENGL/2025ENGINE/2025ENGINE/build/DEPENDENCIES/CEREAL/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Aswin_Game_DEV/OPENGL_TUTORIAL/GetIntoGameDevOPENGL/2025ENGINE/2025ENGINE/build/SCRIPT/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Aswin_Game_DEV/OPENGL_TUTORIAL/GetIntoGameDevOPENGL/2025ENGINE/2025ENGINE/build/ENGINE/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Aswin_Game_DEV/OPENGL_TUTORIAL/GetIntoGameDevOPENGL/2025ENGINE/2025ENGINE/build/APP/cmake_install.cmake")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "C:/Aswin_Game_DEV/OPENGL_TUTORIAL/GetIntoGameDevOPENGL/2025ENGINE/2025ENGINE/build/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
if(CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_COMPONENT MATCHES "^[a-zA-Z0-9_.+-]+$")
    set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
  else()
    string(MD5 CMAKE_INST_COMP_HASH "${CMAKE_INSTALL_COMPONENT}")
    set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INST_COMP_HASH}.txt")
    unset(CMAKE_INST_COMP_HASH)
  endif()
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "C:/Aswin_Game_DEV/OPENGL_TUTORIAL/GetIntoGameDevOPENGL/2025ENGINE/2025ENGINE/build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
