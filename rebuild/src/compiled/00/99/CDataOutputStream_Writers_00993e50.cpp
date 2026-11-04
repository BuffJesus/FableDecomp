// lib_data_stream.cpp, retail 0x00993E50-0x009942D0: the value writers of
// CDataOutputStream (Ego_r names and order). Each sends the value's bytes
// through the virtual Write; the vector writers expand WriteFloat, which is
// defined first in the same file.
#include "fable_data_output_stream.h"
#include "fable_string.h"
#include "fable_rgb_colour.h"
#include "fable_2d_vector.h"
#include "fable_definition_type_traits.h"
#include "engine/CMatrix3x4.h"
#include "fable_pixel_format_init.h"
#include "fable_surface_types.h"

// Retail 00993E50 (supersedes the fake CDataStream_WriteCBYTE_00993e50 landing).
void CDataOutputStream::WriteUBYTE(unsigned char value)
{
    Write(&value, 1);
}

// Retail 00993E60.
void CDataOutputStream::WriteSBYTE(signed char value)
{
    Write(&value, 1);
}

// Retail 00993E70.
void CDataOutputStream::WriteCBYTE(char value)
{
    Write(&value, 1);
}

// Retail 00993E80 and 00993E90 (supersede the fake WriteCBYTE landings there).
void CDataOutputStream::WriteUWORD(unsigned short value)
{
    Write(&value, 2);
}

void CDataOutputStream::WriteSWORD(short value)
{
    Write(&value, 2);
}

// Retail 00993EC0 (supersedes the fake WriteCBYTE landing there).
void CDataOutputStream::WriteUHUGE(unsigned __int64 value)
{
    Write(&value, 8);
}

// Retail 00993ED0.
void CDataOutputStream::WriteSHUGE(__int64 value)
{
    Write(&value, 8);
}

// Retail 00993EE0.
void CDataOutputStream::WriteFloat(float value)
{
    Write(&value, 4);
}

// Retail 00993EF0.
void CDataOutputStream::WriteDouble(double value)
{
    Write(&value, 8);
}

// Retail 00993F00: the characters and their terminator.
void CDataOutputStream::WriteNullTerminatedWString(const CWideString& text)
{
    const wchar_t* chars = text;
    Write(chars, (text.GetLength() + 1) * sizeof(wchar_t));
}

// Retail 00993F30: the copy goes through CRGBColour's field-order copy ctor.
void CDataOutputStream::WriteColour(const CRGBColour& colour)
{
    CRGBColour value = colour;
    Write(&value, 4);
}

// Retail 00993F60: the format's six-byte CPixelFormatInit.
void CDataOutputStream::WritePixelFormat(const CPixelFormat& format)
{
    CPixelFormatInit init = format.GetInit();
    Write(&init, 6);
}

// Retail 00993F90 (supersedes the CDataOutputStreamMethods stand-in landing).
void CDataOutputStream::Write2DVector(const C2DVector& vector)
{
    WriteFloat(vector.X);
    WriteFloat(vector.Y);
}

// Retail 00993FD0.
void CDataOutputStream::Write3DVector(const C3DVector& vector)
{
    WriteFloat(vector.X);
    WriteFloat(vector.Y);
    WriteFloat(vector.Z);
}

// Retail 00994020.
void CDataOutputStream::Write3DAngles(const C3DAngles& angles)
{
    WriteFloat(angles.AngleXY);
    WriteFloat(angles.AngleYZ);
    WriteFloat(angles.AngleXZ);
}

// Retail 00994070: the twelve elements, row by row.
void CDataOutputStream::WriteMatrix3x4(const CMatrix3x4& matrix)
{
    WriteFloat(matrix.E11);
    WriteFloat(matrix.E12);
    WriteFloat(matrix.E13);
    WriteFloat(matrix.E21);
    WriteFloat(matrix.E22);
    WriteFloat(matrix.E23);
    WriteFloat(matrix.E31);
    WriteFloat(matrix.E32);
    WriteFloat(matrix.E33);
    WriteFloat(matrix.E41);
    WriteFloat(matrix.E42);
    WriteFloat(matrix.E43);
}

// Retail 009942D0: zero bytes up to the next multiple of the alignment.
void CDataOutputStream::AlignData(long alignment)
{
    unsigned long position = GetPosition();
    long mask = alignment - 1;
    if (position & mask)
        WriteRepeatedByte(0, ((position + alignment - 1) & ~mask) - position);
}
