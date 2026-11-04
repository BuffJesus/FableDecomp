#include "fable_string.h"
#include "fable_persist_context.h"
#include "fable_data_input_stream.h"

// The two early returns retain retail's range-check control flow. As with
// the other scalar readers, a rejected read leaves the result unspecified.
__forceinline long CDataInputStream::ReadSLONG()
{
    long value;
    __int64 next = (unsigned long)(StreamPos + 4);
    if (next < 0)
        return value;
    if (next > 0x7fffffff)
        return value;
    if (SourceChunkBytesRemaining >= 4) {
        value = *reinterpret_cast<const long*>(CurrentSourcePtr);
        CurrentSourcePtr += 4;
        SourceChunkBytesRemaining -= 4;
        StreamPos += 4;
    } else {
        ReadWithSrcChunkOverflow(&value, 4);
    }
    return value;
}

void CPersistTraits<long>::TransferBinaryIn(CMemoryDataInputStream& stream, long& value)
{
    value = stream.ReadSLONG();
}