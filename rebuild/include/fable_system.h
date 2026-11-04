#pragma once

#include <stddef.h>

#include "fable_string.h"

struct HWND__;
struct tagMSG;
typedef bool (FABLE_FASTCALL* FableIMEMessageFunction)(
    HWND__*, unsigned int, unsigned int, long, long&);
typedef bool (FABLE_FASTCALL* FableWindowMessageFilter)(tagMSG*);

enum ESystemCode
{
    kSystemCodeSuccess = 1,
    kSystemCodeWindowInitialisationFailed = 4
};

bool FABLE_FASTCALL GFIsSystemErrorFailure(ESystemCode code);

// Retail (PC) layouts. Member names are FableWin.pdb's; offsets come from the
// retail constructors 0x00403B10 and 0x004016F0 (retail strings are 4 bytes).
class CDisplayManagerInit
{
public:
    CDisplayManagerInit();

    long Width;
    long Height;
    long Depth;
    long RefreshRate;
    bool UseRenderManager;
    bool UseShaderRenderManager;
    void* WindowHandle;
    fable_u8 ZBufferDepth;
    fable_u8 NoBackBuffers;
    long MultisampleType;
    long PushBufferSize;
    long KickOffSize;
    bool PresentImmediate;
    bool PresentImmediatelyIfMissedVSync;
    long PresentationIntevalVSyncCount;
    bool SkipConfigDetection;
    unsigned long RenderManagerInitFlags;
    CWideString ShaderResourcePath;
};

// Retail-only; CSystemManager::Initialise hands it to the CSoundManagerLH
// constructor. No PDB names it or its fields.
class CSoundManagerInit
{
public:
    CSoundManagerInit()
    {
        unknown04 = 64;
        unknown00 = 128;
        unknown08 = 64.0f;
        unknown0C = 2.0f;
        unknown10 = 1.0f;
        unknown18 = 0;
        unknown14 = 0;
        unknown1C = 0x40000;
    }

    fable_u32 unknown00;
    fable_u32 unknown04;
    float unknown08;
    float unknown0C;
    float unknown10;
    fable_u32 unknown14;
    fable_u32 unknown18;
    fable_u32 unknown1C;
};

class CInputManagerInit
{
public:
    CInputManagerInit()
    {
        UseMouse = false;
        UseJoystick = false;
        UseKeyboard = false;
    }

    bool UseMouse;
    bool UseJoystick;
    bool UseKeyboard;
};

class CSystemManagerInit
{
public:
    CSystemManagerInit();

    void* InstanceHandle;
    char* CommandLine;
    int WindowsShow;
    CWideString AppName;
    bool ExclusiveMode;
    bool PreferPrimaryDisplayDevice;
    unsigned long Flags;
    bool PreventHardwareAcceleration;
    bool AttachToExternalWindow;
    void* ApplicationWindowHandle;
    bool PreventExclusiveModeChanges;
    bool UseAccurateTimer;
    void (*AppReinitFunc)(void*);
    void* AppReinitContext;
    CCharString FontBankHandle;
    CCharString StreamingFontBankHandle;
    bool WaitWhileInactive;
    bool SkipConfigDetection;
    CSoundManagerInit SoundManagerInit;
    CInputManagerInit InputManagerInit;
    CDisplayManagerInit DisplayManagerInit;
    bool UseTripleBuffer;
    // FableWin: CSystemManager::DefaultErrorFunc / GFIsSystemErrorFailure are
    // __fastcall bool(ESystemCode).
    bool (FABLE_FASTCALL *IsErrorFailureFunc)(ESystemCode);
    char Identifier[17];
    long ScratchBufferSize;
    CWideString LogfileName;
    CWideString TempPath;
    FableIMEMessageFunction IMEMessageFunc;
    FableWindowMessageFilter MessageFilter;
};

void FABLE_FASTCALL FableConstructWideString(CWideString* value);
void FABLE_FASTCALL FableConstructCharString(CCharString* value);
const CWideString& FABLE_FASTCALL FableAssignWideString(
    CWideString* value,
    void* unused,
    const wchar_t* text);
CSystemManagerInit* FABLE_FASTCALL
FableGetSystemManagerInitBoundary();

FABLE_STATIC_ASSERT(offsetof(CDisplayManagerInit, ShaderResourcePath) == 0x38);
FABLE_STATIC_ASSERT(sizeof(CDisplayManagerInit) == 0x3C);
FABLE_STATIC_ASSERT(sizeof(CSoundManagerInit) == 0x20);
FABLE_STATIC_ASSERT(offsetof(CSystemManagerInit, AppName) == 0x0C);
FABLE_STATIC_ASSERT(offsetof(CSystemManagerInit, FontBankHandle) == 0x2C);
FABLE_STATIC_ASSERT(offsetof(CSystemManagerInit, SoundManagerInit) == 0x38);
FABLE_STATIC_ASSERT(offsetof(CSystemManagerInit, InputManagerInit) == 0x58);
FABLE_STATIC_ASSERT(offsetof(CSystemManagerInit, DisplayManagerInit) == 0x5C);
FABLE_STATIC_ASSERT(offsetof(CSystemManagerInit, IsErrorFailureFunc) == 0x9C);
FABLE_STATIC_ASSERT(offsetof(CSystemManagerInit, Identifier) == 0xA0);
FABLE_STATIC_ASSERT(offsetof(CSystemManagerInit, ScratchBufferSize) == 0xB4);
FABLE_STATIC_ASSERT(offsetof(CSystemManagerInit, LogfileName) == 0xB8);
FABLE_STATIC_ASSERT(sizeof(CSystemManagerInit) == 0xC8);
