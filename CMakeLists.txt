cmake_minimum_required(VERSION 3.5)
enable_testing()

# Definitions
set(PROJECT_NAME_STR order_book)
set(BIN_DIR "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BIN_DIR}")

# Project title and define versioning
project(${PROJECT_NAME_STR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Include paths
set(ORDER_BOOK_INCLUDES ${ORDER_BOOK_INCLUDES} ${PROJECT_SOURCE_DIR}/src)
set(ORDER_BOOK_INCLUDES ${ORDER_BOOK_INCLUDES} ${PROJECT_SOURCE_DIR}/orderbook)

# Gather all sources except the main entry point
file(GLOB_RECURSE INC_FILES src/*/*.hpp)
file(GLOB_RECURSE SRC_FILES src/*/*.cc)

# Dependencies
find_package(Boost 1.72.0 COMPONENTS filesystem system unit_test_framework REQUIRED)

# Unit tests
add_subdirectory(test)

# Assign the include directories
include_directories(${Boost_INCLUDE_DIRS})

# Build
set(SOURCE_FILES src/main.cpp ${INC_FILES} ${SRC_FILES})
add_executable(order_book ${SOURCE_FILES})
target_link_libraries(order_book ${Boost_LIBRARIES})
