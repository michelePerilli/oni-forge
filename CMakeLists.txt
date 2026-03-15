cmake_minimum_required(VERSION 3.25...3.31)
project(OniForge VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Dependency Management ---
include(FetchContent)

FetchContent_Declare(
        pugixml
        GIT_REPOSITORY https://github.com/zeux/pugixml.git
        GIT_TAG        v1.14
)

# Download and provide the dependency
FetchContent_MakeAvailable(pugixml)

# --- Project Configuration ---
include_directories(include)

set(SOURCES
        src/OniForgeApp.cpp
        src/component/logger/Logger.cpp
        src/component/xml/XmlDocument.cpp
        src/component/xml/XmlReader.cpp
        src/component/xml/XmlWriter.cpp
        src/repository/TRACRepository.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)

# Link the downloaded library to your executable
target_link_libraries(${PROJECT_NAME} PRIVATE pugixml)