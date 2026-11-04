#include "fable_data_output_stream.h"

void CDataOutputStream::WriteEBOOL(bool value)
{
    unsigned char serializedValue = value ? 1 : 0;
    Write(&serializedValue, 1);
}
