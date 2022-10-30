// lr9.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "lr9.h"

#define MAX_LOADSTRING 100

#define TOTAL_BYTES_READ    1024
#define OFFSET_BYTES 1024

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

BOOL CreateRegistryKey(HKEY hKeyParent, PWCHAR subkey);
BOOL DeleteRegistryKey(HKEY hKeyParent, PWCHAR subkey);
BOOL WriteDwordInRegistry(HKEY hKeyParent, PWCHAR subkey, PWCHAR valueName, DWORD data);
BOOL ReadDwordFromRegistry(HKEY hKeyParent, PWCHAR subkey, PWCHAR valueName, DWORD* readData);
BOOL WriteStringInRegistry(HKEY hKeyParent, PWCHAR subkey, PWCHAR valueName, PWCHAR strData);
BOOL ReadStringFromRegistry(HKEY hKeyParent, PWCHAR subkey, PWCHAR valueName, PWCHAR* readData);
BOOL DeleteRegistryValue(HKEY hKeyParent, PWCHAR subkey, PWCHAR valuename);
BOOL GetText(HWND hWnd, DWORD itemId, TCHAR** res);

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
    LoadStringW(hInstance, IDC_LR9, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LR9));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LR9));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LR9);
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
      CW_USEDEFAULT, 0, 450, 350, nullptr, nullptr, hInstance, nullptr);

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
    switch (message)
    {
    case WM_CREATE:
    {                
        CreateWindow(L"EDIT", L"", ES_LEFT | WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
            200, 10, 200, 20, hWnd, (HMENU)ID_EDIT_SUBKEY, NULL, NULL);
        CreateWindow(L"EDIT", L"", ES_LEFT | WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
            200, 50, 200, 20, hWnd, (HMENU)ID_EDIT_VALUENAME, NULL, NULL);
        CreateWindow(L"EDIT", L"", ES_LEFT | WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
            200, 90, 200, 20, hWnd, (HMENU)ID_EDIT_DATA, NULL, NULL);

        CreateWindow(L"Button", L"CREATE", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE,
            10, 10, 110, 30, hWnd, (HMENU)ID_BUTTON_CREATE, NULL, NULL);
        CreateWindow(L"Button", L"DELETE", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE,
            10, 50, 110, 30, hWnd, (HMENU)ID_BUTTON_DELETE, NULL, NULL);
        CreateWindow(L"Button", L"WRITE DWORD", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE,
            10, 90, 110, 30, hWnd, (HMENU)ID_BUTTON_WRITE_DWORD, NULL, NULL);
        CreateWindow(L"Button", L"WRITE STRING", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE,
            10, 130, 110, 30, hWnd, (HMENU)ID_BUTTON_WRITE_STRING, NULL, NULL);
        CreateWindow(L"Button", L"READ DWORD", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE,
            10, 170, 110, 30, hWnd, (HMENU)ID_BUTTON_READ_DWORD, NULL, NULL);
        CreateWindow(L"Button", L"READ STRING", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE,
            10, 210, 110, 30, hWnd, (HMENU)ID_BUTTON_READ_STRING, NULL, NULL);
        CreateWindow(L"Button", L"DELETE VALUE", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE,
            10, 250, 110, 30, hWnd, (HMENU)ID_BUTTON_DELETE_VALUE, NULL, NULL);

        break;
    }
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case ID_BUTTON_CREATE:
            {                
                TCHAR* text = NULL;
                if (GetText(hWnd, ID_EDIT_SUBKEY, &text))
                {                                    
                    CreateRegistryKey(HKEY_CURRENT_USER, text);
                }                         
            }                
                break;
            case ID_BUTTON_DELETE:
            {
                TCHAR* text = NULL;
                if (GetText(hWnd, ID_EDIT_SUBKEY, &text))
                {
                    DeleteRegistryKey(HKEY_CURRENT_USER, text);
                }
            }
            break;
            case ID_BUTTON_WRITE_DWORD:
            {
                TCHAR* text1 = NULL;
                TCHAR* text2 = NULL;
                TCHAR* text3 = NULL;
                                      
                if (GetText(hWnd, ID_EDIT_SUBKEY, &text1) &&
                    GetText(hWnd, ID_EDIT_VALUENAME, &text2) &&
                    GetText(hWnd, ID_EDIT_DATA, &text3))
                {                                             
                    WriteDwordInRegistry(HKEY_CURRENT_USER, text1, text2, wcstod(text3, 0));                    
                }                
            }                
                break;
            case ID_BUTTON_WRITE_STRING:
            {
                TCHAR* text1 = NULL;
                TCHAR* text2 = NULL;
                TCHAR* text3 = NULL;

                if (GetText(hWnd, ID_EDIT_SUBKEY, &text1) &&
                    GetText(hWnd, ID_EDIT_VALUENAME, &text2) &&
                    GetText(hWnd, ID_EDIT_DATA, &text3))
                {
                    WriteStringInRegistry(HKEY_CURRENT_USER, text1, text2, text3);
                }                
            }                
                break;
            case ID_BUTTON_READ_DWORD:
            {                
                TCHAR* text1 = NULL;
                TCHAR* text2 = NULL;

                if (GetText(hWnd, ID_EDIT_SUBKEY, &text1) &&
                    GetText(hWnd, ID_EDIT_VALUENAME, &text2))
                {
                    DWORD res;
                    TCHAR temp[10];
                    ReadDwordFromRegistry(HKEY_CURRENT_USER, text1, text2, &res);      
                    _itow_s(res, temp, 10);
                    MessageBox(hWnd, temp, L"dword from registry", NULL);
                }                
            }
                break;
            case ID_BUTTON_READ_STRING:
            {
                TCHAR* text1 = NULL;
                TCHAR* text2 = NULL;

                if (GetText(hWnd, ID_EDIT_SUBKEY, &text1) &&
                    GetText(hWnd, ID_EDIT_VALUENAME, &text2))
                {
                    PWCHAR res;
                    ReadStringFromRegistry(HKEY_CURRENT_USER, text1, text2, &res);
                    MessageBox(hWnd, res, L"string from registry", NULL);
                }                
            }                
                break;
            case ID_BUTTON_DELETE_VALUE:
            {
                TCHAR* text1 = NULL;
                TCHAR* text2 = NULL;

                if (GetText(hWnd, ID_EDIT_SUBKEY, &text1) &&
                    GetText(hWnd, ID_EDIT_VALUENAME, &text2))
                {                    
                    DeleteRegistryValue(HKEY_CURRENT_USER, text1, text2);                    
                }
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
            TextOut(hdc, 150, 10, L"subkey", 6);
            TextOut(hdc, 125, 50, L"valueName", 9);
            TextOut(hdc, 170, 90, L"data", 4);
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

BOOL CreateRegistryKey(HKEY hKeyParent, PWCHAR subkey)
{
    DWORD dwDisposition; 
    HKEY  hKey;
    DWORD Ret;
    
    Ret = RegCreateKeyEx(hKeyParent, subkey, 0, NULL, REG_OPTION_NON_VOLATILE,
        KEY_ALL_ACCESS, NULL, &hKey, &dwDisposition);
    if (Ret != ERROR_SUCCESS)
    {        
        return FALSE;
    }
    RegCloseKey(hKey);
    return TRUE;
}

BOOL DeleteRegistryKey(HKEY hKeyParent, PWCHAR subkey)
{    
    DWORD Ret;

    Ret = RegDeleteTree(hKeyParent, subkey);
    if (Ret != ERROR_SUCCESS)
    {
        return FALSE;
    }
    return TRUE;
}

BOOL WriteDwordInRegistry(HKEY hKeyParent, PWCHAR subkey, PWCHAR valueName, DWORD data)
{
    DWORD Ret;
    HKEY hKey;
    
    Ret = RegOpenKeyEx(hKeyParent, subkey, 0, KEY_WRITE, &hKey);
    
    if (Ret == ERROR_SUCCESS)
    {        
        if (ERROR_SUCCESS != RegSetValueEx(hKey, valueName, 0, REG_DWORD, 
            (BYTE*)(&data), sizeof(data)))
        {
            RegCloseKey(hKey);
            return FALSE;
        }
        
        RegCloseKey(hKey);
        return TRUE;
    }
    return FALSE;
}

BOOL ReadDwordFromRegistry(HKEY hKeyParent, PWCHAR subkey, PWCHAR valueName, DWORD* readData)
{
    HKEY hKey;
    DWORD Ret;
    
    Ret = RegOpenKeyEx(hKeyParent, subkey, 0, KEY_READ, &hKey);
    
    if (Ret == ERROR_SUCCESS)
    {
        DWORD data;
        DWORD len = sizeof(DWORD);
        Ret = RegQueryValueEx(hKey, valueName, NULL, NULL, (LPBYTE)(&data), &len);
        
        if (Ret == ERROR_SUCCESS)
        {
            RegCloseKey(hKey);
            (*readData) = data;
            return TRUE;
        }
        RegCloseKey(hKey);
        return FALSE;
    }
    else
    {
        return FALSE;
    }
}

BOOL WriteStringInRegistry(HKEY hKeyParent, PWCHAR subkey, PWCHAR valueName, PWCHAR strData)
{
    DWORD Ret;
    HKEY hKey;
    
    Ret = RegOpenKeyEx(hKeyParent, subkey, 0, KEY_WRITE, &hKey);

    if (Ret == ERROR_SUCCESS)
    {
        if (ERROR_SUCCESS != RegSetValueEx(hKey, valueName, 0, REG_SZ,
            (LPBYTE)(strData), ((((DWORD)lstrlen(strData) + 1)) * 2)))
        {
            RegCloseKey(hKey);
            return FALSE;
        }
        RegCloseKey(hKey);
        return TRUE;
    }
    return FALSE;
}

BOOL ReadStringFromRegistry(HKEY hKeyParent, PWCHAR subkey, PWCHAR valueName, PWCHAR* readData)
{
    HKEY hKey;
    DWORD len = TOTAL_BYTES_READ;
    DWORD readDataLen = len;
    PWCHAR readBuffer = (PWCHAR)malloc(sizeof(PWCHAR) * len);
    
    if (readBuffer == NULL)
        return FALSE;
    
    DWORD Ret = RegOpenKeyEx(hKeyParent, subkey, 0, KEY_READ, &hKey);

    if (Ret == ERROR_SUCCESS)
    {
        Ret = RegQueryValueEx(hKey, valueName, NULL, NULL, (BYTE*)readBuffer, &readDataLen);
        
        while (Ret == ERROR_MORE_DATA)
        {            
            len += OFFSET_BYTES;
            readBuffer = (PWCHAR)realloc(readBuffer, len);
            readDataLen = len;
            Ret = RegQueryValueEx(hKey, valueName, NULL, NULL, (BYTE*)readBuffer, &readDataLen);
        }
        if (Ret != ERROR_SUCCESS)
        {
            RegCloseKey(hKey);
            return FALSE;;
        }
        *readData = readBuffer;
        RegCloseKey(hKey);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

BOOL DeleteRegistryValue(HKEY hKeyParent, PWCHAR subkey, PWCHAR valueName)
{
    DWORD Ret;
    HKEY hKey;

    Ret = RegOpenKeyEx(hKeyParent, subkey, 0, KEY_ALL_ACCESS, &hKey);

    if (Ret == ERROR_SUCCESS)
    {
        if (ERROR_SUCCESS != RegDeleteValue(hKey, valueName))
        {
            RegCloseKey(hKey);
            return FALSE;
        }
        RegCloseKey(hKey);
        return TRUE;
    }
    return FALSE;
}
BOOL GetText(HWND hWnd, DWORD itemId, TCHAR** res)  
{
    int leng = GetWindowTextLength(GetDlgItem(hWnd, itemId));    

    if (leng > 0)
    {
        leng++;
        
        *res = (TCHAR*)malloc(sizeof(TCHAR) * leng);

        GetDlgItemText(hWnd, itemId, *res, leng);        
        if (!res)
        {
            MessageBox(hWnd, L"can't get text from item", NULL, NULL);
            return FALSE;
        }        
        return TRUE;
    }
    MessageBox(hWnd, L"can't get text from item", NULL, NULL);
    return FALSE;
}