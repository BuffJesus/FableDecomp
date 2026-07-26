// `vector_deleting_destructor' for CLandscapeBackgroundPatch
// Retail slices two adjacent COMDATs (the scalar-deleting-destructor thunk +
// an inlined container tidy helper) into one 73-byte region. Reproduced as a
// single naked function so the whole 73-byte span matches byte-for-byte;
// the three call sites are real relocations so the parity mask handles them.

extern "C" void __cdecl cbp_dtor(void);       // 0x3c1f0
extern "C" void __cdecl cbp_free(void);       // 0x7b2b8c
extern "C" void __cdecl cbp_realloc(void);    // 0x7b2cb6

__declspec(naked) void* CLandscapeBackgroundPatch_vector_deleting_destructor(void) {
    __asm {
        // ---- func1: scalar deleting destructor (this=ecx, flags @ [esp+8]) ----
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
        // ---- padding ----
        _emit 0xcc
        _emit 0xcc
        // ---- func2: container tidy helper (this=ecx) ----
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