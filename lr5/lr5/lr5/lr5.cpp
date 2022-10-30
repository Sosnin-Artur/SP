
#include "framework.h"
#include "lr5.h"

#define MAX_LOADSTRING 100
// Global Variables:
HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

HWND hWndCanvas;

const INT buttonWidth = 100;
const INT buttonHeight = 20;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_LR5, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LR5));

	MSG msg;

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

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LR5));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_LR5);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	HWND hWnd = CreateWindowW(szWindowClass, (LPWSTR)"", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, 2 * buttonWidth, 140, nullptr, nullptr, hInstance, nullptr);		

	hWndCanvas = FindWindow(NULL, L"lr5_canvas");

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{	
	switch (message)
	{
	case WM_CREATE:
	{
		CreateWindow(L"BUTTON", L"Draw", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 0, 0, buttonWidth, buttonHeight, hWnd, (HMENU)ID_CB, hInst, NULL);

		CreateWindow(L"BUTTON", L"RED", WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON, 0, buttonHeight, buttonWidth, buttonHeight, hWnd, (HMENU)ID_RBC_RED, hInst, NULL);
		CreateWindow(L"BUTTON", L"GREEN", WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON, 0, 2 * buttonHeight, buttonWidth, buttonHeight, hWnd, (HMENU)ID_RBC_GREEN, hInst, NULL);
		CreateWindow(L"BUTTON", L"BLUE", WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON, 0, 3 * buttonHeight, buttonWidth, buttonHeight, hWnd, (HMENU)ID_RBC_BLUE, hInst, NULL);

		CreateWindow(L"BUTTON", L"CIRCLE", WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON, buttonWidth, 0, buttonWidth, buttonHeight, hWnd, (HMENU)ID_RBS_CIRCLE, hInst, NULL);
		CreateWindow(L"BUTTON", L"SQUIRE", WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON, buttonWidth, buttonHeight, buttonWidth, buttonHeight, hWnd, (HMENU)ID_RBS_SQUIRE, hInst, NULL);
		CreateWindow(L"BUTTON", L"STAR", WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON, buttonWidth, 2 * buttonHeight, buttonWidth, buttonHeight, hWnd, (HMENU)ID_RBS_STAR, hInst, NULL);
		CreateWindow(L"BUTTON", L"RHOMB", WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON, buttonWidth, 3 * buttonHeight, buttonWidth, buttonHeight, hWnd, (HMENU)ID_RBS_RHOMB, hInst, NULL);
		break;
	}
	case WM_COMMAND:
	{
		UINT wmId = LOWORD(wParam);

		if (wmId >= ID_RBC_RED && wmId <= ID_RBC_BLUE)
		{
			if (hWndCanvas == NULL) break;

			SendMessage(hWndCanvas, WM_COLOR, 0, wmId);

			CheckRadioButton(hWnd, ID_RBC_RED, ID_RBC_BLUE, wmId);
		}
		else if (wmId >= ID_RBS_CIRCLE && wmId <= ID_RBS_RHOMB)
		{
			if (hWndCanvas == NULL) break;

			SendMessage(hWndCanvas, WM_SHAPE, 0, wmId);

			CheckRadioButton(hWnd, ID_RBS_CIRCLE, ID_RBS_RHOMB, wmId);
		}
		else switch (wmId)
		{
		case ID_CB:
		{
			if (hWndCanvas == NULL) break;

			SendMessage(hWndCanvas, WM_DRAW, 0, 0);

			HWND hWndCheck = GetDlgItem(hWnd, ID_CB);
			LRESULT state = SendMessage(hWndCheck, BM_GETCHECK, 0, 0);

			if (state == BST_CHECKED)
			{
				SendMessage(hWndCanvas, WM_DRAW, 0, TRUE);
			}
			if (state == BST_UNCHECKED)
			{
				SendMessage(hWndCanvas, WM_DRAW, 0, FALSE);
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
		break;
	}		
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
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
