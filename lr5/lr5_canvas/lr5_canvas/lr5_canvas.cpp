#include "framework.h"
#include "lr5_canvas.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

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

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LR5CANVAS, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LR5CANVAS));

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
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LR5CANVAS));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_LR5CANVAS);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}


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

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static BOOL draw = FALSE;
    static INT color = 0, shape = 0;
    static INT drawX = -1, drawY = -1;    

    switch (message)
    {
    case WM_LBUTTONDOWN:        
        drawX = LOWORD(lParam);
        drawY = HIWORD(lParam);
        InvalidateRect(hWnd, NULL, FALSE);
        break;
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
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
        if (draw) {
            HBRUSH brush = NULL;
            switch (color)
            {
            case ID_RBC_RED:
                brush = CreateSolidBrush(RGB(255, 0, 0));
                break;
            case ID_RBC_GREEN:
                brush = CreateSolidBrush(RGB(0, 255, 0));
                break;
            case ID_RBC_BLUE:
                brush = CreateSolidBrush(RGB(0, 0, 255));
                break;
            default:
                brush = CreateSolidBrush(RGB(255, 255, 255));
                break;
            }
            SelectObject(hdc, brush);

            switch (shape)
            {
            case ID_RBS_CIRCLE:
            {                
                Ellipse(hdc, drawX - 20, drawY - 20, drawX + 40, drawY + 40);
            }
                break;
            case ID_RBS_SQUIRE:
                Rectangle(hdc, drawX - 20, drawY - 20, drawX + 40, drawY + 40);
                break;
            case ID_RBS_STAR:
            {
                POINT p[10] = { 0, -15, 6, -6, 15, -6, 9, 3,
                                12, 15, 0, 9, -12, 15, -9, 3,
                                -15, -6, -6, -6 };
                for (int i = 0; i < 10; i++) {
                    p[i].x += drawX;
                    p[i].y += drawY;
                }
                Polygon(hdc, p, 10);
                break;
            }                
            case ID_RBS_RHOMB:
            {
                POINT p[4] = { drawX, drawY + 40, drawX + 20, drawY,
                    drawX, drawY - 40, drawX - 20, drawY};
                Polygon(hdc, p, 4); 
                break;
            }                
            }
        }
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_DRAW:
        draw = lParam;
        break;
    case WM_COLOR:
        color = lParam;
        break;
    case WM_SHAPE:
        shape = lParam;
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
