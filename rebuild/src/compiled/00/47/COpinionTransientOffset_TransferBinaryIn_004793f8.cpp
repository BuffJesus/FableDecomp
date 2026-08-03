#pragma optimize("s",on)
#include <string.h>

// COpinionTransientOffset::TransferBinaryIn @ 0x004793f8 (81 bytes).
//
// Genuine C++ reconstruction. __fastcall member: this (ecx) = destination
// object (0x18 bytes = 6 dwords), one stack arg = the input stream.
//
// The body is the classic MSVC-inlined CDataInputStream fast-path read of a
// fixed 0x18-byte record.  The engine first validates that advancing the
// stream read-position by 0x18 keeps a 64-bit signed position in [0,0x7FFFFFFF]
// (this is why MSVC materialises the 64-bit `xor edx,edx` guard), then, if the
// current in-memory source window still holds at least 0x18 bytes, copies the
// record straight out of the window (rep movsd of 6 dwords) and bumps the
// window pointers; otherwise it defers to the general windowed reader on the
// stream.  A failed position guard leaves the destination untouched.
//
// Stream layout (offsets confirmed from the disasm and the sibling
// CLandscapeBackgroundPatch stream header):
//   +0x04 : long  ReadPosition          (advanced by 0x18)
//   +0x0C : unsigned char* Cursor       (advanced by 0x18)
//   +0x14 : long  RemainingInSourceChunk(decremented by 0x18)

struct CDataInputStream
{
    long           Pad00;                  // +0x00
    long           ReadPosition;           // +0x04
    long           Pad08;                  // +0x08
    unsigned char* Cursor;                 // +0x0C
    long           Pad10;                  // +0x10
    long           RemainingInSourceChunk; // +0x14

    // General windowed reader (reloc-masked): __fastcall member on the stream,
    // this (ecx) = stream, two stack args (destination, size).
    void ReadWithSrcChunkOverflow(unsigned char* destination, long size);
};

struct COpinionTransientOffset
{
    unsigned char Bytes[0x18];             // the 6-dword record this reader fills

    void TransferBinaryIn(CDataInputStream* stream);
};

void COpinionTransientOffset::TransferBinaryIn(CDataInputStream* stream)
{
    __int64 nextPosition = (unsigned long)(stream->ReadPosition + 0x18);
    if (nextPosition >= 0 && nextPosition <= 0x7FFFFFFF)
    {
        if (stream->RemainingInSourceChunk >= 0x18)
        {
            memcpy(this, stream->Cursor, 0x18);
            stream->Cursor += 0x18;
            stream->RemainingInSourceChunk -= 0x18;
            stream->ReadPosition += 0x18;
        }
        else
        {
            stream->ReadWithSrcChunkOverflow(this->Bytes, 0x18);
        }
    }
}