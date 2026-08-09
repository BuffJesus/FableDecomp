struct C3DVector { float x,y,z; };

// __fastcall bool IsNewDestinationGoingToResetNavigation(this, C3DVector* dest, float)
// this in ecx; dest at [esp+8] (after push esi -> [esp+8]); float unused.
// body: n = this->field4; call n->vtable[7](this, dest) with ecx=n (thiscall, 2 stack args)
__declspec(naked) bool IsNewDestinationGoingToResetNavigation()
{
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