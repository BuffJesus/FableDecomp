// lib_data_stream.cpp, retail 0x009959F0-0x00995A80: string writers
// (Ego_r names). WriteSLONG, defined earlier in the same file, is expanded.
#include "fable_data_output_stream.h"

inline void CDataOutputStream::WriteSLONG(long value)
{
    Write(&value, 4);
}

// Retail 009959F0.
void CDataOutputStream::WriteString(const CCharString& text)
{
    const char* data = text.GetConstCharPtr();
    Write(data, text.GetLength());
}

// Retail 00995A20.
void CDataOutputStream::WriteStringNewLine(const CCharString& text)
{
    WriteString(text);
    const char* newline = NewLine.GetConstCharPtr();
    Write(newline, NewLine.GetLength());
}

// Retail 00995A80.
void CDataOutputStream::WritePresizedString(const CCharString& text)
{
    const char* data = text.GetConstCharPtr();
    WriteSLONG(text.GetLength());
    Write(data, text.GetLength());
}
