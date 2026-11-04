#include <new>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fable_game.h"
#include "test_support/base_class_stubs.h"

namespace
{
    // Every recorded event gets the next sequence number.
    int g_sequence;
    int g_baseAt;
    const void* g_baseThis;
    CGame* g_baseGame;
    int g_timeCalls;
    int g_timeAt[2];
    int g_wideDefaultAt[3];
    const void* g_wideDefaultThis[3];
    int g_wideDefaults;
    int g_charDefaultAt;
    const void* g_charDefaultThis;
    int g_charDefaults;
    int g_wideTextAt;
    const void* g_wideTextThis;
    const wchar_t* g_wideTextArg;
    int g_wideTexts;
    int g_stopWatchAt[3];
    const void* g_stopWatchThis[3];
    int g_stopWatches;
    int g_assignAt[2];
    const void* g_assignThis[2];
    const wchar_t* g_assignArg[2];
    int g_assigns;

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

    bool EmptyText(const wchar_t* text)
    {
        return text != 0 && text[0] == 0;
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
CNewFrontendGameComponent::~CNewFrontendGameComponent() {}

double FABLE_FASTCALL GFGetTime()
{
    const int call = g_timeCalls++;
    if (call < 2)
    {
        g_timeAt[call] = ++g_sequence;
    }
    return call == 0 ? 1.5 : 2.5;
}

CStopWatch::CStopWatch()
{
    if (g_stopWatches < 3)
    {
        g_stopWatchAt[g_stopWatches] = ++g_sequence;
        g_stopWatchThis[g_stopWatches] = this;
    }
    ++g_stopWatches;
}

CWideString::CWideString() : storage_(0)
{
    if (g_wideDefaults < 3)
    {
        g_wideDefaultAt[g_wideDefaults] = ++g_sequence;
        g_wideDefaultThis[g_wideDefaults] = this;
    }
    ++g_wideDefaults;
}

CWideString::CWideString(const wchar_t* text) : storage_(0)
{
    ++g_wideTexts;
    g_wideTextAt = ++g_sequence;
    g_wideTextThis = this;
    g_wideTextArg = text;
}

CWideString::~CWideString() {}

const CWideString& CWideString::operator=(const wchar_t* text)
{
    if (g_assigns < 2)
    {
        g_assignAt[g_assigns] = ++g_sequence;
        g_assignThis[g_assigns] = this;
        g_assignArg[g_assigns] = text;
    }
    ++g_assigns;
    return *this;
}

CCharString::CCharString() : storage_(0)
{
    ++g_charDefaults;
    g_charDefaultAt = ++g_sequence;
    g_charDefaultThis = this;
}

CCharString::~CCharString() {}

int main()
{
    const unsigned long size = sizeof(CNewFrontendGameComponent);
    unsigned char* storage = static_cast<unsigned char*>(malloc(size));
    memset(storage, 0xA5, size);

    unsigned char gameStorage[sizeof(CGame)];
    CGame& game = *reinterpret_cast<CGame*>(gameStorage);

    CNewFrontendGameComponentInit init;
    init.Nothing = 0;
    CNewFrontendGameComponent* component =
        new (storage) CNewFrontendGameComponent(game, init);
    const unsigned char* p = storage;
    double firstTime;
    double secondTime;
    float repeatDelay;
    float interp[3];
    memcpy(&firstTime, p + 0x20, sizeof(firstTime));
    memcpy(&secondTime, p + 0xC0, sizeof(secondTime));
    memcpy(&repeatDelay, p + 0xF8, sizeof(repeatDelay));
    memcpy(interp, p + 0xCC, sizeof(interp));

    // Base, first clock read, the embedded CMainGameComponentInit (three
    // wide strings and one narrow), SaveGame(L""), second clock read, the
    // three stop watches, then the body's two L"" assignments.
    const bool order =
        g_baseAt == 1 &&
        g_timeAt[0] == 2 &&
        g_wideDefaultAt[0] == 3 &&
        g_wideDefaultAt[1] == 4 &&
        g_charDefaultAt == 5 &&
        g_wideDefaultAt[2] == 6 &&
        g_wideTextAt == 7 &&
        g_timeAt[1] == 8 &&
        g_stopWatchAt[0] == 9 &&
        g_stopWatchAt[1] == 10 &&
        g_stopWatchAt[2] == 11 &&
        g_assignAt[0] == 12 &&
        g_assignAt[1] == 13;

    const bool subObjects =
        g_baseThis == p &&
        g_baseGame == &game &&
        g_timeCalls == 2 &&
        g_wideDefaults == 3 &&
        g_charDefaults == 1 &&
        g_wideDefaultThis[0] == p + 0x2C &&
        g_wideDefaultThis[1] == p + 0x30 &&
        g_charDefaultThis == p + 0x34 &&
        g_wideDefaultThis[2] == p + 0x38 &&
        g_wideTexts == 1 &&
        g_wideTextThis == p + 0x84 &&
        EmptyText(g_wideTextArg) &&
        g_stopWatches == 3 &&
        g_stopWatchThis[0] == p + 0xD8 &&
        g_stopWatchThis[1] == p + 0x100 &&
        g_stopWatchThis[2] == p + 0x120 &&
        g_assigns == 2 &&
        g_assignThis[0] == p + 0x2C &&
        EmptyText(g_assignArg[0]) &&
        g_assignThis[1] == p + 0x38 &&
        EmptyText(g_assignArg[1]);

    const bool fields =
        Dword(p, 0x00) != 0xA5A5A5A5UL &&
        Dword(p, 0x04) != 0xA5A5A5A5UL &&
        Dword(p, 0x00) != Dword(p, 0x04) &&
        AllBytes(p, 0x10, 0x1D, 0x00) &&
        firstTime == 1.5 &&
        p[0x28] == 1 &&
        p[0x29] == 0 &&
        p[0x2A] == 0 &&
        p[0x2B] == 0xA5 &&
        AllBytes(p, 0x3C, 0x68, 0x00) &&
        Dword(p, 0x68) == 0xA5A5A5A5UL &&
        AllBytes(p, 0x6C, 0x84, 0x00) &&
        AllBytes(p, 0x88, 0x90, 0x00) &&
        AllBytes(p, 0x90, 0xB0, 0xA5) &&
        p[0xB0] == 0 &&
        p[0xB1] == 0 &&
        Dword(p, 0xB4) == 0 &&
        AllBytes(p, 0xB8, 0xC0, 0xA5) &&
        secondTime == 2.5 &&
        p[0xC8] == 0 &&
        interp[0] == 0.0f &&
        interp[1] == 0.0f &&
        interp[2] == 0.0f &&
        repeatDelay == 0.333f &&
        Dword(p, 0xFC) == 0 &&
        Dword(p, 0x140) == 0 &&
        p[0x144] == 1 &&
        AllBytes(p, 0x145, 0x148, 0xA5);

    if (component != reinterpret_cast<CNewFrontendGameComponent*>(storage) ||
        !order || !subObjects || !fields)
    {
        printf("order=%d subObjects=%d fields=%d\n", order, subObjects, fields);
        puts("FABLETLC_NEW_FRONTEND_GAME_COMPONENT_CTOR FAIL");
        return 1;
    }

    puts("FABLETLC_NEW_FRONTEND_GAME_COMPONENT_CTOR PASS");
    return 0;
}
// Explicit concrete dispatch recorders (retail primary slots 1-4, secondary 0-1).
void CNewFrontendGameComponent::Init() {}
bool CNewFrontendGameComponent::Run(CGameComponent**) { return false; }
void CNewFrontendGameComponent::ChangeTextureColourDepth(long) {}
void CNewFrontendGameComponent::SetQuit() {}
void CNewFrontendGameComponent::OnPreDeviceReset() {}
bool CNewFrontendGameComponent::OnPostDeviceReset() { return true; }
