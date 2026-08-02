#include "rebuild_abi.h"

#include <stdio.h>
#include <string.h>

struct FableVectorBool
{
    unsigned char bytes[0x208];
};

extern "C" void FABLE_FASTCALL
FableVectorBool_BaseDtor_00572636(void* self);

extern "C" void FABLE_FASTCALL
FableVectorBool_dtor_0057463b(FableVectorBool* this_)
{
    *reinterpret_cast<unsigned long*>(this_->bytes + 0x204) = 0xfffffffful;
    FableVectorBool_BaseDtor_00572636(this_);
}

namespace
{
    unsigned int g_baseDtorCalls;
    void* g_baseDtorSelf;
}

extern "C" void FABLE_FASTCALL
FableVectorBool_BaseDtor_00572636(void* self)
{
    ++g_baseDtorCalls;
    g_baseDtorSelf = self;
}

int main()
{
    FableVectorBool obj;
    memset(&obj, 0xA5, sizeof(obj));

    FableVectorBool_dtor_0057463b(&obj);

    if (g_baseDtorCalls != 1)
    {
        return 1;
    }
    if (g_baseDtorSelf != &obj)
    {
        return 2;
    }
    if (*reinterpret_cast<unsigned long*>(&obj.bytes[0x204]) != 0xfffffffful)
    {
        return 3;
    }

    printf("FABLETLC_VECTOR_BOOL_DTOR_0057463B PASS\n");
    return 0;
}