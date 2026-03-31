//The Main Game Engine Application File
#pragma once
#include <Windows.h>
#include "DX11.h"
#include "AppWindow.h"
#include "Renderer.h"
class Machine {
  public:
    Machine(HINSTANCE hInstance, int nCmdShow);
    Machine(const Machine& m) = delete;
    Machine(Machine&& m) = delete;
    ~Machine() = default;
    Machine& operator=(const Machine& m) = delete;
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


