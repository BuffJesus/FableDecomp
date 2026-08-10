struct CWorldMap;

extern "C" void f_a01b50(void);
extern "C" void f_4c7aa0(void);

__declspec(naked) CWorldMap* GetWorldMap(void* self)
{
    (void)self;
    __asm {
        add ecx, 8
        call f_a01b50
        mov ecx, eax
        call f_4c7aa0
        mov edx, dword ptr [eax]
        mov ecx, eax
        jmp dword ptr [edx + 0x30]
    }
}