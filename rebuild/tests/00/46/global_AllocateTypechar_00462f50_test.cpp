#include "rebuild_abi.h"
#include <cstdio>

extern "C" void* FABLE_STDCALL AllocateType_char_462f50(fable_u32 count, void* p2, void* p3);

static char g_buf[8];

// Mock allocator: returns g_buf when count != 0.
extern "C" void* FABLE_CDECL fable_alloc_462f50(fable_u32 count)
{
    (void)count;
    return g_buf;
}

// Mock init helper: no-op.
void FABLE_FASTCALL fable_init_462f50(void* p2, void* p3, void* mem, char* localByte)
{
    (void)p2; (void)p3; (void)mem; (void)localByte;
}

int main()
{
    void* r1 = AllocateType_char_462f50(4, (void*)0x1, (void*)0x2); // count!=0 -> g_buf
    void* r0 = AllocateType_char_462f50(0, (void*)0x1, (void*)0x2); // count==0 -> null
    if (r1 == (void*)g_buf && r0 == 0)
        printf("OK_462f50_ALLOC\n");
    else
        printf("FAIL_462f50\n");
    return 0;
}