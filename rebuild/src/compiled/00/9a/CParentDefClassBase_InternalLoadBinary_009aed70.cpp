#include <new>
#include "fable_def_class_base.h"
#include "fable_data_input_stream.h"
#include "fable_stlport_vector_impl.h"
void CParentDefClassBase::InternalLoadBinary(CDataInputStream& stream)
{
    CDefClassBase::InternalLoadBinary(stream);
    unsigned short count = stream.ReadValue<unsigned short>();
    SubDefInfoMap.reserve(count);
    for (unsigned long i = 0; i < count; ++i)
    {
        unsigned long crc = stream.ReadValue<unsigned long>();
        CSubDefInfo info;
        info.DefIndex = stream.ReadValue<long>();
        info.OriginalParentDefIndex = stream.ReadValue<long>();
        SubDefInfoMap.Insert(crc, info);
    }
    SubDefInfoMap.Dirty = false;
}