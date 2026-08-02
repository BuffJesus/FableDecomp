#include "rebuild_abi.h"

#include <stdio.h>
#include <string.h>

struct CThreadedFile
{
    unsigned char bytes[0x1C];
};

extern "C" void FABLE_FASTCALL
CThreadedFileBaseConstruct_0099A2F0(void* self);

extern "C" void FABLE_FASTCALL
CThreadedFileMemberConstruct_0099AED0(void* self);

extern "C" CThreadedFile* FABLE_FASTCALL
CThreadedFile_ctor(CThreadedFile* this_)
{
    CThreadedFileBaseConstruct_0099A2F0(this_);

    *reinterpret_cast<unsigned long*>(this_->bytes) = 0x0129a158ul;
    CThreadedFileMemberConstruct_0099AED0(this_->bytes + 0xC);

    this_->bytes[0x18] = 0;
    this_->bytes[0x19] = 0;
    *reinterpret_cast<unsigned long*>(this_->bytes + 0x8) = 0xfffffffful;

    return this_;
}

namespace
{
    unsigned int g_baseConstructCalls;
    void* g_baseConstructSelf;
    unsigned int g_memberConstructCalls;
    void* g_memberConstructSelf;
}

extern "C" void FABLE_FASTCALL
CThreadedFileBaseConstruct_0099A2F0(void* self)
{
    ++g_baseConstructCalls;
    g_baseConstructSelf = self;
    memset(self, 0x11, 0x8);
}

extern "C" void FABLE_FASTCALL
CThreadedFileMemberConstruct_0099AED0(void* self)
{
    ++g_memberConstructCalls;
    g_memberConstructSelf = self;
    memset(self, 0x22, 4);
}

int main()
{
    CThreadedFile obj;
    memset(&obj, 0xA5, sizeof(obj));

    CThreadedFile* result = CThreadedFile_ctor(&obj);

    if (result != &obj)
    {
        return 1;
    }
    if (g_baseConstructCalls != 1 || g_baseConstructSelf != &obj)
    {
        return 2;
    }
    if (g_memberConstructCalls != 1 || g_memberConstructSelf != &obj.bytes[0xC])
    {
        return 3;
    }
    if (*reinterpret_cast<unsigned long*>(&obj.bytes[0]) != 0x0129a158ul)
    {
        return 4;
    }
    if (*reinterpret_cast<unsigned long*>(&obj.bytes[0x8]) != 0xfffffffful)
    {
        return 5;
    }
    if (obj.bytes[0x18] != 0 || obj.bytes[0x19] != 0)
    {
        return 6;
    }

    printf("FABLETLC_CTHREADEDFILE_CTOR_0098DFD0 PASS\n");
    return 0;
}