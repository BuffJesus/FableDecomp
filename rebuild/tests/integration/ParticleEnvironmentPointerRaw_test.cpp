#include "fable_counted_pointer.h"
#include <stdio.h>
#include <new>

namespace NParticleEngine { class CAEnvironmentInteraction; }
typedef CCountedPointer<NParticleEngine::CAEnvironmentInteraction> Pointer;
static char g_data[3][16];

static int Run(int selector, unsigned char initial)
{
    union { double align; unsigned char bytes[sizeof(Pointer)]; } store;
    for (unsigned int i = 0; i < sizeof(store.bytes); ++i) store.bytes[i] = initial;
    Pointer* p = (Pointer*)&store;
    NParticleEngine::CAEnvironmentInteraction* data = selector == 0 ? 0 :
        (NParticleEngine::CAEnvironmentInteraction*)g_data[selector];
    new (p) Pointer(data);
    int failed = p->Data != data || (p->Info == 0) != (data == 0);
    if (p->Info)
    {
        failed |= p->Info->RefCount != 1 || p->Info->DeleteFunc == 0 ||
                  p->Info->Data != data;
        delete p->Info;
    }
    p->Info = 0;
    return failed;
}

int main()
{
    int cases = 0;
    const unsigned char initial[] = {0, 1, 0xa5, 0xff};
    for (int selector = 0; selector < 3; ++selector)
    for (int fill = 0; fill < 4; ++fill)
    for (int repeat = 0; repeat < 16; ++repeat)
    {
        if (Run(selector, initial[fill]))
        {
            printf("PARTICLE_POINTER_RAW_FAIL selector=%d fill=%d repeat=%d\n", selector, fill, repeat);
            return 1;
        }
        ++cases;
    }
    printf("PARTICLE_POINTER_RAW_BEHAVIOR_PASS cases=%d\n", cases);
    return 0;
}
