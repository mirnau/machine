#include <Windows.h>
#include "AppWindow.h"

int WINAPI wWinMain(
    HINSTANCE hInstance, 
    HINSTANCE hPrevInstance, PWSTR lpCmdLine,
    int nCmdShow) 
{
  AppWindow aw; 
  return aw.Create(hInstance, nCmdShow);
} 
