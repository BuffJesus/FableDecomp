#include "fable_particle_engine.h"
#include <new>
#include <stdio.h>

static const void* g_calls[3];
static int g_call_count;

// Link-only cleanup boundaries generated for exceptional construction paths.
CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
CCharString::~CCharString() {}
template<> _STL::set<CCharString>::~set() {}

CSymbolMap::CSymbolMap()
    : CheckForDuplicateSymbols(false)
{
    g_calls[g_call_count++] = this;
}

CCharString::CCharString()
{
    *reinterpret_cast<void**>(this) = 0;
    g_calls[g_call_count++] = this;
}

static int Run(unsigned char initial)
{
    union { double align; char bytes[sizeof(NParticleEngine::CParticleEmitterDatabase)]; } store;
    for (unsigned int i = 0; i < sizeof(store.bytes); ++i) store.bytes[i] = initial;
    g_call_count = 0;
    NParticleEngine::CParticleEmitterDatabase* object =
        new (&store) NParticleEngine::CParticleEmitterDatabase;
    if (g_call_count != 3 || g_calls[0] != object ||
        g_calls[1] != reinterpret_cast<char*>(object) + 0x44 ||
        g_calls[2] != reinterpret_cast<char*>(object) + 0x48) return 1;
    const unsigned long* words = reinterpret_cast<const unsigned long*>(object);
    if (words[0x28 / 4] || words[0x2c / 4] || words[0x30 / 4] ||
        words[0x38 / 4] || words[0x3c / 4] || words[0x40 / 4] != 1 ||
        words[0x44 / 4] || words[0x48 / 4]) return 2;
    if (*(reinterpret_cast<const unsigned char*>(object) + 0x35) != 0) return 3;
    return 0;
}

int main()
{
    const unsigned char fills[] = {0, 1, 0x5a, 0xa5, 0xff};
    for (int i = 0; i < 5; ++i)
        if (Run(fills[i])) return 1;
    printf("PARTICLE_DATABASE_CTOR_BEHAVIOR_PASS cases=5\n");
    return 0;
}
