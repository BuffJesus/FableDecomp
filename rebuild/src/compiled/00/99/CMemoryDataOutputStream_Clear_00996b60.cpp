#include "fable_data_output_stream.h"

void CMemoryDataOutputStream::Clear()
{
    if (Data.capacity())
    {
        Data.clear();
        char* buffer = Data._M_start;
        Data._M_start = Data._M_finish = Data._M_end_of_storage._M_data = 0;
        if (buffer)
            Data._M_end_of_storage.deallocate(buffer, 0);
    }
    CurrentPosition = 0;
}
