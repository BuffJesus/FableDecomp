#include <stdio.h>

// ---- function under test (definition) ----
struct EditMapEntry {
    void* a;      // +0x0
    void* map;    // +0x4
    void* c;      // +0x8
};
struct CEditWorldMap {
    char pad[0x3c];
    EditMapEntry* maps;   // +0x3c
};

void* __fastcall GetEditMap(CEditWorldMap* self, void* /*edx*/, int index)
{
    return self->maps[index].map;
}

// ---- test harness ----
int main()
{
    EditMapEntry arr[3];
    arr[0].a = (void*)0x1000; arr[0].map = (void*)0xAAAA0000; arr[0].c = (void*)0x2000;
    arr[1].a = (void*)0x1001; arr[1].map = (void*)0xBBBB1111; arr[1].c = (void*)0x2001;
    arr[2].a = (void*)0x1002; arr[2].map = (void*)0xCCCC2222; arr[2].c = (void*)0x2002;

    CEditWorldMap obj;
    obj.maps = arr;

    // Verify struct offset assumption: maps must be at +0x3c
    if ((char*)&obj.maps - (char*)&obj != 0x3c) {
        printf("BAD_OFFSET\n");
        return 1;
    }

    void* r0; void* r1; void* r2;
    CEditWorldMap* p = &obj;

    // __fastcall via asm shim: ecx=this, arg pushed (callee cleans with ret 4)
    __asm {
        push 0
        mov  ecx, p
        call GetEditMap
        mov  r0, eax
    }
    __asm {
        push 1
        mov  ecx, p
        call GetEditMap
        mov  r1, eax
    }
    __asm {
        push 2
        mov  ecx, p
        call GetEditMap
        mov  r2, eax
    }

    if (r0 != (void*)0xAAAA0000) { printf("FAIL r0=%p\n", r0); return 1; }
    if (r1 != (void*)0xBBBB1111) { printf("FAIL r1=%p\n", r1); return 1; }
    if (r2 != (void*)0xCCCC2222) { printf("FAIL r2=%p\n", r2); return 1; }

    printf("OK_0x0051e580\n");
    return 0;
}