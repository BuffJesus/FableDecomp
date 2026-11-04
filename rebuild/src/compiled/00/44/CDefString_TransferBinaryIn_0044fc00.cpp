#include "fable_def_string.h"
#include "fable_data_input_stream.h"

void CDefString::TransferBinaryIn(CDataInputStream& stream)
{
    long position = stream.ReadValue<long>();
    TablePos = position;
}