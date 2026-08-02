#include <stdio.h>
#include <string.h>
#include "rebuild_abi.h"

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

struct CCharString
{
    CCharStringData* storage_;

    CCharString(const char* text, long length);
    CCharString(const CCharString& rhs);
    ~CCharString();
};

namespace
{
    CCharStringData g_storage;
    int g_ctorCalls = 0;
    int g_copyCalls = 0;
    int g_dtorCalls = 0;
    const char* g_lastText = 0;
    long g_lastLen = 0;
}

CCharString::CCharString(const char* text, long length)
{
    ++g_ctorCalls;
    g_lastText = text;
    g_lastLen = length;
    if (text != 0 && text[0] != '\0')
    {
        g_storage.text = (char*)text;
        g_storage.length = (fable_u32)strlen(text);
        storage_ = &g_storage;
    }
    else
    {
        storage_ = 0;
    }
}

CCharString::CCharString(const CCharString& rhs)
{
    ++g_copyCalls;
    storage_ = rhs.storage_;
}

CCharString::~CCharString()
{
    ++g_dtorCalls;
}

struct CMemoryDataInputStream
{
    void**   vtable;
    long     position;
    long     sourceLength;
    char*    chunkBase;
    long     chunkPosition;
    long     chunkLength;
    char*    sourceBase;

    CCharString ReadNullTerminatedString();
};

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

namespace
{
    long __fastcall FakeGetPosition(CMemoryDataInputStream* self)
    {
        return self->position;
    }

    void ResetFixture()
    {
        g_ctorCalls = 0;
        g_copyCalls = 0;
        g_dtorCalls = 0;
        g_lastText = 0;
        g_lastLen = 0;
        memset(&g_storage, 0, sizeof(g_storage));
    }

    void* g_vtableSlots[3] = { 0, 0, (void*)&FakeGetPosition };

    int VerifyBasicReadAdvancesPosition()
    {
        ResetFixture();

        char buffer[] = "Hello\0World";
        CMemoryDataInputStream stream;
        stream.vtable = g_vtableSlots;
        stream.position = 0;
        stream.sourceLength = (long)sizeof(buffer);
        stream.chunkBase = buffer;
        stream.chunkPosition = 0;
        stream.chunkLength = (long)sizeof(buffer);
        stream.sourceBase = buffer;

        CCharString result = stream.ReadNullTerminatedString();

        if (g_lastText != buffer) return 1;
        if (g_lastLen != -1) return 2;
        if (stream.position != 6) return 3;             // "Hello" (5) + NUL
        if (stream.chunkBase != buffer + 6) return 4;
        if (stream.chunkLength != (long)sizeof(buffer) - 6) return 5;
        if (g_ctorCalls != 1) return 6;
        if (g_copyCalls != 1) return 7;
        if (g_dtorCalls != 1) return 8;
        if (result.storage_ != &g_storage) return 9;

        return 0;
    }

    int VerifySecondReadContinuesFromNewPosition()
    {
        ResetFixture();

        char buffer[] = "Hello\0World";
        CMemoryDataInputStream stream;
        stream.vtable = g_vtableSlots;
        stream.position = 6;
        stream.sourceLength = (long)sizeof(buffer);
        stream.chunkBase = buffer + 6;
        stream.chunkPosition = 0;
        stream.chunkLength = (long)sizeof(buffer) - 6;
        stream.sourceBase = buffer;

        CCharString result = stream.ReadNullTerminatedString();

        if (g_lastText != buffer + 6) return 11;
        if (stream.position != (long)sizeof(buffer)) return 12; // 6 + "World"(5) + NUL
        (void)result;

        return 0;
    }
}

int main()
{
    int result = VerifyBasicReadAdvancesPosition();
    if (result == 0)
        result = VerifySecondReadContinuesFromNewPosition();

    if (result != 0)
    {
        printf("FABLETLC_READ_NULL_TERM_STRING_BEHAVIOR FAIL code=%d\n", result);
        return result;
    }

    printf("FABLETLC_READ_NULL_TERM_STRING_BEHAVIOR PASS\n");
    return 0;
}