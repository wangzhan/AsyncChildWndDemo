#pragma once

class CSimpleWnd
{
public:
    CSimpleWnd();
    virtual ~CSimpleWnd();

public:
    bool CreateWnd(HWND hParent, LPCWSTR csTitle, DWORD dwStyle);
    operator HWND ()
    {
        return m_hWnd;
    }

    void SetHwnd(HWND hwnd) { m_hWnd = hwnd; }

    virtual LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

protected:
    HWND m_hWnd;
};

class CContainerWnd :public CSimpleWnd
{
public:
    CContainerWnd();
    ~CContainerWnd();

public:
    virtual LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    void CreateAsyncChildWnd();

    static DWORD WINAPI TheadProc(LPVOID lpParam);

    void SetChildWnd(HWND hwnd) { m_hChildWnd = hwnd; }

private:
    HWND m_hChildWnd;
};

class CChildWnd : public CSimpleWnd
{
public:
    CChildWnd() {}
    ~CChildWnd() {}

public:
    virtual LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

