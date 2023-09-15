#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#include "include/defines.h"
#include "include/util.h"
#include "include/rebuilder.h"

#define IMAGE_VIEW

HWND label01;
HBITMAP hBitmap01 = NULL;

// Window procedure (callback) to handle messages for our window
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_PAINT:
        PAINTSTRUCT     ps01;
        HDC             hdc01;
        BITMAP          bitmap01;
        HDC             hdcMem01;
        HGDIOBJ         oldBitmap01;

        hdc01 = BeginPaint(hwnd, &ps01);

        hdcMem01 = CreateCompatibleDC(hdc01);
        oldBitmap01 = SelectObject(hdcMem01, hBitmap01);

        GetObject(hBitmap01, sizeof(bitmap01), &bitmap01);
        BitBlt(hdc01, 0, 0, bitmap01.bmWidth, bitmap01.bmHeight, hdcMem01, 0, 0, SRCCOPY);

        SelectObject(hdcMem01, oldBitmap01);
        DeleteDC(hdcMem01);

        EndPaint(hwnd, &ps01);
        break;
    case WM_CREATE:
        hBitmap01 = (HBITMAP)LoadImage(NULL, "back.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        break;
    case WM_CLOSE:
        PostQuitMessage(0); // Post a quit message to exit the application
        break;
    case WM_COMMAND:
        if (LOWORD(wParam) == 1) 
        {
            MessageBox(hwnd, TEXT("Hahaha its just a prank!"), TEXT("Half-Life 3"), 2);
        }
        if (LOWORD(wParam) == 2)
        {
            MessageBox(hwnd, TEXT("Hahaha its just a prank!"), TEXT("Half-Life 3"), 2);
        }
        if (LOWORD(wParam) == 3)
        {
            PostQuitMessage(0);
        }
        break;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // Define the window class
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "MyWindowClass";
    wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);

    // Register the window class
    if (!RegisterClass(&wc))
        return 1;

    // Create the window
    HWND hwnd = CreateWindow(
        "MyWindowClass",    // Class name
        "Half-Life 3",        // Window title
        WS_OVERLAPPEDWINDOW, // Window style
        CW_USEDEFAULT, CW_USEDEFAULT, // Initial position (x, y)
        1120, 695,            // Initial size (width, height)
        NULL,                // Parent window
        NULL,                // Menu
        hInstance,           // Instance handle
        NULL                 // Additional application data
    );

    // label01 = CreateWindow(TEXT("Edit"), TEXT("Label"),
    //              WS_VISIBLE | WS_CHILD,
    //              20, 10, 280, 25,
    //              hwnd, (HMENU) 3, NULL, NULL);

    HWND button_newgame = CreateWindow(TEXT("button"), TEXT("New Game"),    
                 WS_VISIBLE | WS_CHILD | WS_BORDER,
                 20, 50, 120, 20,        
                 hwnd, (HMENU) 1, NULL, NULL);  

    HWND button_loadgame = CreateWindow(TEXT("button"), TEXT("Load Game"),    
                 WS_VISIBLE | WS_CHILD | WS_BORDER,
                 20, 80, 120, 20,        
                 hwnd, (HMENU) 2, NULL, NULL);  

    HWND button_quit = CreateWindow(TEXT("button"), TEXT("Quit"),    
                 WS_VISIBLE | WS_CHILD | WS_BORDER,
                 20, 110, 120, 20,        
                 hwnd, (HMENU) 3, NULL, NULL);  

    if (!hwnd)
    {
        MessageBox(hwnd, "Fatal Engine Error", "Source Engine", ERROR);
        return 2;
    }

    // Show and update the window
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    FILE *fp;
    if ((fp = fopen("hl3.gaminfo", "r")) == NULL)
    {
        const short result = (short)MessageBox(hwnd, "Outdated, rebuild?.", MESSAGEBOX_TITLE, 3);
        if (result == IDYES)
        {
            rebuild_game(hwnd);
            return 0;
        }
    }
    fclose(fp);

    // Message loop
    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}