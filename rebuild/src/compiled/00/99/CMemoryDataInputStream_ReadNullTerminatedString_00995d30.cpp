#include <stddef.h>
#include "rebuild_abi.h"

// CCharStringData layout (retail-verified, see fable_string.h):
//   +0x00 char*  text
//   +0x04 uint32 length      (strlen-computed by the ctor)
//   +0x08 uint32 unknown08
//   +0x0c uint8  flags0C
//   +0x0d..0x10  owners (packed, unaligned)
#pragma pack(push, 1)
struct CCharStringData
{
    char*      text;
    fable_u32  length;
    fable_u32  unknown08;
    fable_u8   flags0C;
    fable_i32  owners;
};
#pragma pack(pop)

// CCharString ABI: a single storage pointer (sizeof == 4), see fable_string.h.
// Real member functions (default calling convention = thiscall) model the
// retail ctor/copy-ctor/dtor calls without an extra fastcall 'edx' slot.
struct CCharString
{
    CCharStringData* storage_;

    CCharString(const char* text, long length);
    CCharString(const CCharString& rhs);
    ~CCharString();
};

// CMemoryDataInputStream layout (retail-verified via OpenMemorySource @ 0x00994570
// and the CMemoryDataInputStream ctor @ 0x00994930):
//   +0x00 vtable
//   +0x04 position           (current byte offset from sourceBase)
//   +0x08 sourceLength       (full source length)
//   +0x0c chunkBase          (current chunk base pointer, tracks position)
//   +0x10 chunkPosition      (unused here)
//   +0x14 chunkLength        (current chunk length, tracks position)
//   +0x18 sourceBase         (full source base pointer)
struct CMemoryDataInputStream
{
    void**   vtable;
    long     position;      // +0x04
    long     sourceLength;  // +0x08
    char*    chunkBase;     // +0x0c
    long     chunkPosition; // +0x10
    long     chunkLength;   // +0x14
    char*    sourceBase;    // +0x18

    CCharString ReadNullTerminatedString();
};

// Single-register virtual call (this-only): typed __fastcall so the sole
// argument lands in ecx with no spurious edx clear, matching 'call [eax+8]'.
// Called directly through the vtable slot expression (not via an
// intermediate variable) so the compiler folds the load into the call
// instruction itself, matching retail's 'call dword ptr [eax+8]'.
typedef long (__fastcall *GetPositionFn)(CMemoryDataInputStream*);

CCharString CMemoryDataInputStream::ReadNullTerminatedString()
{
    long curPos = ((GetPositionFn)vtable[2])(this);
    const char* text = sourceBase + curPos;

    CCharString temp(text, -1);

    long strLen = (long)temp.storage_;
    if (strLen)
        strLen = *(long*)((char*)strLen + 4);
    long consumed = strLen + 1;

    if (consumed > 0)
    {
        __int64 newPosWide = ((GetPositionFn)vtable[2])(this) + (unsigned long)consumed;

        if (newPosWide >= 0 && newPosWide <= 0x7fffffff)
        {
            long newPos = (long)newPosWide;
            long oldPos = position;
            long delta = newPos - oldPos;
            chunkBase += delta;
            chunkLength += (oldPos - newPos);
            position = newPos;
        }
    }

    return temp;
}