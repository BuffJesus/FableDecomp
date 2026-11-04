#include <new>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fable_game.h"
#include "engine/CDisplayEngine.h"
#include "engine/CGamePlayerInterface.h"
#include "engine/CPlayerManager.h"
#include "engine/CWorld.h"
#include "test_support/init_base_class_stubs.h"

class CFileInformation
{
};

CDisplayManager* g_FableDisplayManager_013B8390;

namespace
{
    // Every recorded event gets the next sequence number.
    int g_sequence;
    int g_baseAt;
    int g_interpolationAt;
    int g_eventPackagesAt;
    int g_clientAt;
    int g_fileAt;
    int g_smootherAt;
    int g_fpsCounterAt;
    int g_initCopyAt;
    int g_registerAt;
    int g_registerCalls;
    const void* g_baseThis;
    CGame* g_baseGame;
    const void* g_interpolationThis;
    const void* g_eventPackagesThis;
    const void* g_clientThis;
    const void* g_fileThis;
    const void* g_smootherThis;
    const void* g_fpsCounterThis;
    const void* g_registerManager;
    const CDeviceResetCallback* g_registerCallback;
    unsigned long g_forceRenderedAtRegister;
    unsigned long g_noFramesAtRegister;
    int g_wideCopies;
    int g_charCopies;
    const void* g_wideCopySources[4];
    const void* g_charCopySource;
    const void* g_wideCopyTargets[4];
    const void* g_charCopyTarget;
    const unsigned char* g_component;

    unsigned long Dword(const unsigned char* p, unsigned long offset)
    {
        unsigned long value;
        memcpy(&value, p + offset, sizeof(value));
        return value;
    }

    bool AllBytes(
        const unsigned char* p,
        unsigned long begin,
        unsigned long end,
        unsigned char value)
    {
        for (unsigned long i = begin; i < end; ++i)
        {
            if (p[i] != value)
            {
                return false;
            }
        }
        return true;
    }
}

// Recorders for the real callees.
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}

void CDeviceResetCallback::OnPreDeviceReset() {}
bool CDeviceResetCallback::OnPostDeviceReset() { return true; }
void CGameComponent::SetQuit() {}

CGameComponent::CGameComponent(CGame& game)
{
    g_baseAt = ++g_sequence;
    g_baseThis = this;
    g_baseGame = &game;
    Game = &game;
}
// ~CGameComponent is inline and empty (retail 0x00415940); no stand-in.
void CGameComponent::Init() {}
bool CGameComponent::Run(CGameComponent**) { return false; }
CMainGameComponent::~CMainGameComponent() {}

CInterpolationInfo::CInterpolationInfo()
{
    g_interpolationAt = ++g_sequence;
    g_interpolationThis = this;
}

CGameEventPackageSet::CGameEventPackageSet()
{
    g_eventPackagesAt = ++g_sequence;
    g_eventPackagesThis = this;
}

CNetworkClient::CNetworkClient()
{
    g_clientAt = ++g_sequence;
    g_clientThis = this;
}

CFrameRateSmoother::CFrameRateSmoother()
{
    g_smootherAt = ++g_sequence;
    g_smootherThis = this;
}

CRoughFrameCounter::CRoughFrameCounter()
{
    g_fpsCounterAt = ++g_sequence;
    g_fpsCounterThis = this;
}

void CDisplayManager::AddDeviceResetCallback(CDeviceResetCallback* callback)
{
    g_registerAt = ++g_sequence;
    ++g_registerCalls;
    g_registerManager = this;
    g_registerCallback = callback;
    g_forceRenderedAtRegister = Dword(g_component, 0x50);
    g_noFramesAtRegister = Dword(g_component, 0x161E4);
}

CAFile::CAFile()
{
    g_fileAt = ++g_sequence;
    g_fileThis = this;
}

CFileInformation CAFile::GetFileInformation() { return CFileInformation(); }

void CDiskFileWin32::Open(const CWideString&, FileAccessType, unsigned long) {}
void CDiskFileWin32::Close() {}
void CDiskFileWin32::Read(void*, long, CASuspendableProcess*) {}
void CDiskFileWin32::Write(const void*, long, CASuspendableProcess*) {}
void CDiskFileWin32::SetPosition(long) {}
void CDiskFileWin32::MovePosition(long) {}
long CDiskFileWin32::GetPosition() const { return 0; }
void CDiskFileWin32::SetLength(long) {}
CAFile* CDiskFileWin32::Clone() const { return 0; }
CFileInformation CDiskFileWin32::GetFileInformation() { return CFileInformation(); }

CWideString::CWideString() : storage_(0) {}
CWideString::CWideString(const CWideString& other) : storage_(other.storage_)
{
    if (g_wideCopies < 4)
    {
        g_wideCopySources[g_wideCopies] = &other;
        g_wideCopyTargets[g_wideCopies] = this;
    }
    if (++g_wideCopies == 1)
    {
        g_initCopyAt = ++g_sequence;
    }
}
CWideString::~CWideString() {}

CCharString::CCharString() : storage_(0) {}
CCharString::CCharString(const CCharString& other) : storage_(other.storage_)
{
    ++g_charCopies;
    g_charCopySource = &other;
    g_charCopyTarget = this;
}
CCharString::~CCharString() {}

int main()
{
    const unsigned long size = sizeof(CMainGameComponent);
    unsigned char* storage = static_cast<unsigned char*>(malloc(size));
    memset(storage, 0xA5, size);
    g_component = storage;

    unsigned char managerStorage[4];
    g_FableDisplayManager_013B8390 =
        reinterpret_cast<CDisplayManager*>(managerStorage);

    unsigned char gameStorage[sizeof(CGame)];
    CGame& game = *reinterpret_cast<CGame*>(gameStorage);

    CMainGameComponentInit init;
    CMainGameComponent* component = new (storage) CMainGameComponent(game, init);
    const unsigned char* p = storage;
    float speed;
    float framerate;
    double renderStart;
    memcpy(&speed, p + 0x38, sizeof(speed));
    memcpy(&framerate, p + 0x3C, sizeof(framerate));
    memcpy(&renderStart, p + 0x161A8, sizeof(renderStart));

    const bool order =
        g_baseAt == 1 &&
        g_interpolationAt == 2 &&
        g_eventPackagesAt == 3 &&
        g_clientAt == 4 &&
        g_fileAt == 5 &&
        g_smootherAt == 6 &&
        g_fpsCounterAt == 7 &&
        g_initCopyAt == 8 &&
        g_registerAt == 9;

    const bool subObjects =
        g_baseThis == p &&
        g_baseGame == &game &&
        g_interpolationThis == p + 0x70 &&
        g_eventPackagesThis == p + 0xA4 &&
        g_clientThis == p + 0x13AB8 &&
        g_fileThis == p + 0x16154 &&
        g_smootherThis == p + 0x16178 &&
        g_fpsCounterThis == p + 0x161BC &&
        g_wideCopies == 3 &&
        g_charCopies == 1 &&
        g_wideCopySources[0] == &init.InitialWorldName &&
        g_wideCopyTargets[0] == p + 0x161D0 &&
        g_wideCopySources[1] == &init.InitialWorldHolySiteName &&
        g_wideCopyTargets[1] == p + 0x161D4 &&
        g_charCopySource == &init.InitialQuestName &&
        g_charCopyTarget == p + 0x161D8 &&
        g_wideCopySources[2] == &init.SaveGameName &&
        g_wideCopyTargets[2] == p + 0x161DC;

    // The component registers its CDeviceResetCallback base (this + 4)
    // after clearing LastGameTurnForceRendered and before clearing
    // DebugNoFramesUnableToRender.
    const bool registration =
        g_registerCalls == 1 &&
        g_registerManager == managerStorage &&
        reinterpret_cast<const unsigned char*>(g_registerCallback) == p + 4 &&
        static_cast<CDeviceResetCallback*>(component) == g_registerCallback &&
        g_forceRenderedAtRegister == 0 &&
        g_noFramesAtRegister == 0xA5A5A5A5UL;

    const bool fields =
        Dword(p, 0x00) != 0xA5A5A5A5UL &&
        Dword(p, 0x04) != 0xA5A5A5A5UL &&
        Dword(p, 0x00) != Dword(p, 0x04) &&
        AllBytes(p, 0x10, 0x14, 0xA5) &&
        AllBytes(p, 0x14, 0x34, 0x00) &&
        p[0x34] == 0 &&
        speed == 3.0f &&
        framerate == 30.0f &&
        Dword(p, 0x40) == 0 &&
        p[0x44] == 1 &&
        Dword(p, 0x48) == 0 &&
        Dword(p, 0x4C) == 0 &&
        Dword(p, 0x50) == 0 &&
        AllBytes(p, 0x54, 0x70, 0xA5) &&
        AllBytes(p, 0x16138, 0x16154, 0x00) &&
        p[0x16170] == 0 &&
        p[0x16171] == 0 &&
        AllBytes(p, 0x16174, 0x16178, 0xA5) &&
        renderStart == 0.0 &&
        AllBytes(p, 0x161B0, 0x161B8, 0xA5) &&
        p[0x161B8] == 0 &&
        AllBytes(p, 0x161C4, 0x161D0, 0x00) &&
        p[0x161E0] == 0 &&
        p[0x161E1] == 1 &&
        p[0x161E2] == 0 &&
        Dword(p, 0x161E4) == 0;

    if (component != reinterpret_cast<CMainGameComponent*>(storage) ||
        !order || !subObjects || !registration || !fields)
    {
        printf("order=%d subObjects=%d registration=%d fields=%d\n",
            order, subObjects, registration, fields);
        puts("FABLETLC_MAIN_GAME_COMPONENT_CTOR FAIL");
        return 1;
    }

    puts("FABLETLC_MAIN_GAME_COMPONENT_CTOR PASS");
    return 0;
}
// Explicit concrete dispatch recorders (retail primary slots 1-4, secondary 0-1).
void CMainGameComponent::Init() {}
bool CMainGameComponent::Run(CGameComponent**) { return false; }
void CMainGameComponent::ChangeTextureColourDepth(long) {}
void CMainGameComponent::SetQuit() {}
void CMainGameComponent::OnPreDeviceReset() {}
bool CMainGameComponent::OnPostDeviceReset() { return true; }
void CMainGameComponent::Update() {}
void CMainGameComponent::GetInputs() {}
void CMainGameComponent::Render() {}
void CMainGameComponent::PostInit() {}
bool CMainGameComponent::IsEditorActive() const { return false; }
