#include "fable_counted_pointer.h"
namespace NParticleEngine { class CAEnvironmentInteraction; }

template<typename T> CCountedPointer<T>::CCountedPointer(const CCountedPointer<T>& other)
    : Data(other.Data), Info(other.Info)
{
    if (Info) ++Info->RefCount;
}

template CCountedPointer<NParticleEngine::CAEnvironmentInteraction>::CCountedPointer(
    const CCountedPointer<NParticleEngine::CAEnvironmentInteraction>& other);
