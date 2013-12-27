cmake_minimum_required(VERSION 2.8)
include(GNUInstallDirs)

# Include build policy from directories
add_subdirectory(src)
add_subdirectory(include)

# Provide build options to the user
option(WANT_SHARED "Should a shared library be built?" ON)
option(WANT_STATIC "Should a static library be built?" ON)

if (WANT_STATIC)
  option(WANT_DEMO "Should the demo be build?" ON)
endif()
