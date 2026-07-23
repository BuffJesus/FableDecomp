#include <cstdio>

static int g_dtor_calls = 0;
static int g_free_calls = 0;
static void* g_last_free = 0;

extern "C" void __cdecl cbp_dtor(void)    { ++g_dtor_calls; }
extern "C" void __cdecl cbp_free(void)    { ++g_free_calls; }
extern "C" void __cdecl cbp_realloc(void) { }

__declspec(naked) void* CLandscapeBackgroundPatch_vector_deleting_destructor(void) {
    __asm {
        push esi
        mov  esi, ecx
        call cbp_dtor
        test byte ptr [esp+8], 1
        je   skip_free
        push esi
        call cbp_free
        add  esp, 4
    skip_free:
        mov  eax, esi
        pop  esi
        ret  4
        _emit 0xcc
        _emit 0xcc
        push edi
        mov  edi, ecx
        mov  eax, [edi+4]
        cmp  eax, eax
        mov  ecx, [edi]
        jne  do_realloc
        mov  [edi+4], ecx
        pop  edi
        ret
    do_realloc:
        push esi
        mov  esi, eax
        sub  esi, eax
        push esi
        push eax
        push ecx
        call cbp_realloc
        add  esp, 0Ch
        add  eax, esi
        pop  esi
        mov  [edi+4], eax
        pop  edi
        ret
    }
}

// Call the func1 entry: this in ecx, one stack arg (flags), callee pops 4 (ret 4).
typedef void* (__fastcall *DelDtorFn)(void* self, void* edx_ignored, unsigned int flags);

int main() {
    int obj = 0;
    void* self = &obj;
    DelDtorFn fn = (DelDtorFn)(void*)&CLandscapeBackgroundPatch_vector_deleting_destructor;

    // flags bit0 clear -> dtor runs, no free, returns self
    g_dtor_calls = g_free_calls = 0;
    void* r1 = fn(self, 0, 0);
    if (r1 != self || g_dtor_calls != 1 || g_free_calls != 0) {
        std::printf("FAIL path-nofree r=%p dtor=%d free=%d\n", r1, g_dtor_calls, g_free_calls);
        return 1;
    }

    // flags bit0 set -> dtor runs, free runs, returns self
    g_dtor_calls = g_free_calls = 0;
    void* r2 = fn(self, 0, 1);
    if (r2 != self || g_dtor_calls != 1 || g_free_calls != 1) {
        std::printf("FAIL path-free r=%p dtor=%d free=%d\n", r2, g_dtor_calls, g_free_calls);
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_0044be30_TEST_PASS\n");
    return 0;
}