#include <Windows.h>
#include "Engine.h"
#include "DebugConsole.h"
#include "Failure.h"

Machine::Engine::Engine(HINSTANCE hInstance, int nCmdShow) 
  : 
  m_hInstance(hInstance), 
  m_nCmdShow(nCmdShow), 
  m_dx11(),
  m_renderer(m_dx11),
  m_isRunning(true),
  m_mainWindow(hInstance, nCmdShow) 
{}

int Machine::Engine::Run() {

  CreateDebugConsole();

  if(m_hInstance) {

    m_mainWindow.Create();
    m_mainWindow.m_onResize = m_renderer.GetResizeCallback();
    m_dx11.Init(m_mainWindow.m_hwnd);   
    m_renderer.Init(m_mainWindow.GetWindowSize());
  
    return RunGameLoop();
  } 
  return -1;
}

int Machine::Engine::RunGameLoop() {
  MSG msg{};

  while(m_isRunning) {
    if(PeekMessage(&msg, nullptr, 0,0, PM_REMOVE)) { 
      TranslateMessage(&msg);
      DispatchMessage(&msg);
      if(msg.message == WM_QUIT)
        m_isRunning = false;
    }
    Update();
    m_renderer.Render();
  }   

  return 0;
}

void Machine::Engine::Update() {

}
