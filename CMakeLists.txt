cmake_minimum_required(VERSION 3.5)
project(HiddenLayers)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

add_subdirectory("external/glad")
add_subdirectory("external/SDL" EXCLUDE_FROM_ALL)
add_subdirectory("external/SDL_net" EXCLUDE_FROM_ALL)

file(GLOB SOURCES "src/*.c")
add_executable(${PROJECT_NAME} WIN32 ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE "include")
target_link_libraries(${PROJECT_NAME} glad SDL3::SDL3 SDL3_net::SDL3_net)
add_custom_command(
  TARGET ${PROJECT_NAME} PRE_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different "${CMAKE_SOURCE_DIR}/resources" "${CMAKE_BINARY_DIR}/$<CONFIGURATION>"
)

