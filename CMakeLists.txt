# Root CMakeLists.txt
cmake_minimum_required(VERSION 3.20)
project(git_files_scanner VERSION 1.0.0 LANGUAGES CXX)

# Main executable
add_executable(git_files_scanner)

# Include helper modules
include(cmake/CompilerOptions.cmake)
include(cmake/Dependencies.cmake)
include(cmake/Install.cmake)
include(cmake/Tests.cmake)

target_sources(git_files_scanner
    PRIVATE
        src/main.cpp
        src/git_scanner.cpp
)

# Link libraries and set properties
#target_link_libraries(git_files_scanner PRIVATE git2)
set_target_properties(git_files_scanner PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

# Enable testing
enable_testing()
add_subdirectory(tests)
