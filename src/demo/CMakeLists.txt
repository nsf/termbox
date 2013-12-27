
add_executable(demo_keyboard keyboard.c)
target_link_libraries(demo_keyboard termbox_static)

add_executable(demo_output output.c)
target_link_libraries(demo_output termbox_static)
