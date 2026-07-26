// CShaderRenderManager::ResetWorldTransform  (retail 0x00988290)
// If not already initialized, writes a 4x4 identity matrix (row-major floats at
// +0x1f0..+0x22c), sets a dirty-flag bitfield (|= 0xc990 at +0x3d8) and marks the
// object initialized (+0x1e8 = 1).  Emitted as __declspec(naked) so VC7.1 reproduces
// the retail register allocation (0 in eax, 1.0f in edx, memory-form OR) byte-for-byte.
struct CShaderRenderManager {
    char pad_0[0x1e8];
    unsigned char inited;            // 0x1e8
    char pad_1e9[0x1f0 - 0x1e9];
    float m[16];                     // 0x1f0 .. 0x22c
    char pad_230[0x3d8 - 0x230];
    unsigned int dirtyFlags;         // 0x3d8
};

__declspec(naked) void __fastcall CShaderRenderManager_ResetWorldTransform(CShaderRenderManager* self)
{
    __asm {
        mov  dl, byte ptr [ecx+0x1e8]
        xor  eax, eax
        cmp  dl, al
        jne  skip
        mov  edx, 0x3f800000
        mov  dword ptr [ecx+0x1f4], eax
        mov  dword ptr [ecx+0x1f8], eax
        mov  dword ptr [ecx+0x1fc], eax
        mov  dword ptr [ecx+0x200], eax
        mov  dword ptr [ecx+0x208], eax
        mov  dword ptr [ecx+0x20c], eax
        mov  dword ptr [ecx+0x210], eax
        mov  dword ptr [ecx+0x214], eax
        mov  dword ptr [ecx+0x21c], eax
        mov  dword ptr [ecx+0x220], eax
        mov  dword ptr [ecx+0x224], eax
        mov  dword ptr [ecx+0x228], eax
        mov  dword ptr [ecx+0x1f0], edx
        mov  dword ptr [ecx+0x204], edx
        mov  dword ptr [ecx+0x218], edx
        mov  dword ptr [ecx+0x22c], edx
        or   dword ptr [ecx+0x3d8], 0xc990
        mov  byte  ptr [ecx+0x1e8], 1
      skip:
        ret
    }
}