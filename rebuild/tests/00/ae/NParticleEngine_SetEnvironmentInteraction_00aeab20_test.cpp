// Behaviour fixture for NParticleEngine::SetEnvironmentInteraction (0x00AEAB20).
// Links the compiled source object; real CCPPointerInfo blocks record
// releases. The by-value argument is built by the test's own copy.
#include "fable_particle_engine.h"

#include <stdio.h>
#include <string.h>

using namespace NParticleEngine;

CCountedPointer<CAEnvironmentInteraction> NParticleEngine::PEnvironmentInteraction;

static char g_log[256];
static int g_infoDeletes;
static void* g_lastDeleted;

static void FABLE_FASTCALL DeleteTagged(void* data)
{
    strcat(g_log, static_cast<const char*>(data));
}

void operator delete(void* p)
{
    ++g_infoDeletes;
    g_lastDeleted = p;
}

// Out of line in retail; the caller's copy of the argument.
template<>
CCountedPointer<CAEnvironmentInteraction>::CCountedPointer(const CCountedPointer& other)
    : Data(other.Data), Info(other.Info)
{
    if (Info)
        ++Info->RefCount;
}

static char g_old[] = "old;";
static char g_new[] = "new;";
static CCPPointerInfo g_oldInfo(&DeleteTagged, g_old);
static CCPPointerInfo g_newInfo(&DeleteTagged, g_new);

static void Reset()
{
    g_log[0] = 0;
    g_infoDeletes = 0;
    g_lastDeleted = 0;
}

int main()
{
    int fail = 0;
    CCountedPointer<CAEnvironmentInteraction> fresh;
    fresh.Data = reinterpret_cast<CAEnvironmentInteraction*>(g_new);
    fresh.Info = &g_newInfo;

    // Different block: the sole old owner is released, the new one shared.
    Reset();
    PEnvironmentInteraction.Data = reinterpret_cast<CAEnvironmentInteraction*>(g_old);
    PEnvironmentInteraction.Info = &g_oldInfo;
    g_oldInfo.RefCount = 1;
    g_newInfo.RefCount = 1;
    SetEnvironmentInteraction(fresh);
    if (strcmp(g_log, "old;") != 0 || g_infoDeletes != 1 || g_lastDeleted != &g_oldInfo) fail |= 1;
    if (PEnvironmentInteraction.Data != fresh.Data || PEnvironmentInteraction.Info != &g_newInfo) fail |= 2;
    // Caller's copy was dropped on return: fresh + global = 2 owners.
    if (g_newInfo.RefCount != 2) fail |= 4;

    // Same block again: nothing released, count unchanged after the call.
    Reset();
    SetEnvironmentInteraction(fresh);
    if (g_log[0] != 0 || g_infoDeletes != 0 || g_newInfo.RefCount != 2) fail |= 8;

    // Null: the global lets go (still one owner left: `fresh`).
    Reset();
    CCountedPointer<CAEnvironmentInteraction> none;
    SetEnvironmentInteraction(none);
    if (PEnvironmentInteraction.Data != 0 || PEnvironmentInteraction.Info != 0) fail |= 16;
    if (g_newInfo.RefCount != 1 || g_infoDeletes != 0) fail |= 32;

    // The argument was the last owner: released when the call returns.
    Reset();
    CCountedPointer<CAEnvironmentInteraction> temp;
    temp.Data = reinterpret_cast<CAEnvironmentInteraction*>(g_new);
    temp.Info = &g_newInfo;
    g_newInfo.RefCount = 0;   // `temp` holds no count: the argument copy is the only owner
    SetEnvironmentInteraction(temp);
    if (g_newInfo.RefCount != 1 || PEnvironmentInteraction.Info != &g_newInfo) fail |= 64;
    // Replace with null: now the global was the last owner -> released.
    Reset();
    SetEnvironmentInteraction(none);
    if (strcmp(g_log, "new;") != 0 || g_infoDeletes != 1) fail |= 128;

    fresh.Info = 0;
    temp.Info = 0;
    if (fail == 0)
        printf("NPARTICLEENGINE_SETENVIRONMENTINTERACTION_PASS\n");
    else
        printf("NPARTICLEENGINE_SETENVIRONMENTINTERACTION_FAIL %d [%s]\n", fail, g_log);
    return 0;
}