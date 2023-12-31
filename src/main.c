/*
Developed by Nils 'AGBDev' Boehm.

Note: This virus does not actually do anything harmful, due to me being bad at
programming in c. But I guess I feel better with this project. You know,
learning and gaining exp with the C programming language. I hope my code is not
to poorly written, I'll put in good effort tho. So please dont by too harsh ;)

This is the main file for the project, it checks if we are running on windows
and then uses the windows api to draw a window.
*/

#include <Windows.h>
#include <commctrl.h>
#include <stdio.h>
#include <stdlib.h>
#include <winnt.h>
#include <winuser.h>

#include "common/defines.h"
#include "common/util.h"
#include "files/files.h"

static HWND main_window_handle;
static HINSTANCE main_window_instane;
static HMENU hMenubar;
static HMENU hMenuFile;
static HMENU hMenuEntry;
static HMENU hMenuInfo;
static HWND hListView;

// Menu Item IDs
#define MENU_FILE_NEW 1
#define MENU_FILE_OPEN 2
#define MENU_FILE_SAVE 21
#define MENU_FILE_SAVEAS 22
#define MENU_FILE_OPTIONS 3
#define MENU_FILE_QUIT 4
#define MENU_ENTRY_ADD 5
#define MENU_ENTRY_REMOVE 6
#define MENU_INFO_ABOUT 300
#define DIALOG_KEY_ADD 43

static void create_menus();
static void handle_menus(WPARAM wParam);

// File handle
static scpfile_t *pointer_to_current_file;

// list
static void create_lists();
static void update_lists();

// Window procedure (callback) to handle messages for our window
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// dialog bs
static HWND current_dialog;
static char *current_dialog_out;

LRESULT CALLBACK DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static char* display_dialog(char *out, HWND handle);
static void register_dialog(HWND handle);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                       LPSTR lpCmdLine, int nCmdShow)
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
    main_window_handle =
        CreateWindow("MyWindowClass",              // Class name
        WINDOW_HEADER,                // Window title
        WS_OVERLAPPEDWINDOW,          // Window style
        CW_USEDEFAULT, CW_USEDEFAULT, // Initial position (x, y)
        SCREEN_WIDTH, SCREEN_HEIGHT,  // Initial size (width, height)
        NULL,                         // Parent window
        NULL,                         // Menu
        hInstance,                    // Instance handle
        NULL                          // Additional application data
        );

    if (!main_window_handle)
    {
        MessageBox(main_window_handle, "Fatal Error", WINDOW_HEADER, ERROR);
        return 2;
    }

    create_menus();
    create_lists();

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

// Handlers and Creation functions

static void create_menus()
{
    hMenubar = CreateMenu();
    hMenuFile = CreateMenu();

    AppendMenuW(hMenuFile, MF_STRING, MENU_FILE_NEW, L"&New");
    AppendMenuW(hMenuFile, MF_STRING, MENU_FILE_OPEN, L"&Open");
    AppendMenuW(hMenuFile, MF_STRING, MENU_FILE_SAVE, L"&Save");
    AppendMenuW(hMenuFile, MF_STRING, MENU_FILE_SAVEAS, L"&Save As");
    AppendMenuW(hMenuFile, MF_SEPARATOR, 0, NULL);
    AppendMenuW(hMenuFile, MF_STRING, MENU_FILE_QUIT, L"&Quit");
    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)hMenuFile, L"&File");

    hMenuEntry = CreateMenu();
    AppendMenuW(hMenuEntry, MF_STRING, MENU_ENTRY_ADD, L"&Add");
    AppendMenuW(hMenuEntry, MF_STRING, MENU_ENTRY_REMOVE, L"&Remove");
    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)hMenuEntry, L"&Entry");

    hMenuInfo = CreateMenu();

    AppendMenuW(hMenuInfo, MF_STRING, MENU_INFO_ABOUT, L"&About");
    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)hMenuInfo, L"&Info");

    SetMenu(main_window_handle, hMenubar);
    DrawMenuBar(main_window_handle);
}

static void handle_menus(WPARAM wParam)
{
    if (LOWORD(wParam) == MENU_FILE_NEW)
    {
        char filename[MAX_PATH];

        OPENFILENAME ofn;
        ZeroMemory(&filename, sizeof(filename));
        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = NULL; // If you have a window to center over, put its HANDLE here
        ofn.lpstrFilter = "Super Cool Program Files\0*.scp\0Any File\0*.*\0";
        ofn.lpstrFile = filename;
        ofn.nMaxFile = MAX_PATH;
        ofn.lpstrTitle = "Select save path.";
        ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

        if (GetSaveFileName(&ofn))
        {
            scpfile_t *file = scpfile_generate();
            if (file == NULL)
            {
                MessageBox(main_window_handle, TEXT("Error preparing save!"), TEXT("Program"), 1);
                return;
            }
            scpfile_add(file, "test", 23);
            scpfile_save(file, filename);
            char str[200];
            sprintf(str, "Saved %lld entries to %s.", file->size, filename);
            MessageBox(main_window_handle, str, TEXT("Program"), 1);
            pointer_to_current_file = file;
            update_lists();
        }
    }
    if (LOWORD(wParam) == MENU_FILE_OPEN)
    {
        char filename[MAX_PATH];

        OPENFILENAME ofn;
        ZeroMemory(&filename, sizeof(filename));
        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = NULL; // If you have a window to center over, put its HANDLE here
        ofn.lpstrFilter = "Super Cool Program Files\0*.scp\0Any File\0*.*\0";
        ofn.lpstrFile = filename;
        ofn.nMaxFile = MAX_PATH;
        ofn.lpstrTitle = "Select a file to load.";
        ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

        if (GetOpenFileNameA(&ofn))
        {
            scpfile_t *file = scpfile_load(filename);
            if (file == NULL)
            {
                MessageBox(main_window_handle, TEXT("Error loading file!"), TEXT("Program"), 1);
                return;
            }
            char str[200];
            sprintf(str, "Loaded %lld entries from %s.", file->size, filename);
            MessageBox(main_window_handle, str, TEXT("Program"), 1);
            pointer_to_current_file = file;
            update_lists();
        }
    }
    if (LOWORD(wParam) == MENU_FILE_SAVE)
    {
        
    }
    if (LOWORD(wParam) == MENU_FILE_SAVEAS)
    {
        if (pointer_to_current_file == NULL)
        {
            MessageBox(main_window_handle, TEXT("Please open a file."), TEXT(WINDOW_HEADER), MB_OK | MB_ICONERROR);
            return;
        }
        char filename[MAX_PATH];

        OPENFILENAME ofn;
        ZeroMemory(&filename, sizeof(filename));
        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = NULL; // If you have a window to center over, put its HANDLE here
        ofn.lpstrFilter = "Super Cool Program Files\0*.scp\0Any File\0*.*\0";
        ofn.lpstrFile = filename;
        ofn.nMaxFile = MAX_PATH;
        ofn.lpstrTitle = "Select save path.";
        ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

        if (GetSaveFileName(&ofn))
        {
            scpfile_save(pointer_to_current_file, filename);
            update_lists();
        }
    }
    if (LOWORD(wParam) == MENU_FILE_OPTIONS)
    {
    }
    if (LOWORD(wParam) == MENU_FILE_QUIT)
    {
        free(pointer_to_current_file);
        PostQuitMessage(0);
    }
    if (LOWORD(wParam) == MENU_ENTRY_ADD)
    {
        if (pointer_to_current_file == NULL)
        {
            MessageBox(main_window_handle, TEXT("Please open a file."), TEXT(WINDOW_HEADER), MB_OK | MB_ICONERROR);
            return;
        }
        char str[600];
        display_dialog(current_dialog_out, main_window_handle);
        if (strcmp(current_dialog_out, "") == 0)
        {
            MessageBox(main_window_handle, TEXT("Error getting info."), TEXT(WINDOW_HEADER), 2);
        }
        update_lists();
    }
    if (LOWORD(wParam) == MENU_INFO_ABOUT)
    {
        MessageBox(main_window_handle, TEXT("Program made by Nils 'AGBDev' Boehm."), TEXT(WINDOW_HEADER), 2);
    }
}



void create_lists()
{
    hListView = CreateWindowExW(
        0, WC_LISTVIEWW, L"Hallo",
        WS_VISIBLE | WS_CHILD | WS_BORDER | LVS_SHOWSELALWAYS | LVS_REPORT, 0, 0,
        350, 400, main_window_handle, NULL, NULL, NULL);

    LVCOLUMN keyname;
    memset(&keyname, 0, sizeof(LVCOLUMN));
    keyname.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
    keyname.iSubItem = 0;
    keyname.pszText = TEXT("Keyname");
    keyname.cx = 175;
    keyname.fmt = LVCFMT_LEFT;
    ListView_InsertColumn(hListView, 0, &keyname);

    LVCOLUMN keyvalues;
    memset(&keyvalues, 0, sizeof(LVCOLUMN)); // Initialize the structure to zero.
    keyvalues.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
    keyvalues.iSubItem = 1; // Set this to 1 for the second column.
    keyvalues.pszText = TEXT("Keyvalues");
    keyvalues.cx = 175;
    keyvalues.fmt = LVCFMT_LEFT;
    ListView_InsertColumn(hListView, 1, &keyvalues); // Use index 1 for the second column.
}

void update_lists()
{
    if (pointer_to_current_file == NULL) return;

    for (size_t i = 0; i < pointer_to_current_file->size; i++)
    {
        ListView_DeleteAllItems(hListView);
    }

    for (size_t i = 0; i < pointer_to_current_file->size; i++)
    {
        LV_ITEM name;
        memset(&name, 0, sizeof(LV_ITEM)); // Initialize the structure to zero.
        name.mask = LVIF_TEXT;             // Specify that you want to set text.
        name.iItem = i;                    // Set the name index (assuming you're using a loop).
        name.iSubItem = 0;                 // Set the subitem index.
        name.pszText = pointer_to_current_file->key_names[i]; // Assign the text from your data.
        const short res = (short)ListView_InsertItem(hListView, &name);
        if (res == -1)
        {
            MessageBox(main_window_handle, TEXT("Error inserting item!"), TEXT(WINDOW_HEADER), MB_ICONERROR | MB_OK);
        }

        LV_ITEM value;
        memset(&value, 0, sizeof(LV_ITEM)); // Initialize the structure to zero.
        value.mask = LVIF_TEXT;             // Specify that you want to set text.
        value.iItem = i;                    // Set the item index (assuming you're using a loop).
        value.iSubItem = 1;                 // Set the subitem index for the second column.
        value.pszText = TEXT("sex");        // Assign the text from your data.

        const short res_ = (short)ListView_InsertItem(hListView, &value);

        if (res_ == -1)
        {
            MessageBox(main_window_handle, TEXT("Error inserting item!"), TEXT(WINDOW_HEADER), MB_ICONERROR | MB_OK);
        }
    }
}




LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam,
                            LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
        register_dialog(main_window_handle);
        break;
    case WM_CLOSE:
        free(pointer_to_current_file);
        PostQuitMessage(0); // Post a quit message to exit the application
        break;
    case WM_COMMAND:
        handle_menus(wParam);
        break;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

LRESULT DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;
        case WM_COMMAND:
            if (LOWORD(wParam) == DIALOG_KEY_ADD)
            {
                if (current_dialog == NULL) return 0;

                char *f = _strdup(current_dialog_out);
                const char *name = strtok_s(f, "|", &f);
                const char *value = strtok_s(NULL, "|", &f);
                scpfile_add(pointer_to_current_file, name, atoi(value));
                update_lists();
            }
            break;
        default:
            return DefWindowProcW(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

static char* display_dialog(char *out, HWND handle)
{
    HWND dialog = CreateWindowW(L"progDialogClass", L"Dialog", WS_VISIBLE | WS_OVERLAPPEDWINDOW, 400, 400, 200, 200, handle, NULL, NULL, NULL);

    current_dialog = dialog;


    CreateWindowW(L"Static", L"Keyname", WS_VISIBLE | WS_CHILD, 20, 20, 90, 40,
                  dialog, (HMENU)1, NULL, NULL);

    HWND keyname = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD, 20, 40, 90, 20,
                  dialog, (HMENU)1, NULL, NULL);

    CreateWindowW(L"Static", L"Keyvalue", WS_VISIBLE | WS_CHILD, 20, 70, 90, 40,
                  dialog, (HMENU)1, NULL, NULL);

    HWND keyvalue = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD, 20, 90, 90, 20,
                  dialog, (HMENU)1, NULL, NULL);
                  
    CreateWindowW(L"Button", L"Ok", WS_VISIBLE | WS_CHILD, 20, 110, 40, 40,
        dialog, (HMENU)DIALOG_KEY_ADD, NULL, NULL);

    int cTxtLen = GetWindowTextLengthW(keyname);
    PSTR pszMem = (PSTR)VirtualAlloc((LPVOID)NULL,
                                    (DWORD)(cTxtLen + 1), MEM_COMMIT,
                                    PAGE_READWRITE);
    GetWindowText(keyname, pszMem, cTxtLen);
    PSTR pszMem2 = (PSTR)VirtualAlloc((LPVOID)NULL,
                                     (DWORD)(cTxtLen + 1), MEM_COMMIT,
                                     PAGE_READWRITE);
    GetWindowText(keyvalue, pszMem2, cTxtLen);
    char ret[600];

    sprintf_s(ret, 600, "%s|%s", pszMem, pszMem2);


    out = _strdup(ret);

    current_dialog_out = out;

    return out;
}

static void register_dialog(HWND handle)
{
    WNDCLASSW dialog = {0};

    dialog.hbrBackground = (HBRUSH)COLOR_WINDOW;
    dialog.hCursor = LoadCursor(NULL, IDC_CROSS);
    dialog.hInstance = main_window_instane;
    dialog.lpszClassName = L"progDialogClass";
    dialog.lpfnWndProc = DialogProc;

    RegisterClassW(&dialog);
}
