cmake_minimum_required(VERSION 3.15)
project(MeiMei_Client VERSION 5.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Point to the submodule
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/imgui)

# Gather all required ImGui core and backend files
file(GLOB IMGUI_SOURCES
    ${IMGUI_DIR}/*.cpp
    ${IMGUI_DIR}/backends/imgui_impl_win32.cpp
    ${IMGUI_DIR}/backends/imgui_impl_dx11.cpp
)

# Build as a dynamic library (DLL)
add_library(MeiMei_Client SHARED
    MeiMei_Client_Unlinked_Full.cpp
    ${IMGUI_SOURCES}
)

# Include directories for headers
target_include_directories(MeiMei_Client PRIVATE 
    ${IMGUI_DIR} 
    ${IMGUI_DIR}/backends
)

# Link DirectX 11
target_link_libraries(MeiMei_Client PRIVATE d3d11.lib)
