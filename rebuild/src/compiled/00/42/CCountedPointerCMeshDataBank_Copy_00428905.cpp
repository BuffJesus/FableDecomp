#include "fable_counted_pointer.h"
class CMeshDataBank;
template <typename T> CCountedPointer<T>::CCountedPointer(const CCountedPointer<T>& other)
    : Data(other.Data), Info(other.Info)
{
    if (Info) ++Info->RefCount;
}
template CCountedPointer<CMeshDataBank>::CCountedPointer(const CCountedPointer<CMeshDataBank>& other);
