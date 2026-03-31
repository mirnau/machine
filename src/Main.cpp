#include <Windows.h>
#include "Machine.h"

int WINAPI wWinMain(
    HINSTANCE hInstance, 
    HINSTANCE hPrevInstance, 
    PWSTR lpCmdLine,
    int nCmdShow) 
{
  Machine m{hInstance, nCmdShow};
  return m.Run(); 
} 
