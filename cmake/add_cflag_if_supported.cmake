include(CheckCCompilerFlag)

function(add_cflag_if_supported flag)
    string(REPLACE "-" "_" flagclean ${flag})
    string(REPLACE "=" "_" flagclean ${flagclean})
    string(REPLACE "," "_" flagclean ${flagclean})
    string(REPLACE "+" "p" flagclean ${flagclean})
    string(TOUPPER ${flagclean} flagclean)

    check_c_compiler_flag(${flag} C_SUPPORTS${flagclean})

    if (C_SUPPORTS${flagclean})
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${flag}" PARENT_SCOPE)
    endif()
endfunction()
