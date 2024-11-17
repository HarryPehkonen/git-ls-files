# cmake/Install.cmake
include(GNUInstallDirs)

install(TARGETS git_files_scanner
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
