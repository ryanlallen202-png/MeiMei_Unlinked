cmake_minimum_required(VERSION 3.15)
project(MeiMei_Client VERSION 5.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Auto-download ImGui during build (No submodule needed)
include(FetchContent)
FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG v1.90.4
)
FetchContent_MakeAvailable(imgui)

set(IMGUI_DIR ${imgui_SOURCE_DIR})

file(GLOB IMGUI_SOURCES
    ${IMGUI_DIR}/*.cpp
    ${IMGUI_DIR}/backends/imgui_impl_win32.cpp
    ${IMGUI_DIR}/backends/imgui_impl_dx11.cpp
)

add_library(MeiMei_Client SHARED
    MeiMei_Client_Unlinked_Full.cpp
    ${IMGUI_SOURCES}
)

target_include_directories(MeiMei_Client PRIVATE 
    ${IMGUI_DIR} 
    ${IMGUI_DIR}/backends
)

target_link_libraries(MeiMei_Client PRIVATE d3d11.lib)
