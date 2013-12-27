file(GLOB TERMBOX_SRCS *.c)

include_directories(../include)

if (WANT_STATIC)
  add_library(termbox_static STATIC ${TERMBOX_SRCS})
  install(TARGETS termbox_static ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
endif()

if (WANT_SHARED)
  add_library(termbox_shared SHARED ${TERMBOX_SRCS})
  install(TARGETS termbox_shared
    ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
endif()

if (WANT_DEMO)
  add_subdirectory(demo)
endif()
