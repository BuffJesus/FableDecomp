struct Inner;

struct InnerVtbl {
    void* s0;
    void* s1;
    void* s2;
    void* s3;
    void* s4;
    void* s5;
    void* s6;
    bool (__fastcall* slot7)(void* self, int, void* p);
};

struct Inner {
    InnerVtbl* vtbl;
};

struct CTCCreatureNavigation {
    void* pad0;
    Inner* m_field4;
};

__declspec(naked) bool __fastcall IsNewDestinationGoingToResetNavigation(CTCCreatureNavigation* self, int edx, void* dest, float f) {
    __asm {
        push esi
        mov esi, dword ptr [esp+8]
        mov eax, ecx
        mov ecx, dword ptr [eax+4]
        mov edx, dword ptr [ecx]
        push esi
        push eax
        call dword ptr [edx+0x1c]
        pop esi
        ret 4
    }
}