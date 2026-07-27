#include "fable_visual_boot.h"
#include "fable_visual_d3d9.h"
#include "frontend_startup_sequence.h"
#include "retail_video_bridge.h"

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
    __declspec(dllimport) FableBool FABLE_STDCALL SetWindowTextA(
        FableWindow window,
        const char* text);
    __declspec(dllimport) FableWordParameter FABLE_STDCALL SetTimer(
        FableWindow window,
        FableWordParameter identifier,
        FableUint intervalMilliseconds,
        void* timerProcedure);
    __declspec(dllimport) FableBool FABLE_STDCALL KillTimer(
        FableWindow window,
        FableWordParameter identifier);
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
    const int kBootTitleResource = 102;
    const char kWindowClassName[] = "FableDecompVisualBootCheckpoint";
    const char kWindowTitle[] = "FableDecomp - Visual Boot Checkpoint";
    const char kProgressReadyWindowTitle[] =
        "FableDecomp - Retail Progress Display Ready";
    const char kProgressActiveWindowTitle[] =
        "FableDecomp - Retail Progress Display Active";
    const char kRetailVideoStartingWindowTitle[] =
        "FableDecomp - Retail WMV Starting";
    const char kRetailVideoPlayingWindowTitle[] =
        "FableDecomp - Retail WMV Playing - Recovered Boot + D3D9";
    const char kRetailVideoUpscaledPlayingWindowTitle[] =
        "FableDecomp - Retail WMV Playing - AI 2x Enhanced";
    const char kRetailVideoSequenceCompleteWindowTitle[] =
        "FableDecomp - Retail Boot Movies Complete - Frontend Checkpoint Ready - Post-Movie Startup Ordered";
    const char kRetailVideoSequenceFailedWindowTitle[] =
        "FableDecomp - Retail Boot Movies Complete - Frontend Startup Boundary Failed";
    const char kRetailVideoCompleteWindowTitle[] =
        "FableDecomp - Retail Movie Complete - Frontend Ready";
#if defined(FABLETLC_RETAIL_FRONTEND_ARTWORK)
    const char kRetailAssetReadyWindowTitle[] =
        "FableDecomp - Retail Frontend Asset + Progress Display Ready";
    const char kRetailAssetActiveWindowTitle[] =
        "FableDecomp - Retail Frontend Asset + Progress Display Active";
    const char kD3DRetailAssetReadyWindowTitle[] =
        "FableDecomp - D3D9 Retail Frontend + Progress Display Ready";
    const char kD3DRetailAssetActiveWindowTitle[] =
        "FableDecomp - D3D9 Retail Frontend + Progress Display Active";
    const char kD3DRetailAssetPresentedReadyWindowTitle[] =
        "FableDecomp - D3D9 Presented via Render2D - Retail Frontend + Progress Display Ready";
    const char kD3DRetailAssetPresentedActiveWindowTitle[] =
        "FableDecomp - D3D9 Presented via Render2D - Retail Frontend + Progress Display Active";
#else
    const char kD3DCheckpointReadyWindowTitle[] =
        "FableDecomp - D3D9 Visual Checkpoint Ready";
    const char kD3DCheckpointActiveWindowTitle[] =
        "FableDecomp - D3D9 Visual Checkpoint Active";
    const char kD3DCheckpointPresentedReadyWindowTitle[] =
        "FableDecomp - D3D9 Presented via Render2D - Visual Checkpoint Ready";
    const char kD3DCheckpointPresentedActiveWindowTitle[] =
        "FableDecomp - D3D9 Presented via Render2D - Visual Checkpoint Active";
#endif

    const FableUint kImageBitmap = 0;
    const FableUint kLoadCreatedDibSection = 0x00002000;
    const int kBlackBrush = 4;
    const int kArrowCursor = 32512;
    const FableUint kClassRedrawHorizontal = 0x0002;
    const FableUint kClassRedrawVertical = 0x0001;
    const FableUint kMessageDestroy = 0x0002;
    const FableUint kMessageSize = 0x0005;
    const FableUint kMessagePaint = 0x000F;
    const FableUint kMessageEraseBackground = 0x0014;
    const FableUint kMessageKeyDown = 0x0100;
    const FableUint kMessageKeyUp = 0x0101;
    const FableUint kMessageTimer = 0x0113;
    const FableWordParameter kRetailVideoTimer = 1;
    const FableWordParameter kEscapeKey = 0x1B;
    const char* const kRetailBootMovies[] = {
        "lionhead_logo.wmv",
        "microsoft_logo.wmv",
        "intro_comp.wmv"
    };
    const char* const kRetailBootPlayingWindowTitles[] = {
        "FableDecomp - Retail WMV Playing 1/3 - Lionhead",
        "FableDecomp - Retail WMV Playing 2/3 - Microsoft",
        "FableDecomp - Retail WMV Playing 3/3 - Intro"
    };
    const char* const kRetailBootUpscaledPlayingWindowTitles[] = {
        "FableDecomp - Retail WMV Playing 1/3 - Lionhead - AI 2x",
        "FableDecomp - Retail WMV Playing 2/3 - Microsoft - AI 2x",
        "FableDecomp - Retail WMV Playing 3/3 - Intro - AI 2x"
    };
    const unsigned int kRetailBootMovieCount =
        sizeof(kRetailBootMovies) / sizeof(kRetailBootMovies[0]);
    const FableDword kOverlappedWindow = 0x00CF0000UL;
    const int kUseDefaultPosition = static_cast<int>(0x80000000UL);
    const int kShowNormal = 1;
    const int kHalftoneStretch = 4;
    const FableDword kCopySource = 0x00CC0020UL;

    FableBitmap g_BootArtwork = 0;
    FableBitmapInfo g_BootArtworkInfo = {};
    FableBitmap g_BootTitleArtwork = 0;
    FableBitmapInfo g_BootTitleArtworkInfo = {};
    bool g_RetailProgressDisplayPresent = false;
    bool g_RetailProgressDisplayActive = false;
    FableWindow g_RetailVideoWindow = 0;
    FableInstanceHandle g_RetailVideoInstance = 0;
    unsigned int g_RetailBootMovieIndex = 0;
    bool g_RetailBootSequenceActive = false;
    bool g_RetailVideoReachedRunningState = false;
    bool g_RetailVideoEscapePressed = false;
    unsigned long g_FrontendPostMovieStepMask = 0;

    bool FABLE_FASTCALL RunVisualFrontendPostMovieStep(
        void*,
        FableFrontendPostMovieStep step)
    {
        g_FrontendPostMovieStepMask |=
            1UL << static_cast<unsigned long>(step);
        return true;
    }

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

    bool PaintBootArtworkD3D9(FableWindow window)
    {
        if (!FableIsVisualD3D9Active())
            return false;

        FableRectangle client = {};
        GetClientRect(window, &client);
        if (!FableRenderVisualD3D9(
                client.right - client.left,
                client.bottom - client.top))
        {
            return false;
        }

        FablePaint paint = {};
        BeginPaint(window, &paint);
        EndPaint(window, &paint);
        return true;
    }

    bool StartRetailBootMovie()
    {
        if (
            !g_RetailBootSequenceActive ||
            g_RetailBootMovieIndex >= kRetailBootMovieCount)
        {
            return false;
        }

        g_RetailVideoReachedRunningState = false;
        return FableStartRetailVideo(
            g_RetailVideoWindow,
            g_RetailVideoInstance,
            kRetailBootMovies[g_RetailBootMovieIndex]);
    }

    FableResult FABLE_STDCALL VisualBootWindowProcedure(
        FableWindow window,
        FableUint message,
        FableWordParameter wordParameter,
        FableLongParameter longParameter)
    {
        switch (message)
        {
        case kMessageSize:
        {
            const fable_i32 clientWidth =
                static_cast<fable_i32>(
                    longParameter & 0xFFFF);
            const fable_i32 clientHeight =
                static_cast<fable_i32>(
                    (longParameter >> 16) & 0xFFFF);
            if (FableIsRetailVideoActive())
            {
                FableResizeRetailVideo(
                    clientWidth,
                    clientHeight);
            }
            if (
                FableIsVisualD3D9Active() &&
                clientWidth > 0 &&
                clientHeight > 0 &&
                FableResizeVisualD3D9(
                    clientWidth,
                    clientHeight))
            {
                FableRenderVisualD3D9(
                    clientWidth,
                    clientHeight);
            }
            return 0;
        }

        case kMessagePaint:
            if (!PaintBootArtworkD3D9(window))
                PaintBootArtwork(window);
            return 0;

        case kMessageKeyDown:
            if (
                wordParameter == kEscapeKey &&
                !g_RetailVideoEscapePressed &&
                FableIsRetailVideoActive())
            {
                g_RetailVideoEscapePressed = true;
                FableSkipRetailVideo();
                return 0;
            }
            break;

        case kMessageKeyUp:
            if (wordParameter == kEscapeKey)
            {
                g_RetailVideoEscapePressed = false;
                return 0;
            }
            break;

        case kMessageTimer:
            if (wordParameter == kRetailVideoTimer)
            {
                if (
                    !g_RetailVideoReachedRunningState &&
                    FableRetailVideoHasAdvanced())
                {
                    g_RetailVideoReachedRunningState = true;
                    SetWindowTextA(
                        window,
                        g_RetailBootSequenceActive
                            ? (
                                FableIsRetailVideoUsingUpscaledSource()
                                    ? kRetailBootUpscaledPlayingWindowTitles[
                                        g_RetailBootMovieIndex]
                                    : kRetailBootPlayingWindowTitles[
                                        g_RetailBootMovieIndex]
                            )
                            : (
                                FableIsRetailVideoUsingUpscaledSource()
                                    ? kRetailVideoUpscaledPlayingWindowTitle
                                    : kRetailVideoPlayingWindowTitle
                            ));
                }

                const FableRetailVideoProcessResult result =
                    FableProcessRetailVideo();
                if (result == FableRetailVideoCompleted)
                {
                    if (
                        g_RetailBootSequenceActive &&
                        ++g_RetailBootMovieIndex <
                            kRetailBootMovieCount)
                    {
                        if (StartRetailBootMovie())
                        {
                            SetWindowTextA(
                                window,
                                kRetailVideoStartingWindowTitle);
                        }
                        else
                        {
                            KillTimer(window, kRetailVideoTimer);
                            SetWindowTextA(
                                window,
                                FableGetRetailVideoStatus());
                        }
                    }
                    else
                    {
                        KillTimer(window, kRetailVideoTimer);
                        FableShutdownRetailVideo();
                        bool frontendStartupReady = true;
                        if (g_RetailBootSequenceActive)
                        {
                            g_FrontendPostMovieStepMask = 0;
                            FableFrontendPostMovieServices services = {
                                true,
                                &RunVisualFrontendPostMovieStep,
                                window
                            };
                            frontendStartupReady =
                                FableRunFrontendPostMovieStartup(
                                    services) &&
                                g_FrontendPostMovieStepMask == 0x1FF;
                        }
                        SetWindowTextA(
                            window,
                            g_RetailBootSequenceActive
                                ? (
                                    frontendStartupReady
                                        ? kRetailVideoSequenceCompleteWindowTitle
                                        : kRetailVideoSequenceFailedWindowTitle
                                )
                                : kRetailVideoCompleteWindowTitle);
                    }
                }
                else if (result == FableRetailVideoFailed)
                {
                    KillTimer(window, kRetailVideoTimer);
                    SetWindowTextA(
                        window,
                        FableGetRetailVideoStatus());
                    FableShutdownRetailVideo();
                }
            }
            return 0;

        case kMessageEraseBackground:
            return 1;

        case kMessageDestroy:
            KillTimer(window, kRetailVideoTimer);
            FableShutdownRetailVideo();
            FableShutdownVisualD3D9();
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

void FABLE_FASTCALL FableSetVisualProgressDisplayState(
    bool present,
    bool active)
{
    g_RetailProgressDisplayPresent = present;
    g_RetailProgressDisplayActive = active;
}

bool FABLE_FASTCALL FableIsRetailVisualAssetEmbedded()
{
#if defined(FABLETLC_RETAIL_FRONTEND_ARTWORK)
    return true;
#else
    return false;
#endif
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
#if defined(FABLETLC_RETAIL_FRONTEND_ARTWORK)
    g_BootTitleArtwork = static_cast<FableBitmap>(LoadImageA(
        instance,
        IntegerResource(kBootTitleResource),
        kImageBitmap,
        0,
        0,
        kLoadCreatedDibSection));
    if (g_BootTitleArtwork == 0)
    {
        DeleteObject(g_BootArtwork);
        g_BootArtwork = 0;
        return 1;
    }
    GetObjectA(
        g_BootTitleArtwork,
        sizeof(g_BootTitleArtworkInfo),
        &g_BootTitleArtworkInfo);
#endif

    if (
        commandLine != 0 &&
        strstr(commandLine, "--verify-visual-resource") != 0)
    {
        DeleteObject(g_BootArtwork);
        g_BootArtwork = 0;
#if defined(FABLETLC_RETAIL_FRONTEND_ARTWORK)
        DeleteObject(g_BootTitleArtwork);
        g_BootTitleArtwork = 0;
#endif
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
#if defined(FABLETLC_RETAIL_FRONTEND_ARTWORK)
        DeleteObject(g_BootTitleArtwork);
        g_BootTitleArtwork = 0;
#endif
        return 2;
    }

    FableRectangle windowBounds = {0, 0, 1280, 720};
    AdjustWindowRectEx(
        &windowBounds,
        kOverlappedWindow,
        0,
        0);

    const char* windowTitle = kWindowTitle;
    if (g_RetailProgressDisplayPresent)
    {
#if defined(FABLETLC_RETAIL_FRONTEND_ARTWORK)
        windowTitle = g_RetailProgressDisplayActive
            ? kRetailAssetActiveWindowTitle
            : kRetailAssetReadyWindowTitle;
#else
        windowTitle = g_RetailProgressDisplayActive
            ? kProgressActiveWindowTitle
            : kProgressReadyWindowTitle;
#endif
    }

    FableWindow window = CreateWindowExA(
        0,
        kWindowClassName,
        windowTitle,
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
#if defined(FABLETLC_RETAIL_FRONTEND_ARTWORK)
        DeleteObject(g_BootTitleArtwork);
        g_BootTitleArtwork = 0;
#endif
        return 3;
    }

    if (FableInitialiseVisualD3D9(
            window,
            1280,
            720,
            g_BootArtworkInfo.width,
            g_BootArtworkInfo.height,
            g_BootArtworkInfo.widthBytes,
            g_BootArtworkInfo.bitsPerPixel,
            g_BootArtworkInfo.pixels,
            g_BootTitleArtworkInfo.width,
            g_BootTitleArtworkInfo.height,
            g_BootTitleArtworkInfo.widthBytes,
            g_BootTitleArtworkInfo.bitsPerPixel,
            g_BootTitleArtworkInfo.pixels))
    {
#if defined(FABLETLC_RETAIL_FRONTEND_ARTWORK)
        SetWindowTextA(
            window,
            g_RetailProgressDisplayActive
                ? kD3DRetailAssetActiveWindowTitle
                : kD3DRetailAssetReadyWindowTitle);
#else
        SetWindowTextA(
            window,
            g_RetailProgressDisplayActive
                ? kD3DCheckpointActiveWindowTitle
                : kD3DCheckpointReadyWindowTitle);
#endif
    }

    ShowWindow(window, showCommand == 0 ? kShowNormal : showCommand);
    UpdateWindow(window);
    if (FableWasVisualD3D9Presented())
    {
#if defined(FABLETLC_RETAIL_FRONTEND_ARTWORK)
        SetWindowTextA(
            window,
            g_RetailProgressDisplayActive
                ? kD3DRetailAssetPresentedActiveWindowTitle
                : kD3DRetailAssetPresentedReadyWindowTitle);
#else
        SetWindowTextA(
            window,
            g_RetailProgressDisplayActive
                ? kD3DCheckpointPresentedActiveWindowTitle
                : kD3DCheckpointPresentedReadyWindowTitle);
#endif
    }

    if (
        commandLine != 0 &&
        strstr(commandLine, "--retail-video") != 0)
    {
        g_RetailVideoWindow = window;
        g_RetailVideoInstance = instance;
        g_RetailBootMovieIndex = 0;
        g_RetailBootSequenceActive = false;
        g_RetailVideoReachedRunningState = false;
        g_RetailVideoEscapePressed = false;
        FableSetRetailVideoPreferUpscaled(
            strstr(commandLine, "--retail-video-upscaled") != 0 &&
            strstr(commandLine, "--retail-video-original") == 0);

        const char* selectedMovie = 0;
        if (strstr(commandLine, "--retail-video=intro") != 0)
            selectedMovie = "intro_comp.wmv";
        else if (
            strstr(
                commandLine,
                "--retail-video=attract") != 0)
        {
            selectedMovie = "fable_attract_english.wmv";
        }
        else if (
            strstr(
                commandLine,
                "--retail-video=lionhead") != 0)
        {
            selectedMovie = "lionhead_logo.wmv";
        }
        else if (
            strstr(
                commandLine,
                "--retail-video=microsoft") != 0)
        {
            selectedMovie = "microsoft_logo.wmv";
        }
        else
        {
            g_RetailBootSequenceActive = true;
            selectedMovie =
                kRetailBootMovies[g_RetailBootMovieIndex];
        }

        if (FableStartRetailVideo(
                window,
                instance,
                selectedMovie))
        {
            SetWindowTextA(window, kRetailVideoStartingWindowTitle);
            SetTimer(
                window,
                kRetailVideoTimer,
                50,
                0);
        }
        else
        {
            SetWindowTextA(
                window,
                FableGetRetailVideoStatus());
        }
    }

    FableMessage message = {};
    while (GetMessageA(&message, 0, 0, 0) > 0)
    {
        TranslateMessage(&message);
        DispatchMessageA(&message);
    }

    FableShutdownRetailVideo();
    FableShutdownVisualD3D9();
    UnregisterClassA(kWindowClassName, instance);
    DeleteObject(g_BootArtwork);
    g_BootArtwork = 0;
#if defined(FABLETLC_RETAIL_FRONTEND_ARTWORK)
    DeleteObject(g_BootTitleArtwork);
    g_BootTitleArtwork = 0;
#endif
    return static_cast<long>(message.wordParameter);
}
