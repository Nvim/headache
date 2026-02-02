cmake_minimum_required(VERSION 3.30)

project(headache LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

add_library(lib INTERFACE)
target_compile_features(lib INTERFACE cxx_std_23)
target_compile_options(lib INTERFACE
	-pedantic
	-Wall
	-Wpedantic
	-Werror
	-Wcast-qual
	-Wconversion-null
	-Wmissing-declarations
	-Woverlength-strings
	-Wpointer-arith
	-Wunused-local-typedefs
	-Wunused-result
	-Wvarargs
	-Wvla
	-Wwrite-strings
	-Wextra
)

Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.12.0
)

FetchContent_MakeAvailable(Catch2)

add_executable(any_test "${PROJECT_SOURCE_DIR}/src/any_test.cpp")
target_include_directories(any_test PUBLIC "${PROJECT_SOURCE_DIR}/src")
target_link_libraries(any_test PRIVATE lib Catch2::Catch2WithMain)

add_executable(shared_ptr_test "${PROJECT_SOURCE_DIR}/src/shared_ptr_test.cpp")
target_include_directories(shared_ptr_test PUBLIC "${PROJECT_SOURCE_DIR}/src")
target_link_libraries(shared_ptr_test PRIVATE lib Catch2::Catch2WithMain)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)
catch_discover_tests(any_test)
catch_discover_tests(shared_ptr_test)
