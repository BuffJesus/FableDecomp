#include "retail_video_bridge.h"
#include "fable_video_system.h"

#include <string.h>

typedef long FableVideoResult;
typedef unsigned long FableVideoDword;
typedef unsigned long FableVideoUlong;
typedef void* FableVideoWindow;
typedef void* FableVideoRegistryKey;

struct FableVideoGuid
{
    FableVideoDword data1;
    unsigned short data2;
    unsigned short data3;
    unsigned char data4[8];
};

struct FableVideoRectangle
{
    long left;
    long top;
    long right;
    long bottom;
};

typedef FableVideoResult (FABLE_STDCALL *FableQueryInterface)(
    void* object,
    const FableVideoGuid& interfaceId,
    void** result);
typedef FableVideoUlong (FABLE_STDCALL *FableRelease)(
    void* object);
typedef FableVideoResult (FABLE_STDCALL *FableRenderFile)(
    void* graph,
    const wchar_t* fileName,
    const wchar_t* playList);
typedef FableVideoResult (FABLE_STDCALL *FableMediaRun)(
    void* mediaControl);
typedef FableVideoResult (FABLE_STDCALL *FableMediaStop)(
    void* mediaControl);
typedef FableVideoResult (FABLE_STDCALL *FableMediaGetState)(
    void* mediaControl,
    long timeoutMilliseconds,
    long* state);
typedef FableVideoResult (FABLE_STDCALL *FableMediaGetEvent)(
    void* mediaEvent,
    long* eventCode,
    long* parameter1,
    long* parameter2,
    long timeoutMilliseconds);
typedef FableVideoResult (FABLE_STDCALL *FableMediaFreeEventParams)(
    void* mediaEvent,
    long eventCode,
    long parameter1,
    long parameter2);
typedef FableVideoResult (FABLE_STDCALL *FableVideoPutLong)(
    void* videoWindow,
    long value);
typedef FableVideoResult (FABLE_STDCALL *FableVideoSetWindowPosition)(
    void* videoWindow,
    long left,
    long top,
    long width,
    long height);

extern "C"
{
    __declspec(dllimport) FableVideoResult FABLE_STDCALL CoInitializeEx(
        void* reserved,
        FableVideoDword concurrencyModel);
    __declspec(dllimport) void FABLE_STDCALL CoUninitialize();
    __declspec(dllimport) FableVideoResult FABLE_STDCALL CoCreateInstance(
        const FableVideoGuid& classId,
        void* outerUnknown,
        FableVideoDword context,
        const FableVideoGuid& interfaceId,
        void** result);
    __declspec(dllimport) int FABLE_STDCALL MultiByteToWideChar(
        unsigned int codePage,
        FableVideoDword flags,
        const char* source,
        int sourceLength,
        wchar_t* destination,
        int destinationLength);
    __declspec(dllimport) int FABLE_STDCALL GetClientRect(
        FableVideoWindow window,
        FableVideoRectangle* rectangle);
    __declspec(dllimport) FableVideoDword FABLE_STDCALL GetModuleFileNameA(
        void* module,
        char* fileName,
        FableVideoDword size);
    __declspec(dllimport) FableVideoDword FABLE_STDCALL GetFileAttributesA(
        const char* fileName);
    __declspec(dllimport) long FABLE_STDCALL RegOpenKeyExA(
        FableVideoRegistryKey key,
        const char* subKey,
        FableVideoDword options,
        FableVideoDword desiredAccess,
        FableVideoRegistryKey* result);
    __declspec(dllimport) long FABLE_STDCALL RegQueryValueExA(
        FableVideoRegistryKey key,
        const char* valueName,
        FableVideoDword* reserved,
        FableVideoDword* type,
        unsigned char* data,
        FableVideoDword* dataSize);
    __declspec(dllimport) long FABLE_STDCALL RegCloseKey(
        FableVideoRegistryKey key);
}

namespace
{
    const FableVideoGuid kClassFilterGraph = {
        0xE436EBB3UL,
        0x524F,
        0x11CE,
        {0x9F, 0x53, 0x00, 0x20, 0xAF, 0x0B, 0xA7, 0x70}
    };
    const FableVideoGuid kInterfaceGraphBuilder = {
        0x56A868A9UL,
        0x0AD4,
        0x11CE,
        {0xB0, 0x3A, 0x00, 0x20, 0xAF, 0x0B, 0xA7, 0x70}
    };
    const FableVideoGuid kInterfaceMediaControl = {
        0x56A868B1UL,
        0x0AD4,
        0x11CE,
        {0xB0, 0x3A, 0x00, 0x20, 0xAF, 0x0B, 0xA7, 0x70}
    };
    const FableVideoGuid kInterfaceVideoWindow = {
        0x56A868B4UL,
        0x0AD4,
        0x11CE,
        {0xB0, 0x3A, 0x00, 0x20, 0xAF, 0x0B, 0xA7, 0x70}
    };
    const FableVideoGuid kInterfaceMediaEvent = {
        0x56A868B6UL,
        0x0AD4,
        0x11CE,
        {0xB0, 0x3A, 0x00, 0x20, 0xAF, 0x0B, 0xA7, 0x70}
    };
    const FableVideoDword kInvalidFileAttributes = 0xFFFFFFFFUL;
    const FableVideoDword kKeyQueryValue = 0x00020019UL;
    const FableVideoDword kKeyWow64_64Key = 0x00000100UL;
    const FableVideoDword kRegistryString = 1;
    const FableVideoDword kClassContextInProcess = 1;
    const FableVideoDword kApartmentThreaded = 2;
    const unsigned int kAnsiCodePage = 0;
    const long kWindowChild = 0x40000000L;
    const long kWindowClipSiblings = 0x04000000L;
    const long kWindowClipChildren = 0x02000000L;
    const long kOleTrue = -1;
    const long kOleFalse = 0;
    const long kStateRunning = 2;
    const long kEventComplete = 1;
    const long kEventUserAbort = 2;
    const long kEventErrorAbort = 3;
    const char kSteamUninstallKey[] =
        "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\"
        "Steam App 204030";
    const char kVideoDirectorySuffix[] =
        "\\data\\Video\\";
    const char kDefaultMovieName[] =
        "microsoft_logo.wmv";

    void* g_Graph = 0;
    void* g_MediaControl = 0;
    void* g_MediaEvent = 0;
    void* g_VideoWindow = 0;
    bool g_ComInitialised = false;
    bool g_SkipRequested = false;
    bool g_PreferUpscaled = false;
    bool g_UsingUpscaledSource = false;
    const char* g_Status = "not-requested";

    bool Succeeded(FableVideoResult result)
    {
        return result >= 0;
    }

    void** VTable(void* object)
    {
        return *reinterpret_cast<void***>(object);
    }

    void ReleaseObject(void*& object)
    {
        if (object != 0)
        {
            FableRelease release =
                reinterpret_cast<FableRelease>(
                    VTable(object)[2]);
            release(object);
            object = 0;
        }
    }

    bool ResolveInstalledMoviePath(
        char* path,
        unsigned int capacity,
        const char* movieName)
    {
        if (
            path == 0 ||
            capacity == 0 ||
            movieName == 0 ||
            movieName[0] == '\0')
            return false;

        FableVideoRegistryKey key = 0;
        FableVideoRegistryKey localMachine =
            reinterpret_cast<FableVideoRegistryKey>(0x80000002UL);
        if (RegOpenKeyExA(
                localMachine,
                kSteamUninstallKey,
                0,
                kKeyQueryValue | kKeyWow64_64Key,
                &key) != 0)
        {
            return false;
        }

        FableVideoDword type = 0;
        FableVideoDword byteCount = capacity;
        const long queryResult = RegQueryValueExA(
            key,
            "InstallLocation",
            0,
            &type,
            reinterpret_cast<unsigned char*>(path),
            &byteCount);
        RegCloseKey(key);
        if (
            queryResult != 0 ||
            type != kRegistryString ||
            byteCount == 0)
        {
            return false;
        }

        path[capacity - 1] = '\0';
        const unsigned int length =
            static_cast<unsigned int>(strlen(path));
        const unsigned int directoryLength =
            static_cast<unsigned int>(
                strlen(kVideoDirectorySuffix));
        const unsigned int movieNameLength =
            static_cast<unsigned int>(strlen(movieName));
        if (
            length +
            directoryLength +
            movieNameLength +
            1 > capacity)
            return false;

        memcpy(
            path + length,
            kVideoDirectorySuffix,
            directoryLength);
        memcpy(
            path + length + directoryLength,
            movieName,
            movieNameLength + 1);
        return GetFileAttributesA(path) != kInvalidFileAttributes;
    }

    bool ResolveUpscaledMoviePath(
        char* path,
        unsigned int capacity,
        const char* movieName)
    {
        if (
            path == 0 ||
            capacity == 0 ||
            movieName == 0 ||
            movieName[0] == '\0')
        {
            return false;
        }

        const FableVideoDword length =
            GetModuleFileNameA(0, path, capacity);
        if (length == 0 || length >= capacity)
            return false;

        char* lastBackslash = strrchr(path, '\\');
        char* lastSlash = strrchr(path, '/');
        char* separator = lastBackslash;
        if (lastSlash != 0 && (separator == 0 || lastSlash > separator))
            separator = lastSlash;
        if (separator == 0)
            return false;
        separator[1] = '\0';

        const char directory[] = "upscaled-video\\";
        const unsigned int directoryLength =
            static_cast<unsigned int>(strlen(directory));
        const unsigned int prefixLength =
            static_cast<unsigned int>(strlen(path));
        const unsigned int movieNameLength =
            static_cast<unsigned int>(strlen(movieName));
        if (
            prefixLength +
            directoryLength +
            movieNameLength +
            1 > capacity)
        {
            return false;
        }

        memcpy(path + prefixLength, directory, directoryLength);
        memcpy(
            path + prefixLength + directoryLength,
            movieName,
            movieNameLength + 1);
        return GetFileAttributesA(path) != kInvalidFileAttributes;
    }

    bool QueryGraphInterface(
        const FableVideoGuid& interfaceId,
        void** result)
    {
        FableQueryInterface queryInterface =
            reinterpret_cast<FableQueryInterface>(
                VTable(g_Graph)[0]);
        return Succeeded(queryInterface(
            g_Graph,
            interfaceId,
            result));
    }
}

bool FABLE_FASTCALL FableStartRetailVideo(
    void* parentWindow,
    void* instance,
    const char* explicitMoviePath)
{
    (void)instance;
    FableShutdownRetailVideo();
    g_Status = "starting";
    g_SkipRequested = false;
    g_UsingUpscaledSource = false;
    if (parentWindow == 0)
    {
        g_Status = "missing-parent-window";
        return false;
    }

    char discoveredPath[1024];
    const char* moviePath = explicitMoviePath;
    if (moviePath == 0 || moviePath[0] == '\0')
    {
        if (!ResolveInstalledMoviePath(
                discoveredPath,
                sizeof(discoveredPath),
                kDefaultMovieName))
        {
            g_Status = "movie-path-not-found";
            return false;
        }
        moviePath = discoveredPath;
    }
    else if (
        strchr(moviePath, '\\') == 0 &&
        strchr(moviePath, '/') == 0 &&
        strchr(moviePath, ':') == 0)
    {
        if (
            g_PreferUpscaled &&
            ResolveUpscaledMoviePath(
                discoveredPath,
                sizeof(discoveredPath),
                moviePath))
        {
            moviePath = discoveredPath;
            g_UsingUpscaledSource = true;
        }
        else if (!ResolveInstalledMoviePath(
                    discoveredPath,
                    sizeof(discoveredPath),
                    moviePath))
        {
            g_Status = "named-movie-not-found";
            return false;
        }
        else
        {
            moviePath = discoveredPath;
        }
    }
    else if (
        GetFileAttributesA(moviePath) ==
        kInvalidFileAttributes)
    {
        g_Status = "explicit-movie-not-found";
        return false;
    }

    wchar_t wideMoviePath[1024];
    if (MultiByteToWideChar(
            kAnsiCodePage,
            0,
            moviePath,
            -1,
            wideMoviePath,
            sizeof(wideMoviePath) / sizeof(wideMoviePath[0])) == 0)
    {
        g_Status = "movie-path-conversion-failed";
        return false;
    }

    const FableVideoResult initialiseResult =
        CoInitializeEx(0, kApartmentThreaded);
    if (!Succeeded(initialiseResult))
    {
        g_Status = "com-initialise-failed";
        return false;
    }
    g_ComInitialised = true;

    if (!Succeeded(CoCreateInstance(
            kClassFilterGraph,
            0,
            kClassContextInProcess,
            kInterfaceGraphBuilder,
            &g_Graph)) ||
        g_Graph == 0)
    {
        g_Status = "filter-graph-create-failed";
        FableShutdownRetailVideo();
        return false;
    }

    FableRenderFile renderFile =
        reinterpret_cast<FableRenderFile>(
            VTable(g_Graph)[13]);
    if (!Succeeded(renderFile(
            g_Graph,
            wideMoviePath,
            0)))
    {
        g_Status = "directshow-render-file-failed";
        FableShutdownRetailVideo();
        return false;
    }

    if (!QueryGraphInterface(
            kInterfaceMediaControl,
            &g_MediaControl) ||
        !QueryGraphInterface(
            kInterfaceMediaEvent,
            &g_MediaEvent) ||
        !QueryGraphInterface(
            kInterfaceVideoWindow,
            &g_VideoWindow))
    {
        g_Status = "directshow-interface-query-failed";
        FableShutdownRetailVideo();
        return false;
    }

    FableVideoPutLong putAutoShow =
        reinterpret_cast<FableVideoPutLong>(
            VTable(g_VideoWindow)[13]);
    FableVideoPutLong putWindowStyle =
        reinterpret_cast<FableVideoPutLong>(
            VTable(g_VideoWindow)[9]);
    FableVideoPutLong putOwner =
        reinterpret_cast<FableVideoPutLong>(
            VTable(g_VideoWindow)[29]);
    FableVideoPutLong putMessageDrain =
        reinterpret_cast<FableVideoPutLong>(
            VTable(g_VideoWindow)[31]);
    FableVideoPutLong putVisible =
        reinterpret_cast<FableVideoPutLong>(
            VTable(g_VideoWindow)[19]);
    if (
        !Succeeded(putAutoShow(g_VideoWindow, kOleFalse)) ||
        !Succeeded(putOwner(
            g_VideoWindow,
            reinterpret_cast<long>(parentWindow))) ||
        !Succeeded(putMessageDrain(
            g_VideoWindow,
            reinterpret_cast<long>(parentWindow))) ||
        !Succeeded(putWindowStyle(
            g_VideoWindow,
            kWindowChild |
            kWindowClipSiblings |
            kWindowClipChildren)))
    {
        g_Status = "directshow-window-owner-failed";
        FableShutdownRetailVideo();
        return false;
    }

    FableVideoRectangle client = {};
    if (GetClientRect(parentWindow, &client))
    {
        FableResizeRetailVideo(
            client.right - client.left,
            client.bottom - client.top);
    }
    if (!Succeeded(putVisible(g_VideoWindow, kOleTrue)))
    {
        g_Status = "directshow-window-visible-failed";
        FableShutdownRetailVideo();
        return false;
    }

    FableMediaRun run =
        reinterpret_cast<FableMediaRun>(
            VTable(g_MediaControl)[7]);
    if (!Succeeded(run(g_MediaControl)))
    {
        g_Status = "directshow-run-failed";
        FableShutdownRetailVideo();
        return false;
    }
    g_Status = "playing-requested";
    return true;
}

void FABLE_FASTCALL FableSetRetailVideoPreferUpscaled(
    bool preferUpscaled)
{
    g_PreferUpscaled = preferUpscaled;
}

void FABLE_FASTCALL FableResizeRetailVideo(
    fable_i32 clientWidth,
    fable_i32 clientHeight)
{
    if (
        g_VideoWindow != 0 &&
        clientWidth > 0 &&
        clientHeight > 0)
    {
        FableVideoSetWindowPosition setWindowPosition =
            reinterpret_cast<FableVideoSetWindowPosition>(
                VTable(g_VideoWindow)[39]);
        setWindowPosition(
            g_VideoWindow,
            0,
            0,
            clientWidth,
            clientHeight);
    }
}

bool FABLE_FASTCALL FableRetailVideoHasAdvanced()
{
    if (g_MediaControl == 0)
        return false;

    long state = 0;
    FableMediaGetState getState =
        reinterpret_cast<FableMediaGetState>(
            VTable(g_MediaControl)[10]);
    return Succeeded(getState(
        g_MediaControl,
        0,
        &state)) && state == kStateRunning;
}

FableRetailVideoProcessResult FABLE_FASTCALL
FableProcessRetailVideo()
{
    if (g_SkipRequested)
    {
        g_SkipRequested = false;
        g_Status = "skipped";
        return FableRetailVideoCompleted;
    }

    if (g_MediaEvent == 0)
        return FableRetailVideoFailed;

    FableMediaGetEvent getEvent =
        reinterpret_cast<FableMediaGetEvent>(
            VTable(g_MediaEvent)[8]);
    FableMediaFreeEventParams freeEventParams =
        reinterpret_cast<FableMediaFreeEventParams>(
            VTable(g_MediaEvent)[12]);

    for (;;)
    {
        long eventCode = 0;
        long parameter1 = 0;
        long parameter2 = 0;
        if (!Succeeded(getEvent(
                g_MediaEvent,
                &eventCode,
                &parameter1,
                &parameter2,
                0)))
        {
            return FableRetailVideoPending;
        }

        freeEventParams(
            g_MediaEvent,
            eventCode,
            parameter1,
            parameter2);
        if (eventCode == kEventComplete)
        {
            g_Status = "completed";
            return FableRetailVideoCompleted;
        }
        if (
            eventCode == kEventUserAbort ||
            eventCode == kEventErrorAbort)
        {
            g_Status = eventCode == kEventUserAbort
                ? "user-aborted"
                : "playback-error";
            return FableRetailVideoFailed;
        }
    }
}

void FABLE_FASTCALL FableSkipRetailVideo()
{
    if (FableIsRetailVideoActive())
        g_SkipRequested = true;
}

bool FABLE_FASTCALL FableIsRetailVideoActive()
{
    return
        g_Graph != 0 &&
        g_MediaControl != 0 &&
        g_MediaEvent != 0 &&
        g_VideoWindow != 0;
}

bool FABLE_FASTCALL FableIsRetailVideoUsingUpscaledSource()
{
    return g_UsingUpscaledSource;
}

const char* FABLE_FASTCALL FableGetRetailVideoStatus()
{
    return g_Status;
}

void FABLE_FASTCALL FableShutdownRetailVideo()
{
    if (g_VideoWindow != 0)
    {
        FableVideoPutLong putVisible =
            reinterpret_cast<FableVideoPutLong>(
                VTable(g_VideoWindow)[19]);
        FableVideoPutLong putOwner =
            reinterpret_cast<FableVideoPutLong>(
                VTable(g_VideoWindow)[29]);
        FableVideoPutLong putMessageDrain =
            reinterpret_cast<FableVideoPutLong>(
                VTable(g_VideoWindow)[31]);
        putVisible(g_VideoWindow, kOleFalse);
        putMessageDrain(g_VideoWindow, 0);
        putOwner(g_VideoWindow, 0);
    }
    if (g_MediaControl != 0)
    {
        FableMediaStop stop =
            reinterpret_cast<FableMediaStop>(
                VTable(g_MediaControl)[9]);
        stop(g_MediaControl);
    }
    ReleaseObject(g_VideoWindow);
    ReleaseObject(g_MediaEvent);
    ReleaseObject(g_MediaControl);
    ReleaseObject(g_Graph);
    if (g_ComInitialised)
    {
        CoUninitialize();
        g_ComInitialised = false;
    }
}
