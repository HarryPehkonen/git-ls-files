# tests/CMakeLists.txt
add_executable(git_scanner_test git_scanner_test.cpp ../src/git_scanner.cpp)
target_link_libraries(git_scanner_test PRIVATE GTest::GTest GTest::Main)
add_test(NAME git_scanner_test COMMAND git_scanner_test)

