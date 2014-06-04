#include "stdafx.h"
#include "assert.h"
#include "SimpleWnd.h"
#include "Resource.h"

extern TCHAR* szWindowClass;
extern HINSTANCE hInst;
extern TCHAR *m_lpszClass;

extern CSimpleWnd* GetUserData(HWND hwnd);
extern void SetUserData(HWND hwnd, CSimpleWnd *pWnd);
extern INT_PTR CALLBACK AboutEx(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);


CSimpleWnd::CSimpleWnd()
: m_hWnd(NULL)
{
}


CSimpleWnd::~CSimpleWnd()
{
    SetUserData(m_hWnd, NULL);
}

bool CSimpleWnd::CreateWnd(HWND hParent, LPCWSTR csTitle, DWORD dwStyle)
{
    HWND hwnd = ::CreateWindow(m_lpszClass, csTitle, dwStyle,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, hParent, NULL, hInst, this);
    assert(hwnd == m_hWnd);
    return m_hWnd != NULL;
}

LRESULT CSimpleWnd::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int wmId, wmEvent;
    PAINTSTRUCT ps;
    HDC hdc;

    switch (message)
    {
    case WM_COMMAND:
        wmId = LOWORD(wParam);
        wmEvent = HIWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, AboutEx);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        // TODO: Add any drawing code here...
        EndPaint(hWnd, &ps);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        break;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

INT_PTR CALLBACK AboutEx(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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

CContainerWnd::CContainerWnd()
{

}

CContainerWnd::~CContainerWnd()
{

}

LRESULT CContainerWnd::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        CreateAsyncChildWnd();
        break;
    case WM_SIZE:
        SetWindowPos(m_hChildWnd, NULL, 0, 0, LOWORD(lParam), HIWORD(lParam), SWP_NOOWNERZORDER);
        break;
    default:
        break;
    }

    return __super::WndProc(hWnd, message, wParam, lParam);
}

void CContainerWnd::CreateAsyncChildWnd()
{
    //CChildWnd *pWnd = new CChildWnd();
    //pWnd->CreateWnd(m_hWnd, _T("child wnd"), WS_CHILDWINDOW);
    //RECT rcParent;
    //GetClientRect(m_hWnd, &rcParent);
    //SetWindowPos(*pWnd, NULL, 0, 0, rcParent.right, rcParent.bottom, SWP_NOOWNERZORDER);
    //ShowWindow(*pWnd, SW_SHOW);
    //UpdateWindow(*pWnd);

    DWORD dwThreadId = 0;
    HANDLE handle = ::CreateThread(NULL, 0, &CContainerWnd::TheadProc, this, 0, &dwThreadId);
    CloseHandle(handle);
}

DWORD CContainerWnd::TheadProc(LPVOID lpParam)
{
    CContainerWnd *pParentWnd = (CContainerWnd*)lpParam;
    CChildWnd *pWnd = new CChildWnd();
    pWnd->CreateWnd(*pParentWnd, _T("child wnd"), WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_TABSTOP |
        WS_VISIBLE);
    RECT rcParent;
    GetClientRect(*pParentWnd, &rcParent);
    SetWindowPos(*pWnd, NULL, 0, 0, rcParent.right, rcParent.bottom, SWP_NOOWNERZORDER);
    ShowWindow(*pWnd, SW_SHOW);
    UpdateWindow(*pWnd);

    pParentWnd->SetChildWnd(*pWnd);

    // Main message loop:
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    delete pWnd;
    return 0;
}

LRESULT CChildWnd::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    switch (message)
    {
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        // TODO: Add any drawing code here...
        ::FillRect(hdc, &(ps.rcPaint), CreateSolidBrush(RGB(0, 0, 255)));
        EndPaint(hWnd, &ps);
        return 0;
    default:
        break;
    }

    return __super::WndProc(hWnd, message, wParam, lParam);
}
