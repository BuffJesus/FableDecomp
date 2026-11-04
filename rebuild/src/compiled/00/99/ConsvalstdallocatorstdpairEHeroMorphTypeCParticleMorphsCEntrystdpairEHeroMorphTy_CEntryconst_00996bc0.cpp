// CMemoryDataOutputStream::ReserveSize(unsigned long) @ 0x00996BC0 (FableWin
// ?ReserveSize@CMemoryDataOutputStream@@QAEXK@Z): reserves room for `size`
// bytes of data (vector<char>::reserve 0x00997330, a tail call).
#include "fable_data_output_stream.h"

void CMemoryDataOutputStream::ReserveSize(unsigned long size)
{
    Data.reserve(size);
}
