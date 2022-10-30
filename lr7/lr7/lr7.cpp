// lr7.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "lr7.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

HWND hWnd;
HANDLE mutex;
HANDLE semaphore;

const INT step = 10;
static POINT mutexCircleCoords;
static POINT semaphoreCircleCoords;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

DWORD WINAPI MutexMoveForward(LPVOID lpParam);
DWORD WINAPI SemaphoreMoveForward(LPVOID lpParam);

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
	LoadStringW(hInstance, IDC_LR7, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LR7));

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
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LR7));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_LR7);
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
	static HANDLE threads[4];
	static BOOL mutexThreadsExists = FALSE;	
	static BOOL semaphoreThreadsExists = FALSE;
	static BOOL mutexFlag = TRUE;	
	static BOOL semaphoreFlag = TRUE;
	
	switch (msg)
	{
	case WM_CREATE:
	{
		semaphoreCircleCoords.y = 0;

		hWnd = hwnd;
		mutex = CreateMutex(NULL, FALSE, NULL);
		semaphore = CreateSemaphore(NULL, 1, 12, NULL);

		CreateWindow(L"Button", L"Mutex", WS_CHILD | WS_VISIBLE | WS_BORDER, 200, 500, 100, 30, hwnd,
			(HMENU)ID_MUTEX_BUTTON, hInst, NULL);
		CreateWindow(L"Button", L"Semaphore", WS_CHILD | WS_VISIBLE | WS_BORDER, 300, 500, 100, 30, hwnd,
			(HMENU)ID_SEMAPHORE_BUTTON, hInst, NULL);
		CreateWindow(L"Button", L"Stop", WS_CHILD | WS_VISIBLE | WS_BORDER, 400, 500, 100, 30, hwnd,
			(HMENU)ID_STOP_BUTTON, hInst, NULL);
		break;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case ID_MUTEX_BUTTON:
			if (mutexFlag) {
				if (mutexThreadsExists)
				{
					ResumeThread(threads[0]);
					ResumeThread(threads[1]);
				}
				else
				{					
					threads[0] = CreateThread(NULL, NULL, MutexMoveForward, (LPVOID)step, NULL, NULL);					
					threads[1] = CreateThread(NULL, NULL, MutexMoveForward, (LPVOID)-step, NULL, NULL);
					mutexThreadsExists = TRUE;
					WaitForMultipleObjects(2, threads, FALSE, 2);
				}
				mutexFlag = FALSE;
			}
			break;
		case ID_SEMAPHORE_BUTTON:
			if (semaphoreFlag) {
				if (semaphoreThreadsExists)
				{
					ResumeThread(threads[2]);
					ResumeThread(threads[3]);
				}
				else
				{
					threads[2] = CreateThread(NULL, NULL, SemaphoreMoveForward, (LPVOID)step, NULL, NULL);
					threads[3] = CreateThread(NULL, NULL, SemaphoreMoveForward, (LPVOID)-step, NULL, NULL);
					semaphoreThreadsExists = TRUE;
					WaitForMultipleObjects(2, threads+2, FALSE, 2);
				}
				semaphoreFlag = FALSE;
			}
			break;
		case ID_STOP_BUTTON:
			if (!mutexFlag)
			{
				SuspendThread(threads[0]);
				SuspendThread(threads[1]);			
				mutexFlag = TRUE;				
			}
			if (!semaphoreFlag)
			{				
				SuspendThread(threads[2]);
				SuspendThread(threads[3]);				
				semaphoreFlag = TRUE;
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

DWORD WINAPI MutexMoveForward(LPVOID lpParam)
{
	static INT size = 10;	
	INT s = step;

	HBRUSH brush;

	RECT rect;
	GetClientRect(hWnd, &rect);

	POINT centre;
	centre.x = (rect.right - rect.left) / 2;
	centre.y = 0;
	
	mutexCircleCoords.x = centre.x;
	mutexCircleCoords.y = centre.y;

	s = (INT)lpParam;	
	
	HDC hdc = GetDC(hWnd);

	brush = CreateSolidBrush(RGB(255, 0, 0));
	SelectObject(hdc, brush);

	DWORD waitRes;

	while (TRUE)
	{		
		waitRes = WaitForSingleObject(mutex, INFINITE);
		switch (waitRes)
		{
			case WAIT_OBJECT_0:
				__try
				{
					RECT t;
					t.left = mutexCircleCoords.x - size;
					t.top = mutexCircleCoords.y - size;
					t.right = mutexCircleCoords.x + size;
					t.bottom = mutexCircleCoords.y + size;

					mutexCircleCoords.y += 3;
					mutexCircleCoords.x += s;

					if (mutexCircleCoords.y >= rect.bottom + step)
					{						
						mutexCircleCoords.y = centre.y;
					}
					InvalidateRect(hWnd, &t, TRUE);
					Sleep(2);
					Ellipse(hdc, mutexCircleCoords.x - size, mutexCircleCoords.y - size, mutexCircleCoords.x + size, mutexCircleCoords.y + size);
					Sleep(60);
				}
				__finally
				{
					ReleaseMutex(mutex);
				}
				break;
			case WAIT_ABANDONED:
				return FALSE;
		}		
	}
	DeleteObject(brush);
	return TRUE;
}

DWORD WINAPI SemaphoreMoveForward(LPVOID lpParam)
{
	static INT size = 10;
	INT s = step;

	HBRUSH brush;

	RECT rect;
	GetClientRect(hWnd, &rect);

	POINT centre;
	centre.x = (rect.right - rect.left) / 2;
	centre.y = 0;

	semaphoreCircleCoords.x = centre.x;
	semaphoreCircleCoords.y = centre.y;

	s = (INT)lpParam;

	HDC hdc = GetDC(hWnd);

	brush = CreateSolidBrush(RGB(0, 255, 0));
	SelectObject(hdc, brush);

	DWORD waitRes;

	while (TRUE)
	{
		waitRes = WaitForSingleObject(semaphore, INFINITE);
		switch (waitRes)
		{
		case WAIT_OBJECT_0:
			__try
			{
				RECT t;
				t.left = semaphoreCircleCoords.x - size;
				t.top = semaphoreCircleCoords.y - size;
				t.right = semaphoreCircleCoords.x + size;
				t.bottom = semaphoreCircleCoords.y + size;

				semaphoreCircleCoords.y += 3;
				semaphoreCircleCoords.x += s;

				if (semaphoreCircleCoords.y >= rect.bottom + step)
				{					
					semaphoreCircleCoords.y = centre.y;
				}
				InvalidateRect(hWnd, &t, TRUE);
				Sleep(2);
				Ellipse(hdc, semaphoreCircleCoords.x - size, semaphoreCircleCoords.y - size, semaphoreCircleCoords.x + size, semaphoreCircleCoords.y + size);
				Sleep(60);
			}
			__finally
			{
				ReleaseSemaphore(semaphore, 1, NULL);
			}
			break;
		case WAIT_TIMEOUT:
			return FALSE;
		}
	}
	DeleteObject(brush);
	return TRUE;
}

