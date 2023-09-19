/*
Developed by Nils 'AGBDev' Boehm.

Note: This virus does not actually do anything harmful, due to me being bad at programming in c.
But I guess I feel better with this project. You know, learning and gaining exp with the C programming language.
I hope my code is not to poorly written, I'll put in good effort tho. So please dont by too harsh ;)

This is the main file for the project, it checks if we are running on windows and then uses the windows api to draw a window.
*/

#include <Windows.h>
#include <commctrl.h>
#include <stdio.h>
#include <stdlib.h>

#include "defines.h"

static HWND main_window_handle;
static HINSTANCE main_window_instane;
static HMENU hMenubar;
static HMENU hMenuFile;
static HMENU hMenuInfo;
static HWND hListView;

// Menu Items
#define MENU_FILE_NEW 1
#define MENU_FILE_OPEN 2
#define MENU_FILE_OPTIONS 3
#define MENU_FILE_QUIT 4
#define MENU_INFO_ABOUT 5

// Window procedure (callback) to handle messages for our window
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// Create and draw menubar and its items
static void create_menus();

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // Define the window class
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "MyWindowClass";
    wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);

    main_window_instane = wc.hInstance;

    if (!main_window_instane)
        return 1;

    // Register the window class
    if (!RegisterClass(&wc))
        return 1;

    // Create the window
    main_window_handle = CreateWindow(
        "MyWindowClass",    // Class name
        WINDOW_HEADER,        // Window title
        WS_OVERLAPPEDWINDOW, // Window style
        CW_USEDEFAULT, CW_USEDEFAULT, // Initial position (x, y)
        SCREEN_WIDTH, SCREEN_HEIGHT,            // Initial size (width, height)
        NULL,                // Parent window
        NULL,                // Menu
        hInstance,           // Instance handle
        NULL                 // Additional application data
    );

    if (!main_window_handle)
    {
        MessageBox(main_window_handle, "Fatal Error", WINDOW_HEADER, ERROR);
        return 2;
    }

    hListView = CreateWindowExW(0, WC_LISTVIEWW, L"Hallo", WS_VISIBLE | WS_CHILD | WS_BORDER | LVS_SHOWSELALWAYS | LVS_REPORT, 0, 0, 250, 400, main_window_handle, NULL, NULL, NULL);

    create_menus();

    // Show and update the window
    ShowWindow(main_window_handle, nCmdShow);
    UpdateWindow(main_window_handle);

    // Message loop
    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
        break;
    case WM_CLOSE:
        PostQuitMessage(0); // Post a quit message to exit the application
        break;
    case WM_COMMAND:
        if (LOWORD(wParam) == MENU_FILE_NEW)
        {
            MessageBox(hwnd, TEXT("Hahaha its just a prank!"), TEXT(WINDOW_HEADER), 2);
        }
        if (LOWORD(wParam) == MENU_FILE_OPEN)
        {
            char filename[ MAX_PATH ];

            OPENFILENAME ofn;
            ZeroMemory( &filename, sizeof( filename ) );
            ZeroMemory( &ofn,      sizeof( ofn ) );
            ofn.lStructSize  = sizeof( ofn );
            ofn.hwndOwner    = NULL;  // If you have a window to center over, put its HANDLE here
            ofn.lpstrFilter  = "Super Cool Program Files\0*.scp\0Any File\0*.*\0";
            ofn.lpstrFile    = filename;
            ofn.nMaxFile     = MAX_PATH;
            ofn.lpstrTitle   = "Select a file to load.";
            ofn.Flags        = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;
  
            if (GetOpenFileNameA( &ofn ))
            {
            }
        }
        if (LOWORD(wParam) == MENU_FILE_OPTIONS)
        {
            
        }
        if (LOWORD(wParam) == MENU_FILE_QUIT)
        {
            PostQuitMessage(0);
        }
        if (LOWORD(wParam) == MENU_INFO_ABOUT)
        {
            MessageBox(hwnd, TEXT(WINDOW_HEADER), TEXT(WINDOW_HEADER), 2);
        }
        break;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

static void create_menus()
{
    hMenubar = CreateMenu();
    hMenuFile = CreateMenu();

    AppendMenuW(hMenuFile, MF_STRING, MENU_FILE_NEW, L"&New");
    AppendMenuW(hMenuFile, MF_STRING, MENU_FILE_OPEN, L"&Open");
    AppendMenuW(hMenuFile, MF_SEPARATOR, 0, NULL);
    AppendMenuW(hMenuFile, MF_STRING, MENU_FILE_QUIT, L"&Quit");
    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)hMenuFile, L"&File");

    hMenuInfo = CreateMenu();

    AppendMenuW(hMenuInfo, MF_STRING, MENU_INFO_ABOUT, L"&About");
    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)hMenuInfo, L"&Info");

    SetMenu(main_window_handle, hMenubar);
    DrawMenuBar(main_window_handle);
}