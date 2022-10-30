// lr8.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "lr8.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name


#define N 500

int processesCount = 0;
DWORD processesIds[N];

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

DWORD DisplayProcesses(HWND);
DWORD DisplayModules(HWND, DWORD);
void SetPriority(HWND, DWORD, DWORD);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LR8, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LR8));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LR8));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_LR8);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Store instance handle in our global variable

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, 800, 540, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int wmId, wmEvent;
    static DWORD processId;
    static HWND listBox1;

    switch (message)
    {
    case WM_CREATE:
    {
        listBox1 = CreateWindow(L"LISTBOX", L"LB1", WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL | WS_HSCROLL | LBS_NOTIFY,
            0, 0, 400, 500, hWnd, (HMENU)ID_LB_PROCESSES, hInst, NULL);
        CreateWindow(L"LISTBOX", L"LB2", WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL | WS_HSCROLL,
            400, 0, 400, 500, hWnd, (HMENU)ID_LB_MODULES, hInst, NULL);       

        DisplayProcesses(hWnd);
        
        break;
    }
    case WM_COMMAND:
    {
        wmId = LOWORD(wParam);
        wmEvent = HIWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {        
        case ID_LB_PROCESSES:
            switch (wmEvent)
            {                
            case LBN_SELCHANGE:
            {                
                INT ind = SendDlgItemMessage(hWnd, ID_LB_PROCESSES, LB_GETCURSEL, NULL, NULL);                
                if (ind != -1)
                {
                    DisplayModules(hWnd, ind);
                }                
            }
                break;
            }
            break;
        case ID_IDLE:
            SetPriority(hWnd, processId, IDLE_PRIORITY_CLASS);
            break;
        case ID_NORMAL:
            SetPriority(hWnd, processId, NORMAL_PRIORITY_CLASS);
            break;
        case ID_HIGH:
            SetPriority(hWnd, processId, HIGH_PRIORITY_CLASS);
            break;
        case ID_REAL_TIME:
            SetPriority(hWnd, processId, REALTIME_PRIORITY_CLASS);
            break;
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_CONTEXTMENU:        
        if ((HWND)wParam == listBox1) 
        {                                
            INT ind = SendDlgItemMessage(hWnd, ID_LB_PROCESSES, LB_GETCURSEL, NULL, NULL);
            if (ind != -1) 
            {
                processId = processesIds[ind];
                HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processesIds[ind]);
                int currentClass = GetPriorityClass(hProcess);
                CloseHandle(hProcess);

                HMENU hPopupMenu = CreatePopupMenu();
                AppendMenu(hPopupMenu, currentClass == IDLE_PRIORITY_CLASS ? MF_CHECKED : MF_STRING, ID_IDLE, L"Idle");
                AppendMenu(hPopupMenu, currentClass == NORMAL_PRIORITY_CLASS ? MF_CHECKED : MF_STRING, ID_NORMAL, L"Normal");
                AppendMenu(hPopupMenu, currentClass == HIGH_PRIORITY_CLASS ? MF_CHECKED : MF_STRING, ID_HIGH, L"High");
                AppendMenu(hPopupMenu, currentClass == REALTIME_PRIORITY_CLASS ? MF_CHECKED : MF_STRING, ID_REAL_TIME, L"RealTime");
                TrackPopupMenu(hPopupMenu, TPM_TOPALIGN | TPM_LEFTALIGN, LOWORD(lParam), HIWORD(lParam), 0, hWnd, NULL);
            }
        }                    
        break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: Add any drawing code that uses hdc here...
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

DWORD DisplayProcesses(HWND hWnd)
{
    processesCount = 0;
    SendDlgItemMessage(hWnd, ID_LB_PROCESSES, LB_RESETCONTENT, NULL, NULL);

    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (INVALID_HANDLE_VALUE == hSnap)
    {
        MessageBox(hWnd, L"can't get handle of snapshot process!\n", NULL, NULL);
        return FALSE;
    }

    PROCESSENTRY32 pe = { 0 };
    pe.dwSize = sizeof(PROCESSENTRY32);    

    TCHAR buff[1024];
    Process32First(hSnap, &pe);    
    do
    {
        wsprintf(buff, L"%s %x",
            pe.szExeFile, pe.th32ProcessID);
        processesIds[processesCount++] = pe.th32ProcessID;
        SendDlgItemMessage(hWnd, ID_LB_PROCESSES, LB_ADDSTRING, NULL, (LPARAM)buff);
    } 
    while (Process32Next(hSnap, &pe));

    CloseHandle(hSnap);
    return TRUE;
}


DWORD DisplayModules(HWND hWnd, DWORD pId)
{
    SendDlgItemMessage(hWnd, ID_LB_MODULES, LB_RESETCONTENT, NULL, NULL);
    
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, processesIds[pId]);

    if (INVALID_HANDLE_VALUE == hSnap) 
    {        
        return FALSE;
    }

    MODULEENTRY32 me;
    me.dwSize = sizeof(MODULEENTRY32);
    TCHAR buff[1024];
    Module32First(hSnap, &me);        
    do 
    {
        wsprintf(buff, L"%x, %x, %s, %x",
            me.modBaseAddr, me.modBaseSize,
            me.szModule, me.th32ProcessID);
        SendDlgItemMessage(hWnd, ID_LB_MODULES, LB_ADDSTRING, NULL, (LPARAM)buff);
    } 
    while (Module32Next(hSnap, &me));
    CloseHandle(hSnap);
    return TRUE;
}

void SetPriority(HWND hWnd, DWORD processId, DWORD priorityClass) 
{    
    if (processId >= 0)
    {
        HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
        SetPriorityClass(hProcess, priorityClass);
        CloseHandle(hProcess);
        DisplayProcesses(hWnd);
    }    
    else
    {
        MessageBox(hWnd, L"can't set priority", NULL, NULL);
    }
}