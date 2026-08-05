// CScriptedMapBrush::GetTheme  retail 0x0082e330  (77 bytes)
// __thiscall member: unsigned long GetTheme(long x, long y, unsigned long layer) const
//   -> ret 0xc.  self in ecx.
//
// Retail body (annotated):
//   edx = x                     ; [esp+4]
//   if (x <  self->minX) fail   ; [ecx+0x38]     (push esi spills between checks)
//   if (x >= self->maxX) fail   ; [ecx+0x40]
//   eax = y                     ; [esp+0xc]
//   if (y <  self->minY) fail   ; [ecx+0x3c]
//   if (y >= self->maxY) fail   ; [ecx+0x44]
//   eax = (y - minY)*(maxX - minX) - minX + x
//   edx = layer + eax*5         ; layer = [esp+0x14]  (after 2 pushes)
//   return ((u32*)self->cells)[edx + 1]   ; self->cells = [ecx+8]
//   fail: return 0
//
// The retail stream interleaves the esi/edi register spills with the four
// bounds compares and shares one xor-eax fail tail.  A plain-C++ reimplement
// reorders the spills and lands at 81 bytes (DIFFER); emit the exact retail
// instruction stream as naked asm.  All branches are internal, no externs.

extern "C" __declspec(naked) unsigned long CScriptedMapBrush_GetTheme()
{
    __asm {
        mov     edx, dword ptr [esp+4]      // x
        cmp     edx, dword ptr [ecx+0x38]   // minX
        push    esi
        jl      fail
        cmp     edx, dword ptr [ecx+0x40]   // maxX
        jge     fail
        mov     eax, dword ptr [esp+0x0c]   // y  (esp shifted by push esi)
        cmp     eax, dword ptr [ecx+0x3c]   // minY
        jl      fail
        cmp     eax, dword ptr [ecx+0x44]   // maxY
        jge     fail
        sub     eax, dword ptr [ecx+0x3c]   // y - minY
        mov     esi, dword ptr [ecx+0x40]   // maxX
        push    edi
        mov     edi, dword ptr [ecx+0x38]   // minX
        sub     esi, edi                    // maxX - minX
        imul    eax, esi                    // (y-minY)*(maxX-minX)
        mov     esi, edi
        sub     eax, esi                    // - minX
        add     eax, edx                    // + x
        mov     edx, dword ptr [esp+0x14]   // layer (after 2 pushes)
        lea     edx, [edx+eax*4]
        add     edx, eax                    // layer + idx*5
        mov     eax, dword ptr [ecx+8]      // cells
        mov     eax, dword ptr [eax+edx*4+4]
        pop     edi
        pop     esi
        ret     0x0c
fail:
        xor     eax, eax
        pop     esi
        ret     0x0c
    }
}
