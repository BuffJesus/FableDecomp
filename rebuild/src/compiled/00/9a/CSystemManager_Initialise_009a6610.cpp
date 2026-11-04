// CSystemManager::Initialise (0x9A6610, 998 bytes). Source shape follows the
// FableWin debug build's body (0x2F89480) with its debug-only asserts, error
// dialogs and CDebugManager dropped; retail adds the CSoundManagerLH slot and
// builds CProfileManager after the base-class guard.
#include "fable_system_manager.h"

extern "C" __declspec(dllimport) void* FABLE_STDCALL ConvertThreadToFiber(
    void* parameter);
extern "C" __declspec(dllimport) long FABLE_STDCALL CoInitialize(
    void* reserved);
extern "C" __declspec(dllimport) void FABLE_STDCALL InitializeCriticalSection(
    void* criticalSection);
extern "C" __declspec(dllimport) void* FABLE_STDCALL GetCurrentProcess();

// Private helpers the original CSystemManager.cpp defines beside Initialise;
// retail inlines all four here.
inline bool CSystemManager::InitFromExternalApplicationWindow(HWND__* window)
{
    ApplicationWinHandle = window;
    return true;
}

inline void CSystemManager::InitTimers()
{
}

inline void CSystemManager::InitMaths()
{
    GFInitMaths();
    NVectorMath::GFInitVectorMath();
}

inline void CSystemManager::SetAsActive(bool active)
{
    if (active && !ApplicationActive && EverBeenActive == true)
        Restore = true;
    ApplicationActive = active;
    EverBeenActive = true;
}

ESystemCode CSystemManager::Initialise(CSystemManagerInit* init)
{
    WindowInitialised = false;
    DDrawInitialised = false;
    WindowValid = false;
    RestoreExclusive = false;
    Restore = false;
    EverBeenActive = false;
    WaitWhileInactive = init->WaitWhileInactive;
    MouseZMove = 0;
    ForceMusicPlay = false;
    ForceMusicPlayOffset = 0.0f;
    AttachedToExternalWindow = init->AttachToExternalWindow;
    PreventExclusiveModeChanges = init->PreventExclusiveModeChanges;
    ExclusiveMode = init->ExclusiveMode;
    TempPath = init->TempPath;
    IMEMessageFunc = init->IMEMessageFunc;
    MessageFilter = init->MessageFilter;

    if (CInitBaseClass::Initialise() == true)
    {
        MainFibre = ConvertThreadToFiber(this);
        CoInitialize(0);
        InitializeCriticalSection(CriticalSection);
        PreferPrimaryDevice = init->PreferPrimaryDisplayDevice;
        WinInstanceHandle = init->InstanceHandle;
        WinCommandLine = init->CommandLine;
        WinShow = init->WindowsShow;
        WinAppName = init->AppName;
        InitGUIDFromName(init->Identifier);

        if (init->IsErrorFailureFunc == 0)
            ErrorFunc = DefaultErrorFunc;
        else
            ErrorFunc = init->IsErrorFailureFunc;

        ApplicationActive = false;
        InitFlags = init->Flags;
        AppReinitFunc = init->AppReinitFunc;
        AppReinitContext = init->AppReinitContext;
        UsingAccurateTimer = init->UseAccurateTimer;
        InitTimers();

        if (init->Flags & 4)
        {
            if (!AttachedToExternalWindow)
            {
                if (!InitWindow(
                        init->DisplayManagerInit.Width,
                        init->DisplayManagerInit.Height))
                {
                    Uninitialise();
                    return kSystemCodeWindowInitialisationFailed;
                }
                WindowInitialised = true;
            }
            else if (InitFromExternalApplicationWindow(
                         static_cast<HWND__*>(init->ApplicationWindowHandle)) == true)
            {
                WindowInitialised = true;
            }
        }

        void* process = GetCurrentProcess();

        if (init->ScratchBufferSize > 0)
            GScratchBuffer.Initialise(init->ScratchBufferSize);

        if (init->Flags & 4)
        {
            CDisplayManagerInit displayInit(init->DisplayManagerInit);
            displayInit.WindowHandle = ApplicationWinHandle;
            displayInit.SkipConfigDetection = init->SkipConfigDetection;
            PDisplayManager.Reset(new CDisplayManager(*this));
            PDisplayManager.Data->Init(displayInit, ExclusiveMode);
        }

        if (init->Flags & 1)
        {
            PInputManager.reset(
                new CInputManagerDX(init->InputManagerInit, ExclusiveMode));
        }

        CTBaseSingletonNoDefaultAlloc<CDriveManager>::Set(new CDriveManager);
        const ESystemCode driveCode =
            CTBaseSingletonNoDefaultAlloc<CDriveManager>::Get().Initialise(
                this);
        if (driveCode != kSystemCodeSuccess)
        {
            Uninitialise();
            return driveCode;
        }

        if (init->Flags & 0x10)
        {
            PSoundManager.Reset(
                new CSoundManagerLH(init->SoundManagerInit, ExclusiveMode));
        }

        if (init->FontBankHandle.GetLength() != 0 ||
            init->StreamingFontBankHandle.GetLength() != 0)
        {
            PFontManager.reset(new CFontManager(
                init->FontBankHandle,
                init->StreamingFontBankHandle));
        }

        PGraphicBankManager.reset(new CGraphicBankManager);
        PMeshBankManager.reset(new CMeshBankManager);

        if (PDisplayManager.Data != 0)
        {
            PDisplayManager.Data->AddDeviceResetCallback(
                PMeshBankManager.Data);
        }
    }

    ProfileManager.Reset(new CProfileManager);
    InitMaths();
    InitSystemColours();
    ++NoInstances;
    InitApplicationName();
    SetAsActive(true);
    return kSystemCodeSuccess;
}
