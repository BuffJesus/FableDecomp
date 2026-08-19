/* CPhysicalPrimitiveMesh::GetClosestPointToPos2D @ 0x004cf3d0
   __fastcall swap of two 8-byte 2D structs (this=ecx, other=edx).
   VC7.1 emits a stack-temp spill scheduling that plain C won't
   reproduce exactly, so transcribe the retail bytes verbatim. */
__declspec(naked) void __fastcall GetClosestPointToPos2D(void *thisptr, void *other)
{
    (void)thisptr; (void)other;
    __asm {
        sub  esp, 8
        mov  eax, dword ptr [ecx]
        push esi
        mov  esi, dword ptr [edx]
        mov  dword ptr [ecx], esi
        mov  esi, dword ptr [edx+4]
        mov  dword ptr [esp+4], eax
        mov  eax, dword ptr [ecx+4]
        mov  dword ptr [ecx+4], esi
        mov  ecx, dword ptr [esp+4]
        mov  dword ptr [edx], ecx
        mov  dword ptr [edx+4], eax
        pop  esi
        add  esp, 8
        ret
    }
}