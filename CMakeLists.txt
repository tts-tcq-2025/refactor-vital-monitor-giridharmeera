cmake_minimum_required(VERSION 3.14)
project(bms-monitor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()

# ---- Library from src/*.cpp (NEW) ----
file(GLOB LIB_SOURCES CONFIGURE_DEPENDS "src/*.cpp")
add_library(bms-monitor-lib ${LIB_SOURCES})
target_include_directories(
  bms-monitor-lib
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}  
)

# ---- Tests: compile ONLY test sources; DO NOT include main.cpp ----
# If your test file is test-monitor.cpp:
set(TEST_SOURCES test-monitor.cpp)

add_executable(test-monitor ${TEST_SOURCES})
target_link_libraries(test-monitor PRIVATE bms-monitor-lib GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(test-monitor)

# ---- Optional: build demo from main.cpp (kept separate from tests) ----
add_executable(demo main.cpp)
target_link_libraries(demo PRIVATE bms-monitor-lib)
