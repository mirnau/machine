//The Main Game Engine Application File
#pragma once
#include <Windows.h>
#include "DX11.h"
#include "AppWindow.h"
#include "Renderer.h"

namespace Machine {

class Engine {
public:
    Engine(HINSTANCE hInstance, int nCmdShow);
    Engine(const Engine& m) = delete;
    Engine(Engine&& m) = delete;
    ~Engine() = default;
    Engine& operator=(const Engine& m) = delete;
    int Run();
    void Update();

private:
    AppWindow m_mainWindow;
    Graphics::DX11 m_graphics;
    Graphics::Renderer m_renderer;
    HINSTANCE m_hInstance;
    int m_nCmdShow;
    bool m_isRunning;

private:
    int RunGameLoop();
};
}

