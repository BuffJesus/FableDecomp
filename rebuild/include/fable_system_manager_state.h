#pragma once

#include "fable_counted_pointer.h"
#include "fable_system.h"
#include "fable_window_class.h"
#include "fable_gui_colours.h"

#include "fable_init_base_class.h"

// FableWin.pdb enum ESystemReturn (CSystemManager::Update's result).
enum ESystemReturn
{
    ESR_OK = 1,
    ESR_QUIT = 2,
    ESR_SYSTEM_LOCK = 3
};

class CDebugManager;
class CInputManager;
class CDisplayManager;
class CGraphicBankManager;
class CMeshBankManager;
class CASoundManager;
class CFontManager;
class CProfileManager;

// Retail layout (0x180). Names are Ego_r.pdb's; retail inserts PSoundManager
// at +0x7C, so every Ego_r offset from PFontManager on is +8 here.
class CSystemManager : public CInitBaseClass
{
public:
    CSystemManager();
    virtual ~CSystemManager();

    ESystemCode Initialise(CSystemManagerInit* init);
    void Uninitialise();
    void DisplayCriticalMessage(const char* message);
    void OnPreDeviceReset(); // 009A52B0
    bool OnPostDeviceReset(); // 009A52C0

    // FableWin ?Update@CSystemManager@@QAE?AW4ESystemReturn@@XZ (0x009A6460).
    ESystemReturn Update();

    // FableWin ?IsApplicationActive@CSystemManager@@QAE_NXZ (0x009A57B0,
    // out of line; CMainGameComponent::UpdateRegular 0x004162B5 calls it).
    bool IsApplicationActive();

    // Ego_r ?IsApplicationInputFocus@CSystemManager@@QAE_NXZ (retail 0x009A4FB0, out of line; the DirectInput
    // devices ask it when re-acquiring).
    bool IsApplicationInputFocus();
    // Ego_r ?GetApplicationWinHandle@CSystemManager@@QAEPAUHWND__@@XZ, inline in retail (CMouseDX::SetPos 0x00AB4700).
    void* GetApplicationWinHandle()
    {
        return ApplicationWinHandle;
    }

    // Ego_r ?GetSize@CSystemManager@@QBEJXZ (retail 0x009A5010, sizeof),
    // ?GetDebugManager@CSystemManager@@QAEPAVCDebugManager@@XZ (0x009A52D0) and
    // ?GetApplicationName@CSystemManager@@QBE?AVCCharString@@XZ (0x009A5460).
    long GetSize() const;
    // Ego_r ?ExceptionHandler@CSystemManager@@QAEHH@Z, retail 0x009A5020.
    int ExceptionHandler(int code);
    CDebugManager* GetDebugManager();
    CCharString GetApplicationName() const;

    // FableWin ?UpdateActive@CSystemManager@@QAEXXZ (retail 0x009A6370): pump
    // messages, track focus, and block while inactive when asked to.
    void UpdateActive();

    // FableWin ?IsAttachedToExternalWindow@CSystemManager@@QBE_NXZ; inline in
    // retail (UpdateActive 0x009A6370 reads +0xEC).
    bool IsAttachedToExternalWindow() const
    {
        return AttachedToExternalWindow;
    }

    // FableWin ?SetExclusiveMode@CSystemManager@@QAE_N_N@Z (retail 0x009A59A0;
    // caller CMainGameComponent::SetExclusiveMode 0x004163B7).
    bool SetExclusiveMode(bool exclusive);

    // FableWin ?GetForceMusicPlay@CSystemManager@@QBE_NAAM@Z and
    // ?ResetForceMusicPlay@CSystemManager@@QAEXXZ; inline in retail
    // (CNewFrontendGameComponent::Run 0x0042EC7C).
    bool GetForceMusicPlay(float& offset) const
    {
        offset = ForceMusicPlayOffset;
        return ForceMusicPlay;
    }

    void ResetForceMusicPlay()
    {
        ForceMusicPlayOffset = 0.0f;
        ForceMusicPlay = false;
    }

    // FableWin ?GetDisplayManager@CSystemManager@@QAEPAVCDisplayManager@@XZ
    // (lib_system_manager.hpp:206); inline in retail (`mov ecx,[eax+0x60]`).
    CDisplayManager* GetDisplayManager()
    {
        return PDisplayManager.Data;
    }

    // FableWin ?GetProfileManager@CSystemManager@@QAEPAVCProfileManager@@XZ;
    // inline in retail (CMainGameComponent::Run 0x004189C2 reads +0xF8).
    CProfileManager* GetProfileManager()
    {
        return ProfileManager.Data;
    }

private:
    friend struct CSystemManagerTestAccess;
    static bool FABLE_FASTCALL DefaultErrorFunc(ESystemCode code);
    void InitGUIDFromName(const char* name);
    // FableWin WinProcedure(HWND__*, uint, uint, long), retail 009A5B60.
    static void FABLE_FASTCALL ActivateWindow(CSystemManager*, bool);
    static long FABLE_STDCALL WinProcedure(HWND__*, unsigned int, unsigned int, long);
    bool InitWindow(long width, long height);
    // Ego_r ?InitFromExternalApplicationWindow@CSystemManager@@AAE_NPAUHWND__@@@Z,
    // out of line at 0x009A5240 (Initialise 0x009A6610 expands it);
    // ?UninitFromExternalWindow@CSystemManager@@AAEXXZ, retail 0x009A5250.
    bool InitFromExternalApplicationWindow(HWND__* window);
    void UninitFromExternalWindow();
    // Ego_r ?UninitWindow@CSystemManager@@AAEXXZ, retail 0x009A5210.
    void UninitWindow();
    void InitTimers();
    void InitMaths();
    void InitSystemColours();
    void InitApplicationName();
    void SetAsActive(bool active);
    // FableWin ?SetHasInputFocus@CSystemManager@@AAEX_N@Z (retail 0x009A5A30) and
    // ?ProcessWindowMessages@CSystemManager@@AAEXXZ (retail 0x009A4F20).
    void SetHasInputFocus(bool hasFocus);
    void ProcessWindowMessages();

public:
    bool WindowsQuit;                   // +0x08
    bool ApplicationActive;             // +0x09
    bool ApplicationHasInputFocus;      // +0x0A
    fable_u32 ApplicationGUID[4];      // +0x0C _GUID
    fable_u32 BestGUID[4];             // +0x1C _GUID
    void* PBestGUID;                    // +0x2C
    bool PreferPrimaryDevice;           // +0x30
    fable_u32 CriticalSection[6];       // +0x34 _RTL_CRITICAL_SECTION
    void* MainFibre;                    // +0x4C
    CCountedPointer<CDebugManager> LibDebugManager;
    CCountedPointer<CInputManager> PInputManager;
    CCountedPointer<CDisplayManager> PDisplayManager;
    CDebugManager* DebugManager;
    CCountedPointer<CGraphicBankManager> PGraphicBankManager;
    CCountedPointer<CMeshBankManager> PMeshBankManager;
    // Retail-only slot (not in Ego_r); Initialise resets it to a new
    // CSoundManagerLH. Name not recovered.
    CCountedPointer<CASoundManager> PSoundManager;
    CCountedPointer<CFontManager> PFontManager;
    bool WindowInitialised;             // +0x8C
    bool DDrawInitialised;
    bool ExclusiveMode;
    void* WinInstanceHandle;            // +0x90
    void* ApplicationWinHandle;
    int WinShow;
    char* WinCommandLine;
    CWideString WinAppName;             // +0xA0
    FableWindowClass WinClass;            // +0xA4 tagWNDCLASSEXW
    bool ResolutionSet;                 // +0xD4
    unsigned long InitFlags;            // +0xD8
    bool EverBeenActive;                // +0xDC
    bool MouseButton0Pressed;
    bool MouseButton1Pressed;
    bool MouseButton2Pressed;
    bool MouseButton3Pressed;
    bool MouseButton4Pressed;
    long MouseZMove;                    // +0xE4
    bool WindowValid;                   // +0xE8
    bool RestoreExclusive;
    bool Restore;
    bool WaitWhileInactive;
    bool AttachedToExternalWindow;
    bool PreventExclusiveModeChanges;
    bool UsingAccurateTimer;
    // FableWin ?SetAppReinitFunc@CSystemManager@@QAEXP6IXPAX@Z0@Z: the
    // callback is __fastcall (P6I). Inline in retail (CMainGameComponent::Init
    // 0x004184BD stores both fields directly). The field below keeps its
    // older cdecl spelling (landed CSystemManager::Initialise tests depend on
    // it), hence the cast; retyping it to FABLE_FASTCALL is a follow-up.
    void SetAppReinitFunc(void (FABLE_FASTCALL *func)(void*), void* context)
    {
        AppReinitFunc = reinterpret_cast<void (*)(void*)>(func);
        AppReinitContext = context;
    }

    void (*AppReinitFunc)(void*);       // +0xF0
    void* AppReinitContext;
    CCountedPointer<CProfileManager> ProfileManager;    // +0xF8
    CGuiColoursInfo SystemColours;       // +0x100 CGuiColoursInfo
    CWideString TempPath;               // +0x164
    CCharString ApplicationName;
    bool ForceMusicPlay;                // +0x16C
    float ForceMusicPlayOffset;
    bool (FABLE_FASTCALL *ErrorFunc)(ESystemCode);      // +0x174
    FableIMEMessageFunction IMEMessageFunc;
    FableWindowMessageFilter MessageFilter;

    static fable_u8 NoInstances;        // 0x13CA608
};

FABLE_STATIC_ASSERT(offsetof(CSystemManager, WindowsQuit) == 0x08);
FABLE_STATIC_ASSERT(offsetof(CSystemManager, CriticalSection) == 0x34);
FABLE_STATIC_ASSERT(offsetof(CSystemManager, PInputManager) == 0x58);
FABLE_STATIC_ASSERT(offsetof(CSystemManager, PDisplayManager) == 0x60);
FABLE_STATIC_ASSERT(offsetof(CSystemManager, PGraphicBankManager) == 0x6C);
FABLE_STATIC_ASSERT(offsetof(CSystemManager, PSoundManager) == 0x7C);
FABLE_STATIC_ASSERT(offsetof(CSystemManager, PFontManager) == 0x84);
FABLE_STATIC_ASSERT(offsetof(CSystemManager, WinAppName) == 0xA0);
FABLE_STATIC_ASSERT(offsetof(CSystemManager, InitFlags) == 0xD8);
FABLE_STATIC_ASSERT(offsetof(CSystemManager, MouseZMove) == 0xE4);
FABLE_STATIC_ASSERT(offsetof(CSystemManager, UsingAccurateTimer) == 0xEE);
FABLE_STATIC_ASSERT(offsetof(CSystemManager, ProfileManager) == 0xF8);
FABLE_STATIC_ASSERT(offsetof(CSystemManager, TempPath) == 0x164);
FABLE_STATIC_ASSERT(offsetof(CSystemManager, ForceMusicPlayOffset) == 0x170);
FABLE_STATIC_ASSERT(offsetof(CSystemManager, MessageFilter) == 0x17C);
FABLE_STATIC_ASSERT(sizeof(CSystemManager) == 0x180);

