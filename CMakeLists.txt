cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(imaginary-time-event-chain CXX)

find_package(ALPS REQUIRED PATHS ${ALPS_ROOT_DIR} $ENV{ALPS_HOME} NO_SYSTEM_ENVIRONMENT_PATH)
message(STATUS "Found ALPS: ${ALPS_ROOT_DIR} (revision: ${ALPS_VERSION})")
include(${ALPS_USE_FILE})

add_executable(imaginary-time-event-chain main.cpp)
target_link_libraries(imaginary-time-event-chain ${ALPS_LIBRARIES})
add_alps_test(imaginary-time-event-chain)
