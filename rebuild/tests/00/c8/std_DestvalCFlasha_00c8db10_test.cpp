#include <cstdio>

namespace NCreatureHitNotification
{
    struct CFlashListNode;

    struct CFlash
    {
        CFlashListNode* FlashGeometryHead;
        long            FlashGeometrySize;
        long            CreationFrame;
    };
}

using namespace NCreatureHitNotification;

struct allocator_CFlash
{
    int unused_; // avoid empty-class layout aliasing with the CFlash local
};

static int   g_flag_seen = -1;
static void* g_this_seen = 0;

// Real thiscall-shaped callee: this = CFlash* in ecx, one stack argument
// (flag). Genuine member function -- this-in-ecx, flag-on-stack -- called
// through a small naked forwarder so both sides of the call agree on the
// exact register/stack contract without writing the banned __fastcall
// keyword anywhere.
struct CFlashDtorHelper
{
    void Run(int flag)
    {
        g_this_seen = this;
        g_flag_seen = flag;
    }
};

extern "C" __declspec(naked) void CFlash_ScalarDeletingDestructor_thiscall(CFlash* /*self*/, int /*flag*/)
{
    __asm
    {
        // Entry: ecx = self (as passed by the retail-shaped caller), and
        // [esp+4] = flag (the caller's "push 1"). Forward both to Run,
        // which is a genuine this-in-ecx / flag-on-stack member function.
        mov eax, [esp+4]
        push eax
        call CFlashDtorHelper::Run
        ret 4
    }
}

extern "C" __declspec(naked) void __fastcall Dest_val_CFlash_a(allocator_CFlash* /*a*/, CFlash* /*_Ptr*/)
{
    __asm
    {
        mov eax, ecx
        push 1
        mov ecx, edx
        call CFlash_ScalarDeletingDestructor_thiscall
        ret
    }
}

int main()
{
    allocator_CFlash alloc;
    CFlash flash;
    flash.FlashGeometryHead = 0;
    flash.FlashGeometrySize = 0;
    flash.CreationFrame = 7;

    Dest_val_CFlash_a(&alloc, &flash);

    if (g_this_seen == (void*)&flash && g_flag_seen == 1)
        std::printf("DEST_VAL_CFLASH_OK\n");
    else
        std::printf("DEST_VAL_CFLASH_BAD this=%p flag=%d\n", g_this_seen, g_flag_seen);

    return 0;
}