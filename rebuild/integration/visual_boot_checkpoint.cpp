#include "fable_visual_boot.h"

#include <string.h>

// The recovered VC7.1 compiler bundle does not yet contain a Platform SDK.
// Keep this checkpoint buildable with that compiler by declaring only the
// stable Win32 surface used here. These declarations can disappear when the
// period-correct SDK is added.
typedef int FableBool;
typedef unsigned short FableAtom;
typedef unsigned short FableWord;
typedef unsigned long FableDword;
typedef long FableLong;
typedef unsigned int FableUint;
typedef unsigned long FableWordParameter;
typedef long FableLongParameter;
typedef long FableResult;
typedef void* FableWindow;
typedef void* FableDeviceContext;
typedef void* FableBitmap;
typedef void* FableBrush;
typedef void* FableGraphicsObject;
typedef void* FableCursor;
typedef void* FableIcon;
typedef void* FableMenu;

struct FablePoint
{
    FableLong x;
    FableLong y;
};

struct FableRectangle
{
    FableLong left;
    FableLong top;
    FableLong right;
    FableLong bottom;
};

struct FableMessage
{
    FableWindow window;
    FableUint message;
    FableWordParameter wordParameter;
    FableLongParameter longParameter;
    FableDword time;
    FablePoint point;
};

struct FablePaint
{
    FableDeviceContext deviceContext;
    FableBool erase;
    FableRectangle paint;
    FableBool restore;
    FableBool incrementalUpdate;
    unsigned char reserved[32];
};

struct FableBitmapInfo
{
    FableLong type;
    FableLong width;
    FableLong height;
    FableLong widthBytes;
    FableWord planes;
    FableWord bitsPerPixel;
    void* pixels;
};

typedef FableResult (FABLE_STDCALL *FableWindowProcedure)(
    FableWindow,
    FableUint,
    FableWordParameter,
    FableLongParameter);

struct FableWindowClass
{
    FableUint size;
    FableUint style;
    FableWindowProcedure procedure;
    int classExtraBytes;
    int windowExtraBytes;
    FableInstanceHandle instance;
    FableIcon icon;
    FableCursor cursor;
    FableBrush background;
    const char* menuName;
    const char* className;
    FableIcon smallIcon;
};

extern "C"
{
    __declspec(dllimport) FableDeviceContext FABLE_STDCALL BeginPaint(
        FableWindow window,
        FablePaint* paint);
    __declspec(dllimport) FableBool FABLE_STDCALL EndPaint(
        FableWindow window,
        const FablePaint* paint);
    __declspec(dllimport) int FABLE_STDCALL FillRect(
        FableDeviceContext context,
        const FableRectangle* rectangle,
        FableBrush brush);
    __declspec(dllimport) FableBool FABLE_STDCALL GetClientRect(
        FableWindow window,
        FableRectangle* rectangle);
    __declspec(dllimport) void* FABLE_STDCALL LoadImageA(
        FableInstanceHandle instance,
        const char* name,
        FableUint type,
        int desiredWidth,
        int desiredHeight,
        FableUint loadFlags);
    __declspec(dllimport) FableCursor FABLE_STDCALL LoadCursorA(
        FableInstanceHandle instance,
        const char* name);
    __declspec(dllimport) FableAtom FABLE_STDCALL RegisterClassExA(
        const FableWindowClass* windowClass);
    __declspec(dllimport) FableBool FABLE_STDCALL UnregisterClassA(
        const char* className,
        FableInstanceHandle instance);
    __declspec(dllimport) FableBool FABLE_STDCALL AdjustWindowRectEx(
        FableRectangle* rectangle,
        FableDword style,
        FableBool hasMenu,
        FableDword extendedStyle);
    __declspec(dllimport) FableWindow FABLE_STDCALL CreateWindowExA(
        FableDword extendedStyle,
        const char* className,
        const char* title,
        FableDword style,
        int x,
        int y,
        int width,
        int height,
        FableWindow parent,
        FableMenu menu,
        FableInstanceHandle instance,
        void* parameter);
    __declspec(dllimport) FableBool FABLE_STDCALL ShowWindow(
        FableWindow window,
        int showCommand);
    __declspec(dllimport) FableBool FABLE_STDCALL UpdateWindow(
        FableWindow window);
    __declspec(dllimport) FableResult FABLE_STDCALL DefWindowProcA(
        FableWindow window,
        FableUint message,
        FableWordParameter wordParameter,
        FableLongParameter longParameter);
    __declspec(dllimport) void FABLE_STDCALL PostQuitMessage(int exitCode);
    __declspec(dllimport) FableBool FABLE_STDCALL GetMessageA(
        FableMessage* message,
        FableWindow window,
        FableUint minimumMessage,
        FableUint maximumMessage);
    __declspec(dllimport) FableBool FABLE_STDCALL TranslateMessage(
        const FableMessage* message);
    __declspec(dllimport) FableResult FABLE_STDCALL DispatchMessageA(
        const FableMessage* message);

    __declspec(dllimport) FableGraphicsObject FABLE_STDCALL GetStockObject(
        int object);
    __declspec(dllimport) int FABLE_STDCALL GetObjectA(
        FableGraphicsObject object,
        int size,
        void* destination);
    __declspec(dllimport) FableDeviceContext FABLE_STDCALL CreateCompatibleDC(
        FableDeviceContext context);
    __declspec(dllimport) FableGraphicsObject FABLE_STDCALL SelectObject(
        FableDeviceContext context,
        FableGraphicsObject object);
    __declspec(dllimport) int FABLE_STDCALL SetStretchBltMode(
        FableDeviceContext context,
        int mode);
    __declspec(dllimport) FableBool FABLE_STDCALL SetBrushOrgEx(
        FableDeviceContext context,
        int x,
        int y,
        FablePoint* previous);
    __declspec(dllimport) FableBool FABLE_STDCALL StretchBlt(
        FableDeviceContext destination,
        int destinationX,
        int destinationY,
        int destinationWidth,
        int destinationHeight,
        FableDeviceContext source,
        int sourceX,
        int sourceY,
        int sourceWidth,
        int sourceHeight,
        FableDword operation);
    __declspec(dllimport) FableBool FABLE_STDCALL DeleteDC(
        FableDeviceContext context);
    __declspec(dllimport) FableBool FABLE_STDCALL DeleteObject(
        FableGraphicsObject object);
}

namespace
{
    const int kBootArtworkResource = 101;
    const char kWindowClassName[] = "FableDecompVisualBootCheckpoint";
    const char kWindowTitle[] = "FableDecomp - Visual Boot Checkpoint";

    const FableUint kImageBitmap = 0;
    const FableUint kLoadCreatedDibSection = 0x00002000;
    const int kBlackBrush = 4;
    const int kArrowCursor = 32512;
    const FableUint kClassRedrawHorizontal = 0x0002;
    const FableUint kClassRedrawVertical = 0x0001;
    const FableUint kMessageDestroy = 0x0002;
    const FableUint kMessagePaint = 0x000F;
    const FableUint kMessageEraseBackground = 0x0014;
    const FableDword kOverlappedWindow = 0x00CF0000UL;
    const int kUseDefaultPosition = static_cast<int>(0x80000000UL);
    const int kShowNormal = 1;
    const int kHalftoneStretch = 4;
    const FableDword kCopySource = 0x00CC0020UL;

    FableBitmap g_BootArtwork = 0;
    FableBitmapInfo g_BootArtworkInfo = {};

    const char* IntegerResource(int identifier)
    {
        return reinterpret_cast<const char*>(
            static_cast<unsigned long>(identifier));
    }

    void PaintBootArtwork(FableWindow window)
    {
        FablePaint paint = {};
        FableDeviceContext destination = BeginPaint(window, &paint);

        FableRectangle client = {};
        GetClientRect(window, &client);
        const int clientWidth = client.right - client.left;
        const int clientHeight = client.bottom - client.top;

        FableBrush background = static_cast<FableBrush>(
            GetStockObject(kBlackBrush));
        FillRect(destination, &client, background);

        if (
            g_BootArtwork != 0 &&
            g_BootArtworkInfo.width > 0 &&
            g_BootArtworkInfo.height > 0)
        {
            int drawWidth = clientWidth;
            int drawHeight =
                (clientWidth * g_BootArtworkInfo.height) /
                g_BootArtworkInfo.width;

            if (drawHeight > clientHeight)
            {
                drawHeight = clientHeight;
                drawWidth =
                    (clientHeight * g_BootArtworkInfo.width) /
                    g_BootArtworkInfo.height;
            }

            const int drawLeft = (clientWidth - drawWidth) / 2;
            const int drawTop = (clientHeight - drawHeight) / 2;

            FableDeviceContext source = CreateCompatibleDC(destination);
            FableGraphicsObject previous =
                SelectObject(source, g_BootArtwork);
            SetStretchBltMode(destination, kHalftoneStretch);
            SetBrushOrgEx(destination, 0, 0, 0);
            StretchBlt(
                destination,
                drawLeft,
                drawTop,
                drawWidth,
                drawHeight,
                source,
                0,
                0,
                g_BootArtworkInfo.width,
                g_BootArtworkInfo.height,
                kCopySource);
            SelectObject(source, previous);
            DeleteDC(source);
        }

        EndPaint(window, &paint);
    }

    FableResult FABLE_STDCALL VisualBootWindowProcedure(
        FableWindow window,
        FableUint message,
        FableWordParameter wordParameter,
        FableLongParameter longParameter)
    {
        switch (message)
        {
        case kMessagePaint:
            PaintBootArtwork(window);
            return 0;

        case kMessageEraseBackground:
            return 1;

        case kMessageDestroy:
            PostQuitMessage(0);
            return 0;
        }

        return DefWindowProcA(
            window,
            message,
            wordParameter,
            longParameter);
    }
}

long FABLE_FASTCALL FableRunVisualBootCheckpoint(
    FableInstanceHandle instance,
    char* commandLine,
    int showCommand)
{
    g_BootArtwork = static_cast<FableBitmap>(LoadImageA(
        instance,
        IntegerResource(kBootArtworkResource),
        kImageBitmap,
        0,
        0,
        kLoadCreatedDibSection));
    if (g_BootArtwork == 0)
        return 1;

    GetObjectA(
        g_BootArtwork,
        sizeof(g_BootArtworkInfo),
        &g_BootArtworkInfo);

    if (
        commandLine != 0 &&
        strstr(commandLine, "--verify-visual-resource") != 0)
    {
        DeleteObject(g_BootArtwork);
        g_BootArtwork = 0;
        return 0;
    }

    FableWindowClass windowClass = {};
    windowClass.size = sizeof(windowClass);
    windowClass.style =
        kClassRedrawHorizontal | kClassRedrawVertical;
    windowClass.procedure = VisualBootWindowProcedure;
    windowClass.instance = instance;
    windowClass.cursor = LoadCursorA(0, IntegerResource(kArrowCursor));
    windowClass.background = static_cast<FableBrush>(
        GetStockObject(kBlackBrush));
    windowClass.className = kWindowClassName;

    if (RegisterClassExA(&windowClass) == 0)
    {
        DeleteObject(g_BootArtwork);
        g_BootArtwork = 0;
        return 2;
    }

    FableRectangle windowBounds = {0, 0, 1280, 720};
    AdjustWindowRectEx(
        &windowBounds,
        kOverlappedWindow,
        0,
        0);

    FableWindow window = CreateWindowExA(
        0,
        kWindowClassName,
        kWindowTitle,
        kOverlappedWindow,
        kUseDefaultPosition,
        kUseDefaultPosition,
        windowBounds.right - windowBounds.left,
        windowBounds.bottom - windowBounds.top,
        0,
        0,
        instance,
        0);
    if (window == 0)
    {
        UnregisterClassA(kWindowClassName, instance);
        DeleteObject(g_BootArtwork);
        g_BootArtwork = 0;
        return 3;
    }

    ShowWindow(window, showCommand == 0 ? kShowNormal : showCommand);
    UpdateWindow(window);

    FableMessage message = {};
    while (GetMessageA(&message, 0, 0, 0) > 0)
    {
        TranslateMessage(&message);
        DispatchMessageA(&message);
    }

    UnregisterClassA(kWindowClassName, instance);
    DeleteObject(g_BootArtwork);
    g_BootArtwork = 0;
    return static_cast<long>(message.wordParameter);
}
