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

FetchContent_Declare(
        SDL2
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG        release-2.28.5
)

FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG        v1.91.0
)

FetchContent_Declare(
        freetype
        GIT_REPOSITORY https://github.com/freetype/freetype.git
        GIT_TAG        VER-2-14-2
)

set(FT_DISABLE_ZLIB      ON CACHE BOOL "" FORCE)
set(FT_DISABLE_BZIP2     ON CACHE BOOL "" FORCE)
set(FT_DISABLE_PNG       ON CACHE BOOL "" FORCE)
set(FT_DISABLE_HARFBUZZ  ON CACHE BOOL "" FORCE)
set(FT_DISABLE_BROTLI    ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(pugixml SDL2 imgui freetype)

# --- ImGui static library ---
add_library(imgui_lib STATIC
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl2.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
        ${imgui_SOURCE_DIR}/misc/freetype/imgui_freetype.cpp
)

target_include_directories(imgui_lib PUBLIC
        ${imgui_SOURCE_DIR}
        ${imgui_SOURCE_DIR}/backends
)
target_include_directories(imgui_lib PRIVATE
        ${freetype_SOURCE_DIR}/include
)
target_compile_definitions(imgui_lib PUBLIC IMGUI_ENABLE_FREETYPE)
target_link_libraries(imgui_lib PUBLIC SDL2::SDL2)
target_link_libraries(imgui_lib PRIVATE freetype)

# --- Sources ---
set(SOURCES
        src/main.cpp
        src/gui/OniForgeApp.cpp
        src/gui/OniForgeTheme.cpp
        src/gui/OniGuiRenderer.cpp
        src/gui/views/ONCCView.cpp
        src/gui/views/ONCVView.cpp
        src/gui/views/TRACView.cpp
        src/gui/views/AddFileModal.cpp
        src/component/logger/Logger.cpp
        src/component/xml/XmlDocument.cpp
        src/component/xml/XmlReader.cpp
        src/component/xml/XmlWriter.cpp
        src/repository/ONCCRepository.cpp
        src/repository/ONCVRepository.cpp
        src/repository/TRACRepository.cpp
        src/service/ProjectCatalogService.cpp
        src/service/VanillaCatalogService.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

if (MINGW)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wa,-mbig-obj)
endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE SDL_MAIN_HANDLED)

target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/assets
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        pugixml
        imgui_lib
        SDL2::SDL2
        opengl32
)