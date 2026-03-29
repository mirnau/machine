#include <Windows.h>
#include "AppWindow.h"

AppWindow::AppWindow() : m_hwnd(nullptr), m_hInstance(nullptr){}

int AppWindow::Create(HINSTANCE hInstance, int nCmdShow) {

  m_hInstance = hInstance;
  const wchar_t CLASS_NAME[] = L"Machine Engine Window";
  WNDCLASSEXW wc = {};
  wc.cbSize = sizeof(WNDCLASSEX);
  wc.lpfnWndProc = AppWindow::WindowProc;
  wc.hInstance = hInstance;
  wc.lpszMenuName = NULL;
  wc.lpszClassName = CLASS_NAME;

  if(!RegisterClassExW(&wc))    //Create the window.
  {
    DWORD dwError = GetLastError();
    if(dwError != ERROR_CLASS_ALREADY_EXISTS)
      return HRESULT_FROM_WIN32(dwError);
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
    return HRESULT_FROM_WIN32(dwError);
  }

  ShowWindow(m_hwnd, nCmdShow);

  // Run the message loop.

  MSG msg = { };
  while (GetMessage(&msg, NULL, 0, 0) > 0) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  return 0;
}

LRESULT CALLBACK AppWindow::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  AppWindow* instance = reinterpret_cast<AppWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

  if(uMsg == WM_NCCREATE) {
    auto* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
    instance = static_cast<AppWindow*>(cs->lpCreateParams);
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
  }

  return DefWindowProcW(m_hwnd, uMsg, wParam, lParam);

} 
