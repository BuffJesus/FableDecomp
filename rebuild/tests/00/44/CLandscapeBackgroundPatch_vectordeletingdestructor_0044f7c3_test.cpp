#include <cstdio>

static int g_dtor_calls = 0;
static int g_delete_calls = 0;
static void* g_deleted = 0;

extern "C" void __fastcall CLBP_realdtor(void* self) { g_dtor_calls++; }
extern "C" void __cdecl CLBP_opdelete(void* p) { g_delete_calls++; g_deleted = p; }
extern "C" void* __cdecl CLBP_opnew(unsigned int n) { return 0; }
extern "C" void __fastcall CLBP_ctor(void* self) {}

__declspec(naked) void* __fastcall vector_deleting_destructor(void* self, int edx_dummy, unsigned char flags)
{
    __asm {
        push    esi
        mov     esi, ecx
        mov     dword ptr [esi], 01230BA0h
        call    CLBP_realdtor
        test    byte ptr [esp+8], 1
        je      skip_delete
        push    esi
        call    CLBP_opdelete
        pop     ecx
    skip_delete:
        mov     eax, esi
        pop     esi
        ret     4
        push    0C8h
        call    CLBP_opnew
        test    eax, eax
        pop     ecx
        je      new_failed
        mov     ecx, eax
        jmp     CLBP_ctor
    new_failed:
        xor     eax, eax
        ret
    }
}

int main()
{
    unsigned int obj = 0;

    // flags = 0: destruct, no delete, returns self, vptr written
    void* r = vector_deleting_destructor(&obj, 0, 0);
    if (r != &obj)                { std::printf("FAIL ret\n"); return 1; }
    if (obj != 0x1230BA0u)        { std::printf("FAIL vptr\n"); return 1; }
    if (g_dtor_calls != 1)        { std::printf("FAIL dtor\n"); return 1; }
    if (g_delete_calls != 0)      { std::printf("FAIL nodelete\n"); return 1; }

    // flags = 1: destruct AND delete
    obj = 0;
    r = vector_deleting_destructor(&obj, 0, 1);
    if (r != &obj)                { std::printf("FAIL ret2\n"); return 1; }
    if (g_dtor_calls != 2)        { std::printf("FAIL dtor2\n"); return 1; }
    if (g_delete_calls != 1)      { std::printf("FAIL delete\n"); return 1; }
    if (g_deleted != &obj)        { std::printf("FAIL delptr\n"); return 1; }

    std::printf("CLandscapeBackgroundPatch_0044f7c3_TEST PASS\n");
    return 0;
}