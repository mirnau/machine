#include <Windows.h>
#include <iostream>
#include "Failure.h"
#include "Engine.h"


int WINAPI wWinMain(
    HINSTANCE hInstance, 
    HINSTANCE hPrevInstance, 
    PWSTR lpCmdLine,
    int nCmdShow) 
{
    try {
        Machine::Engine m{hInstance, nCmdShow};
        return m.Run(); 
    } catch (const Machine::Failure& e) {
#if defined DEBUG
        std::cerr << e.what() << std::endl;
        std::cin.get();
#else
        MessageBoxA(nullptr, e.what(), "Fatal Error", MB_OK | MB_ICONERROR);
#endif
        return -1;
    }
}   
