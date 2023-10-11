#include <windows.h>

#define internal static 
#define local_persist static 
#define global_variable static 
//TODO (maxim) This is flobal for now.
global_variable bool Running;
global_variable BITMAPINFO BitmapInfo; 
global_variable void *BitmapMemory;
global_variable HBITMAP BitmapHandle;



internal void Win32ResizeDIBSection(int Width, int Height) {
    //TODO (maxim) : Bulletproof this.
    //maybe don't free first, free after, then free if that false.
    

    //TODO (maxim) : Free our DIBSection
    if (BitmapHandle) {
        DeleteObject(BitmapHandle); 
    }

    BitmapInfo.bmiHeader.biSize = sizeof(BitmapInfo.bmiHeader);
    BitmapInfo.bmiHeader.biWidth = Width; 
    BitmapInfo.bmiHeader.biHeight = Height;
    BitmapInfo.bmiHeader.biPlanes = 1;
    BitmapInfo.bmiHeader.biBitCount = 32;
    BitmapInfo.bmiHeader.biCompression = BI_RGB;

    HDC DeviceContext = CreateCompatibleDC(0); // ??? stop whatch at 56:07

    BitmapHandle = CreateDIBSection(
            DeviceContext, &BitmapInfo,
            DIB_RGB_COLORS,
            &BitmapMemory,
            0,
            0);

    ReleaseDC(0, DeviceContext);
}

internal void 
Win32UpdateWindow(HDC DeviceContext, int X, int Y, int Width, int Height) {
    StretchDIBits(DeviceContext,
                 X, Y, Width, Height,
                 X, Y, Width, Height,
                 const VOID *lpBits,
                 const BITMAPINFO *lpBitsIndo,
                 DIB_RGB_COLORS, SRCCOPY);    
}

LRESULT CALLBACK Win32MainWindowCallback(HWND Window,
                                    UINT Message,
                                    WPARAM wParam,
                                    LPARAM lParam)
{
    LRESULT Result = 0;

    switch (Message) {
        case WM_SIZE :
            {
                RECT ClientRec;
                GetClientRect(Window, &ClientRec);
                int Width = ClientRec.right - ClientRec.left;
                int Height = ClientRec.bottom - ClientRec.top;
                Win32ResizeDIBSection(Width, Height);
                OutputDebugStringA("WM_SIZE\n");
            } break;
        case WM_CLOSE :
            {
                //TODO (maxim) : handle this with a message to the user?
                Running = false;
            } break;
        case WM_ACTIVATEAPP :
            {
                OutputDebugStringA("WM_ACTIVATEAPP\n");
            } break;
        case WM_DESTROY :
            {
                //TODO (maxim) : handle this as an error - recreate window?
                Running = false;
            } break;
        case WM_PAINT :
            {
                PAINTSTRUCT Paint;
                HDC DeviceContext = BeginPaint(Window, &Paint);
                int X = Paint.rcPaint.left;
                int Y = Paint.rcPaint.top;
                int Height = Paint.rcPaint.bottom - Paint.rcPaint.top;
                int Width = Paint.rcPaint.right - Paint.rcPaint.left;
                // Win32UpdateWindow(Window);
                Win32UpdateWindow(DeviceContext, X, Y, Width, Height);
                EndPaint(Window, &Paint);
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
    // WindowClass.style = CS_OWNDC|CS_HREDRAW|CS_VREDRAW;
    WindowClass.lpfnWndProc = Win32MainWindowCallback;
    WindowClass.hInstance = Instance;
    // WindowClass.hIcon;
    WindowClass.lpszClassName = "HandmadeHeroWindowClass";


    if (RegisterClass(&WindowClass)) {
        HWND WindowHandle = CreateWindowEx(
                0,
                WindowClass.lpszClassName,
                "Hendmade hero",
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
            Running = true;
            while(Running){

                MSG Message;
                BOOL MessageResult = GetMessageA(&Message, 0, 0, 0);
                if (MessageResult > 0) {
                    TranslateMessage(&Message);
                    DispatchMessageA(&Message);
                } else {
                    break;
                }

            }
        } else {
            //TODO (maxim) :logging
        }
    } else {
        //TODO (maxim) :logging
    }
    return (0);
}

