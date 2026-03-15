cmake_minimum_required(VERSION 3.25...3.31)
project(OniForge VERSION 0.1.0 LANGUAGES CXX)

# Force C++26 standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add the 'include' directory to the compiler search path
# This allows: #include "OniParser.hpp" instead of "../../include/OniParser.hpp"
include_directories(include)

# Define the source files for the project
set(SOURCES
        src/OniForgeApp.cpp
)

# Create the executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Print a message when the configuration is successful
message(STATUS "Configuration successful for ${PROJECT_NAME} (v${PROJECT_VERSION})")