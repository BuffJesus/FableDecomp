// GetTypeBalancedAttractiveness @ 0x0057fc46  (CTattooDef)
// __fastcall: this=ecx; one stack arg = pointer p to {begin,end} (vector-like).
// count = (p->end - p->begin) / 0x48 elements.
// if count==5: return p->begin[0x15c-as-float] * this->+0x54  else return this->+0x54
// Returns float in st(0); ret 4.
__declspec(naked) float GetTypeBalancedAttractiveness()
{
    __asm {
        mov eax, dword ptr [esp+4]
        push esi
        mov esi, dword ptr [eax]
        mov eax, dword ptr [eax+4]
        push edi
        sub eax, esi
        push 0x48
        cdq
        pop edi
        idiv edi
        cmp eax, 5
        jne notfive
        fld dword ptr [esi+0x15c]
        fmul dword ptr [ecx+0x54]
        jmp done
    notfive:
        fld dword ptr [ecx+0x54]
    done:
        pop edi
        pop esi
        ret 4
    }
}