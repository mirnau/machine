#pragma once
#include <Windows.h>

class  AppWindow{ 

    public:
        AppWindow();
        AppWindow(const AppWindow& w)= delete;
        AppWindow(AppWindow&& w) = delete;
        AppWindow& operator=(const AppWindow& w) = delete;
        ~AppWindow() = default;

        int Create(HINSTANCE hInstance, int nCmdShow);

        static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
        LRESULT HandleWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

    private:
        HWND m_hwnd;
};
