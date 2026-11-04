#include "fable_data_output_stream.h"
void CDataOutputStream::WriteNullTerminatedString(const CCharString& text)
{
    const char* data = text.GetConstCharPtr();
    Write(data, text.GetLength() + 1);
}
