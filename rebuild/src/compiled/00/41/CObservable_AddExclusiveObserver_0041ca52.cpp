struct CObservable {
    unsigned char _pad_0x0[0x13c];
    void* exclusiveObserver; /* +0x13c */
};

__declspec(naked) void __fastcall CObservable_AddExclusiveObserver(CObservable* self, void* observer) {
    (void)self; (void)observer;
    __asm {
        mov eax, dword ptr [esp+4]
        mov dword ptr [ecx+0x13c], eax
        ret 4
    }
}