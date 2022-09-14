cmake_minimum_required(VERSION 3.0.0)
project(base_weak_ptr VERSION 0.1.0)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)

add_executable(base_weak_ptr main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

# unit test
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  weak_ptr_unittest
  weak_ptr_unittest.cc
)

target_link_libraries(
  weak_ptr_unittest
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(weak_ptr_unittest)
