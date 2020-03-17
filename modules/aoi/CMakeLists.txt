cmake_minimum_required(VERSION 3.5)
project(aoi VERSION 0.0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)


add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rd/spdlog)

set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rd/json)

set(Boost_USE_MULTITHREADED TRUE)
find_package(Boost 1.71 REQUIRED COMPONENTS system filesystem coroutine)

set(AOI_SRCS
  ${CMAKE_CURRENT_SOURCE_DIR}/src/log.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/task.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/work_group.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/injector.cc
  )

add_library(aoi ${AOI_SRCS})

add_executable(aoi_test ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cc)

target_include_directories(aoi
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/3rd/json/include
  #${CMAKE_CURRENT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}/3rd/subprocess
  )

target_include_directories(aoi_test
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/3rd/json/include
  #${CMAKE_CURRENT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}/3rd/subprocess
  )

target_compile_options(aoi PRIVATE $<$<CXX_COMPILER_ID:GNU>:-Wextra>)

target_link_libraries(aoi
  PRIVATE
  pthread
  spdlog::spdlog
  nlohmann_json::nlohmann_json
  Boost::system
  Boost::filesystem
  Boost::coroutine
  )

target_link_libraries(aoi_test
  PRIVATE
  aoi
  spdlog::spdlog
  nlohmann_json::nlohmann_json
  )
