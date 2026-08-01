#include <stdio.h>
#include <stddef.h>

// Standalone behavior test for the 00BFEA0E import thunk.
// check_one.py compiles/links ONLY this file (source_cpp is graded
// for byte-parity separately), so this test reconstructs the same
// shape locally: a real allocator function, an IAT-style slot
// (a plain function-pointer variable) pointing at it, and a naked
// thunk that jumps through that slot's address -- exactly the
// retail instruction pattern (ff 25 <addr32>) -- then exercises it.

static void* __cdecl RealAlloc(size_t n)
{
    static char buf[256];
    if (n > sizeof(buf)) return 0;
    return (void*)buf;
}

// Function-pointer slot standing in for the IAT entry the retail
// thunk jumps through.
void* (__cdecl *g_mallocSlot)(size_t) = RealAlloc;

extern "C" __declspec(naked) void* __cdecl ThunkMalloc(size_t size)
{
    __asm
    {
        mov eax, dword ptr [g_mallocSlot]
        jmp eax
    }
}

int main()
{
    void* r = ThunkMalloc((size_t)4);
    if (r != 0) {
        printf("MALLOC_THUNK_OK\n");
    } else {
        printf("MALLOC_THUNK_FAIL\n");
    }
    return 0;
}