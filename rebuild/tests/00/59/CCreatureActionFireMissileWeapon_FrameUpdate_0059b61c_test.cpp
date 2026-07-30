#include <stdio.h>

// grow slow-path stub (only reached when full; not exercised by the happy-path test)
extern "C" void slowpath_grow_0059b576(void) { /* no-op stub */ }

// Function under test: push_back fast-path @ 0x0059b61c (__fastcall, ret 4)
__declspec(naked) void FrameUpdate(void* /*arg*/)
{
    __asm {
        mov edx, dword ptr [ecx+0x18]
        mov eax, dword ptr [ecx+0x10]
        sub edx, 4
        cmp eax, edx
        je  do_grow
        test eax, eax
        je  advance
        mov edx, dword ptr [esp+4]
        mov edx, dword ptr [edx]
        mov dword ptr [eax], edx
    advance:
        add dword ptr [ecx+0x10], 4
        ret 4
    do_grow:
        jmp slowpath_grow_0059b576
    }
}

// Vector-like layout: +0x10 = cur ptr, +0x18 = end/capacity ptr.
struct Vec {
    char pad0[0x10];
    int* cur;   // +0x10
    char pad1[4];
    int* end;   // +0x18
};

static void call_push(void* self, int* pval)
{
    __asm {
        mov  eax, pval
        push eax
        mov  ecx, self
        call FrameUpdate
    }
}

int main()
{
    int storage[8] = {0,0,0,0,0,0,0,0};
    Vec v;
    v.cur = &storage[0];
    v.end = &storage[4];   // end-4 = &storage[3]; cur(&storage[0]) != that -> store path

    int value = 0x12345678;
    call_push(&v, &value);

    int ok = 1;
    if (storage[0] != 0x12345678) { printf("FAIL stored=%08x\n", storage[0]); ok = 0; }
    if (v.cur != &storage[1])     { printf("FAIL cur not advanced\n"); ok = 0; }

    int value2 = 0x0BADF00D;
    call_push(&v, &value2); // cur=&storage[1], end-4=&storage[3] -> still room
    if (storage[1] != 0x0BADF00D) { printf("FAIL stored2=%08x\n", storage[1]); ok = 0; }
    if (v.cur != &storage[2])     { printf("FAIL cur2 not advanced\n"); ok = 0; }

    if (ok) { printf("OK_0x0059b61c\n"); return 0; }
    return 1;
}