// lr2.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "lr2.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

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
    LoadStringW(hInstance, IDC_LR2, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LR2));

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

    return (int) msg.wParam;
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

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LR2));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LR2);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

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
    int ind;
    switch (message)
    {
    case WM_CREATE:                
        if (!CreateWindow(L"EDIT", L"", ES_LEFT | WS_BORDER | WS_CHILD | WS_VISIBLE,
                          100, 50, 300, 30, hWnd, (HMENU)ID_TEXT_BOX, NULL, NULL))
            MessageBox(hWnd, L"Couldn't create edit box.", L"Error", MB_OK | MB_ICONERROR);        
        if (!CreateWindow(L"LISTBOX", L"", LBS_NOSEL | WS_VSCROLL | LBS_MULTIPLESEL | WS_BORDER | WS_CHILD | WS_VISIBLE,
                          100, 100, 300, 300, hWnd, (HMENU)ID_LIST_BOX1, NULL, NULL))
            MessageBox(hWnd, L"Couldn't create list box 1.", L"Error", MB_OK | MB_ICONERROR);        
        if (!CreateWindow(L"LISTBOX", L"", LBS_NOSEL | WS_VSCROLL | LBS_SORT | WS_BORDER | WS_CHILD | WS_VISIBLE,
                          450, 100, 300, 300, hWnd, (HMENU)ID_LIST_BOX2, NULL, NULL))
            MessageBox(hWnd, L"Couldn't create list box 2.", L"Error", MB_OK | MB_ICONERROR);        
        if (!CreateWindow(L"Button", L"Add", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE, 
                          100, 400, 100, 30, hWnd, (HMENU)ID_ADD_BUTTON, NULL, NULL))
            MessageBox(hWnd, L"Couldn't create addButton.", L"Error", MB_OK | MB_ICONERROR);        
        if (!CreateWindow(L"Button", L"Clear", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE, 
                          200, 400, 100, 30, hWnd, (HMENU)ID_CLEAR_BUTTON, NULL, NULL))
            MessageBox(hWnd, L"Couldn't create clearButton.", L"Error", MB_OK | MB_ICONERROR);        
        if (!CreateWindow(L"Button", L"Delete", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE, 
                          300, 400, 100, 30, hWnd, (HMENU)ID_DELETE_BUTTON, NULL, NULL))
            MessageBox(hWnd, L"Couldn't create deleteButton.", L"Error", MB_OK | MB_ICONERROR);        
        if (!CreateWindow(L"Button", L"To Right", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE, 
                          400, 400, 100, 30, hWnd, (HMENU)ID_TO_RIGHT_BUTTON, NULL, NULL))
            MessageBox(hWnd, L"Couldn't create toRightButton.", L"Error", MB_OK | MB_ICONERROR);
        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case ID_ADD_BUTTON:
            {
                int length = GetWindowTextLength(GetDlgItem(hWnd, ID_TEXT_BOX));                
                if (length > 0)
                {
                    length++;
                    TCHAR* buf = (TCHAR*)GlobalAlloc(GPTR, sizeof(TCHAR) * (length));
                    GetDlgItemText(hWnd, ID_TEXT_BOX, buf, length);                  
                    if (SendDlgItemMessage(hWnd, ID_LIST_BOX1, LB_FINDSTRINGEXACT, NULL, (LPARAM)buf) == LB_ERR)
                    {
                        SendDlgItemMessage(hWnd, ID_LIST_BOX1, LB_ADDSTRING, NULL, LPARAM(buf));
                    }
                        
                    SetDlgItemText(hWnd, ID_TEXT_BOX, L"");
                    GlobalFree(buf);
                }
                break;
            }
            case ID_DELETE_BUTTON:
            {
                ind = SendDlgItemMessage(hWnd, ID_LIST_BOX1, LB_GETCURSEL, NULL, NULL);
                SendDlgItemMessage(hWnd, ID_LIST_BOX1, LB_DELETESTRING, ind, NULL);

                ind = SendDlgItemMessage(hWnd, ID_LIST_BOX2, LB_GETCURSEL, NULL, NULL);                
                SendDlgItemMessage(hWnd, ID_LIST_BOX2, LB_DELETESTRING, ind, NULL);

                break;
            }
            case ID_CLEAR_BUTTON:
            {
                SendDlgItemMessage(hWnd, ID_LIST_BOX1, LB_RESETCONTENT, NULL, NULL);
                SendDlgItemMessage(hWnd, ID_LIST_BOX2, LB_RESETCONTENT, NULL, NULL);

                break;
            }
            case ID_TO_RIGHT_BUTTON:
                ind = SendDlgItemMessage(hWnd, ID_LIST_BOX1, LB_GETCURSEL, NULL, NULL);
                if (ind != LB_ERR)
                {
                    int length = SendDlgItemMessage(hWnd, ID_LIST_BOX1, LB_GETTEXTLEN, ind, NULL);
                    TCHAR* buf = (TCHAR*)GlobalAlloc(GPTR, sizeof(TCHAR) * (length + 1));
                    SendDlgItemMessage(hWnd, ID_LIST_BOX1, LB_GETTEXT, ind, (LPARAM)buf);
                    if (SendDlgItemMessage(hWnd, ID_LIST_BOX2, LB_FINDSTRINGEXACT, NULL, (LPARAM)buf) == LB_ERR)
                    {
                        SendDlgItemMessage(hWnd, ID_LIST_BOX2, LB_ADDSTRING, NULL, (LPARAM)buf);
                    }                        
                    GlobalFree(buf);
                }
                else
                {
                    MessageBox(hWnd, L"Select the item first", L"Warning", MB_OK | MB_ICONWARNING);
                }
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
