#include "fable_particle_engine.h"

template<typename T> void CCountedPointer<T>::Reset(T* data)
{
    if (Info)
    {
        --Info->RefCount;
        if (Info->RefCount == 0)
        {
            Info->DeleteFunc(Info->Data);
            delete Info;
        }
    }
    Info = 0;
    Data = data;
    if (Data) Info = new CCPPointerInfo(&DeleteData, Data);
}

template void CCountedPointer<NParticleEngine::CParticleEmitterDatabase>::Reset(
    NParticleEngine::CParticleEmitterDatabase*);
template void CCountedPointer<NParticleEngine::CParticleEmitterManager>::Reset(
    NParticleEngine::CParticleEmitterManager*);
template void CCountedPointer<NParticleEngine::CPSCManager>::Reset(
    NParticleEngine::CPSCManager*);
