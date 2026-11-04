#include "test_support/bank_extended_virtual_boundaries.h"
#include "test_support/async_bank_lifetime_stubs.h"
// Behaviour fixture for CParticleEmitterDatabase::OpenBank (0x00AE8980).
// Links the compiled source object. The CParticleDataBank constructor, its
// Reset, OpenReadOnly (reached through the real vtable the test's
// constructor installs) and CreateSymbolMap are recording stand-ins.
#include "fable_particle_engine.h"
#include "test_support/bank_file_member_stubs.h"
#include "test_support/resource_list_stubs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace NParticleEngine;

static char g_log[256];
static void Log(const char* text) { strcat(g_log, text); }
static int g_failNew;
static unsigned int g_newSize;

void* operator new(unsigned int size)
{
    g_newSize = size;
    Log("new;");
    return g_failNew ? 0 : malloc(size);
}

// Base-class and member stand-ins the test's CParticleDataBank needs.
void CIVCountedPointeeBase::DeleteCIVCountedPointeeBase() const {}
CResourceBank::CResourceBank() {}
CResourceBank::~CResourceBank() {}
CBankFile::CBankFile() {}
CBankFile::~CBankFile() {}
template<> _STL::list<CCountedPointer<CBankFileAsyncData> >::~list() {}
CBankFileAsync::CBankFileAsync() {}
CBankFileAsync::~CBankFileAsync() {}

static const CBankFile* g_openThis;
static const CCharString* g_openName;
static unsigned long g_openFlags;
bool CBankFile::OpenReadOnly(const CCharString& bankHandle, unsigned long openFlags)
{
    Log("open;");
    g_openThis = this;
    g_openName = &bankHandle;
    g_openFlags = openFlags;
    return true;
}

static const CBankFile* g_mapThis;
static const CSymbolMap* g_mapTarget;
static bool g_mapFlag;
void CBankFile::CreateSymbolMap(CSymbolMap& symbolMap, bool flag) const
{
    Log("map;");
    g_mapThis = this;
    g_mapTarget = &symbolMap;
    g_mapFlag = flag;
}

static CParticleDataBank* g_constructed;
CParticleDataBank::CParticleDataBank() { g_constructed = this; Log("ctor;"); }
CParticleDataBank::~CParticleDataBank() {}

static const void* g_resetThis;
template<> void CCountedPointer<CParticleDataBank>::Reset(CParticleDataBank* data)
{
    Log("reset;");
    g_resetThis = this;
    Data = data;
}

int main()
{
    int fail = 0;
    static unsigned char storage[sizeof(CParticleEmitterDatabase)];
    memset(storage, 0x3C, sizeof(storage));
    CParticleEmitterDatabase* database = reinterpret_cast<CParticleEmitterDatabase*>(storage);
    CCharString* name = reinterpret_cast<CCharString*>(storage + 0x40);  // any distinct address

    database->OpenBank(*name);
    if (strcmp(g_log, "new;ctor;reset;open;map;") != 0 || g_newSize != 0x234) fail |= 1;
    if (g_resetThis != &database->EmitterBank || database->EmitterBank.Data != g_constructed) fail |= 2;
    if (g_openThis != g_constructed || g_openName != name || g_openFlags != 0x48) fail |= 4;
    // The symbol map is the database's leading member; the flag is false.
    if (g_mapThis != g_constructed || g_mapTarget != reinterpret_cast<CSymbolMap*>(storage) || g_mapFlag) fail |= 8;
    // Nothing else in the database is written.
    if (storage[0] != 0x3C || storage[0x37] != 0x3C || storage[0x3C] != 0x3C || storage[0x4B] != 0x3C) fail |= 16;

    if (fail == 0)
        printf("CPARTICLEEMITTERDATABASE_OPENBANK_PASS\n");
    else
        printf("CPARTICLEEMITTERDATABASE_OPENBANK_FAIL %d [%s]\n", fail, g_log);
    return 0;
}
