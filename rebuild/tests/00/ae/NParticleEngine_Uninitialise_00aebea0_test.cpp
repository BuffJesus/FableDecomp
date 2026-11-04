// Behaviour fixture for NParticleEngine::Uninitialise (0x00AEBEA0).
// Links the compiled source object; the three globals hold real
// CCPPointerInfo blocks whose DeleteFunc records the release order.
#include "fable_particle_engine.h"

#include <stdio.h>
#include <string.h>

using namespace NParticleEngine;

CCountedPointer<CParticleEmitterDatabase> NParticleEngine::PEmitterDatabase;
CCountedPointer<CParticleEmitterManager> NParticleEngine::PEmitterManager;
CCountedPointer<CPSCManager> NParticleEngine::PPSCManager;

static char g_log[256];
static int g_infoDeletes;

static void FABLE_FASTCALL DeleteTagged(void* data)
{
    strcat(g_log, static_cast<const char*>(data));
}

void operator delete(void* p)
{
    ++g_infoDeletes;
    (void)p;  // the blocks are static; nothing to free
}

static char g_db[] = "db;";
static char g_mgr[] = "mgr;";
static char g_psc[] = "psc;";
static CCPPointerInfo g_dbInfo(&DeleteTagged, g_db);
static CCPPointerInfo g_mgrInfo(&DeleteTagged, g_mgr);
static CCPPointerInfo g_pscInfo(&DeleteTagged, g_psc);

template<typename T>
static void Attach(CCountedPointer<T>& p, CCPPointerInfo& info, char* tag, unsigned long refs)
{
    p.Data = reinterpret_cast<T*>(tag);
    p.Info = &info;
    info.RefCount = refs;
}

int main()
{
    int fail = 0;

    // Sole owners: manager, then database, then component manager released.
    Attach(PEmitterDatabase, g_dbInfo, g_db, 1);
    Attach(PEmitterManager, g_mgrInfo, g_mgr, 1);
    Attach(PPSCManager, g_pscInfo, g_psc, 1);
    Uninitialise();
    if (strcmp(g_log, "mgr;db;psc;") != 0 || g_infoDeletes != 3) fail |= 1;
    if (PEmitterDatabase.Data || PEmitterDatabase.Info || PEmitterManager.Data ||
        PEmitterManager.Info || PPSCManager.Data || PPSCManager.Info) fail |= 2;

    // Shared: references dropped, objects kept.
    g_log[0] = 0;
    g_infoDeletes = 0;
    Attach(PEmitterDatabase, g_dbInfo, g_db, 2);
    Attach(PEmitterManager, g_mgrInfo, g_mgr, 3);
    Attach(PPSCManager, g_pscInfo, g_psc, 1);
    Uninitialise();
    if (strcmp(g_log, "psc;") != 0 || g_infoDeletes != 1) fail |= 4;
    if (g_dbInfo.RefCount != 1 || g_mgrInfo.RefCount != 2) fail |= 8;
    if (PEmitterDatabase.Info || PEmitterManager.Data) fail |= 16;

    // Already empty: nothing to release.
    g_log[0] = 0;
    g_infoDeletes = 0;
    Uninitialise();
    if (g_log[0] != 0 || g_infoDeletes != 0) fail |= 32;

    if (fail == 0)
        printf("NPARTICLEENGINE_UNINITIALISE_PASS\n");
    else
        printf("NPARTICLEENGINE_UNINITIALISE_FAIL %d [%s]\n", fail, g_log);
    return 0;
}