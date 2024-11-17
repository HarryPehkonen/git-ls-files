# cmake/CompilerOptions.cmake
function(set_compiler_options target)
    if(MSVC)
        target_compile_options(${target} PRIVATE /W4 /WX)
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra -Werror -pedantic)
    endif()
    
    # Enable static linking
    if(UNIX)
        target_link_options(${target} PRIVATE -static)
    endif()
endfunction()

set_compiler_options(git_files_scanner)
