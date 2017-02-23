cmake_minimum_required(VERSION 2.8)
project(libtermbox C)

include(cmake/add_cflag_if_supported.cmake)

add_cflag_if_supported("-std=gnu99")
add_cflag_if_supported("-Wall")
add_cflag_if_supported("-Wextra")

# Must nog have errors
add_cflag_if_supported("-Werror")

# Source fortification
add_cflag_if_supported("-D_FORTIFY_SOURCE=2")
add_cflag_if_supported("-fstrict-aliasing=1")
add_cflag_if_supported("-Wl,-z,relro")
add_cflag_if_supported("-Wl,-z,now")
add_cflag_if_supported("-fPIC")

# Extra warnings
add_cflag_if_supported("-Wshadow")
add_cflag_if_supported("-Wformat")
add_cflag_if_supported("-Wformat-security")
add_cflag_if_supported("-Wno-sign-conversion")
#add_cflag_if_supported("-Wconversion")
add_cflag_if_supported("-Wmisleading-indentation")
add_cflag_if_supported("-Wno-float-conversion")
add_cflag_if_supported("-Wmaybe-uninitialized")
#add_cflag_if_supported("-Wmissing-variable-declarations")
#add_cflag_if_supported("-Wshorten-64-to-32")
add_cflag_if_supported("-Wimplicit-function-declaration")
add_cflag_if_supported("-Wredundant-decls")
add_cflag_if_supported("-Wundef")
add_cflag_if_supported("-Wswitch")
add_cflag_if_supported("-Wfloat-equal")

if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
	add_cflag_if_supported("-Og")
	add_cflag_if_supported("-g")
elseif()
	add_cflag_if_supported("-O3")
endif()

add_definitions(-D_XOPEN_SOURCE)

set(SRC src/termbox.c src/utf8.c)
#include_directories(src)

add_library(${PROJECT_NAME}-static STATIC ${SRC})
set_target_properties(${PROJECT_NAME}-static PROPERTIES OUTPUT_NAME ${PROJECT_NAME} PREFIX "")

add_library(${PROJECT_NAME}-shared SHARED ${SRC})
set_target_properties(${PROJECT_NAME}-shared PROPERTIES OUTPUT_NAME ${PROJECT_NAME} PREFIX "")

file(GLOB DEMOS src/demo/*.c)
foreach(DEMO ${DEMOS})
	get_filename_component(DEMOEXE ${DEMO} NAME_WE)
	add_executable(${DEMOEXE} ${DEMO})
	add_dependencies(${DEMOEXE} ${PROJECT_NAME}-static)
	target_link_libraries(${DEMOEXE} ${PROJECT_NAME}-static)
endforeach()

install(TARGETS ${PROJECT_NAME}-static ${PROJECT_NAME}-shared
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)

install(FILES src/termbox.h
	DESTINATION include
)
