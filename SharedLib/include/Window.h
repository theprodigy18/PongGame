#pragma once

typedef struct _WndHandle
{
    HWND hwnd;
    HDC  hdc;
} _WndHandle;

typedef _WndHandle* WndHandle;

typedef struct _WndInitProps
{
    const wchar_t* title;
    i32            width;
    i32            height;
} WndInitProps;

bool DROP_CreateWindow(const WndInitProps* pProps, WndHandle* pHandle);
void DROP_DestroyWindow(WndHandle* pHandle);
bool DROP_PollEvents();
