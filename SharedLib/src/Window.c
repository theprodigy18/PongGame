#include "pch.h"
#include "Window.h"

#pragma region INTERNAL
static LRESULT CALLBACK InternalWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;
    default:
        break;
    }

    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

static bool      s_isInitialized = false;
static HINSTANCE s_hInstance     = NULL;
static i32       s_wndCount      = 0;
#pragma endregion

bool DROP_CreateWindow(const WndInitProps* pProps, WndHandle* pHandle)
{
    ASSERT_MSG(pProps, "Window properties are null.");
    *pHandle = NULL;

    if (!s_isInitialized)
    {
        s_hInstance = GetModuleHandleW(NULL);
        if (!s_hInstance)
        {
            ASSERT_MSG(false, "Failed to get module handle.");
            return false;
        }

        WNDCLASSEXW wcex = {
            .cbSize        = sizeof(WNDCLASSEXW),
            .style         = CS_OWNDC,
            .hInstance     = s_hInstance,
            .lpszClassName = L"PongWindowClass",
            .lpszMenuName  = NULL,
            .cbClsExtra    = 0,
            .cbWndExtra    = 0,
            .hCursor       = LoadCursorW(NULL, IDC_ARROW),
            .hIcon         = LoadIconW(NULL, IDI_APPLICATION),
            .hIconSm       = LoadIconW(NULL, IDI_APPLICATION),
            .lpfnWndProc   = InternalWndProc};

        if (!RegisterClassExW(&wcex))
        {
            ASSERT_MSG(false, "Failed to register window class.");
            return false;
        }

        s_isInitialized = true;
    }

    DWORD dwStyle = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
    RECT  rc      = {0, 0, pProps->width, pProps->height};
    AdjustWindowRect(&rc, dwStyle, FALSE);

    HWND hwnd = CreateWindowExW(
        0,
        L"PongWindowClass", pProps->title,
        dwStyle,
        CW_USEDEFAULT, CW_USEDEFAULT,
        rc.right - rc.left,
        rc.bottom - rc.top,
        NULL,
        NULL,
        s_hInstance,
        NULL);
    if (!hwnd)
    {
        ASSERT_MSG(false, "Failed to create window.");
        return false;
    }

    WndHandle handle = ALLOC_SINGLE(_WndHandle);
    ASSERT(handle);
    handle->hwnd = hwnd;
    handle->hdc  = GetDC(hwnd);

    *pHandle = handle;

    ++s_wndCount;

    return true;
}

void DROP_DestroyWindow(WndHandle* pHandle)
{
    ASSERT_MSG(pHandle && *pHandle, "Window handle is null.");
    WndHandle handle = *pHandle;

    if (handle)
    {
        ReleaseDC(handle->hwnd, handle->hdc);
        DestroyWindow(handle->hwnd);

        handle->hwnd = NULL;
        handle->hdc  = NULL;

        FREE(handle);

        --s_wndCount;
    }
    *pHandle = NULL;

    if (s_wndCount == 0)
    {
        UnregisterClassW(L"PongWindowClass", s_hInstance);
        s_hInstance     = NULL;
        s_isInitialized = false;
    }
}

bool DROP_PollEvents()
{
    MSG msg; 
	while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
    {
		if (msg.message == WM_QUIT)
		{
			return false;
		}

		TranslateMessage(&msg);
		DispatchMessageW(&msg);
    }

	return true;
}
