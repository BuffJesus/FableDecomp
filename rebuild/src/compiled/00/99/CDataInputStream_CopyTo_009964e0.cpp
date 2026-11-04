// CDataInputStream::CopyTo(CDataOutputStream&, unsigned long) @ 0x009964E0
// (FableWin ?CopyTo@CDataInputStream@@QAEXAAVCDataOutputStream@@K@Z): copies
// `size` bytes from this stream to `stream` through a zeroed buffer of at most
// 64 KiB, one buffer at a time.
#include "fable_data_input_stream.h"
#include "fable_data_output_stream.h"
#include "fable_carray.h"
#include "fable_stlport_vector_impl.h"

void CDataInputStream::CopyTo(CDataOutputStream& stream, unsigned long size)
{
    CArrayVector<fable_u8> buffer(size < 0x10000 ? size : 0x10000);
    while (size != 0)
    {
        unsigned long bytes = size < buffer.size() ? size : buffer.size();
        Read(buffer.GetAsCArray(), bytes);
        stream.Write(buffer.GetAsCArray(), bytes);
        size -= bytes;
    }
}