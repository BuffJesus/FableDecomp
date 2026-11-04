#include "fable_data_output_stream.h"
#include "fable_date_and_time.h"
inline void CDataOutputStream::WriteSLONG(long value) { Write(&value, 4); }
inline void CDataOutputStream::WriteUBYTE(unsigned char value) { Write(&value, 1); }
inline void CDataOutputStream::WriteUWORD(unsigned short value) { Write(&value, 2); }
void CDataOutputStream::WriteDateAndTime(const CDateAndTime& value)
{
    WriteSLONG(value.Year);
    WriteUBYTE(static_cast<unsigned char>(value.Month));
    WriteUBYTE(static_cast<unsigned char>(value.Day));
    WriteUBYTE(static_cast<unsigned char>(value.Hour));
    WriteUBYTE(static_cast<unsigned char>(value.Minute));
    WriteUWORD(static_cast<unsigned short>(value.Second));
    WriteUWORD(static_cast<unsigned short>(value.Millisecond));
}
