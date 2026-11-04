#include "fable_counted_pointer.h"
#include <stdio.h>
#include <new>

namespace NParticleEngine { class CAEnvironmentInteraction; }
typedef CCountedPointer<NParticleEngine::CAEnvironmentInteraction> Pointer;
static char g_data[3][16];

static int Run(int data, int info, int c1, int c2, int alias)
{
    const unsigned long counts[] = {0, 1, 2, 0xffffffffUL};
    union { double align; char bytes[sizeof(Pointer)]; } sourceStore, destStore;
    Pointer* source = (Pointer*)&sourceStore;
    Pointer* dest = alias ? source : (Pointer*)&destStore;
    CCPPointerInfo* infos[3] = {0, new CCPPointerInfo(0, 0), new CCPPointerInfo(0, 0)};
    infos[1]->RefCount = counts[c1];
    infos[2]->RefCount = counts[c2];
    source->Data = (NParticleEngine::CAEnvironmentInteraction*)g_data[data];
    source->Info = infos[info];
    new (dest) Pointer(*source);
    unsigned long expected1 = counts[c1] + (info == 1 ? 1UL : 0UL);
    unsigned long expected2 = counts[c2] + (info == 2 ? 1UL : 0UL);
    int fail = dest->Data != (NParticleEngine::CAEnvironmentInteraction*)g_data[data] ||
               dest->Info != infos[info] || infos[1]->RefCount != expected1 ||
               infos[2]->RefCount != expected2;
    dest->Info = 0;
    source->Info = 0;
    delete infos[1];
    delete infos[2];
    return fail;
}

int main()
{
    int cases = 0;
    for (int data = 0; data < 3; ++data)
    for (int info = 0; info < 3; ++info)
    for (int c1 = 0; c1 < 4; ++c1)
    for (int c2 = 0; c2 < 4; ++c2)
    for (int alias = 0; alias < 2; ++alias)
    {
        if (Run(data, info, c1, c2, alias))
        {
            printf("PARTICLE_POINTER_COPY_FAIL %d %d %d %d %d\n", data, info, c1, c2, alias);
            return 1;
        }
        ++cases;
    }
    printf("PARTICLE_POINTER_COPY_BEHAVIOR_PASS cases=%d\n", cases);
    return 0;
}
