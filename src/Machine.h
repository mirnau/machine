//The Main Game Engine Application File
#pragma once
#include <Windows.h>
#include "Graphics.h"
#include "AppWindow.h"
class Machine {
  public:
    Machine(HINSTANCE hInstance, int nCmdShow);
    Machine(const Machine& m) = delete;
    Machine(Machine&& m) = delete;
    ~Machine() = default;
    Machine& operator=(const Machine& m) = delete;
    int Run();
    void Update();
    void Render();

  private:
    Graphics::DX11 m_graphics;
    AppWindow m_mainWindow;
    HINSTANCE m_hInstance;
    int m_nCmdShow;
    bool m_isRunning;

  private:
    int RunGameLoop();
};


