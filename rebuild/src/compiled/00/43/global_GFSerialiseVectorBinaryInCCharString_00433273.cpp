// Retail 0x00433273: count followed by null-terminated strings.
#pragma optimize("s", on)
#include "fable_string.h"
#include "fable_data_input_stream.h"
#include "fable_persist_context.h"
__forceinline void CPersistTraits<CCharString>::TransferBinaryIn(CMemoryDataInputStream& stream, CCharString& value)
{ value = stream.ReadNullTerminatedString(); }
template <class T>
void FABLE_FASTCALL GFSerialiseVectorBinaryIn(CMemoryDataInputStream& stream, _STL::vector<T>& value)
{
    CPersistTraits<T> traits;
    long count = stream.ReadValue<long>();
    value.resize(count, T());
    for (long i = 0; i < count; ++i)
        { T* data = value.begin(); traits.TransferBinaryIn(stream, data[i]); }
}
template void FABLE_FASTCALL GFSerialiseVectorBinaryIn<CCharString>(CMemoryDataInputStream&, _STL::vector<CCharString>&);