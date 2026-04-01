cmake_minimum_required(VERSION 3.20)
project(Machine)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(Machine WIN32
    src/main.cpp
    src/AppWindow.cpp
    src/DX11.cpp
    src/Engine.cpp
    src/Renderer.cpp
    src/Vertex.cpp
    src/Failure.cpp
)

target_link_libraries(
    Machine PRIVATE 
    d3d11.lib 
    dxgi.lib 
    d3dcompiler.lib
)
target_compile_features(Machine PRIVATE cxx_std_23)
target_compile_definitions(Machine PRIVATE $<$<CONFIG:Debug>:DEBUG>)

add_custom_command(TARGET Machine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/src/shaders
    $<TARGET_FILE_DIR:Machine>/shaders
) 
