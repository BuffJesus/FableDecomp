struct C3DVector { float x,y,z; };
struct Nav;
struct NavInner {
    unsigned char (__fastcall **vtbl)(void*, int, C3DVector*, Nav*);
};
struct Nav {
    void* f0;
    NavInner* f4;
};

__declspec(naked) bool __fastcall IsNewDestinationGoingToResetNavigation(Nav* self, int /*edx*/, C3DVector* dest)
{
    (void)self; (void)dest;
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