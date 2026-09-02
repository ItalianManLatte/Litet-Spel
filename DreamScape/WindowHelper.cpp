//IMPORTING HEADER FILES --------------------------------------------------------
#include "WindowHelper.h" //Header to access the main Windows API.
#include <iostream> //Header to access the standard input/output.

//DECLARING METHODS -------------------------------------------------------------
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    default:
        break;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}

bool SetupWindow(HINSTANCE instance, UINT width, UINT height, int nCmdShow, HWND & window)
{
    const wchar_t CLASS_NAME[] = L"Demo Window Class";

    WNDCLASS wc = { };

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = instance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    window = CreateWindowEx(0, CLASS_NAME, L"Nightmare Dreamscape", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, width, height, nullptr, nullptr, instance, nullptr);
    
    if (window == nullptr)
    {
        std::cerr << "Error: HWND was nullptr, last error: " << GetLastError() << std::endl;
        return false;
    }

    ShowWindow(window, nCmdShow);
    return true;
}
