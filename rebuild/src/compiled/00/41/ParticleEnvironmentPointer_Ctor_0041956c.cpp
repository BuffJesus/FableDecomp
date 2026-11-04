#pragma optimize("s", on)
#include "fable_particle_engine.h"
#include <new>

template<typename T> CCountedPointer<T>::CCountedPointer(T* data)
{
    Data = data;
    if (data)
    {
        CCPPointerInfo* info = (CCPPointerInfo*)::operator new(sizeof(CCPPointerInfo));
        if (info) new (info) CCPPointerInfo(&DeleteData, Data);
        Info = info;
    }
    else
    {
        Info = 0;
    }
}

template CCountedPointer<NParticleEngine::CAEnvironmentInteraction>::CCountedPointer(
    NParticleEngine::CAEnvironmentInteraction* data);
