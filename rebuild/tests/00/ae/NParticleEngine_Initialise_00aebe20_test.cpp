// Behaviour fixture for NParticleEngine::Initialise (0x00AEBE20).
// Links the compiled source object. operator new, the three constructors and
// the per-type CCountedPointer<T>::Reset copies are recording stand-ins.
#include "fable_particle_engine.h"
#include "fable_symbol_map.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace NParticleEngine;

// Typed database members require constructor and exceptional-cleanup support
// even though this fixture treats those member calls as unlogged boundaries.
CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
CSymbolMap::CSymbolMap() : CheckForDuplicateSymbols(false) {}
CCharString::CCharString() {}
CCharString::~CCharString() {}
template<> _STL::set<CCharString>::~set() {}

CCountedPointer<CParticleEmitterDatabase> NParticleEngine::PEmitterDatabase;
CCountedPointer<CParticleEmitterManager> NParticleEngine::PEmitterManager;
CCountedPointer<CPSCManager> NParticleEngine::PPSCManager;

static char g_log[512];
static void Log(const char* text) { strcat(g_log, text); }
static int g_failNewSize;

void* operator new(unsigned int size)
{
    char line[32];
    sprintf(line, "new%X;", size);
    Log(line);
    if ((int)size == g_failNewSize)
        return 0;
    return malloc(size);
}

static void* g_db;
static void* g_mgr;
static void* g_psc;

CParticleEmitterDatabase::CParticleEmitterDatabase() { g_db = this; Log("dbctor;"); }
CParticleEmitterManager::CParticleEmitterManager() { g_mgr = this; Log("mgrctor;"); }
CPSCManager::CPSCManager() { g_psc = this; Log("pscctor;"); }

static const void* g_resetThis[3];
static const void* g_resetArg[3];

template<> void CCountedPointer<CParticleEmitterDatabase>::Reset(CParticleEmitterDatabase* data)
{
    Log("dbreset;");
    g_resetThis[0] = this;
    g_resetArg[0] = data;
}
template<> void CCountedPointer<CParticleEmitterManager>::Reset(CParticleEmitterManager* data)
{
    Log("mgrreset;");
    g_resetThis[1] = this;
    g_resetArg[1] = data;
}
template<> void CCountedPointer<CPSCManager>::Reset(CPSCManager* data)
{
    Log("pscreset;");
    g_resetThis[2] = this;
    g_resetArg[2] = data;
}

int main()
{
    int fail = 0;

    // Allocate + construct + Reset, database, manager, component manager.
    g_log[0] = 0;
    Initialise();
    if (strcmp(g_log, "new4C;dbctor;dbreset;new1018;mgrctor;mgrreset;new10;pscctor;pscreset;") != 0) fail |= 1;
    if (g_resetThis[0] != &PEmitterDatabase || g_resetArg[0] != g_db) fail |= 2;
    if (g_resetThis[1] != &PEmitterManager || g_resetArg[1] != g_mgr) fail |= 4;
    if (g_resetThis[2] != &PPSCManager || g_resetArg[2] != g_psc) fail |= 8;

    // A failed allocation hands Reset a null pointer and skips the ctor.
    g_log[0] = 0;
    g_failNewSize = 0x1018;
    Initialise();
    if (strcmp(g_log, "new4C;dbctor;dbreset;new1018;mgrreset;new10;pscctor;pscreset;") != 0) fail |= 16;
    if (g_resetArg[1] != 0) fail |= 32;
    g_log[0] = 0;
    g_failNewSize = 0x10;
    Initialise();
    if (strcmp(g_log, "new4C;dbctor;dbreset;new1018;mgrctor;mgrreset;new10;pscreset;") != 0 || g_resetArg[2] != 0) fail |= 64;

    if (fail == 0)
        printf("NPARTICLEENGINE_INITIALISE_PASS\n");
    else
        printf("NPARTICLEENGINE_INITIALISE_FAIL %d [%s]\n", fail, g_log);
    return 0;
}
