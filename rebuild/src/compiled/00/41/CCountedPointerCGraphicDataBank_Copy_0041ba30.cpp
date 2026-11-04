#include "fable_counted_pointer.h"
class CGraphicDataBank;
template <typename T> CCountedPointer<T>::CCountedPointer(const CCountedPointer<T>& other)
    : Data(other.Data), Info(other.Info)
{
    if (Info) ++Info->RefCount;
}
template CCountedPointer<CGraphicDataBank>::CCountedPointer(const CCountedPointer<CGraphicDataBank>& other);
