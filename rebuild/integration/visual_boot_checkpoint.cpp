#include "fable_visual_boot.h"
#include "fable_visual_d3d9.h"
#include "frontend_startup_sequence.h"
#include "retail_video_bridge.h"
#include "detail_screen_tables.h"
#include "frontend_list_layout.h"
#include "frontend_input_dispatch.h"

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

struct FableJoyInfoEx
{
    FableDword size;
    FableDword flags;
    FableDword xPosition;
    FableDword yPosition;
    FableDword zPosition;
    FableDword rPosition;
    FableDword uPosition;
    FableDword vPosition;
    FableDword buttons;
    FableDword buttonNumber;
    FableDword pointOfView;
    FableDword reserved1;
    FableDword reserved2;
};

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

// Minimal Win32 file-enumeration surface. The reconstruction deliberately avoids
// a blanket <windows.h>; these ABI-exact structs + kernel32 imports declare only
// what the real Saved-Games directory walk needs (matches the file's existing
// narrow __declspec(dllimport) style).
typedef void* FableFindHandle;
struct FableFileTime
{
    FableDword lowDateTime;
    FableDword highDateTime;
};
struct FableWin32FindDataW
{
    FableDword attributes;
    FableFileTime creationTime;
    FableFileTime lastAccessTime;
    FableFileTime lastWriteTime;
    FableDword fileSizeHigh;
    FableDword fileSizeLow;
    FableDword reserved0;
    FableDword reserved1;
    wchar_t fileName[260];
    wchar_t alternateFileName[14];
};
struct FableWin32FileAttributeData
{
    FableDword attributes;
    FableFileTime creationTime;
    FableFileTime lastAccessTime;
    FableFileTime lastWriteTime;
    FableDword fileSizeHigh;
    FableDword fileSizeLow;
};
const FableDword kFableFileAttributeDirectory = 0x10;
const int kFableGetFileExInfoStandard = 0;

extern "C"
{
    __declspec(dllimport) FableFindHandle FABLE_STDCALL FindFirstFileW(
        const wchar_t* fileName,
        FableWin32FindDataW* findData);
    __declspec(dllimport) FableBool FABLE_STDCALL FindNextFileW(
        FableFindHandle findHandle,
        FableWin32FindDataW* findData);
    __declspec(dllimport) FableBool FABLE_STDCALL FindClose(
        FableFindHandle findHandle);
    __declspec(dllimport) FableDword FABLE_STDCALL GetEnvironmentVariableW(
        const wchar_t* name,
        wchar_t* value,
        FableDword capacity);
    __declspec(dllimport) FableBool FABLE_STDCALL GetFileAttributesExW(
        const wchar_t* fileName,
        int infoLevel,
        FableWin32FileAttributeData* fileInformation);
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
    __declspec(dllimport) FableBool FABLE_STDCALL ScreenToClient(
        FableWindow window,
        FablePoint* point);
    __declspec(dllimport) void* FABLE_STDCALL LoadImageA(
        FableInstanceHandle instance,
        const char* name,
        FableUint type,
        int desiredWidth,
        int desiredHeight,
        FableUint loadFlags);
    __declspec(dllimport) FableDword FABLE_STDCALL GetModuleFileNameA(
        FableInstanceHandle module,
        char* filename,
        FableDword size);
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
    __declspec(dllimport) FableBool FABLE_STDCALL InvalidateRect(
        FableWindow window,
        const FableRectangle* rectangle,
        FableBool erase);
    __declspec(dllimport) FableBool FABLE_STDCALL SetWindowTextA(
        FableWindow window,
        const char* text);
    __declspec(dllimport) FableWordParameter FABLE_STDCALL SetTimer(
        FableWindow window,
        FableWordParameter identifier,
        FableUint intervalMilliseconds,
        void* timerProcedure);
    __declspec(dllimport) FableDword FABLE_STDCALL GetTickCount();
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
    __declspec(dllimport) FableBool FABLE_STDCALL PlaySoundA(
        const char* sound,
        FableInstanceHandle module,
        FableDword flags);
    __declspec(dllimport) FableUint FABLE_STDCALL joyGetPosEx(
        FableUint joystickIdentifier,
        FableJoyInfoEx* information);

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

fable_u32 FABLE_FASTCALL FableGetVisualFrontendMainMenuAction(
    fable_u32 row)
{
    const fable_u32 actions[FableFrontendMainMenuVisibleRows] = {
        66, 16, 297, 67, 321, 314
    };
    return row < FableFrontendMainMenuVisibleRows ? actions[row] : 0;
}

fable_u32 FABLE_FASTCALL FablePlanVisualFrontendSaveRows(
    const char* autosaveFilename,
    bool autosavePrimaryValid,
    bool autosaveCompanionValid,
    const char* const* manualFilenames,
    const bool* manualPrimaryValid,
    const bool* manualCompanionValid,
    fable_u32 manualSlotCount,
    FableUiSaveBrowserRow* rows,
    fable_u32 rowCapacity)
{
    fable_u32 rowCount = 0;
    if (autosaveFilename != 0 && autosaveFilename[0] != '\0')
    {
        if (rows != 0 && rowCount < rowCapacity)
        {
            rows[rowCount].filename = autosaveFilename;
            rows[rowCount].positionY =
                static_cast<fable_i32>(rowCount * 30);
            rows[rowCount].action =
                autosavePrimaryValid && autosaveCompanionValid
                    ? FableUiSaveBrowserLoadAction
                    : FableUiSaveBrowserInvalidAction;
        }
        ++rowCount;
    }
    if (manualFilenames == 0)
        return rowCount;
    for (fable_u32 slot = 0; slot != manualSlotCount; ++slot)
    {
        const char* filename = manualFilenames[slot];
        if (filename == 0 || filename[0] == '\0')
            continue;
        if (rows != 0 && rowCount < rowCapacity)
        {
            rows[rowCount].filename = filename;
            rows[rowCount].positionY =
                static_cast<fable_i32>(rowCount * 30);
            rows[rowCount].action =
                manualPrimaryValid != 0 &&
                manualCompanionValid != 0 &&
                manualPrimaryValid[slot] &&
                manualCompanionValid[slot]
                    ? FableUiSaveBrowserLoadAction
                    : FableUiSaveBrowserInvalidAction;
        }
        ++rowCount;
    }
    return rowCount;
}

void FABLE_FASTCALL FablePlanVisualFrontendSaveAction(
    const FableUiSaveBrowserRow* rows,
    fable_u32 rowCount,
    fable_u32 selection,
    FableUiSaveBrowserActionPlan* plan)
{
    if (plan == 0)
        return;
    plan->action = FableUiSaveBrowserInvalidAction;
    plan->dispatch = false;
    if (rows == 0 || selection >= rowCount)
        return;
    plan->action = rows[selection].action;
    plan->dispatch = plan->action == FableUiSaveBrowserLoadAction;
}

namespace
{
    // Join <dir>[\]<leaf> into out (UTF-16), inserting a separator only when the
    // directory does not already end in one.
    void JoinSavePath(
        wchar_t* out,
        size_t capacity,
        const wchar_t* dir,
        const wchar_t* leaf)
    {
        size_t n = 0;
        for (; dir[n] != L'\0' && n + 1 < capacity; ++n)
            out[n] = dir[n];
        if (n != 0 && out[n - 1] != L'\\' && out[n - 1] != L'/' &&
            n + 1 < capacity)
            out[n++] = L'\\';
        for (size_t i = 0; leaf[i] != L'\0' && n + 1 < capacity; ++i)
            out[n++] = leaf[i];
        out[n] = L'\0';
    }

    // A save part is valid when the file exists, is not a directory, and holds
    // at least one byte (Fable never writes a zero-length save part).
    bool SavePartNonEmpty(const wchar_t* profileDir, const wchar_t* leaf)
    {
        wchar_t path[520];
        JoinSavePath(path, 520, profileDir, leaf);
        FableWin32FileAttributeData info;
        if (!GetFileAttributesExW(path, kFableGetFileExInfoStandard, &info))
            return false;
        if (info.attributes & kFableFileAttributeDirectory)
            return false;
        return info.fileSizeHigh != 0 || info.fileSizeLow != 0;
    }
}

fable_u32 FABLE_FASTCALL FableEnumerateVisualFrontendSaves(
    const wchar_t* saveDirectory,
    FableUiSaveProfile* out,
    fable_u32 outCapacity)
{
    if (saveDirectory == 0 || saveDirectory[0] == L'\0')
        return 0;
    wchar_t pattern[520];
    JoinSavePath(pattern, 520, saveDirectory, L"*");
    FableWin32FindDataW find;
    const FableFindHandle handle = FindFirstFileW(pattern, &find);
    if (handle == reinterpret_cast<FableFindHandle>(static_cast<long>(-1)))
        return 0;
    fable_u32 count = 0;
    do
    {
        if ((find.attributes & kFableFileAttributeDirectory) == 0)
            continue;
        // Skip "." and ".." pseudo-directories.
        if (find.fileName[0] == L'.' &&
            (find.fileName[1] == L'\0' ||
             (find.fileName[1] == L'.' && find.fileName[2] == L'\0')))
            continue;
        wchar_t profileDir[520];
        JoinSavePath(profileDir, 520, saveDirectory, find.fileName);
        const bool primary = SavePartNonEmpty(profileDir, L"AutoSave");
        const bool companion = SavePartNonEmpty(profileDir, L"AutoSave.qs");
        // Only surface folders that actually hold a save part; a Saves\ folder
        // can contain unrelated stray directories.
        if (!primary && !companion)
            continue;
        if (out != 0 && count < outCapacity)
        {
            fable_u32 i = 0;
            for (; find.fileName[i] != L'\0' && i < 63; ++i)
                out[count].name[i] = find.fileName[i];
            out[count].name[i] = L'\0';
            out[count].primaryValid = primary;
            out[count].companionValid = companion;
        }
        ++count;
    } while (FindNextFileW(handle, &find));
    FindClose(handle);
    return count;
}

fable_u32 FABLE_FASTCALL FableEnumerateVisualFrontendProfiles(
    const wchar_t* saveDirectory,
    FableUiProfileName* out,
    fable_u32 outCapacity)
{
    if (saveDirectory == 0 || saveDirectory[0] == L'\0')
        return 0;
    wchar_t pattern[520];
    JoinSavePath(pattern, 520, saveDirectory, L"*");
    FableWin32FindDataW find;
    const FableFindHandle handle = FindFirstFileW(pattern, &find);
    if (handle == reinterpret_cast<FableFindHandle>(static_cast<long>(-1)))
        return 0;

    fable_u32 count = 0;
    do
    {
        if ((find.attributes & kFableFileAttributeDirectory) == 0)
            continue;
        if (find.fileName[0] == L'.' &&
            (find.fileName[1] == L'\0' ||
             (find.fileName[1] == L'.' && find.fileName[2] == L'\0')))
            continue;
        if (out != 0 && count < outCapacity)
        {
            fable_u32 i = 0;
            for (; find.fileName[i] != L'\0' && i < 63; ++i)
            {
                const wchar_t character = find.fileName[i];
                out[count].name[i] =
                    character <= 0x7F
                        ? static_cast<char>(character)
                        : '?';
            }
            out[count].name[i] = '\0';
        }
        ++count;
    } while (FindNextFileW(handle, &find));
    FindClose(handle);

    const fable_u32 stored =
        outCapacity < count ? outCapacity : count;
    for (fable_u32 i = 0; i < stored; ++i)
    {
        for (fable_u32 j = i + 1; j < stored; ++j)
        {
            const char* left = out[i].name;
            const char* right = out[j].name;
            fable_u32 k = 0;
            while (left[k] != '\0' && right[k] != '\0')
            {
                char leftCharacter = left[k];
                char rightCharacter = right[k];
                if (leftCharacter >= 'A' && leftCharacter <= 'Z')
                    leftCharacter = static_cast<char>(leftCharacter + 32);
                if (rightCharacter >= 'A' && rightCharacter <= 'Z')
                    rightCharacter = static_cast<char>(rightCharacter + 32);
                if (leftCharacter != rightCharacter)
                    break;
                ++k;
            }
            if (left[k] == '\0' || right[k] == '\0')
            {
                if (left[k] == '\0' && right[k] != '\0')
                    continue;
                if (left[k] != '\0' && right[k] == '\0')
                    ;
                else
                    continue;
            }
            else
            {
                char leftCharacter = left[k];
                char rightCharacter = right[k];
                if (leftCharacter >= 'A' && leftCharacter <= 'Z')
                    leftCharacter = static_cast<char>(leftCharacter + 32);
                if (rightCharacter >= 'A' && rightCharacter <= 'Z')
                    rightCharacter = static_cast<char>(rightCharacter + 32);
                if (leftCharacter < rightCharacter)
                    continue;
            }
            FableUiProfileName temporary = out[i];
            out[i] = out[j];
            out[j] = temporary;
        }
    }
    return count;
}

void FABLE_FASTCALL FablePlanVisualFrontendProfileAction(
    fable_u32 mode,
    fable_u32 selection,
    fable_u32 profileCount,
    fable_u32 input,
    FableUiFrontendProfileActionPlan* plan)
{
    if (plan == 0)
        return;
    plan->action = 0;
    plan->dispatch = false;

    switch (input)
    {
    case FableUiFrontendProfileInputDeleteList:
        plan->action = FableRetailFrontendManagerActionDeleteListRefresh;
        plan->dispatch = true;
        return;
    case FableUiFrontendProfileInputDeleteConfirm:
        plan->action = FableRetailFrontendManagerActionDeleteConfirm;
        plan->dispatch = true;
        return;
    case FableUiFrontendProfileInputKeyboardCancel:
        plan->action = FableRetailFrontendManagerActionKeyboardCancel;
        plan->dispatch = true;
        return;
    case FableUiFrontendProfileInputKeyboardConfirm:
        plan->action = FableRetailFrontendManagerActionKeyboardConfirm;
        plan->dispatch = true;
        return;
    case FableUiFrontendProfileInputActivate:
        break;
    default:
        return;
    }

    if (mode == FableUiFrontendProfileModeEmpty)
    {
        plan->action = FableRetailFrontendManagerActionNewProfile;
        plan->dispatch = true;
    }
    else if (mode == FableUiFrontendProfileModeNew)
    {
        plan->action = FableRetailFrontendManagerActionKeyboardConfirm;
        plan->dispatch = true;
    }
    else if (mode == FableUiFrontendProfileModeDeleteConfirm)
    {
        plan->action = FableRetailFrontendManagerActionDeleteConfirm;
        plan->dispatch = true;
    }
    else if (
        mode == FableUiFrontendProfileModeNormal &&
        selection == 0)
    {
        plan->action = FableRetailFrontendManagerActionNewProfile;
        plan->dispatch = true;
    }
    else if (
        mode == FableUiFrontendProfileModeNormal &&
        selection <= profileCount)
    {
        plan->action = FableRetailFrontendManagerActionLoadProfile;
        plan->dispatch = true;
    }
    else if (
        mode == FableUiFrontendProfileModeDelete &&
        selection < profileCount)
    {
        plan->action = FableRetailFrontendManagerActionDeleteRow;
        plan->dispatch = true;
    }
}

fable_u32 FABLE_FASTCALL FableMapUiControllerState(
    fable_u32 xPosition,
    fable_u32 yPosition,
    fable_u32 pointOfView,
    fable_u32 buttons)
{
    const fable_u32 kLowAxis = 0x4000;
    const fable_u32 kHighAxis = 0xC000;
    const fable_u32 kPovCentered = 0xFFFF;
    fable_u32 state = 0;
    if (xPosition < kLowAxis)
        state |= FableUiControllerLeft;
    else if (xPosition > kHighAxis)
        state |= FableUiControllerRight;
    if (yPosition < kLowAxis)
        state |= FableUiControllerUp;
    else if (yPosition > kHighAxis)
        state |= FableUiControllerDown;

    if (pointOfView != kPovCentered)
    {
        if (pointOfView <= 4500 || pointOfView >= 31500)
            state |= FableUiControllerUp;
        if (pointOfView >= 4500 && pointOfView <= 13500)
            state |= FableUiControllerRight;
        if (pointOfView >= 13500 && pointOfView <= 22500)
            state |= FableUiControllerDown;
        if (pointOfView >= 22500 && pointOfView <= 31500)
            state |= FableUiControllerLeft;
    }
    if ((buttons & ((1UL << 0) | (1UL << 7))) != 0)
        state |= FableUiControllerAccept;
    if ((buttons & ((1UL << 1) | (1UL << 6))) != 0)
        state |= FableUiControllerBack;
    return state;
}

fable_u32 FABLE_FASTCALL FableConsumeUiControllerPressed(
    fable_u32 currentState,
    fable_u32* previousState)
{
    if (previousState == 0)
        return 0;
    const fable_u32 pressed = currentState & ~*previousState;
    *previousState = currentState;
    return pressed;
}

fable_u32 FABLE_FASTCALL FableConsumeUiControllerActions(
    fable_u32 currentState,
    fable_u32 currentTimeMs,
    fable_u32* previousState,
    FableUiControllerRepeatState* repeatState)
{
    const fable_u32 kMovementMask =
        FableUiControllerUp |
        FableUiControllerDown |
        FableUiControllerLeft |
        FableUiControllerRight;
    const fable_u32 kInitialRepeatDelayMs = 500;
    const fable_u32 kHeldRepeatDelayMs = 100;
    const fable_u32 pressed =
        FableConsumeUiControllerPressed(currentState, previousState);
    fable_u32 movement = currentState & kMovementMask;

    if ((movement & FableUiControllerUp) != 0)
        movement = FableUiControllerUp;
    else if ((movement & FableUiControllerDown) != 0)
        movement = FableUiControllerDown;
    else if ((movement & FableUiControllerLeft) != 0)
        movement = FableUiControllerLeft;
    else if ((movement & FableUiControllerRight) != 0)
        movement = FableUiControllerRight;

    fable_u32 actions = pressed & ~kMovementMask;
    if (movement == 0)
    {
        repeatState->lastMovement = 0;
        repeatState->isRepeating = false;
        return actions;
    }

    if (repeatState->lastMovement != movement)
    {
        repeatState->lastMovement = movement;
        repeatState->lastSelectionTimeMs = currentTimeMs;
        repeatState->isRepeating = false;
        return actions | movement;
    }

    const fable_u32 repeatDelay = repeatState->isRepeating
        ? kHeldRepeatDelayMs
        : kInitialRepeatDelayMs;
    if (
        currentTimeMs - repeatState->lastSelectionTimeMs >=
        repeatDelay)
    {
        repeatState->lastSelectionTimeMs = currentTimeMs;
        repeatState->isRepeating = true;
        actions |= movement;
    }
    return actions;
}

void FABLE_FASTCALL FableResolveUiComponentTransform(
    const FableUiVector2* localPosition,
    const FableUiVector2* localScale,
    const FableUiResolvedTransform* parent,
    FableUiResolvedTransform* resolved)
{
    resolved->position.x = localPosition->x + parent->position.x;
    resolved->position.y = localPosition->y + parent->position.y;
    resolved->truePosition.x =
        localPosition->x + parent->truePosition.x;
    resolved->truePosition.y =
        localPosition->y + parent->truePosition.y;
    resolved->scale.x = localScale->x * parent->scale.x;
    resolved->scale.y = localScale->y * parent->scale.y;
    resolved->trueScale.x = localScale->x * parent->trueScale.x;
    resolved->trueScale.y = localScale->y * parent->trueScale.y;
}

void FABLE_FASTCALL FableResolveUiGeneratedChildOrigins(
    const FableUiVector2* childLocalPosition,
    const FableUiVector2* uiScale,
    const FableUiResolvedTransform* table,
    FableUiVector2* ordinaryPosition,
    FableUiVector2* truePosition)
{
    ordinaryPosition->x =
        childLocalPosition->x + table->position.x;
    ordinaryPosition->y =
        childLocalPosition->y + table->position.y;
    truePosition->x =
        childLocalPosition->x * uiScale->x +
        table->truePosition.x;
    truePosition->y =
        childLocalPosition->y * uiScale->y +
        table->truePosition.y;
}

void FABLE_FASTCALL FablePlanUiTableLine(
    bool vertical,
    const FableUiVector2* start,
    const FableUiVector2* primarySize,
    const FableUiVector2* secondarySize,
    unsigned long repetitions,
    unsigned long resourceKeyOffset,
    const fable_i32* resourceKeys,
    unsigned int resourceKeyCount,
    FableUiTableLineSecondary* secondaryOutput,
    unsigned int secondaryCapacity,
    FableUiTableLinePlan* plan)
{
    plan->primaryPosition = *start;
    plan->primaryZoom.x = vertical ? 1.0f :
        static_cast<float>(repetitions);
    plan->primaryZoom.y = vertical ?
        static_cast<float>(repetitions) : 1.0f;

    FableUiVector2 cursor = *start;
    if (vertical)
        cursor.y += primarySize->y;
    else
        cursor.x += primarySize->x;

    unsigned int emitted = 0;
    for (
        unsigned long iteration = 0;
        iteration != repetitions;
        ++iteration)
    {
        const fable_i32 resourceKey = static_cast<fable_i32>(
            iteration - resourceKeyOffset);
        bool present = false;
        for (
            unsigned int keyIndex = 0;
            keyIndex != resourceKeyCount;
            ++keyIndex)
        {
            if (resourceKeys[keyIndex] == resourceKey)
            {
                present = true;
                break;
            }
        }
        if (!present)
            continue;

        if (
            secondaryOutput != 0 &&
            emitted < secondaryCapacity)
        {
            secondaryOutput[emitted].position = cursor;
            secondaryOutput[emitted].resourceKey = resourceKey;
        }
        ++emitted;
        if (vertical)
            cursor.y += secondarySize->y;
        else
            cursor.x += secondarySize->x;
    }
    plan->finalCursor = cursor;
    plan->secondaryCount = emitted;
    plan->generatedChildCount = 1 + emitted;
}

namespace
{
    bool HasUiTableSpriteMask(
        unsigned int availableSpriteMask,
        unsigned int spriteKey)
    {
        return (
            availableSpriteMask &
            (1UL << spriteKey)) != 0;
    }

    bool HasUiTableSprite(
        const FableUiTableSpritePlanInput* input,
        unsigned int spriteKey)
    {
        return HasUiTableSpriteMask(
            input->availableSpriteMask,
            spriteKey);
    }

    void AppendUiTableSpritePlanEvent(
        FableUiTableSpritePlanEvent* eventOutput,
        unsigned int eventOutputCapacity,
        FableUiTableSpritePlan* plan,
        unsigned int kind,
        unsigned int primarySpriteKey,
        fable_i32 secondarySpriteKey,
        unsigned int separatorIndex,
        unsigned int repeatCount,
        fable_i32 resourceKeyOffset)
    {
        if (
            eventOutput != 0 &&
            plan->writtenEventCount < eventOutputCapacity)
        {
            FableUiTableSpritePlanEvent& event =
                eventOutput[plan->writtenEventCount];
            event.kind = kind;
            event.primarySpriteKey = primarySpriteKey;
            event.secondarySpriteKey = secondarySpriteKey;
            event.separatorIndex = separatorIndex;
            event.repeatCount = repeatCount;
            event.resourceKeyOffset = resourceKeyOffset;
            ++plan->writtenEventCount;
        }
        ++plan->logicalEventCount;
    }
}

void FABLE_FASTCALL FablePlanUiTableSpriteComposition(
    const FableUiTableSpritePlanInput* input,
    FableUiTableSpritePlanEvent* eventOutput,
    unsigned int eventOutputCapacity,
    FableUiTableSpritePlan* plan)
{
    plan->logicalEventCount = 0;
    plan->writtenEventCount = 0;

    if (HasUiTableSprite(input, 0))
    {
        AppendUiTableSpritePlanEvent(
            eventOutput,
            eventOutputCapacity,
            plan,
            FableUiTableSpriteCorner,
            0,
            -1,
            0,
            0,
            0);
    }
    if (
        input->topRightCornerVisible != 0 &&
        HasUiTableSprite(input, 1))
    {
        AppendUiTableSpritePlanEvent(
            eventOutput,
            eventOutputCapacity,
            plan,
            FableUiTableSpriteCorner,
            1,
            -1,
            0,
            0,
            0);
    }
    if (
        input->bottomLeftCornerVisible != 0 &&
        HasUiTableSprite(input, 2))
    {
        AppendUiTableSpritePlanEvent(
            eventOutput,
            eventOutputCapacity,
            plan,
            FableUiTableSpriteCorner,
            2,
            -1,
            0,
            0,
            0);
    }
    if (
        input->bottomRightCornerVisible != 0 &&
        HasUiTableSprite(input, 3))
    {
        AppendUiTableSpritePlanEvent(
            eventOutput,
            eventOutputCapacity,
            plan,
            FableUiTableSpriteCorner,
            3,
            -1,
            0,
            0,
            0);
    }

    if (HasUiTableSprite(input, 4))
    {
        AppendUiTableSpritePlanEvent(
            eventOutput,
            eventOutputCapacity,
            plan,
            FableUiTableSpriteHorizontalLine,
            4,
            HasUiTableSprite(input, 9) ? 9 : -1,
            0,
            input->horizontalRepeatCount,
            0);
        for (
            unsigned int separator = 0;
            separator != input->horizontalSeparatorCount;
            ++separator)
        {
            AppendUiTableSpritePlanEvent(
                eventOutput,
                eventOutputCapacity,
                plan,
                FableUiTableSpriteHorizontalLine,
                4,
                HasUiTableSprite(input, 12) ? 12 : -1,
                separator,
                input->horizontalRepeatCount,
                input->horizontalInteriorResourceKeyOffset);
        }
    }
    if (HasUiTableSprite(input, 5))
    {
        AppendUiTableSpritePlanEvent(
            eventOutput,
            eventOutputCapacity,
            plan,
            FableUiTableSpriteHorizontalLine,
            5,
            HasUiTableSprite(input, 8) ? 8 : -1,
            0,
            input->horizontalRepeatCount,
            0);
    }
    if (HasUiTableSprite(input, 6))
    {
        AppendUiTableSpritePlanEvent(
            eventOutput,
            eventOutputCapacity,
            plan,
            FableUiTableSpriteVerticalLine,
            6,
            HasUiTableSprite(input, 10) ? 10 : -1,
            0,
            input->verticalRepeatCount,
            0);
        for (
            unsigned int separator = 0;
            separator != input->verticalSeparatorCount;
            ++separator)
        {
            AppendUiTableSpritePlanEvent(
                eventOutput,
                eventOutputCapacity,
                plan,
                FableUiTableSpriteVerticalLine,
                6,
                HasUiTableSprite(input, 12) ? 12 : -1,
                separator,
                input->verticalRepeatCount,
                input->verticalInteriorResourceKeyOffset);
        }
    }
    if (HasUiTableSprite(input, 7))
    {
        AppendUiTableSpritePlanEvent(
            eventOutput,
            eventOutputCapacity,
            plan,
            FableUiTableSpriteVerticalLine,
            7,
            HasUiTableSprite(input, 11) ? 11 : -1,
            0,
            input->verticalRepeatCount,
            0);
    }
}

void FABLE_FASTCALL FablePlanUiTableGeometry(
    const FableUiTableGeometryInput* input,
    FableUiTableInteriorLine* horizontalInteriorOutput,
    unsigned int horizontalInteriorCapacity,
    FableUiTableInteriorLine* verticalInteriorOutput,
    unsigned int verticalInteriorCapacity,
    FableUiTableGeometryPlan* plan)
{
    const float cornerVisibilityEpsilonSquared = 9.999999e-09f;
    const float horizontalExtent =
        input->horizontalEdgeSize.x *
        static_cast<float>(input->horizontalRepeatCount);
    const float verticalExtent =
        input->verticalEdgeSize.y *
        static_cast<float>(input->verticalRepeatCount);

    FableUiVector2 topRight = {horizontalExtent, 0.0f};
    FableUiVector2 bottomLeft = {0.0f, verticalExtent};
    FableUiVector2 bottomRight = {
        horizontalExtent,
        verticalExtent};
    FableUiVector2 topLeft = {0.0f, 0.0f};

    plan->emittedCornerMask = 0;
    plan->horizontalExtent = horizontalExtent;
    plan->verticalExtent = verticalExtent;
    plan->logicalHorizontalInteriorCount =
        input->horizontalSeparatorCount;
    plan->writtenHorizontalInteriorCount = 0;
    plan->logicalVerticalInteriorCount =
        input->verticalSeparatorCount;
    plan->writtenVerticalInteriorCount = 0;

    for (unsigned int corner = 0; corner != 4; ++corner)
    {
        plan->cornerPosition[corner].x = 0.0f;
        plan->cornerPosition[corner].y = 0.0f;
    }

    if (HasUiTableSpriteMask(input->availableSpriteMask, 0))
    {
        plan->cornerPosition[0] = topLeft;
        plan->emittedCornerMask |= 1UL << 0;
        topLeft.x += input->cornerSize[0].x;
        bottomLeft.y += input->cornerSize[0].y;
        topRight.x += input->cornerSize[0].x;
    }

    if (
        HasUiTableSpriteMask(input->availableSpriteMask, 1) &&
        topRight.x * topRight.x + topRight.y * topRight.y >
            cornerVisibilityEpsilonSquared)
    {
        plan->cornerPosition[1] = topRight;
        plan->emittedCornerMask |= 1UL << 1;
        topRight.x +=
            input->cornerSize[1].x -
            input->horizontalEdgeSize.x;
        topRight.y += input->cornerSize[1].y;
        bottomRight.y += input->cornerSize[1].y;
    }

    if (
        HasUiTableSpriteMask(input->availableSpriteMask, 2) &&
        bottomLeft.x * bottomLeft.x + bottomLeft.y * bottomLeft.y >
            cornerVisibilityEpsilonSquared)
    {
        plan->cornerPosition[2] = bottomLeft;
        plan->emittedCornerMask |= 1UL << 2;
        bottomLeft.y -= verticalExtent;
        bottomRight.x += input->cornerSize[2].x;
    }

    if (
        HasUiTableSpriteMask(input->availableSpriteMask, 3) &&
        bottomRight.x * bottomRight.x +
                bottomRight.y * bottomRight.y >
            cornerVisibilityEpsilonSquared)
    {
        plan->cornerPosition[3] = bottomRight;
        plan->emittedCornerMask |= 1UL << 3;
        bottomRight.x -= horizontalExtent;
        bottomRight.y +=
            input->cornerSize[3].y -
            input->verticalEdgeSize.y;
    }

    plan->topLineStart = topLeft;
    plan->bottomLineStart = bottomRight;
    plan->leftLineStart = bottomLeft;
    plan->rightLineStart = topRight;

    fable_i32 horizontalResourceKeyOffset = -1;
    if (
        (plan->emittedCornerMask & (1UL << 0)) != 0 &&
        input->horizontalEdgeSize.x != 0.0f)
    {
        horizontalResourceKeyOffset =
            static_cast<fable_i32>(
                input->cornerSize[0].x /
                input->horizontalEdgeSize.x) -
            1;
    }

    for (
        unsigned int separator = 0;
        separator != input->horizontalSeparatorCount;
        ++separator)
    {
        FableUiTableInteriorLine line = {};
        line.position.x =
            bottomLeft.x + input->horizontalEdgeSize.x;
        line.position.y =
            bottomLeft.y +
            static_cast<float>(
                input->horizontalSeparatorTiles[separator]) *
                input->verticalEdgeSize.y;
        if (input->horizontalEdgeSize.x != 0.0f)
        {
            line.repeatCount = static_cast<unsigned long>(
                (topRight.x - line.position.x) /
                input->horizontalEdgeSize.x);
        }
        line.resourceKeyOffset = horizontalResourceKeyOffset;

        if (
            horizontalInteriorOutput != 0 &&
            plan->writtenHorizontalInteriorCount <
                horizontalInteriorCapacity)
        {
            horizontalInteriorOutput[
                plan->writtenHorizontalInteriorCount] = line;
            ++plan->writtenHorizontalInteriorCount;
        }
    }

    fable_i32 verticalResourceKeyOffset = -1;
    if (
        (plan->emittedCornerMask & (1UL << 0)) != 0 &&
        input->verticalEdgeSize.y != 0.0f)
    {
        verticalResourceKeyOffset =
            static_cast<fable_i32>(
                input->cornerSize[0].y /
                input->verticalEdgeSize.y) -
            1;
    }

    for (
        unsigned int separator = 0;
        separator != input->verticalSeparatorCount;
        ++separator)
    {
        FableUiTableInteriorLine line = {};
        line.position.x =
            topLeft.x +
            static_cast<float>(
                input->verticalSeparatorTiles[separator]) *
                input->horizontalEdgeSize.x;
        line.position.y =
            topLeft.y + input->verticalEdgeSize.y;
        if (input->verticalEdgeSize.y != 0.0f)
        {
            line.repeatCount = static_cast<unsigned long>(
                (bottomRight.y - line.position.y) /
                input->verticalEdgeSize.y);
        }
        line.resourceKeyOffset = verticalResourceKeyOffset;

        if (
            verticalInteriorOutput != 0 &&
            plan->writtenVerticalInteriorCount <
                verticalInteriorCapacity)
        {
            verticalInteriorOutput[
                plan->writtenVerticalInteriorCount] = line;
            ++plan->writtenVerticalInteriorCount;
        }
    }
}

namespace
{
    void RetainUiCountedComponent(
        FableUiCountedComponent* counted)
    {
        if (counted->reference == 0)
            return;
        ++counted->reference->referenceCount;
        if (counted->reference->counters != 0)
            ++counted->reference->counters->retains;
    }

    void ReleaseUiCountedComponent(
        FableUiCountedComponent* counted)
    {
        FableUiCountedReference* reference = counted->reference;
        counted->component = 0;
        counted->reference = 0;
        if (reference == 0)
            return;

        if (reference->counters != 0)
            ++reference->counters->releases;
        --reference->referenceCount;
        if (reference->referenceCount != 0)
            return;

        FableUiComponentLifetimeCounters* counters =
            reference->counters;
        delete reference->component;
        if (counters != 0)
            ++counters->componentDeletions;
        delete reference;
        if (counters != 0)
            ++counters->controlDeletions;
    }

    bool AppendUiCountedComponent(
        FableUiGeneratedComponentVector* generated,
        const FableUiCountedComponent* counted)
    {
        if (
            generated->values == 0 ||
            generated->size == generated->capacity)
        {
            return false;
        }

        generated->values[generated->size] = *counted;
        RetainUiCountedComponent(
            &generated->values[generated->size]);
        ++generated->size;
        return true;
    }

    bool CloneUiTableSprite(
        const FableUiTableRuntimeInput* input,
        unsigned int spriteKey,
        const FableUiVector2* position,
        const FableUiVector2* zoom,
        FableUiGeneratedComponentVector* generated)
    {
        FableUiRuntimeComponent* component =
            new FableUiRuntimeComponent;
        if (component == 0)
            return false;
        if (generated->counters != 0)
            ++generated->counters->componentAllocations;

        component->definitionId =
            input->sprite[spriteKey].definitionId;
        component->sourceSpriteKey = spriteKey;
        component->initialised = 1;
        component->stateMask = 1;
        component->size = input->sprite[spriteKey].size;
        component->state[0].position = *position;
        component->state[0].zoom = *zoom;

        FableUiCountedReference* reference =
            new FableUiCountedReference;
        if (reference == 0)
        {
            delete component;
            if (generated->counters != 0)
                ++generated->counters->componentDeletions;
            return false;
        }
        if (generated->counters != 0)
            ++generated->counters->controlAllocations;

        reference->referenceCount = 1;
        reference->component = component;
        reference->counters = generated->counters;

        FableUiCountedComponent local = {
            component,
            reference};
        const bool appended =
            AppendUiCountedComponent(generated, &local);
        ReleaseUiCountedComponent(&local);
        return appended;
    }

    bool HasUiTableResourceKey(
        const unsigned long* resourceKeys,
        unsigned int resourceKeyCount,
        fable_i32 resourceKey)
    {
        for (
            unsigned int keyIndex = 0;
            keyIndex != resourceKeyCount;
            ++keyIndex)
        {
            if (
                static_cast<fable_i32>(
                    resourceKeys[keyIndex]) == resourceKey)
            {
                return true;
            }
        }
        return false;
    }

    bool ConstructUiTableLine(
        const FableUiTableRuntimeInput* input,
        bool vertical,
        unsigned int primarySpriteKey,
        fable_i32 secondarySpriteKey,
        const FableUiVector2* start,
        unsigned long repetitions,
        fable_i32 resourceKeyOffset,
        const unsigned long* resourceKeys,
        unsigned int resourceKeyCount,
        FableUiGeneratedComponentVector* generated)
    {
        FableUiVector2 primaryZoom = {1.0f, 1.0f};
        if (vertical)
            primaryZoom.y = static_cast<float>(repetitions);
        else
            primaryZoom.x = static_cast<float>(repetitions);

        if (!CloneUiTableSprite(
                input,
                primarySpriteKey,
                start,
                &primaryZoom,
                generated))
        {
            return false;
        }

        FableUiVector2 cursor = *start;
        if (vertical)
        {
            cursor.y +=
                input->sprite[primarySpriteKey].size.y;
        }
        else
        {
            cursor.x +=
                input->sprite[primarySpriteKey].size.x;
        }

        if (
            secondarySpriteKey < 0 ||
            !HasUiTableSpriteMask(
                input->geometry.availableSpriteMask,
                static_cast<unsigned int>(secondarySpriteKey)))
        {
            return true;
        }

        const FableUiVector2 secondaryZoom = {1.0f, 1.0f};
        for (
            unsigned long iteration = 0;
            iteration != repetitions;
            ++iteration)
        {
            const fable_i32 resourceKey =
                static_cast<fable_i32>(iteration) -
                resourceKeyOffset;
            if (!HasUiTableResourceKey(
                    resourceKeys,
                    resourceKeyCount,
                    resourceKey))
            {
                continue;
            }

            if (!CloneUiTableSprite(
                    input,
                    static_cast<unsigned int>(
                        secondarySpriteKey),
                    &cursor,
                    &secondaryZoom,
                    generated))
            {
                return false;
            }
            if (vertical)
            {
                cursor.y += input->sprite[
                    secondarySpriteKey].size.y;
            }
            else
            {
                cursor.x += input->sprite[
                    secondarySpriteKey].size.x;
            }
        }
        return true;
    }
}

void FABLE_FASTCALL FableReleaseUiGeneratedComponents(
    FableUiGeneratedComponentVector* generated)
{
    if (generated == 0)
        return;
    for (
        unsigned int componentIndex = 0;
        componentIndex != generated->size;
        ++componentIndex)
    {
        ReleaseUiCountedComponent(
            &generated->values[componentIndex]);
    }
    generated->size = 0;
}

bool FABLE_FASTCALL FableConstructUiTableComponents(
    const FableUiTableRuntimeInput* input,
    FableUiGeneratedComponentVector* generated)
{
    if (
        input == 0 ||
        generated == 0 ||
        generated->size != 0 ||
        (generated->capacity != 0 && generated->values == 0) ||
        (input->geometry.horizontalSeparatorCount != 0 &&
            input->geometry.horizontalSeparatorTiles == 0) ||
        (input->geometry.verticalSeparatorCount != 0 &&
            input->geometry.verticalSeparatorTiles == 0))
    {
        return false;
    }

    FableUiTableInteriorLine* horizontalInterior = 0;
    FableUiTableInteriorLine* verticalInterior = 0;
    if (input->geometry.horizontalSeparatorCount != 0)
    {
        horizontalInterior = new FableUiTableInteriorLine[
            input->geometry.horizontalSeparatorCount];
    }
    if (input->geometry.verticalSeparatorCount != 0)
    {
        verticalInterior = new FableUiTableInteriorLine[
            input->geometry.verticalSeparatorCount];
    }

    FableUiTableGeometryPlan geometry = {};
    FablePlanUiTableGeometry(
        &input->geometry,
        horizontalInterior,
        input->geometry.horizontalSeparatorCount,
        verticalInterior,
        input->geometry.verticalSeparatorCount,
        &geometry);

    const FableUiVector2 unitZoom = {1.0f, 1.0f};
    bool succeeded = true;
    for (
        unsigned int corner = 0;
        succeeded && corner != 4;
        ++corner)
    {
        if ((geometry.emittedCornerMask & (1UL << corner)) == 0)
            continue;
        succeeded = CloneUiTableSprite(
            input,
            corner,
            &geometry.cornerPosition[corner],
            &unitZoom,
            generated);
    }

    const unsigned long* horizontalLineResourceKeys =
        input->geometry.verticalSeparatorTiles;
    const unsigned int horizontalLineResourceKeyCount =
        input->geometry.verticalSeparatorCount;
    const unsigned long* verticalLineResourceKeys =
        input->geometry.horizontalSeparatorTiles;
    const unsigned int verticalLineResourceKeyCount =
        input->geometry.horizontalSeparatorCount;

    if (
        succeeded &&
        HasUiTableSpriteMask(
            input->geometry.availableSpriteMask,
            4))
    {
        succeeded = ConstructUiTableLine(
            input,
            false,
            4,
            9,
            &geometry.topLineStart,
            input->geometry.horizontalRepeatCount,
            0,
            horizontalLineResourceKeys,
            horizontalLineResourceKeyCount,
            generated);
    }
    for (
        unsigned int line = 0;
        succeeded &&
            line != geometry.writtenHorizontalInteriorCount;
        ++line)
    {
        if (!HasUiTableSpriteMask(
                input->geometry.availableSpriteMask,
                4))
        {
            break;
        }
        succeeded = ConstructUiTableLine(
            input,
            false,
            4,
            12,
            &horizontalInterior[line].position,
            horizontalInterior[line].repeatCount,
            horizontalInterior[line].resourceKeyOffset,
            horizontalLineResourceKeys,
            horizontalLineResourceKeyCount,
            generated);
    }
    if (
        succeeded &&
        HasUiTableSpriteMask(
            input->geometry.availableSpriteMask,
            5))
    {
        succeeded = ConstructUiTableLine(
            input,
            false,
            5,
            8,
            &geometry.bottomLineStart,
            input->geometry.horizontalRepeatCount,
            0,
            horizontalLineResourceKeys,
            horizontalLineResourceKeyCount,
            generated);
    }
    if (
        succeeded &&
        HasUiTableSpriteMask(
            input->geometry.availableSpriteMask,
            6))
    {
        succeeded = ConstructUiTableLine(
            input,
            true,
            6,
            10,
            &geometry.leftLineStart,
            input->geometry.verticalRepeatCount,
            0,
            verticalLineResourceKeys,
            verticalLineResourceKeyCount,
            generated);
    }
    for (
        unsigned int line = 0;
        succeeded &&
            line != geometry.writtenVerticalInteriorCount;
        ++line)
    {
        if (!HasUiTableSpriteMask(
                input->geometry.availableSpriteMask,
                6))
        {
            break;
        }
        succeeded = ConstructUiTableLine(
            input,
            true,
            6,
            12,
            &verticalInterior[line].position,
            verticalInterior[line].repeatCount,
            verticalInterior[line].resourceKeyOffset,
            verticalLineResourceKeys,
            verticalLineResourceKeyCount,
            generated);
    }
    if (
        succeeded &&
        HasUiTableSpriteMask(
            input->geometry.availableSpriteMask,
            7))
    {
        succeeded = ConstructUiTableLine(
            input,
            true,
            7,
            11,
            &geometry.rightLineStart,
            input->geometry.verticalRepeatCount,
            0,
            verticalLineResourceKeys,
            verticalLineResourceKeyCount,
            generated);
    }

    delete[] horizontalInterior;
    delete[] verticalInterior;
    if (!succeeded)
        FableReleaseUiGeneratedComponents(generated);
    return succeeded;
}

void FABLE_FASTCALL FablePlanUiListSelection(
    long requestedChild,
    const unsigned char* visibleChildren,
    unsigned int childCount,
    FableUiListSelectionPlan* plan)
{
    plan->selectedChild = requestedChild;
    plan->firstVisibleChild = -1;
    plan->lastVisibleChild = -1;
    plan->scrollDelta.x = 0.0f;
    plan->scrollDelta.y = 0.0f;
    plan->requestsScroll = false;

    if (
        requestedChild < 0 ||
        static_cast<unsigned long>(requestedChild) >= childCount ||
        visibleChildren[requestedChild] != 0)
    {
        return;
    }

    unsigned int firstVisible = 0xffffffffUL;
    long lastVisible = -1;
    for (unsigned int child = 0; child != childCount; ++child)
    {
        if (
            firstVisible == 0xffffffffUL &&
            visibleChildren[child] != 0)
        {
            firstVisible = child;
        }
        if (
            firstVisible != 0xffffffffUL &&
            visibleChildren[child] == 0)
        {
            lastVisible = static_cast<long>(child) - 1;
            break;
        }
    }
    if (lastVisible == -1)
        lastVisible = static_cast<long>(childCount) - 1;

    plan->firstVisibleChild =
        firstVisible == 0xffffffffUL
            ? -1
            : static_cast<long>(firstVisible);
    plan->lastVisibleChild = lastVisible;

    if (lastVisible < requestedChild)
    {
        plan->scrollDelta.y =
            static_cast<float>(requestedChild - lastVisible) * -25.0f;
        plan->requestsScroll = true;
    }
    else if (firstVisible != 0xffffffffUL)
    {
        plan->scrollDelta.y =
            static_cast<float>(
                static_cast<long>(firstVisible) - requestedChild) *
            25.0f;
        plan->requestsScroll = true;
    }
}

void FABLE_FASTCALL FablePlanUiFrontEndListInitialOffsets(
    unsigned long childCount,
    FableUiFrontEndListOffsetPlan* plan)
{
    (void)childCount;
    plan->interpolationScalar = 1.0f;
    plan->positionOffset.x = 0.0f;
    plan->positionOffset.y = 0.0f;
}

void FABLE_FASTCALL FablePlanUiListInitialOffsets(
    unsigned long childCount,
    const FableUiVector2* definitionPositionOffset,
    unsigned char alphaFalloff,
    bool zeroInitialPosition,
    FableUiFrontEndListOffsetPlan* plan)
{
    if (definitionPositionOffset == 0 || plan == 0)
        return;

    const unsigned long centreOffset =
        (childCount & 1) != 0
            ? childCount >> 1
            : (childCount >> 1) - 1;
    const float centreOffsetFloat =
        static_cast<float>(centreOffset);
    const float alphaStep =
        static_cast<float>(alphaFalloff) *
        0.003921568859368563f;

    plan->interpolationScalar =
        1.0f - alphaStep * centreOffsetFloat;
    if (zeroInitialPosition)
    {
        plan->positionOffset.x = 0.0f;
        plan->positionOffset.y = 0.0f;
    }
    else
    {
        plan->positionOffset.x =
            centreOffsetFloat * definitionPositionOffset->x;
        plan->positionOffset.y =
            -(centreOffsetFloat * definitionPositionOffset->y);
    }
}

void FABLE_FASTCALL FablePlanUiListRecomputeOffsets(
    const FableUiListRecomputeInput* input,
    FableUiListRecomputedChild* childOutput,
    unsigned int childOutputCapacity,
    FableUiListRecomputePlan* plan)
{
    if (input == 0 || plan == 0)
        return;

    plan->logicalChildCount = input->childCount;
    plan->writtenChildCount = 0;
    plan->positionStateMask =
        (1u << 0) | (1u << 1) | (1u << 4) |
        (1u << 5) | (1u << 6);
    plan->alphaStateMask =
        (1u << 1) | (1u << 4) | (1u << 5);

    FableUiVector2 position = input->initialPosition;
    FableUiVector2 positionStep;
    positionStep.x = -input->definitionPositionOffset.x;
    positionStep.y = input->definitionPositionOffset.y;
    const float alphaStep =
        static_cast<float>(input->alphaFalloff) *
        0.003921568859368563f;
    float alphaStepToUse = alphaStep;
    float alpha = input->initialAlpha;

    for (unsigned int child = 0; child != input->childCount; ++child)
    {
        unsigned long byteAlpha = 0;
        if (alpha > 1.0f)
        {
            byteAlpha = 255;
        }
        else if (alpha >= 0.0f)
        {
            byteAlpha =
                static_cast<unsigned long>(alpha * 255.0f);
        }

        if (
            childOutput != 0 &&
            plan->writtenChildCount < childOutputCapacity)
        {
            childOutput[plan->writtenChildCount].position = position;
            childOutput[plan->writtenChildCount].alpha =
                static_cast<unsigned char>(byteAlpha);
            ++plan->writtenChildCount;
        }

        position.y += positionStep.y;
        const float nextAlpha = alpha + alphaStepToUse;
        if (nextAlpha >= 0.999f)
        {
            alphaStepToUse = -alphaStepToUse;
            positionStep.x = -positionStep.x;
            if (alpha >= 1.0f)
            {
                position.x += positionStep.x;
                alpha += alphaStepToUse;
            }
            else
            {
                position.x = 0.0f;
                alpha = 1.0f;
            }
        }
        else
        {
            alpha = nextAlpha;
            position.x += positionStep.x;
        }
    }

    plan->finalPosition = position;
    plan->finalAlpha = alpha;
}

bool FABLE_FASTCALL FableApplyUiListRecomputedStates(
    const FableUiListRecomputeInput* input,
    FableUiRuntimeListChild* children,
    unsigned int childCapacity,
    FableUiRuntimeStateMap* stateSnapshots,
    unsigned int stateSnapshotCapacity,
    FableUiListRecomputePlan* plan)
{
    if (
        input == 0 ||
        plan == 0 ||
        input->childCount > childCapacity ||
        input->childCount > stateSnapshotCapacity ||
        (input->childCount != 0 &&
            (children == 0 || stateSnapshots == 0)))
    {
        return false;
    }

    FableUiListRecomputedChild* recomputed = 0;
    if (input->childCount != 0)
    {
        recomputed = new FableUiListRecomputedChild[
            input->childCount];
    }
    FablePlanUiListRecomputeOffsets(
        input,
        recomputed,
        input->childCount,
        plan);

    const unsigned int positionStates[5] = {0, 1, 4, 5, 6};
    const unsigned int alphaStates[3] = {1, 4, 5};
    for (
        unsigned int childIndex = 0;
        childIndex != input->childCount;
        ++childIndex)
    {
        FableUiRuntimeListChild& child = children[childIndex];
        for (unsigned int stateIndex = 0; stateIndex != 5; ++stateIndex)
        {
            const unsigned int state = positionStates[stateIndex];
            child.states.state[state].position.x =
                recomputed[childIndex].position.x;
            child.states.state[state].position.y =
                recomputed[childIndex].position.y;
            child.states.stateMask |= 1u << state;
        }
        for (unsigned int stateIndex = 0; stateIndex != 3; ++stateIndex)
        {
            const unsigned int state = alphaStates[stateIndex];
            child.states.state[state].colour.alpha =
                recomputed[childIndex].alpha;
            child.states.stateMask |= 1u << state;
        }

        child.currentPosition.x =
            child.states.state[1].position.x;
        child.currentPosition.y =
            child.states.state[1].position.y;
        child.currentColour =
            child.states.state[1].colour;
        stateSnapshots[childIndex] = child.states;
    }
    delete[] recomputed;
    return true;
}

void FABLE_FASTCALL FablePlanUiFrontEndListScroll(
    bool scrollDown,
    unsigned int childCount,
    long selectedChild,
    bool stopsAtEnds,
    unsigned char alphaFalloff,
    unsigned char* childAlphaOutput,
    unsigned int childAlphaCapacity,
    FableUiFrontEndListScrollPlan* plan)
{
    if (plan == 0)
        return;

    plan->previousSelectedChild = selectedChild;
    plan->selectedChild = selectedChild;
    plan->rowTranslation.x = 0.0f;
    plan->rowTranslation.y = 0.0f;
    plan->previousChildState = 0;
    plan->selectedChildState = 0;
    plan->logicalAlphaCount = 0;
    plan->writtenAlphaCount = 0;
    plan->soundRequest = FableUiFrontEndSoundNone;
    plan->soundCriteriaDefinitionOffset = 0;
    plan->moved = false;
    plan->blockedAtBoundary = false;
    plan->requestsInvalidAction = false;
    plan->requestsMoveAction = false;

    if (childCount <= 1)
    {
        plan->requestsInvalidAction = true;
        plan->soundRequest = FableUiFrontEndSoundError;
        plan->soundCriteriaDefinitionOffset = 0x1A4;
        return;
    }

    const long lastChild = static_cast<long>(childCount - 1);
    const bool atBoundary =
        scrollDown ? selectedChild == lastChild : selectedChild == 0;
    if (atBoundary && stopsAtEnds)
    {
        plan->blockedAtBoundary = true;
        plan->requestsInvalidAction = true;
        plan->soundRequest = FableUiFrontEndSoundError;
        plan->soundCriteriaDefinitionOffset = 0x1A4;
        return;
    }

    long nextChild = selectedChild + (scrollDown ? 1 : -1);
    if (nextChild < 0)
        nextChild = lastChild;
    else if (nextChild > lastChild)
        nextChild = 0;

    plan->selectedChild = nextChild;
    plan->rowTranslation.y = scrollDown ? -30.0f : 30.0f;
    plan->previousChildState = 4;
    plan->selectedChildState = 3;
    plan->moved = true;
    plan->requestsMoveAction = true;
    plan->soundRequest = FableUiFrontEndSoundUpDown;
    plan->soundCriteriaDefinitionOffset = 0x194;

    if (!stopsAtEnds)
        return;

    plan->logicalAlphaCount = childCount;
    for (unsigned int child = 0; child != childCount; ++child)
    {
        const long signedDistance =
            nextChild - static_cast<long>(child);
        const unsigned long distance =
            static_cast<unsigned long>(
                signedDistance < 0 ? -signedDistance : signedDistance);
        float alpha =
            1.0f -
            static_cast<float>(distance) *
            static_cast<float>(alphaFalloff) *
            0.003921568859368563f;
        if (alpha < 0.0f)
            alpha = 0.0f;
        unsigned long byteAlpha =
            static_cast<unsigned long>(alpha * 255.0f);
        if (byteAlpha > 255)
            byteAlpha = 255;
        if (
            childAlphaOutput != 0 &&
            plan->writtenAlphaCount < childAlphaCapacity)
        {
            childAlphaOutput[plan->writtenAlphaCount] =
                static_cast<unsigned char>(byteAlpha);
            ++plan->writtenAlphaCount;
        }
    }
}

struct FablePlanUiFrontEndListEventTarget
{
    FableRetailFrontendListProcessContext context;
    unsigned int dispatchedEvent;
};

bool FablePlanUiFrontEndListEventCondition(
    void* component,
    unsigned int event)
{
    if (component == 0)
        return false;
    const FablePlanUiFrontEndListEventTarget* target =
        static_cast<const FablePlanUiFrontEndListEventTarget*>(component);
    if (!target->context.viewportConditionPasses ||
        !target->context.hoverConditionPasses ||
        !target->context.managerActionConditionPasses ||
        !target->context.componentConditionPasses)
    {
        return false;
    }
    return event == FableRetailFrontendListEventUp ||
        event == FableRetailFrontendListEventDown;
}

void FablePlanUiFrontEndListEventProcess(
    void* component,
    unsigned int event)
{
    FablePlanUiFrontEndListEventTarget* target =
        static_cast<FablePlanUiFrontEndListEventTarget*>(component);
    if (target == 0)
        return;
    unsigned int dispatchedEvent = 0;
    if (FableRetailFrontendListProcessEvent(
            event,
            target->context,
            &dispatchedEvent))
    {
        target->dispatchedEvent = dispatchedEvent;
    }
}

void FABLE_FASTCALL FablePlanUiFrontEndListProcessEvent(
    unsigned int event,
    unsigned int childCount,
    long selectedChild,
    bool stopsAtEnds,
    unsigned char alphaFalloff,
    unsigned char* childAlphaOutput,
    unsigned int childAlphaCapacity,
    FableUiFrontEndListScrollPlan* plan)
{
    if (plan == 0)
        return;

    // The visual adapter does not yet own a live NUISystem::CList object, so
    // it presents the visible list as the current component at the proven
    // CManager::ProcessEvent boundary.  This preserves the native manager
    // ordering (current component first, condition then process) while the
    // native object connection remains an explicit integration boundary.
    const FableRetailFrontendListProcessContext context = {
        true,
        true,
        true,
        true};
    FablePlanUiFrontEndListEventTarget target = {
        context,
        0};
    const FableRetailFrontendManagerEventTarget managerTarget = {
        &target,
        FablePlanUiFrontEndListEventCondition,
        FablePlanUiFrontEndListEventProcess};
    if (FableRetailFrontendManagerProcessEvent(
            &managerTarget,
            0,
            0,
            event) == 0)
    {
        *plan = FableUiFrontEndListScrollPlan();
        plan->previousSelectedChild = selectedChild;
        plan->selectedChild = selectedChild;
        return;
    }

    FablePlanUiFrontEndListScroll(
        target.dispatchedEvent == FableUiFrontEndListEventDown,
        childCount,
        selectedChild,
        stopsAtEnds,
        alphaFalloff,
        childAlphaOutput,
        childAlphaCapacity,
        plan);
}

bool FABLE_FASTCALL FableApplyUiFrontEndListScroll(
    bool scrollDown,
    bool wrapping,
    unsigned char alphaFalloff,
    FableUiRuntimeListChild* children,
    unsigned int childCount,
    unsigned int childCapacity,
    long selectedChild,
    FableUiFrontEndListScrollPlan* plan)
{
    if (
        plan == 0 ||
        childCount > childCapacity ||
        selectedChild < 0 ||
        static_cast<unsigned long>(selectedChild) >= childCount ||
        (childCount != 0 && children == 0))
    {
        return false;
    }

    unsigned char* alpha = 0;
    if (childCount != 0)
        alpha = new unsigned char[childCount];
    FablePlanUiFrontEndListScroll(
        scrollDown,
        childCount,
        selectedChild,
        !wrapping,
        alphaFalloff,
        alpha,
        childCount,
        plan);
    if (!plan->moved)
    {
        delete[] alpha;
        return false;
    }

    const unsigned int previous =
        static_cast<unsigned int>(plan->previousSelectedChild);
    const unsigned int selected =
        static_cast<unsigned int>(plan->selectedChild);
    children[previous].currentState =
        plan->previousChildState;
    children[selected].currentState =
        plan->selectedChildState;

    if (scrollDown)
    {
        const FableUiStateColour first =
            children[0].currentColour;
        for (unsigned int child = 0; child + 1 < childCount; ++child)
        {
            children[child].currentColour =
                children[child + 1].currentColour;
        }
        children[childCount - 1].currentColour = first;
    }
    else
    {
        const FableUiStateColour last =
            children[childCount - 1].currentColour;
        for (unsigned int child = childCount - 1; child != 0; --child)
        {
            children[child].currentColour =
                children[child - 1].currentColour;
        }
        children[0].currentColour = last;
    }

    if (!wrapping)
    {
        for (unsigned int child = 0; child != childCount; ++child)
        {
            children[child].currentPosition.x +=
                plan->rowTranslation.x;
            children[child].currentPosition.y +=
                plan->rowTranslation.y;
            children[child].currentColour.red = 255;
            children[child].currentColour.green = 255;
            children[child].currentColour.blue = 255;
            children[child].currentColour.alpha = alpha[child];
        }
    }
    delete[] alpha;
    return true;
}

namespace
{
    const int kBootArtworkResource = 101;
    const int kBootTitleResource = 102;
    const int kBootPointerResource = 105;
    const int kBootMenuResource = 106;
    const int kBootCoastalResource = 107;
    const int kBootCoastalSunbeamResource = 108;
    const int kBootOptionsResource = 109;
    const int kBootHelpersResource = 110;
    const int kBootRedefineScrollPagesResource = 125;
    const int kBootAboutResource = 120;
    const int kBootCreditsResource = 123;
    const int kBootProfilesResource = 124;
    const int kBootSpookyResource = 121;
    const int kBootSpookySunbeamResource = 122;
    const int kBootBuffJesusMenuResource = 111;
    const int kBootTitleSegmentResource = 112;
    const int kBootButtonLeftResource = 113;
    const int kBootButtonMiddleResource = 114;
    const int kBootButtonRightResource = 115;
    const int kBootSoundUpDownResource = 116;
    const int kBootSoundErrorResource = 117;
    const int kBootSoundBackResource = 118;
    const int kBootSoundForwardResource = 119;
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
    const char kD3DBuffJesusPresentedWindowTitle[] =
        "FableDecomp - D3D9 Presented via Render2D - BuffJesus Text Variant";
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
    const FableUint kLoadFromFile = 0x00000010;
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
    const FableUint kMessageChar = 0x0102;
    const FableUint kMessageTimer = 0x0113;
    const FableUint kMessageMouseMove = 0x0200;
    const FableUint kMessageLeftButtonDown = 0x0201;
    const FableUint kMessageLeftButtonUp = 0x0202;
    const FableUint kMessageRightButtonUp = 0x0205;
    const FableUint kMessageMiddleButtonUp = 0x0208;
    const FableUint kMessageMouseWheel = 0x020A;
    const FableWordParameter kRetailVideoTimer = 1;
    const FableWordParameter kFrontendAnimationTimer = 2;
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
    const FableDword kSoundAsync = 0x00000001UL;
    const FableDword kSoundNoDefault = 0x00000002UL;
    const FableDword kSoundResource = 0x00040004UL;
    const FableDword kJoyReturnAll = 0x000000FFUL;

    FableBitmap g_BootArtwork = 0;
    FableBitmapInfo g_BootArtworkInfo = {};
    FableBitmap g_BootTitleArtwork = 0;
    FableBitmapInfo g_BootTitleArtworkInfo = {};
    FableBitmap g_BootForestArtwork = 0;
    FableBitmapInfo g_BootForestArtworkInfo = {};
    FableBitmap g_BootSunbeamArtwork = 0;
    FableBitmapInfo g_BootSunbeamArtworkInfo = {};
    FableBitmap g_BootMenuArtwork = 0;
    FableBitmapInfo g_BootMenuArtworkInfo = {};
    FableBitmap g_BootBuffJesusMenuArtwork = 0;
    FableBitmapInfo g_BootBuffJesusMenuArtworkInfo = {};
    FableBitmap g_BootCoastalArtwork = 0;
    FableBitmapInfo g_BootCoastalArtworkInfo = {};
    FableBitmap g_BootCoastalSunbeamArtwork = 0;
    FableBitmapInfo g_BootCoastalSunbeamArtworkInfo = {};
    FableBitmap g_BootOptionsArtwork = 0;
    FableBitmapInfo g_BootOptionsArtworkInfo = {};
    FableBitmap g_BootHelpersArtwork = 0;
    FableBitmapInfo g_BootHelpersArtworkInfo = {};
    FableBitmap g_BootRedefineScrollPagesArtwork = 0;
    FableBitmapInfo g_BootRedefineScrollPagesArtworkInfo = {};
    FableBitmap g_BootAboutArtwork = 0;
    FableBitmapInfo g_BootAboutArtworkInfo = {};
    FableBitmap g_BootCreditsArtwork = 0;
    FableBitmapInfo g_BootCreditsArtworkInfo = {};
    FableBitmap g_BootProfilesArtwork = 0;
    FableBitmapInfo g_BootProfilesArtworkInfo = {};
    FableBitmap g_BootSpookyArtwork = 0;
    FableBitmapInfo g_BootSpookyArtworkInfo = {};
    FableBitmap g_BootSpookySunbeamArtwork = 0;
    FableBitmapInfo g_BootSpookySunbeamArtworkInfo = {};
    FableBitmap g_BootTitleSegmentArtwork = 0;
    FableBitmapInfo g_BootTitleSegmentArtworkInfo = {};
    FableBitmap g_BootButtonLeftArtwork = 0;
    FableBitmapInfo g_BootButtonLeftArtworkInfo = {};
    FableBitmap g_BootButtonMiddleArtwork = 0;
    FableBitmapInfo g_BootButtonMiddleArtworkInfo = {};
    FableBitmap g_BootButtonRightArtwork = 0;
    FableBitmapInfo g_BootButtonRightArtworkInfo = {};
    bool g_RetailProgressDisplayPresent = false;
    bool g_RetailProgressDisplayActive = false;
    FableWindow g_RetailVideoWindow = 0;
    FableInstanceHandle g_RetailVideoInstance = 0;
    unsigned int g_RetailBootMovieIndex = 0;
    bool g_RetailBootSequenceActive = false;
    bool g_RetailVideoReachedRunningState = false;
    bool g_RetailVideoEscapePressed = false;
    bool g_VisualFrontendVisible = true;
    fable_u32 g_VisualControllerState = 0;
    FableUiControllerRepeatState g_VisualControllerRepeatState = {};
    bool g_VisualMainMenuActive = false;
    unsigned int g_VisualMainMenuSelection = 0;
    bool g_VisualOptionsMenuActive = false;
    unsigned int g_VisualOptionsSelection = 0;
    bool g_VisualOptionsBackHovered = false;
    bool g_VisualSaveMenuActive = false;
    unsigned int g_VisualSaveSelection = 0;
    bool g_VisualAboutMenuActive = false;
    bool g_VisualCreditsMenuActive = false;
    bool g_VisualProfilesMenuActive = false;
    unsigned int g_VisualProfilesMode = FableFrontendProfilesNormal;
    FableUiProfileName g_VisualProfileNames[32] = {};
    const char* g_VisualProfileNamePointers[32] = {};
    unsigned int g_VisualProfileNameCount = 0;
    unsigned int g_VisualProfileSelection = 0;
    char g_VisualProfileEditText[128] = {};
    unsigned int g_VisualProfileEditLength = 0;
    unsigned int g_VisualDetailScreen = 0;
    unsigned int g_VisualDetailSelection = 0;
    // Detail input uses the same decoded records as the render bridge.
    const fable_detail_tables::DetailScreenDefinition (&kVisualDetailScreens)[3] =
        fable_detail_tables::kScreens;
    const unsigned int kVisualRedefineExpandedRowCount = 44;
    const unsigned int (&kVisualRedefineDefaults)[44] =
        fable_detail_tables::kRedefineExpandedDefaults;
    const unsigned int kVisualRedefineUnresolved =
        fable_detail_tables::kRedefineUnresolved;
    const unsigned int kVisualRedefineArrowDefaults[44] = {
        41, 42, 43, 44, 1, 2, 3, 3, 3,
        4, 5, 6, 35, 55, 36, 36, 23, 15,
        21, 26, 16, 39, 56, 57, 58, 59, 60, 61,
        62, 63, 64, 65, 66, 67, 46, 47, 48, 49,
        50, 51, 52, 53, 54, 68
    };
    const unsigned int kVisualRedefineActionIds[44] = {
        60, 60, 60, 60, 9, 7, 8, 31, 45,
        6, 13, 14, 1, 32, 26, 86, 94, 78, 4, 113, 112,
        72, 56, 56, 56, 92, 90, 96, 91, 97, 93, 98, 99,
        100, 55, 55, 55, 55, 55, 55, 55, 55, 55, 53
    };
    unsigned int g_VisualDetailValues[3][10] = {};
    unsigned int g_VisualDetailEntryValues[3][10] = {};
    unsigned int g_VisualDetailSavedValues[3][10] = {};
    unsigned int g_VisualRedefineHover = 0;
    unsigned int g_VisualRedefineResetHover = 0;
    // Detail-screen (Gameplay/Video/Audio) footer + arrow hover mirrors.
    // g_VisualDetailButtonHover: 0=none, 1=Cancel, 2=Defaults, 3=Apply.
    // g_VisualDetailArrowHoverSide: 0=none, 1=left arrow, 2=right arrow;
    // g_VisualDetailArrowHoverRow: the value row under the hovered arrow.
    unsigned int g_VisualDetailButtonHover = 0;
    unsigned int g_VisualDetailArrowHoverRow = 0;
    unsigned int g_VisualDetailArrowHoverSide = 0;
    unsigned int g_VisualRedefineSelection = 0;
    // CRedefinerList owns a 31-entry authored ActionOrder while only nine
    // rows are visible at once.  Keep the logical child separate from the
    // active key-capture row (which is still the 1..9 UI selection above).
    unsigned int g_VisualRedefineListSelection = 0;
    unsigned int g_VisualRedefineKeys[44] = {};
    unsigned int g_VisualRedefineEntryKeys[44] = {};
    unsigned int g_VisualRedefineSavedKeys[44] = {};
    unsigned int g_VisualRedefineCaptureRow = 0;
    bool g_VisualQuitPromptActive = false;
    unsigned int g_VisualQuitHover = 0;
    unsigned long g_FrontendPostMovieStepMask = 0;

    void ResolveVisualGeneratedChildOrigin(
        float parentX,
        float parentY,
        float tableX,
        float tableY,
        float childX,
        float childY,
        FableUiVector2* origin)
    {
        FableUiResolvedTransform parent = {};
        parent.position.x = parentX;
        parent.position.y = parentY;
        parent.truePosition = parent.position;
        parent.scale.x = 1.0f;
        parent.scale.y = 1.0f;
        parent.trueScale = parent.scale;

        FableUiVector2 tablePosition = {tableX, tableY};
        FableUiVector2 tableScale = {1.0f, 1.0f};
        FableUiResolvedTransform table = {};
        FableResolveUiComponentTransform(
            &tablePosition,
            &tableScale,
            &parent,
            &table);

        FableUiVector2 childPosition = {childX, childY};
        FableUiVector2 uiScale = {1.0f, 1.0f};
        FableUiVector2 trueOrigin = {};
        FableResolveUiGeneratedChildOrigins(
            &childPosition,
            &uiScale,
            &table,
            origin,
            &trueOrigin);
    }

    unsigned int VisualProfileItemCount()
    {
        if (g_VisualProfilesMode == FableFrontendProfilesNormal)
            return g_VisualProfileNameCount + 1;
        if (g_VisualProfilesMode == FableFrontendProfilesDelete)
            return g_VisualProfileNameCount;
        return 0;
    }

    int VisualProfileItemY(unsigned int item)
    {
        if (g_VisualProfilesMode == FableFrontendProfilesNormal)
        {
            if (item == 0)
                return 120;
            return 170 + static_cast<int>(
                (item - 1) * FableFrontendProfileSpacing);
        }
        return 180 + static_cast<int>(
            item * FableFrontendProfileSpacing);
    }

    int VisualProfileVisibleItemY(
        unsigned int item,
        unsigned int firstRow)
    {
        const int viewportTop =
            g_VisualProfilesMode == FableFrontendProfilesDelete
                ? 180
                : 120;
        return viewportTop +
            VisualProfileItemY(item) - VisualProfileItemY(firstRow);
    }

    bool FindVisualProfileRow(
        int mouseX,
        int mouseY,
        unsigned int* rowFound)
    {
        const unsigned int count = VisualProfileItemCount();
        const unsigned int visibleRows =
            g_VisualProfilesMode == FableFrontendProfilesDelete ? 7 : 9;
        unsigned int firstRow = 0;
        if (g_VisualProfileSelection >= visibleRows)
            firstRow = g_VisualProfileSelection - visibleRows + 1;
        for (
            unsigned int displayRow = 0;
            displayRow != visibleRows;
            ++displayRow)
        {
            const unsigned int row = firstRow + displayRow;
            if (row >= count)
                break;
            const int rowTop =
                VisualProfileVisibleItemY(row, firstRow) - 7;
            if (
                mouseX >= 180 &&
                mouseX < 460 &&
                mouseY >= rowTop &&
                mouseY < rowTop + 30)
            {
                *rowFound = row;
                return true;
            }
        }
        return false;
    }

    bool FindVisualMainMenuRow(
        int mouseX,
        int mouseY,
        unsigned int* rowFound)
    {
        static const int rowOffsets[FableFrontendMainMenuVisibleRows] = {
            0, 30, 60, 180, 210, 240
        };
        for (
            unsigned int row = 0;
            row != FableFrontendMainMenuVisibleRows;
            ++row)
        {
            // frontend.bin roots the list at (200,200). UI_BUTTON_BIG and
            // UI_BUTTON tables are (-140,-7) and (-80,-7); the horizontal
            // CTable builder places the left generated sprite at local x=60.
            FableUiVector2 origin = {};
            ResolveVisualGeneratedChildOrigin(
                200.0f,
                200.0f,
                row == 0 ? -140.0f : -80.0f,
                -7.0f + static_cast<float>(rowOffsets[row]),
                60.0f,
                0.0f,
                &origin);
            const int rowLeft = static_cast<int>(origin.x);
            const int rowRight = rowLeft + (row == 0 ? 400 : 280);
            const int rowTop = static_cast<int>(origin.y);
            if (
                mouseX >= rowLeft &&
                mouseX < rowRight &&
                mouseY >= rowTop &&
                mouseY < rowTop + 30)
            {
                *rowFound = row;
                return true;
            }
        }
        return false;
    }

    bool FindVisualOptionsMenuRow(
        int mouseX,
        int mouseY,
        unsigned int* rowFound)
    {
        for (unsigned int row = 0; row != 4; ++row)
        {
            FableUiVector2 origin = {};
            ResolveVisualGeneratedChildOrigin(
                200.0f,
                150.0f,
                -80.0f,
                -7.0f + static_cast<float>(row * 30),
                60.0f,
                0.0f,
                &origin);
            const int rowLeft = static_cast<int>(origin.x);
            const int rowTop = static_cast<int>(origin.y);
            if (
                mouseX >= rowLeft &&
                mouseX < rowLeft + 280 &&
                mouseY >= rowTop &&
                mouseY < rowTop + 30)
            {
                *rowFound = row;
                return true;
            }
        }
        return false;
    }

    bool FindVisualSaveMenuRow(
        int mouseX,
        int mouseY,
        unsigned int* rowFound)
    {
        for (unsigned int row = 0; row != 4; ++row)
        {
            const int rowTop = 90 + static_cast<int>(row * 30);
            if (
                mouseX >= 10 &&
                mouseX < 250 &&
                mouseY >= rowTop &&
                mouseY < rowTop + 30)
            {
                *rowFound = row;
                return true;
            }
        }
        return false;
    }

    unsigned int VisualRedefineVisibleRowCount();

    bool FindVisualRedefineRow(
        int mouseX,
        int mouseY,
        unsigned int* rowFound)
    {
        const fable_detail_tables::RedefineListDefinition& list =
            fable_detail_tables::kRedefineList;
        const unsigned int visibleRows = VisualRedefineVisibleRowCount();
        for (unsigned int row = 0; row != visibleRows; ++row)
        {
            FableUiVector2 origin = {};
            ResolveVisualGeneratedChildOrigin(
                static_cast<float>(list.rootX),
                static_cast<float>(list.rootY),
                0.0f,
                static_cast<float>(row * list.rowStep),
                static_cast<float>(list.hitOffsetX),
                static_cast<float>(list.hitOffsetY),
                &origin);
            const int rowLeft = static_cast<int>(origin.x);
            const int rowTop = static_cast<int>(origin.y);
            if (
                mouseX >= rowLeft &&
                mouseX < rowLeft + list.hitWidth &&
                mouseY >= rowTop &&
                mouseY < rowTop + list.hitHeight)
            {
                *rowFound = row;
                return true;
            }
        }
        return false;
    }

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

    // Install-time-extraction asset model (recomp-style, e.g. Sonic Unleashed
    // Recompiled / the Zelda decomps): frontend artwork is extracted from the
    // player's own base-game .big files into loose files alongside the exe, and
    // loaded from disk at runtime.  This lets a finished install ship
    // asset-free and load only from the base game the player supplied.
    //
    // Prefer the loose extracted .bmp (LR_LOADFROMFILE); fall back to the
    // embedded resource so a build without an extraction step still runs.  We
    // read the raw DIB bytes downstream, so a 32-bit BGRA .bmp preserves alpha
    // exactly through the same GetObjectA/DIB path the embedded resources use.
    FableBitmap LoadFrontendArtwork(
        FableInstanceHandle instance,
        int resourceId,
        const char* looseFileName)
    {
        if (looseFileName != 0)
        {
            char path[600];
            const FableDword written =
                GetModuleFileNameA(0, path, 512);
            if (written > 0 && written < 512)
            {
                int cut = static_cast<int>(written);
                while (cut > 0 &&
                       path[cut - 1] != '\\' &&
                       path[cut - 1] != '/')
                {
                    --cut;
                }
                path[cut] = '\0';
                strcat(path, "data\\frontend\\");
                strcat(path, looseFileName);
                FableBitmap loose = static_cast<FableBitmap>(LoadImageA(
                    0,
                    path,
                    kImageBitmap,
                    0,
                    0,
                    kLoadFromFile | kLoadCreatedDibSection));
                if (loose != 0)
                {
                    return loose;
                }
            }
        }
        return static_cast<FableBitmap>(LoadImageA(
            instance,
            IntegerResource(resourceId),
            kImageBitmap,
            0,
            0,
            kLoadCreatedDibSection));
    }

    void PlayVisualFrontendResourceSound(int resource)
    {
#if defined(FABLETLC_RETAIL_FRONTEND_SOUNDS)
        PlaySoundA(
            IntegerResource(resource),
            g_RetailVideoInstance,
            kSoundAsync | kSoundNoDefault | kSoundResource);
#else
        (void)resource;
#endif
    }

    void PlayVisualFrontendListSound(unsigned long request)
    {
        if (request == FableUiFrontEndSoundUpDown)
            PlayVisualFrontendResourceSound(kBootSoundUpDownResource);
        else if (request == FableUiFrontEndSoundError)
            PlayVisualFrontendResourceSound(kBootSoundErrorResource);
    }

    bool RefreshVisualProfileNames()
    {
        wchar_t userProfile[260] = {};
        if (
            GetEnvironmentVariableW(
                L"USERPROFILE",
                userProfile,
                sizeof(userProfile) / sizeof(userProfile[0])) == 0)
        {
            return false;
        }
        wchar_t documents[520];
        JoinSavePath(
            documents,
            sizeof(documents) / sizeof(documents[0]),
            userProfile,
            L"Documents");
        wchar_t games[520];
        JoinSavePath(
            games,
            sizeof(games) / sizeof(games[0]),
            documents,
            L"My Games");
        wchar_t fable[520];
        JoinSavePath(
            fable,
            sizeof(fable) / sizeof(fable[0]),
            games,
            L"Fable");
        wchar_t saves[520];
        JoinSavePath(
            saves,
            sizeof(saves) / sizeof(saves[0]),
            fable,
            L"Saves");

        memset(g_VisualProfileNames, 0, sizeof(g_VisualProfileNames));
        g_VisualProfileNameCount = FableEnumerateVisualFrontendProfiles(
            saves,
            g_VisualProfileNames,
            sizeof(g_VisualProfileNames) / sizeof(g_VisualProfileNames[0]));
        if (g_VisualProfileNameCount >
            sizeof(g_VisualProfileNames) / sizeof(g_VisualProfileNames[0]))
        {
            g_VisualProfileNameCount =
                sizeof(g_VisualProfileNames) /
                sizeof(g_VisualProfileNames[0]);
        }
        for (unsigned int i = 0; i != g_VisualProfileNameCount; ++i)
            g_VisualProfileNamePointers[i] = g_VisualProfileNames[i].name;
        return g_VisualProfileNameCount != 0;
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
            g_VisualFrontendVisible &&
            g_BootArtwork != 0 &&
            g_BootArtworkInfo.width > 0 &&
            g_BootArtworkInfo.height > 0)
        {
            const int drawWidth = clientWidth;
            const int drawHeight = clientHeight;
            const int drawLeft = 0;
            const int drawTop = 0;

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
        const bool presented = g_VisualFrontendVisible
            ? FableRenderVisualD3D9(
                client.right - client.left,
                client.bottom - client.top)
            : FablePresentVisualD3D9Black();
        if (!presented)
        {
            return false;
        }

        FablePaint paint = {};
        BeginPaint(window, &paint);
        EndPaint(window, &paint);
        return true;
    }

    void RevealVisualFrontend(FableWindow window)
    {
        g_VisualFrontendVisible = true;
        if (FableIsVisualD3D9Active())
        {
            FableRectangle client = {};
            if (
                GetClientRect(window, &client) &&
                FableRenderVisualD3D9(
                    client.right - client.left,
                    client.bottom - client.top))
            {
                return;
            }
        }

        InvalidateRect(window, 0, 1);
        UpdateWindow(window);
    }

    const unsigned int kVisualRedefineListChildCount = 31;
    const unsigned int kVisualRedefineListVisibleRows = 9;
    const unsigned int kVisualRedefineMaterializedRowCount = 44;
    const unsigned int kVisualRedefineMaterializedOffsets[31] = {
        0, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
        19, 20, 21, 22, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 43
    };

    unsigned int VisualRedefineVisibleRowCount()
    {
        const unsigned int offset =
            kVisualRedefineMaterializedOffsets[
                g_VisualRedefineListSelection];
        const unsigned int remaining =
            kVisualRedefineMaterializedRowCount - offset;
        return remaining < kVisualRedefineListVisibleRows
            ? remaining
            : kVisualRedefineListVisibleRows;
    }

    unsigned int VisualRedefineMaterializedOffset()
    {
        const unsigned int offset =
            kVisualRedefineMaterializedOffsets[
                g_VisualRedefineListSelection];
        return offset > 35 ? 35 : offset;
    }

    enum VisualFrontendScrollTarget
    {
        VisualFrontendScrollNone = 0,
        VisualFrontendScrollMain = 1,
        VisualFrontendScrollOptions = 2,
        VisualFrontendScrollSaves = 3,
        VisualFrontendScrollProfiles = 4,
        VisualFrontendScrollRedefine = 5
    };

    // Retail CClickable stores left-button state between events 0x1A and
    // 0x1C. Keep this platform-bridge state separate from list selection so
    // release cannot synthesize a second scroll.
    bool g_VisualFrontendScrollArrowPressed = false;
    VisualFrontendScrollTarget g_VisualFrontendScrollArrowTarget =
        VisualFrontendScrollNone;

    // The retail ego_r path is recovered: CMouseDX::ConvertMouseEventToInputEvent
    // @ 0x00c55d20 handles mouse event type 10 by calling
    // CInputEvent::SetAsMouseWheelMovement @ 0x00b6eb00; the game input
    // process then reads it through GetMouseWheelMovement @ 0x00442d70 /
    // 0x006b2e50.  Win32 is still only the platform ingress here, but its
    // payload is passed through the same one-event, signed-delta semantics.
    int SignedMouseWord(unsigned long value)
    {
        const unsigned int word =
            static_cast<unsigned int>(value & 0xFFFFUL);
        return word >= 0x8000U
            ? static_cast<int>(word) - 0x10000
            : static_cast<int>(word);
    }

    bool GetVisualDesignMousePosition(
        FableWindow window,
        FableLongParameter longParameter,
        bool screenPosition,
        int* mouseX,
        int* mouseY)
    {
        FableRectangle client = {};
        if (!GetClientRect(window, &client))
            return false;
        const int clientWidth = client.right - client.left;
        const int clientHeight = client.bottom - client.top;
        if (clientWidth <= 0 || clientHeight <= 0)
            return false;

        FablePoint point = {};
        point.x = SignedMouseWord(
            static_cast<unsigned long>(longParameter));
        point.y = SignedMouseWord(
            static_cast<unsigned long>(longParameter) >> 16);
        if (
            screenPosition &&
            window != 0 &&
            !ScreenToClient(window, &point))
        {
            return false;
        }
        *mouseX = static_cast<int>(point.x) * 640 / clientWidth;
        *mouseY = static_cast<int>(point.y) * 480 / clientHeight;
        return true;
    }

    bool IsVisualFrontendScrollArea(
        VisualFrontendScrollTarget target,
        int mouseX,
        int mouseY)
    {
        const FableFrontendListArrowLayout* layout = 0;
        switch (target)
        {
        case VisualFrontendScrollSaves:
            layout = &kFableFrontendSaveListArrowLayout;
            break;
        case VisualFrontendScrollProfiles:
            layout = g_VisualProfilesMode == FableFrontendProfilesDelete
                ? &kFableFrontendDeleteListArrowLayout
                : &kFableFrontendProfilesListArrowLayout;
            break;
        case VisualFrontendScrollRedefine:
            layout = &kFableFrontendRedefineListArrowLayout;
            break;
        default:
            return false;
        }

        const int top = layout->listY < layout->upY
            ? layout->listY
            : layout->upY;
        const int listBottom = layout->listY + layout->listHeight;
        const int arrowBottom = layout->downY + layout->arrowHeight;
        const int bottom = listBottom > arrowBottom
            ? listBottom
            : arrowBottom;
        return mouseX >= layout->listX &&
            mouseX < layout->arrowX + layout->arrowWidth &&
            mouseY >= top && mouseY < bottom;
    }

    bool FindVisualFrontendScrollArrow(
        VisualFrontendScrollTarget target,
        int mouseX,
        int mouseY,
        bool* scrollDown)
    {
        const FableFrontendListArrowLayout* layout = 0;
        switch (target)
        {
        case VisualFrontendScrollSaves:
            layout = &kFableFrontendSaveListArrowLayout;
            break;
        case VisualFrontendScrollProfiles:
            layout = g_VisualProfilesMode == FableFrontendProfilesDelete
                ? &kFableFrontendDeleteListArrowLayout
                : &kFableFrontendProfilesListArrowLayout;
            break;
        case VisualFrontendScrollRedefine:
            layout = &kFableFrontendRedefineListArrowLayout;
            break;
        default:
            return false;
        }
        if (
            mouseX < layout->arrowX ||
            mouseX >= layout->arrowX + layout->arrowWidth)
        {
            return false;
        }
        if (
            mouseY >= layout->upY &&
            mouseY < layout->upY + layout->arrowHeight)
        {
            *scrollDown = false;
            return true;
        }
        if (
            mouseY >= layout->downY &&
            mouseY < layout->downY + layout->arrowHeight)
        {
            *scrollDown = true;
            return true;
        }
        return false;
    }

    VisualFrontendScrollTarget ActiveVisualFrontendScrollTarget()
    {
        // The authored main/options lists have no UpArrow/DownArrow bindings
        // and serialize Scrolling=false. Mouse-wheel/arrow ingress is only
        // exposed for the arrow-bearing lists below; keyboard/controller
        // navigation keeps its separate authored paths.
        if (g_VisualSaveMenuActive)
            return VisualFrontendScrollSaves;
        if (g_VisualProfilesMenuActive)
        {
            return g_VisualProfilesMode == FableFrontendProfilesNormal ||
                   g_VisualProfilesMode == FableFrontendProfilesDelete
                ? VisualFrontendScrollProfiles
                : VisualFrontendScrollNone;
        }
        if (g_VisualDetailScreen == 4)
            return VisualFrontendScrollRedefine;
        return VisualFrontendScrollNone;
    }

    bool ScrollVisualFrontendList(
        FableWindow window,
        VisualFrontendScrollTarget target,
        bool scrollDown);

    bool DispatchVisualFrontendScrollArrowPress(
        FableWindow window,
        FableLongParameter longParameter,
        bool screenPosition)
    {
        if (
            !g_VisualFrontendVisible ||
            FableIsRetailVideoActive())
        {
            return false;
        }
        const VisualFrontendScrollTarget target =
            ActiveVisualFrontendScrollTarget();
        if (target == VisualFrontendScrollNone)
            return false;
        int mouseX = 0;
        int mouseY = 0;
        if (
            !GetVisualDesignMousePosition(
                window,
                longParameter,
                screenPosition,
                &mouseX,
                &mouseY))
        {
            return false;
        }
        bool scrollDown = false;
        if (!FindVisualFrontendScrollArrow(
                target,
                mouseX,
                mouseY,
                &scrollDown))
        {
            return false;
        }

        // Retail CClickable::ProcessEvent @ 0x0055ad60 dispatches event
        // 0x1a on button press; the list arrow therefore scrolls on the
        // down edge, not on the later unclicked/release edge.
        if (!FableRetailFrontendClickableProcessEvent(
                FableRetailFrontendClickableLeftClicked,
                true,
                &g_VisualFrontendScrollArrowPressed))
        {
            return false;
        }
        g_VisualFrontendScrollArrowTarget = target;
        ScrollVisualFrontendList(window, target, scrollDown);
        return true;
    }

    bool DispatchVisualFrontendScrollArrowRelease()
    {
        if (g_VisualFrontendScrollArrowTarget == VisualFrontendScrollNone)
            return false;
        if (!FableRetailFrontendClickableProcessEvent(
                FableRetailFrontendClickableLeftUnclicked,
                true,
                &g_VisualFrontendScrollArrowPressed))
        {
            return false;
        }
        // The retail unclicked callback releases the component state; the
        // arrow's scroll callback already ran on event 0x1A.
        g_VisualFrontendScrollArrowTarget = VisualFrontendScrollNone;
        return true;
    }

    bool ScrollVisualFrontendList(
        FableWindow window,
        VisualFrontendScrollTarget target,
        bool scrollDown)
    {
        if (target == VisualFrontendScrollMain)
        {
            fable_u32 selected = g_VisualMainMenuSelection;
            fable_u32 soundRequest = FableUiFrontEndSoundNone;
            const bool moved = FableScrollVisualFrontendMainMenu(
                scrollDown,
                &selected,
                &soundRequest);
            PlayVisualFrontendListSound(soundRequest);
            if (moved)
            {
                g_VisualMainMenuSelection = selected;
                RevealVisualFrontend(window);
            }
            return moved;
        }

        FableUiFrontEndListScrollPlan plan = {};
        unsigned int childCount = 0;
        long selectedChild = 0;
        bool stopsAtEnds = false;
        switch (target)
        {
        case VisualFrontendScrollOptions:
            childCount = 4;
            selectedChild = static_cast<long>(g_VisualOptionsSelection);
            break;
        case VisualFrontendScrollSaves:
            childCount = 4;
            selectedChild = static_cast<long>(g_VisualSaveSelection);
            break;
        case VisualFrontendScrollProfiles:
            childCount = VisualProfileItemCount();
            selectedChild = static_cast<long>(g_VisualProfileSelection);
            // Both authored profile Type-43 lists are Scrolling=true and
            // Wrapping=false.  Preserve the retail end error sound.
            stopsAtEnds = true;
            break;
        case VisualFrontendScrollRedefine:
            if (g_VisualRedefineSelection != 0)
                return false;
            childCount = kVisualRedefineListChildCount;
            selectedChild = static_cast<long>(
                g_VisualRedefineListSelection);
            stopsAtEnds = true;
            break;
        default:
            return false;
        }

        FablePlanUiFrontEndListProcessEvent(
            scrollDown
                ? FableUiFrontEndListEventDown
                : FableUiFrontEndListEventUp,
            childCount,
            selectedChild,
            stopsAtEnds,
            0,
            0,
            0,
            &plan);
        PlayVisualFrontendListSound(plan.soundRequest);
        if (!plan.moved)
            return false;

        switch (target)
        {
        case VisualFrontendScrollOptions:
            g_VisualOptionsSelection = static_cast<unsigned int>(
                plan.selectedChild);
            FableSetVisualFrontendOptionsSelection(
                g_VisualOptionsSelection);
            break;
        case VisualFrontendScrollSaves:
            g_VisualSaveSelection = static_cast<unsigned int>(
                plan.selectedChild);
            FableSetVisualFrontendSaveSelection(
                g_VisualSaveSelection);
            break;
        case VisualFrontendScrollProfiles:
            g_VisualProfileSelection = static_cast<unsigned int>(
                plan.selectedChild);
            FableSetVisualFrontendProfilesSelection(
                g_VisualProfileSelection);
            break;
        case VisualFrontendScrollRedefine:
            g_VisualRedefineListSelection = static_cast<unsigned int>(
                plan.selectedChild);
            FableSetVisualFrontendRedefineListSelection(
                g_VisualRedefineListSelection);
            break;
        default:
            break;
        }
        RevealVisualFrontend(window);
        return true;
    }

    void CopyVisualDetailValues(
        unsigned int destination[3][10],
        const unsigned int source[3][10])
    {
        memcpy(destination, source, sizeof(g_VisualDetailValues));
    }

    void LoadVisualDetailDefaults(unsigned int destination[3][10])
    {
        memset(destination, 0, sizeof(g_VisualDetailValues));
        for (unsigned int screen = 0; screen != 3; ++screen)
        {
            for (
                unsigned int row = 0;
                row != kVisualDetailScreens[screen].rowCount;
                ++row)
            {
                destination[screen][row] =
                    kVisualDetailScreens[screen].rows[row].defaultValue;
            }
        }
    }

    void CopyVisualRedefineKeys(
        unsigned int destination[44],
        const unsigned int source[44])
    {
        memcpy(destination, source, sizeof(g_VisualRedefineKeys));
    }

    void SyncVisualRedefineKeys()
    {
        for (unsigned int row = 0; row != kVisualRedefineExpandedRowCount; ++row)
        {
            FableSetVisualFrontendRedefineKey(
                row,
                g_VisualRedefineKeys[row]);
        }
        FableSetVisualFrontendRedefineSelection(
            g_VisualRedefineSelection);
        FableSetVisualFrontendRedefineListSelection(
            g_VisualRedefineListSelection);
        bool changed = false;
        for (unsigned int row = 0;
             row != kVisualRedefineExpandedRowCount;
             ++row)
        {
            if (
                g_VisualRedefineKeys[row] !=
                g_VisualRedefineEntryKeys[row])
            {
                changed = true;
                break;
            }
        }
        FableSetVisualFrontendDetailApplyEnabled(changed);
    }

    void ResetVisualRedefineKeys(bool wasd)
    {
        // UI_RESET_WASD action 311 dispatches
        // CUserProfileManager::ResetAssignedInputsWASD @ 0x00408820.
        // UI_RESET action 284 dispatches ResetAssignedInputs @ 0x004085F0,
        // whose shipped GDD scheme uses the four arrow-key controls.
        CopyVisualRedefineKeys(
            g_VisualRedefineKeys,
            wasd
                ? kVisualRedefineDefaults
                : kVisualRedefineArrowDefaults);
        g_VisualRedefineSelection = 0;
        g_VisualRedefineCaptureRow = 0;
        g_VisualRedefineListSelection = 0;
        SyncVisualRedefineKeys();
    }

    void SyncVisualDetailScreen(unsigned int screen)
    {
        if (screen < 1 || screen > 3)
            return;
        const unsigned int screenIndex = screen - 1;
        for (
            unsigned int row = 0;
            row != kVisualDetailScreens[screenIndex].rowCount;
            ++row)
        {
            FableSetVisualFrontendDetailOptionValue(
                screen,
                row,
                g_VisualDetailValues[screenIndex][row]);
        }
        bool changed = false;
        for (
            unsigned int row = 0;
            row != kVisualDetailScreens[screenIndex].rowCount;
            ++row)
        {
            if (
                g_VisualDetailValues[screenIndex][row] !=
                g_VisualDetailEntryValues[screenIndex][row])
            {
                changed = true;
                break;
            }
        }
        FableSetVisualFrontendDetailApplyEnabled(changed);
    }

    void BeginVisualDetailScreen(unsigned int screen)
    {
        g_VisualDetailSelection = 0;
        if (screen >= 1 && screen <= 3)
        {
            CopyVisualDetailValues(
                g_VisualDetailValues,
                g_VisualDetailSavedValues);
            CopyVisualDetailValues(
                g_VisualDetailEntryValues,
                g_VisualDetailValues);
            SyncVisualDetailScreen(screen);
        }
        else if (screen == 4)
        {
            CopyVisualRedefineKeys(
                g_VisualRedefineKeys,
                g_VisualRedefineSavedKeys);
            CopyVisualRedefineKeys(
                g_VisualRedefineEntryKeys,
                g_VisualRedefineKeys);
            g_VisualRedefineSelection = 0;
            g_VisualRedefineCaptureRow = 0;
            g_VisualRedefineListSelection = 0;
            SyncVisualRedefineKeys();
        }
    }

    void CancelVisualDetailScreen()
    {
        if (g_VisualDetailScreen >= 1 && g_VisualDetailScreen <= 3)
        {
            CopyVisualDetailValues(
                g_VisualDetailValues,
                g_VisualDetailEntryValues);
            SyncVisualDetailScreen(g_VisualDetailScreen);
        }
        else if (g_VisualDetailScreen == 4)
        {
            CopyVisualRedefineKeys(
                g_VisualRedefineKeys,
                g_VisualRedefineEntryKeys);
            g_VisualRedefineSelection = 0;
            g_VisualRedefineCaptureRow = 0;
            g_VisualRedefineListSelection = 0;
            SyncVisualRedefineKeys();
        }
    }

    void ApplyVisualDetailScreen()
    {
        if (g_VisualDetailScreen >= 1 && g_VisualDetailScreen <= 3)
        {
            CopyVisualDetailValues(
                g_VisualDetailSavedValues,
                g_VisualDetailValues);
        }
        else if (g_VisualDetailScreen == 4)
        {
            CopyVisualRedefineKeys(
                g_VisualRedefineSavedKeys,
                g_VisualRedefineKeys);
            g_VisualRedefineSelection = 0;
            g_VisualRedefineCaptureRow = 0;
            g_VisualRedefineListSelection = 0;
            FableSetVisualFrontendRedefineSelection(0);
        }
    }

    bool VisualPointInDetailRect(
        int mouseX,
        int mouseY,
        const fable_detail_tables::DetailRect& rect)
    {
        return mouseX >= rect.left && mouseX < rect.right &&
            mouseY >= rect.top && mouseY < rect.bottom;
    }

    bool VisualPointInDetailFooter(
        unsigned int button,
        int mouseX,
        int mouseY)
    {
        if (button < 1 || button > 3)
            return false;
        return VisualPointInDetailRect(
            mouseX,
            mouseY,
            fable_detail_tables::kFooter[button - 1].hit);
    }

    bool VisualPointInRedefineReset(
        unsigned int column,
        int mouseX,
        int mouseY)
    {
        if (column >= 2)
            return false;
        return VisualPointInDetailRect(
            mouseX,
            mouseY,
            fable_detail_tables::kRedefineReset[column].hit);
    }

    bool VisualIsRedefinableKeyValue(unsigned int keyValue)
    {
        // CKeyRedefiner::IsRedefinableKey @ 0x022B3C70 evaluates the
        // DirectInput scan-code enum. Keep the retail exclusions exact even
        // though the current routed Win32 subset does not synthesize all of
        // these opaque scan-code records.
        switch (keyValue)
        {
        case 0x01:
        case 0x59:
        case 0x5A:
        case 0x5B:
        case 0x62:
        case 0x76:
        case 0x77:
            return false;
        default:
            return true;
        }
    }

    unsigned int VisualVirtualKeyToRedefineValue(
        FableWordParameter virtualKey)
    {
        unsigned int keyValue = 0xFFFFFFFFu;
        if (virtualKey >= 'A' && virtualKey <= 'Z')
        {
            if (virtualKey == 'Q')
                keyValue = 5;
            else if (virtualKey == 'E')
                keyValue = 6;
            else
                keyValue = 9 + static_cast<unsigned int>(virtualKey - 'A');
        }
        else if (virtualKey >= '0' && virtualKey <= '9')
            keyValue = 45 + static_cast<unsigned int>(virtualKey - '0');
        else switch(virtualKey)
        {
        case 0x09:
            keyValue = 4;
            break;
        case 0x20:
            keyValue = 35;
            break;
        case 0x10:
            keyValue = 36;
            break;
        case 0x11:
            keyValue = 37;
            break;
        case 0x12:
            keyValue = 38;
            break;
        case 0x0D:
            keyValue = 39;
            break;
        case 0x08:
            keyValue = 40;
            break;
        case 0x26:
            keyValue = 41;
            break;
        case 0x28:
            keyValue = 42;
            break;
        case 0x25:
            keyValue = 43;
            break;
        case 0x27:
            keyValue = 44;
            break;
        }
        if (!VisualIsRedefinableKeyValue(keyValue))
            return 0xFFFFFFFFu;
        return keyValue;
    }

    bool VisualRedefineRowsMayShare(
        unsigned int first,
        unsigned int second)
    {
        if (
            first >= kVisualRedefineExpandedRowCount ||
            second >= kVisualRedefineExpandedRowCount)
        {
            return false;
        }
        // Retail AreAllowedToCoexist @ 0x005578A0 has two exact groups:
        // {8,31,45} and {26,86}.
        const unsigned int firstAction = kVisualRedefineActionIds[first];
        const unsigned int secondAction = kVisualRedefineActionIds[second];
        const bool firstGroup =
            (firstAction == 8 || firstAction == 31 || firstAction == 45) &&
            (secondAction == 8 || secondAction == 31 || secondAction == 45);
        const bool secondGroup =
            (firstAction == 26 || firstAction == 86) &&
            (secondAction == 26 || secondAction == 86);
        return firstGroup || secondGroup;
    }

    bool ApplyVisualRedefinition(unsigned int keyValue)
    {
        if (
            g_VisualDetailScreen != 4 ||
            g_VisualRedefineSelection == 0 ||
            g_VisualRedefineCaptureRow >=
                kVisualRedefineExpandedRowCount ||
            keyValue >= 69 ||
            keyValue == 7)
        {
            return false;
        }
        const unsigned int selected = g_VisualRedefineCaptureRow;
        for (unsigned int row = 0;
             row != kVisualRedefineExpandedRowCount;
             ++row)
        {
            if (
                row != selected &&
                g_VisualRedefineKeys[row] == keyValue &&
                !VisualRedefineRowsMayShare(row, selected))
            {
                // CRedefinerList::NotifyOfRedefinition invokes
                // ClearDuplicateDefinitions on incompatible aliases.
                g_VisualRedefineKeys[row] = 8;
                FableSetVisualFrontendRedefineKey(row, 8);
            }
        }
        g_VisualRedefineKeys[selected] = keyValue;
        FableSetVisualFrontendRedefineKey(selected, keyValue);
        g_VisualRedefineSelection = 0;
        g_VisualRedefineCaptureRow = 0;
        FableSetVisualFrontendRedefineSelection(0);
        FableSetVisualFrontendDetailApplyEnabled(true);
        return true;
    }

    bool AdjustVisualDetailControl(
        unsigned int screen,
        int mouseX,
        int mouseY)
    {
        if (screen < 1 || screen > 3)
            return false;
        const unsigned int screenIndex = screen - 1;
        for (
            unsigned int row = 0;
            row != kVisualDetailScreens[screenIndex].rowCount;
        ++row)
        {
            const int rowTop =
                kVisualDetailScreens[screenIndex].rows[row].y +
                fable_detail_tables::kArrow.controlTopOffset;
            if (mouseY < rowTop || mouseY >= rowTop + 30)
                continue;
            unsigned int& value =
                g_VisualDetailValues[screenIndex][row];
            const unsigned int valueCount =
                kVisualDetailScreens[screenIndex].rows[row].valueCount;
            const unsigned int previousValue = value;
            if (
                mouseX >= fable_detail_tables::kArrow.leftHit.left &&
                mouseX < fable_detail_tables::kArrow.leftHit.right)
            {
                if (value != 0)
                    --value;
            }
            else if (
                mouseX >= fable_detail_tables::kArrow.rightHit.left &&
                mouseX < fable_detail_tables::kArrow.rightHit.right)
            {
                // The retail slider's right action advances through the
                // value list cyclically.  Keep mouse arrows consistent with
                // the recovered keyboard/controller path below, which uses
                // the same modulo transition.
                if (valueCount > 1)
                    value = (value + 1) % valueCount;
            }
            else if (
                mouseX >= fable_detail_tables::kArrow.leftHit.right &&
                mouseX < fable_detail_tables::kArrow.rightHit.left &&
                valueCount > 1)
            {
                value = static_cast<unsigned int>(
                    (mouseX - 440) * (valueCount - 1) / 121);
            }
            else
            {
                return false;
            }
            if (value == previousValue)
                return false;
            FableSetVisualFrontendDetailOptionValue(
                screen,
                row,
                value);
            return true;
        }
        return false;
    }

    bool ActivateVisualMainMenuSelection(FableWindow window)
    {
        if (!g_VisualMainMenuActive)
            return false;
        const fable_u32 action =
            FableGetVisualFrontendMainMenuAction(
                g_VisualMainMenuSelection);
        if (action == 66)
        {
            // RefreshAvailableSavedGamesForProfile emits the autosave first,
            // then ascending manual slots, before used key 0x08 activates
            // UI_FRONTEND_PROFILE_SAVED_GAMES_MENU.
            g_VisualMainMenuActive = false;
            g_VisualSaveMenuActive = true;
            g_VisualSaveSelection = 0;
            g_VisualOptionsBackHovered = false;
            FableSetVisualFrontendSaveMenu(true);
            PlayVisualFrontendResourceSound(kBootSoundForwardResource);
            RevealVisualFrontend(window);
            return true;
        }
        if (action == 16)
        {
            // GotoProfileMenu refreshes the runtime profile range before
            // entering UI_FRONTEND_LIST_FOR_PROFILES. Keep the names sourced
            // from the user's Saves directory and let the D3D9 row renderer
            // consume that range.
            RefreshVisualProfileNames();
            g_VisualMainMenuActive = false;
            g_VisualProfilesMenuActive = true;
            g_VisualProfilesMode =
                g_VisualProfileNameCount == 0
                    ? FableFrontendProfilesEmpty
                    : FableFrontendProfilesNormal;
            g_VisualProfileSelection = 0;
            g_VisualOptionsBackHovered = false;
            FableSetVisualFrontendProfilesMenu(
                true,
                g_VisualProfileNamePointers,
                g_VisualProfileNameCount);
            FableSetVisualFrontendProfilesMode(g_VisualProfilesMode);
            PlayVisualFrontendResourceSound(kBootSoundForwardResource);
            RevealVisualFrontend(window);
            return true;
        }
        if (action == 297)
        {
            // Action 297 -> used-key 0x18 ->
            // UI_FRONTEND_OPTIONS_SUB_MENU.
            g_VisualMainMenuActive = false;
            g_VisualOptionsMenuActive = true;
            g_VisualOptionsSelection = 0;
            g_VisualOptionsBackHovered = false;
            FableSetVisualFrontendOptionsMenu(true);
            PlayVisualFrontendResourceSound(kBootSoundForwardResource);
            RevealVisualFrontend(window);
            return true;
        }
        if (action == 314)
        {
            // Action 314 -> used-key 0x1a ->
            // UI_FRONTEND_QUIT_PROMPT.
            g_VisualMainMenuActive = false;
            g_VisualQuitPromptActive = true;
            g_VisualQuitHover = 0;
            FableSetVisualFrontendQuitPrompt(true);
            PlayVisualFrontendResourceSound(kBootSoundForwardResource);
            RevealVisualFrontend(window);
            return true;
        }
        if (action == 10)
        {
            // Games for Windows - LIVE.  Retail
            // CFrontEndManager::Action @ 0x0059A238 has NO case for action
            // 0xa: the switch falls straight to the default `return`, so the
            // row is dispatched but leaves the main menu unchanged with no
            // forward sound.  Consume the activation here so the input is
            // treated as handled (retail-faithful no-op) instead of falling
            // through to the press-start/options activators.
            return true;
        }
        if (action == 321)
        {
            // Action 321 (0x141) -> used key 0x1c ->
            // GotoNextScreen -> UI_FRONTEND_ABOUT_MENU (#449).
            g_VisualMainMenuActive = false;
            g_VisualAboutMenuActive = true;
            g_VisualOptionsBackHovered = false;
            FableSetVisualFrontendAboutMenu(true);
            PlayVisualFrontendResourceSound(kBootSoundForwardResource);
            RevealVisualFrontend(window);
            return true;
        }
        if (action == 67)
        {
            // Action 67 (0x43) -> UI_FRONTEND_CREDITS_MENU.  Its compiled
            // scrolling container starts at y=480; the D3D9 route presents
            // that authored initial frame and leaves Back as action 86.
            g_VisualMainMenuActive = false;
            g_VisualCreditsMenuActive = true;
            g_VisualOptionsBackHovered = false;
            FableSetVisualFrontendCreditsMenu(true);
            PlayVisualFrontendResourceSound(kBootSoundForwardResource);
            RevealVisualFrontend(window);
            return true;
        }
        return false;
    }

    bool ActivateVisualProfileSelection(FableWindow window)
    {
        if (!g_VisualProfilesMenuActive)
            return false;
        FableUiFrontendProfileActionPlan actionPlan = {};
        FablePlanVisualFrontendProfileAction(
            g_VisualProfilesMode,
            g_VisualProfileSelection,
            g_VisualProfileNameCount,
            FableUiFrontendProfileInputActivate,
            &actionPlan);
        if (g_VisualProfilesMode == FableFrontendProfilesEmpty)
        {
            // Empty-profile definition action 0x125 is the same
            // SetDefaultValuesForNewProfile -> GotoNewProfileScreen route as
            // the normal list's New Profile row.
            if (!actionPlan.dispatch ||
                actionPlan.action != FableRetailFrontendManagerActionNewProfile)
                return true;
            g_VisualProfilesMode = FableFrontendProfilesNew;
            memset(g_VisualProfileEditText, 0, sizeof(g_VisualProfileEditText));
            g_VisualProfileEditLength = 0;
            FableSetVisualFrontendProfilesMode(
                FableFrontendProfilesNew);
            PlayVisualFrontendResourceSound(kBootSoundForwardResource);
            RevealVisualFrontend(window);
            return true;
        }
        if (g_VisualProfilesMode == FableFrontendProfilesNew)
        {
            // Action 0x126 is owned by CFrontEndManager::Action and forwards
            // to CVirtualKeyboard::Confirm.  The checkpoint has no live
            // manager/keyboard object, so dispatch stops at this verified
            // action boundary; it must not fabricate a profile write.
            (void)actionPlan;
            return true;
        }
        if (g_VisualProfilesMode == FableFrontendProfilesDeleteConfirm)
        {
            // Action 0xd6 is the retail delete confirmation callback.  The
            // native manager owns DeleteProfile, list refresh, and the
            // resulting screen transition; keep this visual checkpoint at
            // that exact boundary until its live manager is connected.
            if (!actionPlan.dispatch ||
                actionPlan.action !=
                    FableRetailFrontendManagerActionDeleteConfirm)
                return true;
            return true;
        }
        if (
            g_VisualProfilesMode == FableFrontendProfilesNormal &&
            g_VisualProfileSelection == 0)
        {
            if (!actionPlan.dispatch ||
                actionPlan.action != FableRetailFrontendManagerActionNewProfile)
                return true;
            g_VisualProfilesMode = FableFrontendProfilesNew;
            memset(g_VisualProfileEditText, 0, sizeof(g_VisualProfileEditText));
            g_VisualProfileEditLength = 0;
            FableSetVisualFrontendProfilesMode(
                FableFrontendProfilesNew);
            PlayVisualFrontendResourceSound(kBootSoundForwardResource);
            RevealVisualFrontend(window);
            return true;
        }
        if (
            g_VisualProfilesMode == FableFrontendProfilesNormal &&
            g_VisualProfileSelection < VisualProfileItemCount())
        {
            if (!actionPlan.dispatch ||
                actionPlan.action != FableRetailFrontendManagerActionLoadProfile)
                return true;
            FableSetVisualFrontendActiveProfile(
                g_VisualProfileNames[g_VisualProfileSelection - 1].name);
            // Action 0x124 is LoadProfile.  Until the exact CUserProfileManager
            // instance is connected to this checkpoint, preserve the route
            // boundary and return to the main frontend without fabricating a
            // save/profile write.
            g_VisualProfilesMenuActive = false;
            g_VisualMainMenuActive = true;
            FableSetVisualFrontendProfilesMenu(false, 0, 0);
            FableSetVisualFrontendMainMenu(true);
            PlayVisualFrontendResourceSound(kBootSoundForwardResource);
            RevealVisualFrontend(window);
            return true;
        }
        if (g_VisualProfilesMode == FableFrontendProfilesDelete)
        {
            // Delete-list rows dispatch action 0xd7.  Its exact manager body
            // prepares the confirmation title and enters the delete-profile
            // screen; persistence remains at the native manager boundary.
            if (
                actionPlan.dispatch &&
                actionPlan.action ==
                    FableRetailFrontendManagerActionDeleteRow)
            {
                g_VisualProfilesMode =
                    FableFrontendProfilesDeleteConfirm;
                FableSetVisualFrontendProfilesMode(
                    FableFrontendProfilesDeleteConfirm);
                PlayVisualFrontendResourceSound(
                    kBootSoundForwardResource);
                RevealVisualFrontend(window);
            }
            return true;
        }
        return true;
    }

    void AppendVisualProfileEditCharacter(unsigned int character)
    {
        if (
            !g_VisualProfilesMenuActive ||
            g_VisualProfilesMode != FableFrontendProfilesNew)
        {
            return;
        }
        if (FableRetailFrontendProcessTextInputCharacter(
                static_cast<unsigned short>(character),
                g_VisualProfileEditText,
                &g_VisualProfileEditLength))
        {
            FableSetVisualFrontendProfileEditText(g_VisualProfileEditText);
        }
    }

    void RemoveVisualProfileEditCharacter()
    {
        if (
            !g_VisualProfilesMenuActive ||
            g_VisualProfilesMode != FableFrontendProfilesNew)
        {
            return;
        }
        if (FableRetailFrontendProcessTextInputCharacter(
                8,
                g_VisualProfileEditText,
                &g_VisualProfileEditLength))
        {
            FableSetVisualFrontendProfileEditText(g_VisualProfileEditText);
        }
    }

    bool ActivateVisualOptionsSelection(FableWindow window)
    {
        if (
            !g_VisualOptionsMenuActive ||
            g_VisualOptionsSelection >= 4)
        {
            return false;
        }
        // Retail dispatcher/Init2 map:
        // row 0 action 9   -> key 0x01 Gameplay
        // row 1 action 13  -> key 0x05 Video
        // row 2 action 12  -> key 0x04 Audio
        // row 3 action 283 -> key 0x16 Redefine
        const unsigned int detailScreens[4] = {
            1, 3, 2, 4
        };
        g_VisualOptionsMenuActive = false;
        g_VisualDetailScreen =
            detailScreens[g_VisualOptionsSelection];
        g_VisualRedefineHover = 0;
        g_VisualRedefineResetHover = 0;
        BeginVisualDetailScreen(g_VisualDetailScreen);
        FableSetVisualFrontendOptionsMenu(false);
        FableSetVisualFrontendDetailScreen(
            g_VisualDetailScreen);
        PlayVisualFrontendResourceSound(kBootSoundForwardResource);
        RevealVisualFrontend(window);
        return true;
    }

    bool ActivateVisualPressStart(FableWindow window)
    {
        if (
            !g_VisualFrontendVisible ||
            g_VisualMainMenuActive ||
            g_VisualOptionsMenuActive ||
            g_VisualSaveMenuActive ||
            g_VisualAboutMenuActive ||
            g_VisualCreditsMenuActive ||
            g_VisualProfilesMenuActive ||
            g_VisualDetailScreen != 0 ||
            g_VisualQuitPromptActive ||
            FableIsRetailVideoActive())
        {
            return false;
        }
        // UI_FRONTEND_BUTTON_INVISIBLE (#625) dispatches action 229.
        g_VisualMainMenuActive = true;
        g_VisualMainMenuSelection = 0;
        FableSetVisualFrontendMainMenu(true);
        PlayVisualFrontendResourceSound(kBootSoundForwardResource);
        RevealVisualFrontend(window);
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
                if (g_VisualFrontendVisible)
                {
                    FableRenderVisualD3D9(
                        clientWidth,
                        clientHeight);
                }
                else
                {
                    FablePresentVisualD3D9Black();
                }
            }
            return 0;
        }

        case kMessagePaint:
            if (!PaintBootArtworkD3D9(window))
                PaintBootArtwork(window);
            return 0;

        case kMessageMouseWheel:
        {
            if (FableIsRetailVideoActive())
                break;
            const VisualFrontendScrollTarget target =
                ActiveVisualFrontendScrollTarget();
            if (target == VisualFrontendScrollNone)
                break;
            int mouseX = 0;
            int mouseY = 0;
            if (
                !GetVisualDesignMousePosition(
                    window,
                    longParameter,
                    true,
                    &mouseX,
                    &mouseY) ||
                !IsVisualFrontendScrollArea(target, mouseX, mouseY))
            {
                break;
            }
            const int wheelDelta = static_cast<short>(
                (static_cast<unsigned long>(wordParameter) >> 16) &
                0xFFFFUL);
            if (wheelDelta == 0)
                break;
            // CMouseDX emits one CInputEvent per mouse event. Its recovered
            // event reader stores the signed payload in thousandths before
            // CNewFrontendGameComponent maps type 0x0e to action 0x24 or
            // 0x25 by +/-0.0001; it does not turn a larger delta into
            // repeated list events.
            const float wheelMovement =
                FableRetailFrontendMouseWheelMovementFromRaw(
                    static_cast<short>(wheelDelta));
            unsigned int action = 0;
            if (!FableRetailFrontendWheelAction(wheelMovement, &action))
                break;
            unsigned int listEvent = 0;
            if (!FableRetailFrontendListEventFromAction(action, &listEvent))
                break;
            ScrollVisualFrontendList(
                window,
                target,
                listEvent == FableRetailFrontendListEventDown);
            return 0;
        }

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
            if (
                !FableIsRetailVideoActive() &&
                g_VisualDetailScreen == 4 &&
                g_VisualRedefineSelection != 0)
            {
                if (wordParameter == kEscapeKey)
                {
                    // Retail event 0x21 with Escape selected calls
                    // CKeyRedefiner::CancelSelection.
                    g_VisualRedefineSelection = 0;
                    g_VisualRedefineCaptureRow = 0;
                    FableSetVisualFrontendRedefineSelection(0);
                    RevealVisualFrontend(window);
                    return 0;
                }
                const unsigned int keyValue =
                    VisualVirtualKeyToRedefineValue(wordParameter);
                if (ApplyVisualRedefinition(keyValue))
                {
                    RevealVisualFrontend(window);
                    return 0;
                }
            }
            if (
                !FableIsRetailVideoActive() &&
                (wordParameter == 0x26 || wordParameter == 0x28))
            {
                const bool scrollDown = wordParameter == 0x28;
                if (g_VisualMainMenuActive)
                {
                    fable_u32 selected = g_VisualMainMenuSelection;
                    fable_u32 soundRequest =
                        FableUiFrontEndSoundNone;
                    const bool moved =
                        FableScrollVisualFrontendMainMenu(
                            scrollDown,
                            &selected,
                            &soundRequest);
                    PlayVisualFrontendListSound(soundRequest);
                    if (moved)
                    {
                        g_VisualMainMenuSelection = selected;
                        RevealVisualFrontend(window);
                    }
                    return 0;
                }
                if (g_VisualOptionsMenuActive)
                {
                    FableUiFrontEndListScrollPlan plan = {};
                    FablePlanUiFrontEndListProcessEvent(
                        scrollDown
                            ? FableUiFrontEndListEventDown
                            : FableUiFrontEndListEventUp,
                        4,
                        static_cast<long>(
                            g_VisualOptionsSelection),
                        false,
                        0,
                        0,
                        0,
                        &plan);
                    PlayVisualFrontendListSound(plan.soundRequest);
                    if (plan.moved)
                    {
                        g_VisualOptionsSelection =
                            static_cast<unsigned int>(
                                plan.selectedChild);
                        FableSetVisualFrontendOptionsSelection(
                            g_VisualOptionsSelection);
                        RevealVisualFrontend(window);
                    }
                    return 0;
                }
                if (g_VisualSaveMenuActive)
                {
                    FableUiFrontEndListScrollPlan plan = {};
                    FablePlanUiFrontEndListProcessEvent(
                        scrollDown
                            ? FableUiFrontEndListEventDown
                            : FableUiFrontEndListEventUp,
                        4,
                        static_cast<long>(g_VisualSaveSelection),
                        false,
                        0,
                        0,
                        0,
                        &plan);
                    PlayVisualFrontendListSound(plan.soundRequest);
                    if (plan.moved)
                    {
                        g_VisualSaveSelection =
                            static_cast<unsigned int>(
                                plan.selectedChild);
                        FableSetVisualFrontendSaveSelection(
                            g_VisualSaveSelection);
                        RevealVisualFrontend(window);
                    }
                    return 0;
                }
                if (g_VisualProfilesMenuActive)
                {
                    FableUiFrontEndListScrollPlan plan = {};
                    FablePlanUiFrontEndListProcessEvent(
                        scrollDown
                            ? FableUiFrontEndListEventDown
                            : FableUiFrontEndListEventUp,
                        static_cast<long>(VisualProfileItemCount()),
                        static_cast<long>(g_VisualProfileSelection),
                        true,
                        0,
                        0,
                        0,
                        &plan);
                    PlayVisualFrontendListSound(plan.soundRequest);
                    if (plan.moved)
                    {
                        g_VisualProfileSelection =
                            static_cast<unsigned int>(plan.selectedChild);
                        FableSetVisualFrontendProfilesSelection(
                            g_VisualProfileSelection);
                        RevealVisualFrontend(window);
                    }
                    return 0;
                }
                if (
                    g_VisualDetailScreen == 4 &&
                    g_VisualRedefineSelection == 0)
                {
                    FableUiFrontEndListScrollPlan plan = {};
                    FablePlanUiFrontEndListProcessEvent(
                        scrollDown
                            ? FableUiFrontEndListEventDown
                            : FableUiFrontEndListEventUp,
                        kVisualRedefineListChildCount,
                        static_cast<long>(
                            g_VisualRedefineListSelection),
                        true,
                        0,
                        0,
                        0,
                        &plan);
                    PlayVisualFrontendListSound(plan.soundRequest);
                    if (plan.moved)
                    {
                        g_VisualRedefineListSelection =
                            static_cast<unsigned int>(
                                plan.selectedChild);
                        FableSetVisualFrontendRedefineListSelection(
                            g_VisualRedefineListSelection);
                        RevealVisualFrontend(window);
                    }
                    return 0;
                }
                if (
                    g_VisualDetailScreen >= 1 &&
                    g_VisualDetailScreen <= 3)
                {
                    const unsigned int screenIndex =
                        g_VisualDetailScreen - 1;
                    FableUiFrontEndListScrollPlan plan = {};
                    FablePlanUiFrontEndListProcessEvent(
                        scrollDown
                            ? FableUiFrontEndListEventDown
                            : FableUiFrontEndListEventUp,
                        static_cast<long>(
                            kVisualDetailScreens[screenIndex].rowCount),
                        static_cast<long>(
                            g_VisualDetailSelection),
                        false,
                        0,
                        0,
                        0,
                        &plan);
                    PlayVisualFrontendListSound(plan.soundRequest);
                    if (plan.moved)
                    {
                        g_VisualDetailSelection =
                            static_cast<unsigned int>(
                                plan.selectedChild);
                    }
                    return 0;
                }
            }
            if (
                !FableIsRetailVideoActive() &&
                g_VisualProfilesMenuActive &&
                g_VisualProfilesMode == FableFrontendProfilesNew &&
                wordParameter == 0x08)
            {
                RemoveVisualProfileEditCharacter();
                RevealVisualFrontend(window);
                return 0;
            }
            if (
                !FableIsRetailVideoActive() &&
                (wordParameter == 0x25 || wordParameter == 0x27) &&
                g_VisualDetailScreen >= 1 &&
                g_VisualDetailScreen <= 3)
            {
                const unsigned int screenIndex =
                    g_VisualDetailScreen - 1;
                const unsigned int row =
                    g_VisualDetailSelection;
                const unsigned int valueCount =
                    kVisualDetailScreens[screenIndex].rows[row].valueCount;
                if (valueCount != 0)
                {
                    unsigned int value =
                        g_VisualDetailValues[screenIndex][row];
                    if (wordParameter == 0x25)
                    {
                        value =
                            value == 0
                                ? valueCount - 1
                                : value - 1;
                    }
                    else
                    {
                        value = (value + 1) % valueCount;
                    }
                    g_VisualDetailValues[screenIndex][row] = value;
                    SyncVisualDetailScreen(g_VisualDetailScreen);
                    PlayVisualFrontendListSound(
                        FableUiFrontEndSoundUpDown);
                    RevealVisualFrontend(window);
                }
                return 0;
            }
            if (
                wordParameter == 0x0D &&
                !FableIsRetailVideoActive() &&
                (
                    ActivateVisualMainMenuSelection(window) ||
                    ActivateVisualProfileSelection(window) ||
                    ActivateVisualOptionsSelection(window) ||
                    ActivateVisualPressStart(window)
                ))
            {
                return 0;
            }
            if (
                wordParameter == kEscapeKey &&
                !FableIsRetailVideoActive())
            {
                if (g_VisualQuitPromptActive)
                {
                    // Retail action 86: GotoPreviousScreen(false).
                    g_VisualQuitPromptActive = false;
                    g_VisualMainMenuActive = true;
                    FableSetVisualFrontendQuitPrompt(false);
                    FableSetVisualFrontendMainMenu(true);
                    PlayVisualFrontendResourceSound(
                        kBootSoundBackResource);
                    RevealVisualFrontend(window);
                    return 0;
                }
                if (g_VisualDetailScreen != 0)
                {
                    // Each detail screen was pushed by GotoNextScreen;
                    // action 86 reverts its active sliders and returns to
                    // UI_FRONTEND_OPTIONS_SUB_MENU.
                    CancelVisualDetailScreen();
                    g_VisualDetailScreen = 0;
                    g_VisualOptionsMenuActive = true;
                    FableSetVisualFrontendDetailScreen(0);
                    FableSetVisualFrontendOptionsMenu(true);
                    PlayVisualFrontendResourceSound(
                        kBootSoundBackResource);
                    RevealVisualFrontend(window);
                    return 0;
                }
                if (g_VisualOptionsMenuActive)
                {
                    // UI_BACK (#563) also dispatches retail action 86.
                    g_VisualOptionsMenuActive = false;
                    g_VisualMainMenuActive = true;
                    FableSetVisualFrontendOptionsMenu(false);
                    FableSetVisualFrontendMainMenu(true);
                    PlayVisualFrontendResourceSound(
                        kBootSoundBackResource);
                    RevealVisualFrontend(window);
                    return 0;
                }
                if (g_VisualSaveMenuActive)
                {
                    // UI_HELPERS/UI_BACK dispatches retail action 86.
                    g_VisualSaveMenuActive = false;
                    g_VisualMainMenuActive = true;
                    FableSetVisualFrontendSaveMenu(false);
                    FableSetVisualFrontendMainMenu(true);
                    PlayVisualFrontendResourceSound(
                        kBootSoundBackResource);
                    RevealVisualFrontend(window);
                    return 0;
                }
                if (g_VisualProfilesMenuActive)
                {
                    if (
                        g_VisualProfilesMode ==
                            FableFrontendProfilesDeleteConfirm)
                    {
                        g_VisualProfilesMode = FableFrontendProfilesDelete;
                        FableSetVisualFrontendProfilesMode(
                            FableFrontendProfilesDelete);
                        RevealVisualFrontend(window);
                        return 0;
                    }
                    if (g_VisualProfilesMode == FableFrontendProfilesNew)
                    {
                        FableUiFrontendProfileActionPlan actionPlan = {};
                        FablePlanVisualFrontendProfileAction(
                            FableUiFrontendProfileModeNew,
                            0,
                            g_VisualProfileNameCount,
                            FableUiFrontendProfileInputKeyboardCancel,
                            &actionPlan);
                        if (!actionPlan.dispatch ||
                            actionPlan.action !=
                                FableRetailFrontendManagerActionKeyboardCancel)
                            return 0;
                        g_VisualProfilesMode =
                            g_VisualProfileNameCount == 0
                                ? FableFrontendProfilesEmpty
                                : FableFrontendProfilesNormal;
                        memset(
                            g_VisualProfileEditText,
                            0,
                            sizeof(g_VisualProfileEditText));
                        g_VisualProfileEditLength = 0;
                        FableSetVisualFrontendProfilesMode(
                            g_VisualProfilesMode);
                        RevealVisualFrontend(window);
                        return 0;
                    }
                    g_VisualProfilesMenuActive = false;
                    g_VisualMainMenuActive = true;
                    FableSetVisualFrontendProfilesMenu(false, 0, 0);
                    FableSetVisualFrontendMainMenu(true);
                    PlayVisualFrontendResourceSound(
                        kBootSoundBackResource);
                    RevealVisualFrontend(window);
                    return 0;
                }
                if (g_VisualAboutMenuActive)
                {
                    // About screen has no selectable rows; UI_HELPERS/UI_BACK
                    // (#563) dispatches retail action 86 -> GotoPreviousScreen
                    // back to UI_FRONTEND_MAIN_MENU.
                    g_VisualAboutMenuActive = false;
                    g_VisualMainMenuActive = true;
                    FableSetVisualFrontendAboutMenu(false);
                    FableSetVisualFrontendMainMenu(true);
                    PlayVisualFrontendResourceSound(
                        kBootSoundBackResource);
                    RevealVisualFrontend(window);
                    return 0;
                }
                if (g_VisualCreditsMenuActive)
                {
                    // Credits uses UI_HELPERS_CREDITS/UI_BACK, whose retail
                    // action is the same 86 previous-screen dispatch.
                    g_VisualCreditsMenuActive = false;
                    g_VisualMainMenuActive = true;
                    FableSetVisualFrontendCreditsMenu(false);
                    FableSetVisualFrontendMainMenu(true);
                    PlayVisualFrontendResourceSound(
                        kBootSoundBackResource);
                    RevealVisualFrontend(window);
                    return 0;
                }
            }
            break;

        case kMessageChar:
            if (
                !FableIsRetailVideoActive() &&
                g_VisualProfilesMenuActive &&
                g_VisualProfilesMode == FableFrontendProfilesNew)
            {
                AppendVisualProfileEditCharacter(
                    static_cast<unsigned int>(wordParameter));
                RevealVisualFrontend(window);
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

        case kMessageRightButtonUp:
        case kMessageMiddleButtonUp:
            if (
                !FableIsRetailVideoActive() &&
                g_VisualDetailScreen == 4 &&
                g_VisualRedefineSelection != 0 &&
                ApplyVisualRedefinition(
                    message == kMessageRightButtonUp ? 3 : 2))
            {
                RevealVisualFrontend(window);
                return 0;
            }
            break;

        case kMessageLeftButtonDown:
            if (DispatchVisualFrontendScrollArrowPress(
                    window,
                    longParameter,
                    false))
            {
                return 0;
            }
            break;

        case kMessageLeftButtonUp:
            if (
                !FableIsRetailVideoActive() &&
                DispatchVisualFrontendScrollArrowRelease())
            {
                return 0;
            }
            if (
                !FableIsRetailVideoActive() &&
                g_VisualDetailScreen == 4 &&
                g_VisualRedefineSelection != 0 &&
                ApplyVisualRedefinition(1))
            {
                RevealVisualFrontend(window);
                return 0;
            }
            if (
                g_VisualFrontendVisible &&
                !FableIsRetailVideoActive() &&
                (g_VisualMainMenuActive ||
                 g_VisualOptionsMenuActive ||
                 g_VisualSaveMenuActive ||
                 g_VisualProfilesMenuActive ||
                 g_VisualAboutMenuActive ||
                 g_VisualCreditsMenuActive ||
                 g_VisualDetailScreen != 0 ||
                 g_VisualQuitPromptActive))
            {
                FableRectangle client = {};
                if (!GetClientRect(window, &client))
                    break;
                const int clientWidth = client.right - client.left;
                const int clientHeight = client.bottom - client.top;
                if (clientWidth <= 0 || clientHeight <= 0)
                    break;
                const int mouseX =
                    static_cast<int>(longParameter & 0xFFFF) *
                    640 / clientWidth;
                const int mouseY =
                    static_cast<int>(
                        (longParameter >> 16) & 0xFFFF) *
                    480 / clientHeight;
                if (g_VisualOptionsMenuActive)
                {
                    if (
                        mouseX >= 20 &&
                        mouseX < 270 &&
                        mouseY >= 420 &&
                        mouseY < 450)
                    {
                        // Retail action 86: GotoPreviousScreen(false).
                        g_VisualOptionsMenuActive = false;
                        g_VisualMainMenuActive = true;
                        FableSetVisualFrontendOptionsMenu(false);
                        FableSetVisualFrontendMainMenu(true);
                        PlayVisualFrontendResourceSound(
                            kBootSoundBackResource);
                        RevealVisualFrontend(window);
                        return 0;
                    }
                    unsigned int row = 0;
                    if (FindVisualOptionsMenuRow(mouseX, mouseY, &row))
                    {
                        g_VisualOptionsSelection = row;
                        if (ActivateVisualOptionsSelection(window))
                            return 0;
                    }
                }
                else if (g_VisualSaveMenuActive)
                {
                    if (
                        mouseX >= 20 &&
                        mouseX < 270 &&
                        mouseY >= 420 &&
                        mouseY < 450)
                    {
                        g_VisualSaveMenuActive = false;
                        g_VisualMainMenuActive = true;
                        FableSetVisualFrontendSaveMenu(false);
                        FableSetVisualFrontendMainMenu(true);
                        PlayVisualFrontendResourceSound(
                            kBootSoundBackResource);
                        RevealVisualFrontend(window);
                        return 0;
                    }
                    unsigned int row = 0;
                    if (FindVisualSaveMenuRow(mouseX, mouseY, &row))
                    {
                        g_VisualSaveSelection = row;
                        FableSetVisualFrontendSaveSelection(row);
                        RevealVisualFrontend(window);
                        return 0;
                    }
                }
                else if (g_VisualProfilesMenuActive)
                {
                    if (
                        g_VisualProfilesMode ==
                            FableFrontendProfilesDeleteConfirm)
                    {
                        if (
                            mouseX >= 362 &&
                            mouseX < 618 &&
                            mouseY >= 405 &&
                            mouseY < 440)
                        {
                            // The retail YES control dispatches 0xd6.  The
                            // manager owns the destructive operation; this
                            // call intentionally stops at that boundary.
                            ActivateVisualProfileSelection(window);
                            return 0;
                        }
                        if (
                            mouseX >= 20 &&
                            mouseX < 276 &&
                            mouseY >= 405 &&
                            mouseY < 440)
                        {
                            g_VisualProfilesMode =
                                FableFrontendProfilesDelete;
                            FableSetVisualFrontendProfilesMode(
                                FableFrontendProfilesDelete);
                            RevealVisualFrontend(window);
                            return 0;
                        }
                        return 0;
                    }
                    if (
                        g_VisualProfilesMode == FableFrontendProfilesNormal &&
                        mouseX >= 362 &&
                        mouseX < 618 &&
                        mouseY >= 420 &&
                        mouseY < 450 &&
                        g_VisualProfileNameCount != 0)
                    {
                        FableUiFrontendProfileActionPlan actionPlan = {};
                        FablePlanVisualFrontendProfileAction(
                            FableUiFrontendProfileModeNormal,
                            0,
                            g_VisualProfileNameCount,
                            FableUiFrontendProfileInputDeleteList,
                            &actionPlan);
                        if (!actionPlan.dispatch ||
                            actionPlan.action !=
                                FableRetailFrontendManagerActionDeleteListRefresh)
                            return 0;
                        // UI_DELETE in UI_HELPERS_PROFILE_DELETE enters the
                        // separate delete-list definition.
                        g_VisualProfilesMode = FableFrontendProfilesDelete;
                        g_VisualProfileSelection = 0;
                        FableSetVisualFrontendProfilesMode(
                            FableFrontendProfilesDelete);
                        PlayVisualFrontendResourceSound(
                            kBootSoundForwardResource);
                        RevealVisualFrontend(window);
                        return 0;
                    }
                    if (
                        mouseX >= 20 &&
                        mouseX < 270 &&
                        mouseY >= 420 &&
                        mouseY < 450)
                    {
                        g_VisualProfilesMenuActive = false;
                        g_VisualMainMenuActive = true;
                        FableSetVisualFrontendProfilesMenu(false, 0, 0);
                        FableSetVisualFrontendMainMenu(true);
                        PlayVisualFrontendResourceSound(
                            kBootSoundBackResource);
                        RevealVisualFrontend(window);
                        return 0;
                    }
                    if (
                        mouseX >= 200 &&
                        mouseX < 440 &&
                        mouseY >= 113 &&
                        mouseY < 113 + FableFrontendProfileListHeight)
                    {
                        unsigned int row = 0;
                        if (FindVisualProfileRow(mouseX, mouseY, &row))
                        {
                            g_VisualProfileSelection = row;
                            FableSetVisualFrontendProfilesSelection(row);
                            if (
                                g_VisualProfilesMode ==
                                    FableFrontendProfilesNormal)
                            {
                                ActivateVisualProfileSelection(window);
                                return 0;
                            }
                            if (
                                g_VisualProfilesMode ==
                                FableFrontendProfilesDelete)
                            {
                                ActivateVisualProfileSelection(window);
                                return 0;
                            }
                            RevealVisualFrontend(window);
                            return 0;
                        }
                    }
                }
                else if (g_VisualAboutMenuActive)
                {
                    // About shares the UI_HELPERS Back button region with the
                    // other subscreens; a click there dispatches retail action
                    // 86 -> GotoPreviousScreen back to the main menu.  The
                    // screen has no selectable rows.
                    if (
                        mouseX >= 20 &&
                        mouseX < 270 &&
                        mouseY >= 420 &&
                        mouseY < 450)
                    {
                        g_VisualAboutMenuActive = false;
                        g_VisualMainMenuActive = true;
                        FableSetVisualFrontendAboutMenu(false);
                        FableSetVisualFrontendMainMenu(true);
                        PlayVisualFrontendResourceSound(
                            kBootSoundBackResource);
                        RevealVisualFrontend(window);
                        return 0;
                    }
                }
                else if (g_VisualCreditsMenuActive)
                {
                    if (
                        mouseX >= 20 &&
                        mouseX < 270 &&
                        mouseY >= 420 &&
                        mouseY < 450)
                    {
                        g_VisualCreditsMenuActive = false;
                        g_VisualMainMenuActive = true;
                        FableSetVisualFrontendCreditsMenu(false);
                        FableSetVisualFrontendMainMenu(true);
                        PlayVisualFrontendResourceSound(
                            kBootSoundBackResource);
                        RevealVisualFrontend(window);
                        return 0;
                    }
                }
                else if (g_VisualDetailScreen != 0)
                {
                    if (VisualPointInDetailFooter(3, mouseX, mouseY))
                    {
                        // UI_ACCEPT action 87 saves the current profile and
                        // returns to the previous screen.
                        ApplyVisualDetailScreen();
                        g_VisualDetailScreen = 0;
                        g_VisualRedefineHover = 0;
                        g_VisualRedefineResetHover = 0;
                        g_VisualDetailButtonHover = 0;
                        g_VisualDetailArrowHoverRow = 0;
                        g_VisualDetailArrowHoverSide = 0;
                        g_VisualOptionsMenuActive = true;
                        FableSetVisualFrontendDetailScreen(0);
                        FableSetVisualFrontendOptionsMenu(true);
                        PlayVisualFrontendResourceSound(
                            kBootSoundBackResource);
                        RevealVisualFrontend(window);
                        return 0;
                    }
                    if (VisualPointInDetailFooter(1, mouseX, mouseY))
                    {
                        // UI_CANCEL action 86 restores activation values.
                        CancelVisualDetailScreen();
                        g_VisualDetailScreen = 0;
                        g_VisualRedefineHover = 0;
                        g_VisualRedefineResetHover = 0;
                        g_VisualDetailButtonHover = 0;
                        g_VisualDetailArrowHoverRow = 0;
                        g_VisualDetailArrowHoverSide = 0;
                        g_VisualOptionsMenuActive = true;
                        FableSetVisualFrontendDetailScreen(0);
                        FableSetVisualFrontendOptionsMenu(true);
                        PlayVisualFrontendResourceSound(
                            kBootSoundBackResource);
                        RevealVisualFrontend(window);
                        return 0;
                    }
                    if (
                        g_VisualDetailScreen <= 3 &&
                        VisualPointInDetailFooter(2, mouseX, mouseY))
                    {
                        // Actions 324/325/326 set the profile group defaults
                        // and reset each active slider from that profile.
                        const unsigned int screenIndex =
                            g_VisualDetailScreen - 1;
                        for (
                            unsigned int row = 0;
                            row != kVisualDetailScreens[screenIndex].rowCount;
                            ++row)
                        {
                            g_VisualDetailValues[screenIndex][row] =
                                kVisualDetailScreens[screenIndex]
                                    .rows[row].defaultValue;
                        }
                        SyncVisualDetailScreen(g_VisualDetailScreen);
                        RevealVisualFrontend(window);
                        return 0;
                    }
                    if (g_VisualDetailScreen == 4)
                    {
                        if (
                            g_VisualRedefineListSelection == 0 &&
                            (VisualPointInRedefineReset(0, mouseX, mouseY) ||
                             VisualPointInRedefineReset(1, mouseX, mouseY)))
                        {
                            // UI_HELPERS_REDEFINE contributes parent y=20.
                            // Its reset controls are at local (0/320,385);
                            // their table graphics begin 16 pixels above the
                            // clickable parent. Actions are 311 (WASD) and
                            // 284 (arrows).
                            ResetVisualRedefineKeys(mouseX < 320);
                            RevealVisualFrontend(window);
                            return 0;
                        }
                        unsigned int row = 0;
                        if (FindVisualRedefineRow(
                                mouseX,
                                mouseY,
                                &row))
                        {
                            // CKeyRedefiner::OnLeftUnclicked installs the
                            // one active capture target and replaces this
                            // row's value with TEXT_GUI_PRESS_CONTROL.
                            g_VisualRedefineCaptureRow =
                                VisualRedefineMaterializedOffset() + row;
                            g_VisualRedefineSelection = row + 1;
                            FableSetVisualFrontendRedefineSelection(
                                g_VisualRedefineSelection);
                            RevealVisualFrontend(window);
                            return 0;
                        }
                    }
                    if (AdjustVisualDetailControl(
                            g_VisualDetailScreen,
                            mouseX,
                            mouseY))
                    {
                        // Retail profile setters apply the in-memory value
                        // immediately; persistence waits for action 87.
                        RevealVisualFrontend(window);
                        return 0;
                    }
                }
                else if (g_VisualQuitPromptActive)
                {
                    if (
                        mouseX >= 20 &&
                        mouseX < 276 &&
                        mouseY >= 389 &&
                        mouseY < 453)
                    {
                        // UI_FRONTEND_QUIT_NO dispatches retail action 86.
                        g_VisualQuitPromptActive = false;
                        g_VisualMainMenuActive = true;
                        FableSetVisualFrontendQuitPrompt(false);
                        FableSetVisualFrontendMainMenu(true);
                        PlayVisualFrontendResourceSound(
                            kBootSoundBackResource);
                        RevealVisualFrontend(window);
                        return 0;
                    }
                    if (
                        mouseX >= 362 &&
                        mouseX < 618 &&
                        mouseY >= 389 &&
                        mouseY < 453)
                    {
                        // Retail action 296 calls CMainGameComponent::SetQuit.
                        // Leaving the message loop performs the same ordered
                        // video, D3D9, and window-class shutdown here.
                        PostQuitMessage(0);
                        return 0;
                    }
                }
                else if (g_VisualMainMenuActive)
                {
                    unsigned int row = 0;
                    if (FindVisualMainMenuRow(mouseX, mouseY, &row))
                    {
                        g_VisualMainMenuSelection = row;
                        if (ActivateVisualMainMenuSelection(window))
                            return 0;
                    }
                }
            }
            if (ActivateVisualPressStart(window))
                return 0;
            break;

        case kMessageMouseMove:
        {
            if (
                !g_VisualMainMenuActive &&
                !g_VisualOptionsMenuActive &&
                !g_VisualSaveMenuActive &&
                !g_VisualProfilesMenuActive &&
                !g_VisualAboutMenuActive &&
                !g_VisualCreditsMenuActive &&
                g_VisualDetailScreen == 0 &&
                !g_VisualQuitPromptActive)
                break;
            FableRectangle client = {};
            if (!GetClientRect(window, &client))
                break;
            const int clientWidth = client.right - client.left;
            const int clientHeight = client.bottom - client.top;
            if (clientWidth <= 0 || clientHeight <= 0)
                break;
            const int mouseX =
                static_cast<int>(longParameter & 0xFFFF) *
                640 / clientWidth;
            const int mouseY =
                static_cast<int>((longParameter >> 16) & 0xFFFF) *
                480 / clientHeight;
            if (g_VisualQuitPromptActive)
            {
                unsigned int hover = 0;
                if (
                    mouseX >= 20 &&
                    mouseX < 276 &&
                    mouseY >= 389 &&
                    mouseY < 453)
                {
                    hover = 1;
                }
                else if (
                    mouseX >= 362 &&
                    mouseX < 618 &&
                    mouseY >= 389 &&
                    mouseY < 453)
                {
                    hover = 2;
                }
                if (g_VisualQuitHover != hover)
                {
                    g_VisualQuitHover = hover;
                    FableSetVisualFrontendQuitHover(hover);
                    RevealVisualFrontend(window);
                }
                return 0;
            }
            if (g_VisualOptionsMenuActive)
            {
                const bool backHovered =
                    mouseX >= 20 &&
                    mouseX < 270 &&
                    mouseY >= 420 &&
                    mouseY < 450;
                if (
                    g_VisualOptionsBackHovered != backHovered)
                {
                    g_VisualOptionsBackHovered = backHovered;
                    FableSetVisualFrontendOptionsBackHovered(
                        backHovered);
                    RevealVisualFrontend(window);
                }
                unsigned int row = 0;
                if (FindVisualOptionsMenuRow(mouseX, mouseY, &row))
                {
                    if (g_VisualOptionsSelection != row)
                    {
                        g_VisualOptionsSelection = row;
                        FableSetVisualFrontendOptionsSelection(row);
                        RevealVisualFrontend(window);
                    }
                }
                return 0;
            }
            if (g_VisualSaveMenuActive)
            {
                const bool backHovered =
                    mouseX >= 20 &&
                    mouseX < 270 &&
                    mouseY >= 420 &&
                    mouseY < 450;
                if (g_VisualOptionsBackHovered != backHovered)
                {
                    g_VisualOptionsBackHovered = backHovered;
                    FableSetVisualFrontendOptionsBackHovered(
                        backHovered);
                    RevealVisualFrontend(window);
                }
                unsigned int row = 0;
                if (
                    FindVisualSaveMenuRow(mouseX, mouseY, &row) &&
                    g_VisualSaveSelection != row)
                {
                    g_VisualSaveSelection = row;
                    FableSetVisualFrontendSaveSelection(row);
                    RevealVisualFrontend(window);
                }
                return 0;
            }
            if (g_VisualProfilesMenuActive)
            {
                const bool backHovered =
                    mouseX >= 20 &&
                    mouseX < 270 &&
                    mouseY >= 420 &&
                    mouseY < 450;
                if (g_VisualOptionsBackHovered != backHovered)
                {
                    g_VisualOptionsBackHovered = backHovered;
                    FableSetVisualFrontendOptionsBackHovered(
                        backHovered);
                    RevealVisualFrontend(window);
                }
                if (
                    mouseX >= 180 &&
                    mouseX < 460 &&
                    mouseY >= 113 &&
                        mouseY < 113 + FableFrontendProfileListHeight)
                    {
                        unsigned int row = 0;
                        if (!FindVisualProfileRow(mouseX, mouseY, &row))
                            return 0;
                    if (
                        row < VisualProfileItemCount() &&
                        g_VisualProfileSelection != row)
                    {
                        g_VisualProfileSelection = row;
                        FableSetVisualFrontendProfilesSelection(row);
                        RevealVisualFrontend(window);
                    }
                }
                return 0;
            }
            if (g_VisualAboutMenuActive)
            {
                // About shares the UI_HELPERS Back button with Options/Save
                // (same rect + shared g_VisualOptionsBackHovered state); it has
                // no selectable rows, so only the Back hover is tracked.
                const bool backHovered =
                    mouseX >= 20 &&
                    mouseX < 270 &&
                    mouseY >= 420 &&
                    mouseY < 450;
                if (g_VisualOptionsBackHovered != backHovered)
                {
                    g_VisualOptionsBackHovered = backHovered;
                    FableSetVisualFrontendOptionsBackHovered(
                        backHovered);
                    RevealVisualFrontend(window);
                }
                return 0;
            }
            if (g_VisualCreditsMenuActive)
            {
                // Credits shares the same live Back helper geometry.
                const bool backHovered =
                    mouseX >= 20 &&
                    mouseX < 270 &&
                    mouseY >= 420 &&
                    mouseY < 450;
                if (g_VisualOptionsBackHovered != backHovered)
                {
                    g_VisualOptionsBackHovered = backHovered;
                    FableSetVisualFrontendOptionsBackHovered(
                        backHovered);
                    RevealVisualFrontend(window);
                }
                return 0;
            }
            if (g_VisualDetailScreen != 0)
            {
                if (g_VisualDetailScreen == 4)
                {
                    unsigned int row = 0;
                    unsigned int hover = 0;
                    // The page atlas owns all nonzero list positions.  The
                    // live hover strip is only valid on the initial page;
                    // do not retain a hidden row hover while the list is
                    // scrolled, or it can reappear when page zero returns.
                    if (FindVisualRedefineRow(mouseX, mouseY, &row))
                        hover = row + 1;
                    if (g_VisualRedefineHover != hover)
                    {
                        g_VisualRedefineHover = hover;
                        FableSetVisualFrontendRedefineHover(hover);
                        RevealVisualFrontend(window);
                    }
                    unsigned int resetHover = 0;
                    // AUDIT #2: the reset hover band was [389,453) while its
                    // click band is [389,429) -- a 24px dead strip that lit
                    // but did not click.  Match the click rect so highlight ==
                    // clickable.
                    if (
                        g_VisualRedefineListSelection == 0 &&
                        VisualPointInRedefineReset(0, mouseX, mouseY))
                        resetHover = 1;
                    else if (
                        g_VisualRedefineListSelection == 0 &&
                        VisualPointInRedefineReset(1, mouseX, mouseY))
                        resetHover = 2;
                    if (g_VisualRedefineResetHover != resetHover)
                    {
                        g_VisualRedefineResetHover = resetHover;
                        FableSetVisualFrontendRedefineResetHover(resetHover);
                        RevealVisualFrontend(window);
                    }
                }
                // Footer button hover.  The click handler footer rects for
                // Cancel/Apply are gated only by g_VisualDetailScreen!=0, so
                // they also fire on Redefine (screen 4) -- AUDIT #1: track the
                // Cancel/Apply hover on screens 1-4 so Redefine's footer lights
                // too, but keep Defaults gated to 1-3 (no screen-4 art/rect).
                unsigned int buttonHover = 0;
                if (VisualPointInDetailFooter(1, mouseX, mouseY))
                {
                    buttonHover = 1;
                }
                else if (VisualPointInDetailFooter(3, mouseX, mouseY))
                {
                    buttonHover = 3;
                }
                else if (
                    g_VisualDetailScreen <= 3 &&
                    VisualPointInDetailFooter(2, mouseX, mouseY))
                {
                    buttonHover = 2;
                }
                if (g_VisualDetailButtonHover != buttonHover)
                {
                    g_VisualDetailButtonHover = buttonHover;
                    FableSetVisualFrontendDetailButtonHover(buttonHover);
                    RevealVisualFrontend(window);
                }
                if (g_VisualDetailScreen >= 1 && g_VisualDetailScreen <= 3)
                {
                    // Arrow hover mirrors AdjustVisualDetailControl's row scan:
                    // only rows < the decoded screen row count are valid (Video has 3;
                    // rows 3-9 have rowY 0), so phantom rows never highlight.
                    const unsigned int screenIndex =
                        g_VisualDetailScreen - 1;
                    unsigned int arrowRow = 0;
                    unsigned int arrowSide = 0;
                    for (
                        unsigned int detailRow = 0;
                        detailRow !=
                            kVisualDetailScreens[screenIndex].rowCount;
                        ++detailRow)
                    {
                        const int rowTop =
                            kVisualDetailScreens[screenIndex]
                                .rows[detailRow].y +
                            fable_detail_tables::kArrow.controlTopOffset;
                        if (mouseY < rowTop || mouseY >= rowTop + 30)
                            continue;
                        if (
                            mouseX >=
                                fable_detail_tables::kArrow.leftHit.left &&
                            mouseX <
                                fable_detail_tables::kArrow.leftHit.right)
                        {
                            arrowRow = detailRow;
                            arrowSide = 1;
                        }
                        else if (
                            mouseX >=
                                fable_detail_tables::kArrow.rightHit.left &&
                            mouseX <
                                fable_detail_tables::kArrow.rightHit.right)
                        {
                            arrowRow = detailRow;
                            arrowSide = 2;
                        }
                        break;
                    }
                    if (
                        g_VisualDetailArrowHoverRow != arrowRow ||
                        g_VisualDetailArrowHoverSide != arrowSide)
                    {
                        g_VisualDetailArrowHoverRow = arrowRow;
                        g_VisualDetailArrowHoverSide = arrowSide;
                        FableSetVisualFrontendDetailArrowHover(
                            arrowRow,
                            arrowSide);
                        RevealVisualFrontend(window);
                    }
                }
                return 0;
            }
            unsigned int row = 0;
            if (FindVisualMainMenuRow(mouseX, mouseY, &row))
            {
                if (g_VisualMainMenuSelection != row)
                {
                    g_VisualMainMenuSelection = row;
                    FableSetVisualFrontendMainMenuSelection(row);
                    RevealVisualFrontend(window);
                }
            }
            return 0;
        }

        case kMessageTimer:
            if (
                wordParameter == kFrontendAnimationTimer &&
                g_VisualFrontendVisible)
            {
                fable_u32 controllerState = 0;
                if (!FableIsRetailVideoActive())
                {
                    FableJoyInfoEx controller = {};
                    controller.size = sizeof(controller);
                    controller.flags = kJoyReturnAll;
                    if (joyGetPosEx(0, &controller) == 0)
                    {
                        controllerState =
                            FableMapUiControllerState(
                                controller.xPosition,
                                controller.yPosition,
                                controller.pointOfView,
                                controller.buttons);
                    }
                }
                const fable_u32 controllerActions =
                    FableConsumeUiControllerActions(
                        controllerState,
                        GetTickCount(),
                        &g_VisualControllerState,
                        &g_VisualControllerRepeatState);
                if ((controllerActions & FableUiControllerUp) != 0)
                {
                    VisualBootWindowProcedure(
                        window,
                        kMessageKeyDown,
                        0x26,
                        0);
                }
                else if (
                    (controllerActions & FableUiControllerDown) != 0)
                {
                    VisualBootWindowProcedure(
                        window,
                        kMessageKeyDown,
                        0x28,
                        0);
                }
                else if (
                    (controllerActions & FableUiControllerLeft) != 0)
                {
                    VisualBootWindowProcedure(
                        window,
                        kMessageKeyDown,
                        0x25,
                        0);
                }
                else if (
                    (controllerActions & FableUiControllerRight) != 0)
                {
                    VisualBootWindowProcedure(
                        window,
                        kMessageKeyDown,
                        0x27,
                        0);
                }
                else if (
                    (controllerActions & FableUiControllerAccept) != 0)
                {
                    VisualBootWindowProcedure(
                        window,
                        kMessageKeyDown,
                        0x0D,
                        0);
                }
                else if (
                    (controllerActions & FableUiControllerBack) != 0)
                {
                    VisualBootWindowProcedure(
                        window,
                        kMessageKeyDown,
                        kEscapeKey,
                        0);
                }
                InvalidateRect(window, 0, 0);
                return 0;
            }
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
                        RevealVisualFrontend(window);
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
                    RevealVisualFrontend(window);
                }
            }
            return 0;

        case kMessageEraseBackground:
            return 1;

        case kMessageDestroy:
            KillTimer(window, kRetailVideoTimer);
            KillTimer(window, kFrontendAnimationTimer);
            g_VisualControllerState = 0;
            g_VisualControllerRepeatState.lastMovement = 0;
            g_VisualControllerRepeatState.lastSelectionTimeMs = 0;
            g_VisualControllerRepeatState.isRepeating = false;
            g_VisualDetailSelection = 0;
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

bool FABLE_FASTCALL FableShouldPlayBootVideos(const char* commandLine)
{
    if (
        commandLine == 0 ||
        strstr(commandLine, "--skip-boot-videos") != 0)
    {
        return false;
    }

    return
        strstr(commandLine, "--retail-video") != 0 ||
        strstr(commandLine, "--buff-jesus") != 0;
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
    g_BootTitleArtwork = LoadFrontendArtwork(
        instance,
        kBootTitleResource,
        "visual_boot_title.bmp");
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
#if defined(FABLETLC_RETAIL_FRONTEND_ANIMATION)
    g_BootForestArtwork = LoadFrontendArtwork(
        instance,
        103,
        "visual_boot_forest.bmp");
    g_BootSunbeamArtwork = LoadFrontendArtwork(
        instance,
        104,
        "visual_boot_sunbeam.bmp");
    if (g_BootForestArtwork == 0 || g_BootSunbeamArtwork == 0)
    {
        DeleteObject(g_BootSunbeamArtwork);
        DeleteObject(g_BootForestArtwork);
        DeleteObject(g_BootTitleArtwork);
        DeleteObject(g_BootArtwork);
        g_BootSunbeamArtwork = 0;
        g_BootForestArtwork = 0;
        g_BootTitleArtwork = 0;
        g_BootArtwork = 0;
        return 1;
    }
    GetObjectA(
        g_BootForestArtwork,
        sizeof(g_BootForestArtworkInfo),
        &g_BootForestArtworkInfo);
    GetObjectA(
        g_BootSunbeamArtwork,
        sizeof(g_BootSunbeamArtworkInfo),
        &g_BootSunbeamArtworkInfo);
#endif
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
#if defined(FABLETLC_RETAIL_FRONTEND_ANIMATION)
        DeleteObject(g_BootSunbeamArtwork);
        DeleteObject(g_BootForestArtwork);
        g_BootSunbeamArtwork = 0;
        g_BootForestArtwork = 0;
#endif
#endif
        return 0;
    }

    const bool retailVideoRequested =
        FableShouldPlayBootVideos(commandLine);
    g_VisualFrontendVisible = !retailVideoRequested;
    g_VisualMainMenuActive = false;
    g_VisualMainMenuSelection = 0;
    g_VisualOptionsMenuActive = false;
    g_VisualOptionsSelection = 0;
    g_VisualOptionsBackHovered = false;
    g_VisualSaveMenuActive = false;
    g_VisualSaveSelection = 0;
    g_VisualAboutMenuActive = false;
    g_VisualCreditsMenuActive = false;
    g_VisualProfilesMenuActive = false;
    g_VisualProfilesMode = FableFrontendProfilesNormal;
    memset(g_VisualProfileNames, 0, sizeof(g_VisualProfileNames));
    memset(g_VisualProfileNamePointers, 0, sizeof(g_VisualProfileNamePointers));
    g_VisualProfileNameCount = 0;
    g_VisualProfileSelection = 0;
    memset(g_VisualProfileEditText, 0, sizeof(g_VisualProfileEditText));
    g_VisualProfileEditLength = 0;
    g_VisualDetailScreen = 0;
    g_VisualRedefineHover = 0;
    g_VisualRedefineResetHover = 0;
    g_VisualRedefineSelection = 0;
    g_VisualRedefineCaptureRow = 0;
    g_VisualRedefineListSelection = 0;
    CopyVisualRedefineKeys(
        g_VisualRedefineKeys,
        kVisualRedefineDefaults);
    CopyVisualRedefineKeys(
        g_VisualRedefineEntryKeys,
        kVisualRedefineDefaults);
    CopyVisualRedefineKeys(
        g_VisualRedefineSavedKeys,
        kVisualRedefineDefaults);
    LoadVisualDetailDefaults(g_VisualDetailValues);
    LoadVisualDetailDefaults(g_VisualDetailEntryValues);
    LoadVisualDetailDefaults(g_VisualDetailSavedValues);
    g_VisualQuitPromptActive = false;
    g_VisualQuitHover = 0;

    FableWindowClass windowClass = {};
    windowClass.size = sizeof(windowClass);
    windowClass.style =
        kClassRedrawHorizontal | kClassRedrawVertical;
    windowClass.procedure = VisualBootWindowProcedure;
    windowClass.instance = instance;
    windowClass.cursor =
#if defined(FABLETLC_RETAIL_FRONTEND_ARTWORK)
        LoadCursorA(instance, IntegerResource(kBootPointerResource));
    if (windowClass.cursor == 0)
    {
        windowClass.cursor =
            LoadCursorA(0, IntegerResource(kArrowCursor));
    }
#else
        LoadCursorA(0, IntegerResource(kArrowCursor));
#endif
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
#if defined(FABLETLC_RETAIL_FRONTEND_ANIMATION)
        DeleteObject(g_BootSunbeamArtwork);
        DeleteObject(g_BootForestArtwork);
        g_BootSunbeamArtwork = 0;
        g_BootForestArtwork = 0;
#endif
#endif
        return 2;
    }

    // The authored frontend is laid out in a 640x480 design space and the
    // retail reference captures are 1024x768. Keep the existing 1280x720
    // developer window as the default, but make the reference capture size
    // an explicit launch contract instead of relying on an external resize.
    const bool retailFrontendReferenceSize =
        commandLine != 0 &&
        strstr(commandLine, "--retail-frontend-reference-size") != 0;
    const int frontendClientWidth =
        retailFrontendReferenceSize ? 1024 : 1280;
    const int frontendClientHeight =
        retailFrontendReferenceSize ? 768 : 720;
    FableRectangle windowBounds = {
        0,
        0,
        frontendClientWidth,
        frontendClientHeight};
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
#if defined(FABLETLC_RETAIL_FRONTEND_ANIMATION)
        DeleteObject(g_BootSunbeamArtwork);
        DeleteObject(g_BootForestArtwork);
        g_BootSunbeamArtwork = 0;
        g_BootForestArtwork = 0;
#endif
#endif
        return 3;
    }

#if defined(FABLETLC_RETAIL_FRONTEND_MENU)
    g_BootMenuArtwork = LoadFrontendArtwork(
        instance,
        kBootMenuResource,
        "visual_boot_main_menu.bmp");
    g_BootBuffJesusMenuArtwork = LoadFrontendArtwork(
        instance,
        kBootBuffJesusMenuResource,
        "visual_boot_main_menu_buff_jesus.bmp");
    g_BootCoastalArtwork = LoadFrontendArtwork(
        instance,
        kBootCoastalResource,
        "visual_boot_coastal.bmp");
    g_BootCoastalSunbeamArtwork = LoadFrontendArtwork(
        instance,
        kBootCoastalSunbeamResource,
        "visual_boot_coastal_sunbeam.bmp");
    if (
        g_BootMenuArtwork != 0 &&
        g_BootCoastalArtwork != 0 &&
        g_BootCoastalSunbeamArtwork != 0)
    {
        GetObjectA(
            g_BootMenuArtwork,
            sizeof(g_BootMenuArtworkInfo),
            &g_BootMenuArtworkInfo);
        if (g_BootBuffJesusMenuArtwork != 0)
        {
            GetObjectA(
                g_BootBuffJesusMenuArtwork,
                sizeof(g_BootBuffJesusMenuArtworkInfo),
                &g_BootBuffJesusMenuArtworkInfo);
        }
        GetObjectA(
            g_BootCoastalArtwork,
            sizeof(g_BootCoastalArtworkInfo),
            &g_BootCoastalArtworkInfo);
        GetObjectA(
            g_BootCoastalSunbeamArtwork,
            sizeof(g_BootCoastalSunbeamArtworkInfo),
            &g_BootCoastalSunbeamArtworkInfo);
    }
    else
    {
        DeleteObject(g_BootCoastalSunbeamArtwork);
        DeleteObject(g_BootCoastalArtwork);
        DeleteObject(g_BootBuffJesusMenuArtwork);
        DeleteObject(g_BootMenuArtwork);
        g_BootCoastalSunbeamArtwork = 0;
        g_BootCoastalArtwork = 0;
        g_BootBuffJesusMenuArtwork = 0;
        g_BootMenuArtwork = 0;
    }
    // SPOOKY graveyard background for UI_FRONTEND_ABOUT_MENU: a 640x1920
    // 4-frame sheet + 640x1440 3-frame sunbeam, animated like forest/coastal.
    g_BootSpookyArtwork = LoadFrontendArtwork(
        instance,
        kBootSpookyResource,
        "visual_boot_spooky.bmp");
    g_BootSpookySunbeamArtwork = LoadFrontendArtwork(
        instance,
        kBootSpookySunbeamResource,
        "visual_boot_spooky_sunbeam.bmp");
    if (
        g_BootSpookyArtwork != 0 &&
        g_BootSpookySunbeamArtwork != 0)
    {
        GetObjectA(
            g_BootSpookyArtwork,
            sizeof(g_BootSpookyArtworkInfo),
            &g_BootSpookyArtworkInfo);
        GetObjectA(
            g_BootSpookySunbeamArtwork,
            sizeof(g_BootSpookySunbeamArtworkInfo),
            &g_BootSpookySunbeamArtworkInfo);
    }
    else
    {
        DeleteObject(g_BootSpookySunbeamArtwork);
        DeleteObject(g_BootSpookyArtwork);
        g_BootSpookySunbeamArtwork = 0;
        g_BootSpookyArtwork = 0;
    }
#if defined(FABLETLC_RETAIL_FRONTEND_SUBSCREENS)
    g_BootOptionsArtwork = LoadFrontendArtwork(
        instance,
        kBootOptionsResource,
        "visual_boot_options_menu.bmp");
    g_BootHelpersArtwork = LoadFrontendArtwork(
        instance,
        kBootHelpersResource,
        "visual_boot_helpers.bmp");
    g_BootRedefineScrollPagesArtwork = LoadFrontendArtwork(
        instance,
        kBootRedefineScrollPagesResource,
        "visual_boot_redefine_scroll_pages.bmp");
    g_BootTitleSegmentArtwork = LoadFrontendArtwork(
        instance,
        kBootTitleSegmentResource,
        "visual_boot_title_rule_segment.bmp");
    g_BootButtonLeftArtwork = LoadFrontendArtwork(
        instance,
        kBootButtonLeftResource,
        "visual_boot_options_button_left.bmp");
    g_BootButtonMiddleArtwork = LoadFrontendArtwork(
        instance,
        kBootButtonMiddleResource,
        "visual_boot_options_button_middle.bmp");
    g_BootButtonRightArtwork = LoadFrontendArtwork(
        instance,
        kBootButtonRightResource,
        "visual_boot_options_button_right.bmp");
    g_BootAboutArtwork = LoadFrontendArtwork(
        instance,
        kBootAboutResource,
        "visual_boot_about_menu.bmp");
    g_BootCreditsArtwork = LoadFrontendArtwork(
        instance,
        kBootCreditsResource,
        "visual_boot_credits_menu.bmp");
    g_BootProfilesArtwork = LoadFrontendArtwork(
        instance,
        kBootProfilesResource,
        "visual_boot_profiles_menu.bmp");
    if (
        g_BootOptionsArtwork != 0 &&
        g_BootHelpersArtwork != 0 &&
        g_BootRedefineScrollPagesArtwork != 0 &&
        g_BootTitleSegmentArtwork != 0 &&
        g_BootButtonLeftArtwork != 0 &&
        g_BootButtonMiddleArtwork != 0 &&
        g_BootButtonRightArtwork != 0 &&
        g_BootAboutArtwork != 0 &&
        g_BootCreditsArtwork != 0 &&
        g_BootProfilesArtwork != 0)
    {
        GetObjectA(
            g_BootOptionsArtwork,
            sizeof(g_BootOptionsArtworkInfo),
            &g_BootOptionsArtworkInfo);
        GetObjectA(
            g_BootHelpersArtwork,
            sizeof(g_BootHelpersArtworkInfo),
            &g_BootHelpersArtworkInfo);
        GetObjectA(
            g_BootRedefineScrollPagesArtwork,
            sizeof(g_BootRedefineScrollPagesArtworkInfo),
            &g_BootRedefineScrollPagesArtworkInfo);
        GetObjectA(
            g_BootTitleSegmentArtwork,
            sizeof(g_BootTitleSegmentArtworkInfo),
            &g_BootTitleSegmentArtworkInfo);
        GetObjectA(
            g_BootButtonLeftArtwork,
            sizeof(g_BootButtonLeftArtworkInfo),
            &g_BootButtonLeftArtworkInfo);
        GetObjectA(
            g_BootButtonMiddleArtwork,
            sizeof(g_BootButtonMiddleArtworkInfo),
            &g_BootButtonMiddleArtworkInfo);
        GetObjectA(
            g_BootButtonRightArtwork,
            sizeof(g_BootButtonRightArtworkInfo),
            &g_BootButtonRightArtworkInfo);
        GetObjectA(
            g_BootAboutArtwork,
            sizeof(g_BootAboutArtworkInfo),
            &g_BootAboutArtworkInfo);
        GetObjectA(
            g_BootCreditsArtwork,
            sizeof(g_BootCreditsArtworkInfo),
            &g_BootCreditsArtworkInfo);
        GetObjectA(
            g_BootProfilesArtwork,
            sizeof(g_BootProfilesArtworkInfo),
            &g_BootProfilesArtworkInfo);
    }
    else
    {
        DeleteObject(g_BootCreditsArtwork);
        DeleteObject(g_BootProfilesArtwork);
        DeleteObject(g_BootAboutArtwork);
        DeleteObject(g_BootButtonRightArtwork);
        DeleteObject(g_BootButtonMiddleArtwork);
        DeleteObject(g_BootButtonLeftArtwork);
        DeleteObject(g_BootTitleSegmentArtwork);
        DeleteObject(g_BootHelpersArtwork);
        DeleteObject(g_BootRedefineScrollPagesArtwork);
        DeleteObject(g_BootOptionsArtwork);
        g_BootAboutArtwork = 0;
        g_BootCreditsArtwork = 0;
        g_BootProfilesArtwork = 0;
        g_BootButtonRightArtwork = 0;
        g_BootButtonMiddleArtwork = 0;
        g_BootButtonLeftArtwork = 0;
        g_BootTitleSegmentArtwork = 0;
        g_BootHelpersArtwork = 0;
        g_BootRedefineScrollPagesArtwork = 0;
        g_BootOptionsArtwork = 0;
    }
#endif
#endif

    FableBitmapInfo emptyAnimationArtwork = {};
    const bool useFrontendAnimation =
        commandLine == 0 ||
        strstr(commandLine, "--retail-frontend-static") == 0;
    const bool haveFrontendAnimation =
        g_BootForestArtworkInfo.pixels != 0 &&
        g_BootSunbeamArtworkInfo.pixels != 0;
    const FableBitmapInfo& forestArtworkInfo =
        haveFrontendAnimation
            ? g_BootForestArtworkInfo
            : emptyAnimationArtwork;
    const FableBitmapInfo& sunbeamArtworkInfo =
        haveFrontendAnimation
            ? g_BootSunbeamArtworkInfo
            : emptyAnimationArtwork;
    const bool useBuffJesusMenu =
        commandLine != 0 &&
        strstr(commandLine, "--buff-jesus") != 0 &&
        g_BootBuffJesusMenuArtwork != 0;
    const FableBitmapInfo& menuArtworkInfo =
        useBuffJesusMenu
            ? g_BootBuffJesusMenuArtworkInfo
            : g_BootMenuArtworkInfo;
    if (FableInitialiseVisualD3D9(
            window,
            frontendClientWidth,
            frontendClientHeight,
            g_BootArtworkInfo.width,
            g_BootArtworkInfo.height,
            g_BootArtworkInfo.widthBytes,
            g_BootArtworkInfo.bitsPerPixel,
            g_BootArtworkInfo.pixels,
            g_BootTitleArtworkInfo.width,
            g_BootTitleArtworkInfo.height,
            g_BootTitleArtworkInfo.widthBytes,
            g_BootTitleArtworkInfo.bitsPerPixel,
            g_BootTitleArtworkInfo.pixels,
            forestArtworkInfo.width,
            forestArtworkInfo.height,
            forestArtworkInfo.widthBytes,
            forestArtworkInfo.bitsPerPixel,
            forestArtworkInfo.pixels,
            sunbeamArtworkInfo.width,
            sunbeamArtworkInfo.height,
            sunbeamArtworkInfo.widthBytes,
            sunbeamArtworkInfo.bitsPerPixel,
            sunbeamArtworkInfo.pixels,
            menuArtworkInfo.width,
            menuArtworkInfo.height,
            menuArtworkInfo.widthBytes,
            menuArtworkInfo.bitsPerPixel,
            menuArtworkInfo.pixels,
            g_BootCoastalArtworkInfo.width,
            g_BootCoastalArtworkInfo.height,
            g_BootCoastalArtworkInfo.widthBytes,
            g_BootCoastalArtworkInfo.bitsPerPixel,
            g_BootCoastalArtworkInfo.pixels,
            g_BootCoastalSunbeamArtworkInfo.width,
            g_BootCoastalSunbeamArtworkInfo.height,
            g_BootCoastalSunbeamArtworkInfo.widthBytes,
            g_BootCoastalSunbeamArtworkInfo.bitsPerPixel,
            g_BootCoastalSunbeamArtworkInfo.pixels,
            g_BootOptionsArtworkInfo.width,
            g_BootOptionsArtworkInfo.height,
            g_BootOptionsArtworkInfo.widthBytes,
            g_BootOptionsArtworkInfo.bitsPerPixel,
            g_BootOptionsArtworkInfo.pixels,
            g_BootHelpersArtworkInfo.width,
            g_BootHelpersArtworkInfo.height,
            g_BootHelpersArtworkInfo.widthBytes,
            g_BootHelpersArtworkInfo.bitsPerPixel,
            g_BootHelpersArtworkInfo.pixels,
            g_BootTitleSegmentArtworkInfo.width,
            g_BootTitleSegmentArtworkInfo.height,
            g_BootTitleSegmentArtworkInfo.widthBytes,
            g_BootTitleSegmentArtworkInfo.bitsPerPixel,
            g_BootTitleSegmentArtworkInfo.pixels,
            g_BootButtonLeftArtworkInfo.width,
            g_BootButtonLeftArtworkInfo.height,
            g_BootButtonLeftArtworkInfo.widthBytes,
            g_BootButtonLeftArtworkInfo.bitsPerPixel,
            g_BootButtonLeftArtworkInfo.pixels,
            g_BootButtonMiddleArtworkInfo.width,
            g_BootButtonMiddleArtworkInfo.height,
            g_BootButtonMiddleArtworkInfo.widthBytes,
            g_BootButtonMiddleArtworkInfo.bitsPerPixel,
            g_BootButtonMiddleArtworkInfo.pixels,
            g_BootButtonRightArtworkInfo.width,
            g_BootButtonRightArtworkInfo.height,
            g_BootButtonRightArtworkInfo.widthBytes,
            g_BootButtonRightArtworkInfo.bitsPerPixel,
            g_BootButtonRightArtworkInfo.pixels,
            g_BootAboutArtworkInfo.width,
            g_BootAboutArtworkInfo.height,
            g_BootAboutArtworkInfo.widthBytes,
            g_BootAboutArtworkInfo.bitsPerPixel,
            g_BootAboutArtworkInfo.pixels,
            g_BootCreditsArtworkInfo.width,
            g_BootCreditsArtworkInfo.height,
            g_BootCreditsArtworkInfo.widthBytes,
            g_BootCreditsArtworkInfo.bitsPerPixel,
            g_BootCreditsArtworkInfo.pixels,
            g_BootProfilesArtworkInfo.width,
            g_BootProfilesArtworkInfo.height,
            g_BootProfilesArtworkInfo.widthBytes,
            g_BootProfilesArtworkInfo.bitsPerPixel,
            g_BootProfilesArtworkInfo.pixels,
            g_BootSpookyArtworkInfo.width,
            g_BootSpookyArtworkInfo.height,
            g_BootSpookyArtworkInfo.widthBytes,
            g_BootSpookyArtworkInfo.bitsPerPixel,
            g_BootSpookyArtworkInfo.pixels,
            g_BootSpookySunbeamArtworkInfo.width,
            g_BootSpookySunbeamArtworkInfo.height,
            g_BootSpookySunbeamArtworkInfo.widthBytes,
            g_BootSpookySunbeamArtworkInfo.bitsPerPixel,
            g_BootSpookySunbeamArtworkInfo.pixels,
            g_BootRedefineScrollPagesArtworkInfo.width,
            g_BootRedefineScrollPagesArtworkInfo.height,
            g_BootRedefineScrollPagesArtworkInfo.widthBytes,
            g_BootRedefineScrollPagesArtworkInfo.bitsPerPixel,
            g_BootRedefineScrollPagesArtworkInfo.pixels))
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

    FableSetVisualFrontendAnimationStatic(
        !useFrontendAnimation && haveFrontendAnimation);

    ShowWindow(window, showCommand == 0 ? kShowNormal : showCommand);
    UpdateWindow(window);
#if defined(FABLETLC_RETAIL_FRONTEND_ANIMATION)
    SetTimer(window, kFrontendAnimationTimer, 50, 0);
#endif
    if (FableWasVisualD3D9Presented())
    {
#if defined(FABLETLC_RETAIL_FRONTEND_ARTWORK)
        if (useBuffJesusMenu)
        {
            SetWindowTextA(
                window,
                kD3DBuffJesusPresentedWindowTitle);
        }
        else
        {
            SetWindowTextA(
                window,
                g_RetailProgressDisplayActive
                    ? kD3DRetailAssetPresentedActiveWindowTitle
                    : kD3DRetailAssetPresentedReadyWindowTitle);
        }
#else
        SetWindowTextA(
            window,
            g_RetailProgressDisplayActive
                ? kD3DCheckpointPresentedActiveWindowTitle
                : kD3DCheckpointPresentedReadyWindowTitle);
#endif
    }

    if (retailVideoRequested)
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
            RevealVisualFrontend(window);
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
#if defined(FABLETLC_RETAIL_FRONTEND_MENU)
#if defined(FABLETLC_RETAIL_FRONTEND_SUBSCREENS)
    DeleteObject(g_BootButtonRightArtwork);
    DeleteObject(g_BootButtonMiddleArtwork);
    DeleteObject(g_BootButtonLeftArtwork);
    DeleteObject(g_BootTitleSegmentArtwork);
    DeleteObject(g_BootHelpersArtwork);
    DeleteObject(g_BootRedefineScrollPagesArtwork);
    DeleteObject(g_BootOptionsArtwork);
    g_BootButtonRightArtwork = 0;
    g_BootButtonMiddleArtwork = 0;
    g_BootButtonLeftArtwork = 0;
    g_BootTitleSegmentArtwork = 0;
    g_BootHelpersArtwork = 0;
    g_BootRedefineScrollPagesArtwork = 0;
    g_BootOptionsArtwork = 0;
#endif
    DeleteObject(g_BootCoastalSunbeamArtwork);
    DeleteObject(g_BootCoastalArtwork);
    DeleteObject(g_BootBuffJesusMenuArtwork);
    DeleteObject(g_BootMenuArtwork);
    g_BootCoastalSunbeamArtwork = 0;
    g_BootCoastalArtwork = 0;
    g_BootBuffJesusMenuArtwork = 0;
    g_BootMenuArtwork = 0;
#endif
    DeleteObject(g_BootArtwork);
    g_BootArtwork = 0;
#if defined(FABLETLC_RETAIL_FRONTEND_ARTWORK)
    DeleteObject(g_BootTitleArtwork);
    g_BootTitleArtwork = 0;
#if defined(FABLETLC_RETAIL_FRONTEND_ANIMATION)
    DeleteObject(g_BootSunbeamArtwork);
    DeleteObject(g_BootForestArtwork);
    g_BootSunbeamArtwork = 0;
    g_BootForestArtwork = 0;
#endif
#endif
    return static_cast<long>(message.wordParameter);
}
