cmake_minimum_required(VERSION 3.5)
project(aoi VERSION 0.0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rd/spdlog)


set(Boost_USE_MULTITHREADED TRUE)
find_package(Boost 1.71 REQUIRED COMPONENTS system filesystem coroutine)

set(AOI_SRCS
  ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/log.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/task.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/work_group.cc
  )

add_executable(aoi ${AOI_SRCS})

target_include_directories(aoi
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}/3rd/subprocess
  )

target_compile_options(aoi PRIVATE $<$<CXX_COMPILER_ID:GNU>:-Wextra>)

target_link_libraries(aoi
  PUBLIC
  pthread spdlog::spdlog
  PRIVATE
  Boost::system
  Boost::filesystem
  Boost::coroutine
  )
