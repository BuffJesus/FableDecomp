#include "fable_particle_engine.h"
#include <new>
#include <stdio.h>

static const void* g_log[16];
static int g_calls;

CCharString::~CCharString()
{
    g_log[g_calls++] = this;
}

namespace NParticleEngine
{
    CPSCManager::CPSCManager()
    {
        for (unsigned int i = 0; i < sizeof(data_); ++i) data_[i] = 0;
    }
}

static int Run(int count, int has_storage, unsigned char fill)
{
    union { double align; char bytes[sizeof(NParticleEngine::CPSCManager)]; } store;
    NParticleEngine::CPSCManager* manager = new (&store) NParticleEngine::CPSCManager;
    char* entries = has_storage ? (char*)::operator new[](count * 16) : 0;
    for (int i = 0; i < count * 16; ++i) entries[i] = fill;
    reinterpret_cast<char**>(manager)[0] = entries;
    reinterpret_cast<char**>(manager)[1] = entries ? entries + count * 16 : 0;
    g_calls = 0;
    manager->~CPSCManager();
    if (g_calls != 2 * count) return 1;
    for (int i = 0; i < count; ++i)
    {
        if (g_log[2 * i] != entries + i * 16 + 12 ||
            g_log[2 * i + 1] != entries + i * 16) return 2;
    }
    return 0;
}

int main()
{
    int cases = 0;
    const unsigned char fills[] = {0, 0xa5, 0xff};
    for (int count = 0; count < 5; ++count)
    for (int storage = 0; storage < 2; ++storage)
    for (int fill = 0; fill < 3; ++fill)
    {
        if (count && !storage) continue;
        if (Run(count, storage, fills[fill])) return 1;
        ++cases;
    }
    printf("PSC_MANAGER_DESTRUCTOR_BEHAVIOR_PASS cases=%d\n", cases);
    return 0;
}
