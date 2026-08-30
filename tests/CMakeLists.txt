set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)

add_subdirectory(${CMAKE_SOURCE_DIR}/third-party/googletest
                    ${CMAKE_CURRENT_BINARY_DIR}/build_gtest)

add_executable(run_tests tests.cpp)

target_link_libraries(run_tests PRIVATE gtest_main relativity_lib)

include(GoogleTest)
gtest_discover_tests(run_tests)
