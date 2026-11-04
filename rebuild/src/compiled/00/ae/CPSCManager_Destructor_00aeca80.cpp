#include "fable_particle_engine.h"
#include <new>

namespace NParticleEngine
{
    CPSCManager::~CPSCManager()
    {
        char* entry = *reinterpret_cast<char**>(this);
        char* end = *reinterpret_cast<char**>(reinterpret_cast<char*>(this) + 4);
        while (entry != end)
        {
            reinterpret_cast<CCharString*>(entry + 12)->~CCharString();
            reinterpret_cast<CCharString*>(entry)->~CCharString();
            entry += 16;
        }
        void* storage = *reinterpret_cast<void**>(this);
        if (storage) ::operator delete[](storage);
    }
}
