#include <minwindef.h>
#include <windows.h>
#include <stdio.h>


LRESULT CALLBACK MainWindowCallback(HWND Window,
                                    UINT Message,
                                    WPARAM wParam,
                                    LPARAM lParam)
{

    LRESULT Result = 0;


    switch (Message) {
        case WM_SIZE :
            {
                OutputDebugStringA("WM_SIZE\n");
            } break;
        case WM_DESTROY :
            {
                OutputDebugStringA("WM_DESTROYk\n");
            } break;
        case WM_CLOSE :
            {
                OutputDebugStringA("WM_CLOSE\n");
            } break;
        case WM_ACTIVATEAPP :
            {
                OutputDebugStringA("WM_ACTIVATEAPP\n");
            } break;
        default :
            {
                // OutputDebugStringA("default\n");
                Result = DefWindowProc(Window, Message, wParam, lParam);

            } break;
    }
    return (Result);
}


int CALLBACK WinMain(
        HINSTANCE Instance,
        HINSTANCE PrevInstance,
        LPSTR CommandLine,
        int ShowCode)
{

    WNDCLASS WindowClass = {};

    //TODO(maxim) check if HREDRAW/ still matter
    WindowClass.style = CS_OWNDC|CS_HREDRAW|CS_VREDRAW;
    WindowClass.lpfnWndProc = MainWindowCallback;
    WindowClass.hInstance = Instance;
    WindowClass.lpszMenuName = "HandmadeHeroWindowClass";


    if (RegisterClass(&WindowClass)) {
        HWND WindowHandle = CreateWindowEx(
                0,
                WindowClass.lpszClassName,
                "HandmadeHeroWindowClass",
                WS_OVERLAPPEDWINDOW|WS_VISIBLE,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                0,
                0,
                Instance,
                0);
        if(WindowHandle) {
            for( ; ; ){

                MSG Message;
                BOOL MessageResult = GetMessage(&Message, 0, 0, 0); 
                if (MessageResult > 0) {
                    TranslateMessage(&Message);
                    DispatchMessageA(&Message);
                } else {
                    break;
                }

            }
        }
    } 
    return (0);
}

