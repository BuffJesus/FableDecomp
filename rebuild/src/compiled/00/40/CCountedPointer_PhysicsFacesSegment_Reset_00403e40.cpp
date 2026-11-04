#include "fable_game_text_bank.h"

template<typename T>
void CCountedPointer<T>::Reset(T* data)
{
    Release();
    Info = 0;
    Data = data;
    if (Data)
        Info = new CCPPointerInfo(&DeleteData, (void*)Data);
}

template void CCountedPointer<NGameText::CDataBank>::Reset(NGameText::CDataBank* data);

// Retain the folded physics-interface instantiation used by existing callers.
class CIPhysicsFacesSegmentBase { public: virtual ~CIPhysicsFacesSegmentBase(); };
template void CCountedPointer<const CIPhysicsFacesSegmentBase>::Reset(const CIPhysicsFacesSegmentBase*);
