#include <Windows.h>
#include "Machine.h"

Machine::Machine(HINSTANCE hInstance, int nCmdShow) 
  : 
    m_hInstance(hInstance), 
    m_nCmdShow(nCmdShow), 
    m_graphics(),
    m_renderer(m_graphics),
    m_isRunning(true),
    m_mainWindow(hInstance, nCmdShow) 
{}

int Machine::Run() {

  if(m_hInstance) {
    if(FAILED(m_mainWindow.Create()))
      return -1;

    m_mainWindow.m_onResize = [this](UINT w, UINT h) {
      m_renderer.Resize(w, h);
    };

    if(FAILED(m_graphics.Init(m_mainWindow.m_hwnd)))
      return -1;

    if(FAILED(m_renderer.Init())) {
      return -1;
    }

    return RunGameLoop();
  } 
  return -1;
}

int Machine::RunGameLoop() {
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

void Machine::Update() {

}
