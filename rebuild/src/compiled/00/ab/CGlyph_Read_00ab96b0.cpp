#include "fable_font_instances.h"
#include "fable_data_input_stream.h"
void CGlyph::Read(CDataInputStream& stream)
{
    Left = stream.ReadFloat();
    Top = stream.ReadFloat();
    Right = stream.ReadFloat();
    Bottom = stream.ReadFloat();
    Offset = stream.ReadSWORD();
    Width = stream.ReadSWORD();
    Advance = stream.ReadSWORD();
}
