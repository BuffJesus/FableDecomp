// CDataOutputStream::CopyFrom(CDataInputStream&, unsigned long) @ 0x00996970
// (FableWin ?CopyFrom@CDataOutputStream@@QAEXAAVCDataInputStream@@K@Z): copies
// `size` bytes of `stream` into this stream (CDataInputStream::CopyTo 0x009964E0).
#include "fable_data_output_stream.h"
#include "fable_data_input_stream.h"

void CDataOutputStream::CopyFrom(CDataInputStream& stream, unsigned long size)
{
    stream.CopyTo(*this, size);
}