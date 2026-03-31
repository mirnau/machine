cmake_minimum_required(VERSION 3.20)
project(Machine)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(Machine WIN32
    src/main.cpp
    src/AppWindow.cpp
    src/DX11.cpp
    src/Machine.cpp
    src/Renderer.cpp
)

target_link_libraries(Machine PRIVATE d3d11.lib dxgi.lib)
