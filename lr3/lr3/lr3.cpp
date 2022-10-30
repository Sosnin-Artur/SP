// lr3.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "lr3.h"

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
    srand(time(NULL));
    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LR3, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LR3));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LR3));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LR3);
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
    HBRUSH hBrush;
    HPEN hPen;
    HDC hdc;

    const int maxPointsCount = 5;

    static INT maxX = 1000;
    static INT maxY = 500;
    static INT minX = 100;
    static INT minY = 60;

    switch (message)
    {
    case WM_CREATE:
    {        
        CreateWindow(L"Button", L"Draw", BS_DEFPUSHBUTTON | WS_CHILD | WS_VISIBLE | BS_OWNERDRAW | WS_BORDER, 
                     10, 10, 40, 50, hWnd, (HMENU)ID_BUTTON_DRAW, NULL, NULL);
        CreateWindow(L"Button", L"Clear", BS_DEFPUSHBUTTON | WS_CHILD | WS_VISIBLE | BS_OWNERDRAW | WS_BORDER, 
                     50, 10, 40, 50, hWnd, (HMENU)ID_BUTTON_CLEAR, NULL, NULL);
        break;
    }
    case WM_DRAWITEM:
    {
        LPDRAWITEMSTRUCT pdis = (LPDRAWITEMSTRUCT)lParam;
        
        switch (pdis->CtlID)
        {
        case ID_BUTTON_DRAW:
        {
            DrawText(pdis->hDC, L"Draw", -1, &pdis->rcItem, DT_CENTER | DT_VCENTER);
                        
            hPen = CreatePen(PS_SOLID, 2, RGB(97, 85, 51));
            SelectObject(pdis->hDC, hPen);
            hBrush = CreateSolidBrush(RGB(97, 85, 51));
            SelectObject(pdis->hDC, hBrush);
            Ellipse(pdis->hDC, 13, 15, 17, 35);            
            
            DeleteObject(hBrush);
            DeleteObject(hPen);

            POINT poly[4];
            hBrush = CreateSolidBrush(RGB(0, 0, 0));
            SelectObject(pdis->hDC, hBrush);
            poly[0].x = 7;  poly[0].y = 40;
            poly[1].x = 10; poly[1].y = 30;
            poly[2].x = 20; poly[2].y = 30;
            poly[3].x = 23; poly[3].y = 40;
            
            Polygon(pdis->hDC, poly, 4);
            DeleteObject(hBrush);
            break;
        }
        case ID_BUTTON_CLEAR:
        {
            DrawText(pdis->hDC, L"Clear", -1, &pdis->rcItem, DT_CENTER | DT_VCENTER);            
            
            SelectObject(pdis->hDC, CreatePen(PS_SOLID, 2, RGB(0, 0, 0)));
            MoveToEx(pdis->hDC, 5, 25, NULL);
            LineTo(pdis->hDC, 10, 40);            
            LineTo(pdis->hDC, 20, 40);
            LineTo(pdis->hDC, 25, 25);
            break;
        }
        }
        break;
    }    
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
            
        case ID_BUTTON_DRAW:
        {
            hdc = GetDC(hWnd);
            for (int i = 0; i < 2; i++)
            {
                hBrush = CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256));                    
                hPen = CreatePen(PS_SOLID, 2, RGB(rand() % 256, rand() % 256, rand() % 256));
                    
                SelectObject(hdc, hPen);
                SelectObject(hdc, hBrush);
                    
                Ellipse(hdc, rand() % maxX + minX, rand() % maxY + minY, rand() % maxX + minX, rand() % maxY + minY);
                Rectangle(hdc, rand() % maxX + minX, rand() % maxY + minY, rand() % maxX + minX, rand() % maxY + minY);
                    
                MoveToEx(hdc, rand() % maxX + minX, rand() % maxY + minY, NULL);
                LineTo(hdc, rand() % maxX + minX, rand() % maxY + minY);

                const int count = rand() % maxPointsCount;
                POINT *poly = new POINT[count];
                for (int j = 0; j < count; j++)
                {
                    poly[j].x = rand() % maxX + minX;
                    poly[j].y = rand() % maxY + minY;
                }
                Polygon(hdc, poly, count);

                Arc(hdc, rand() % maxX + minX, rand() % maxY + minY, rand() % maxX + minX, rand() % maxY + minY, 
                            rand() % maxX + minX, rand() % maxY + minY, rand() % maxX + minX, rand() % maxY + minY);

                DeleteObject(hPen);
                DeleteObject(hBrush);
            }

            for (int i = 0; i < 10; i++)
            {
                hBrush = CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256));
                hPen = CreatePen(PS_SOLID, 2, RGB(rand() % 256, rand() % 256, rand() % 256));

                SelectObject(hdc, hPen);
                SelectObject(hdc, hBrush);

                int ind = rand() % 5;

                switch (ind)
                {
                case 0:
                    Ellipse(hdc, rand() % maxX + minX, rand() % maxY + minY, rand() % maxX + minX, rand() % maxY + minY);
                    break;
                case 1:
                    Rectangle(hdc, rand() % maxX + minX, rand() % maxY + minY, rand() % maxX + minX, rand() % maxY + minY);
                    break;
                case 2:
                    MoveToEx(hdc, rand() % maxX + minX, rand() % maxY + minY, NULL);
                    LineTo(hdc, rand() % maxX + minX, rand() % maxY + minY);
                    break;
                case 3:
                {
                    const int count = rand() % maxPointsCount;
                    POINT* poly = new POINT[count];
                    for (int j = 0; j < count; j++)
                    {
                        poly[j].x = rand() % maxX + minX;
                        poly[j].y = rand() % maxY + minY;
                    }
                    Polygon(hdc, poly, count);
                    break;
                }                        
                case 4:                        
                    Arc(hdc, rand() % maxX + minX, rand() % maxY + minY, rand() % maxX + minX, rand() % maxY + minY,
                        rand() % maxX + minX, rand() % maxY + minY, rand() % maxX + minX, rand() % maxY + minY);
                    break;
                }                    
                DeleteObject(hPen);
                DeleteObject(hBrush);
            }                
            break;
        }
        case ID_BUTTON_CLEAR:
        {
            HDC hdc = GetDC(hWnd);
            RECT rect;
            GetClientRect(hWnd, &rect);    
            rect.left += minX;
            rect.top += minY;
            FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW + 1));
            break;
        }            
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
