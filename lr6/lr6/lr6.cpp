// lr6.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "lr6.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

HWND hWnd;
const INT step = 10;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
DWORD WINAPI DrawCircle(LPVOID lpParam);

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
    LoadStringW(hInstance, IDC_LR6, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LR6));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LR6));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LR6);
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
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HANDLE threads[2];	
	static BOOL threads_exist = FALSE;
	static BOOL stop_flag = TRUE;

	static POINT circleCoordsFirst;
	static POINT circleCoordsSecond;

	switch (msg)
	{
	case WM_CREATE:
	{
		circleCoordsFirst.x = 0;
		circleCoordsSecond.x = -4*step;
		hWnd = hwnd;
		
		CreateWindow(L"Button", L"Start", WS_CHILD | WS_VISIBLE | WS_BORDER, 200, 500, 100, 30, hwnd,
			(HMENU)ID_START_BUTTON, hInst, NULL);
		CreateWindow(L"Button", L"Stop", WS_CHILD | WS_VISIBLE | WS_BORDER, 300, 500, 100, 30, hwnd,
			(HMENU)ID_STOP_BUTTON, hInst, NULL);
		break;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case ID_START_BUTTON:
			if (stop_flag) {
				if (threads_exist)
				{
					ResumeThread(threads[0]);
					ResumeThread(threads[1]);					
				}
				else
				{										
					threads[0] = CreateThread(NULL, NULL, DrawCircle, &circleCoordsFirst, NULL, NULL);
					threads[1] = CreateThread(NULL, NULL, DrawCircle, &circleCoordsSecond, NULL, NULL);
					threads_exist = TRUE;
				}
				stop_flag = FALSE;
			}
			break;

		case ID_STOP_BUTTON:
			if (!stop_flag)
			{
				SuspendThread(threads[0]);
				SuspendThread(threads[1]);				
				stop_flag = TRUE;
			}
			break;
		}
		break;
	}	
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
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

DWORD WINAPI DrawCircle(LPVOID lpParam)
{
	static INT size = 10;
	static INT halfPeriod = 180;

	PAINTSTRUCT ps;
	HBRUSH brush;

	RECT rect;
	GetClientRect(hWnd, &rect);
	
	POINT centre;
	centre.x = 0;
	centre.y = (rect.bottom - rect.top)/2;
	POINT coords = *((POINT*)lpParam);

	HDC hdc = GetDC(hWnd);

	brush = CreateSolidBrush(RGB(255, 0, 0));
	SelectObject(hdc, brush);	
	
	while (TRUE)
	{		
		coords.y = centre.y - sin((float)coords.x / halfPeriod) * 100;
		coords.x += step;
		if (coords.x >= rect.right + step)
		{
			coords.x = 0;
			coords.y = centre.y;
		}		
		InvalidateRect(hWnd, &rect, TRUE);
		Sleep(2);
		Ellipse(hdc, coords.x - size, coords.y - size, coords.x + size, coords.y + size);		
		Sleep(60);		
	}
	DeleteObject(brush);
	return 0;
}

