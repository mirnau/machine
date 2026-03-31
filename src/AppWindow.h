#pragma once
#include <Windows.h>

class  AppWindow{

        public:
                HWND m_hwnd;

        public:
                AppWindow();
                AppWindow(HINSTANCE hInstance, int nCmdShow);
                AppWindow(const AppWindow& w)= delete;
                AppWindow(AppWindow&& w) = delete;
                AppWindow& operator=(const AppWindow& w) = delete;
                ~AppWindow() = default;

                int Create();
                static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
                LRESULT HandleWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

        private:
                HINSTANCE m_hInstance;
                int m_nCmdShow;
};
