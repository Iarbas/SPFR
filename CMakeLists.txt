cmake_minimum_required (VERSION 3.11)
project (SPFR)
# ------------------------------------------------- Project Settings --------------------------------------------------
# Get the absolute path to the parent directory.
get_filename_component(PARENT_DIR ${PROJECT_SOURCE_DIR} DIRECTORY)

message(STATUS "Installation path of the binaries, libs and headers: " ${PARENT_DIR})

# Define the locations all necessary files.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PARENT_DIR}/build/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PARENT_DIR}/build/lib)
set(CMAKE_HEADERS_OUTPUT_DIRECTORY ${PARENT_DIR}/build/lib/include)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PARENT_DIR}/build/bin)

# C++ options.
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_REQUIRED_FLAGS -std=c++17 -lstdc++fs)

# Standard directory for the config files.
set (CONFIG_DIRECTORY ${PARENT_DIR}/build/configs)

# Configure a header file to pass some of the CMake settings to the source code
configure_file ( test/cmake_config.h.in ${CMAKE_HEADERS_OUTPUT_DIRECTORY}/cmake_config.h ) 

# ----------------------------------------------- Program Configuration -----------------------------------------------
include_directories(include
                    ${CMAKE_HEADERS_OUTPUT_DIRECTORY})

# %%%%%%% Library %%%%%%%
ADD_LIBRARY( ${PROJECT_NAME} STATIC src/readConfig.cpp )
target_link_libraries( ${PROJECT_NAME} stdc++fs )
add_dependencies( ${PROJECT_NAME} copy_headers_reader )

# %%%%%%% Executable %%%%%%%
add_executable(${PROJECT_NAME}_test test/test.cpp)
set_property(TARGET ${PROJECT_NAME}_test PROPERTY CXX_STANDARD 17)
set_property(TARGET ${PROJECT_NAME}_test PROPERTY CXX_STANDARD_REQUIRED ON)
target_link_libraries(${PROJECT_NAME}_test ${PROJECT_NAME} )

add_dependencies( ${PROJECT_NAME}_test ${PROJECT_NAME} copy_config)

# ----------------------------------------------------- Aftermath -----------------------------------------------------
# Create the additional folders to organize the configs in a better way.
add_custom_target(create_folders)

add_custom_command(
        TARGET create_folders PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_HEADERS_OUTPUT_DIRECTORY}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CONFIG_DIRECTORY})

# Copy the headers in the projects own file managment. 
add_custom_target( copy_headers_reader )
add_dependencies( copy_headers_reader create_folders )

add_custom_command(
        TARGET copy_headers_reader PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy include/readConfig.hpp ${CMAKE_HEADERS_OUTPUT_DIRECTORY}/)
        
# Copy the config file in the projects own file managment. 
add_custom_target(copy_config)
add_dependencies( copy_config create_folders )

add_custom_command(
        TARGET copy_config PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_CURRENT_LIST_DIR}/example_config/test.config ${CONFIG_DIRECTORY}/test.config)
