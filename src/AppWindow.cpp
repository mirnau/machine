#include <Windows.h>
#include <mmeapi.h>
#include <winnt.h>
#include <winuser.h>
#include "AppWindow.h"
#include "Failure.h"
#include "MiniMath.h"

AppWindow::AppWindow(HINSTANCE hInstance, int nCmdShow) 
: m_hInstance(hInstance), m_nCmdShow(nCmdShow), m_hwnd(nullptr){} 

void AppWindow::Create() {

  const wchar_t CLASS_NAME[] = L"Machine Engine Window";
  WNDCLASSEXW wc = {};
  wc.cbSize = sizeof(WNDCLASSEX);
  wc.lpfnWndProc = AppWindow::WindowProc;
  wc.hInstance = m_hInstance;
  wc.lpszMenuName = NULL;
  wc.lpszClassName = CLASS_NAME;

  if(!RegisterClassExW(&wc))    //Create the window.
  {
    DWORD dwError = GetLastError();
    if(dwError != ERROR_CLASS_ALREADY_EXISTS)
      throw Machine::Failure::Graphics(HRESULT_FROM_WIN32(dwError));
  }

  m_hwnd = CreateWindowExW(
    0,                              // Optional window styles.
    CLASS_NAME,                     // Window class
    L"Learn to Program Windows",    // Window text
    WS_OVERLAPPEDWINDOW,            // Window style

    // Size and position
    CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

    NULL,       // Parent window    
    NULL,       // Menu
    m_hInstance,  // Instance handle
    this        // Additional application data
  );

  if (m_hwnd == nullptr) {
    DWORD dwError = GetLastError();
    throw Machine::Failure::Graphics(HRESULT_FROM_WIN32(dwError));
  }

  ShowWindow(m_hwnd, m_nCmdShow);
}

LRESULT CALLBACK AppWindow::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  AppWindow* instance = reinterpret_cast<AppWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

  if(uMsg == WM_NCCREATE) {
    auto* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
    instance = static_cast<AppWindow*>(cs->lpCreateParams);
    instance->m_hwnd = hwnd;
    SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(instance));
  }

  if(instance)
    return instance->HandleWndProc(uMsg, wParam, lParam);
  else
    return DefWindowProcW(hwnd, uMsg, wParam, lParam);
};

LRESULT AppWindow::HandleWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam) {

  switch(uMsg) 
  {
    case WM_PAINT: {
      PAINTSTRUCT ps;
      HDC hdc = BeginPaint(m_hwnd, &ps);
      FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW+1));
      EndPaint(m_hwnd, &ps);
      return 0;
    }
    case WM_CLOSE: {
      if(MessageBoxW(m_hwnd, L"Really Quit?", L"Machine", MB_OKCANCEL) == IDOK) {
        DestroyWindow(m_hwnd);
      }
      return 0;
    }
    case WM_DESTROY:
      {
        PostQuitMessage(0);
        return 0;
      }
    case WM_CREATE:
    case WM_SIZE: {
      if(m_onResize) {
        m_onResize(GetWindowSize());
      }
      return 0; 
    }

  }

  return DefWindowProcW(m_hwnd, uMsg, wParam, lParam);

}

const uint2 AppWindow::GetWindowSize() const {
  RECT rect;
  GetClientRect(m_hwnd, &rect);
  const uint2 size{rect.right - rect.left, rect.bottom - rect.top};
  return size;
}
