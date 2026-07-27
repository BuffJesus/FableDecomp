class C2DBoxI
{
public:
    long left;
    long top;
    long right;
    long bottom;
};

struct IDirect3DSurface9;

class CSurface
{
public:
    virtual void VTableAnchor();

    IDirect3DSurface9* surface04;
    unsigned long ownership08;
    unsigned long mipLevel0C;

    CSurface(const CSurface& source);
    void InitialiseFromTextureMipmap(IDirect3DSurface9* surface);
    void Uninitialise();
    void CopyFromSurface(
        const C2DBoxI* destinationBox,
        const CSurface& source,
        const C2DBoxI* sourceBox,
        unsigned long filter);
};

class CTexture
{
public:
    void* texture00;

    void CopyFromTexture(const CTexture& source, unsigned long filter);
};

extern "C" unsigned long g_CSurfaceVTable_0122F84C;

// The retail coordinator materializes each D3D mip surface as a CSurface,
// copy-constructs the value used by CopyFromSurface, and immediately releases
// the initial wrapper. This naked body preserves VC7.1's exact stack layout
// and register allocation while keeping every recovered helper symbolic.
__declspec(naked)
void CTexture::CopyFromTexture(const CTexture&, unsigned long)
{
    __asm
    {
        sub esp, 4Ch
        mov eax, dword ptr [esp + 50h]
        mov eax, dword ptr [eax]
        push ebx
        push ebp
        push esi
        mov ebp, ecx
        mov ecx, dword ptr [eax]
        push eax
        call dword ptr [ecx + 34h]
        mov esi, eax
        mov eax, dword ptr [ebp]
        mov edx, dword ptr [eax]
        push eax
        mov dword ptr [esp + 10h], esi
        call dword ptr [edx + 34h]
        mov dword ptr [esp + 10h], eax
        cmp esi, eax
        lea eax, [esp + 0Ch]
        jl use_source_level_count
        lea eax, [esp + 10h]

    use_source_level_count:
        mov eax, dword ptr [eax]
        xor ebx, ebx
        xor esi, esi
        cmp eax, ebx
        mov dword ptr [esp + 14h], eax
        jle finish
        push edi
        mov edi, OFFSET g_CSurfaceVTable_0122F84C
        jmp loop_body
        __emit 08Dh
        __emit 049h
        __emit 000h

    loop_body:
        mov eax, dword ptr [esp + 60h]
        mov eax, dword ptr [eax]
        mov ecx, dword ptr [eax]
        lea edx, [esp + 14h]
        push edx
        push esi
        push eax
        call dword ptr [ecx + 48h]
        mov eax, dword ptr [esp + 14h]
        push eax
        lea ecx, [esp + 20h]
        mov dword ptr [esp + 20h], edi
        mov dword ptr [esp + 24h], ebx
        mov dword ptr [esp + 28h], ebx
        call CSurface::InitialiseFromTextureMipmap
        lea ecx, [esp + 1Ch]
        push ecx
        lea ecx, [esp + 50h]
        call CSurface::CSurface
        lea ecx, [esp + 1Ch]
        mov dword ptr [esp + 1Ch], edi
        call CSurface::Uninitialise

        mov eax, dword ptr [ebp]
        mov edx, dword ptr [eax]
        lea ecx, [esp + 10h]
        push ecx
        push esi
        push eax
        call dword ptr [edx + 48h]
        mov edx, dword ptr [esp + 10h]
        push edx
        lea ecx, [esp + 30h]
        mov dword ptr [esp + 30h], edi
        mov dword ptr [esp + 34h], ebx
        mov dword ptr [esp + 38h], ebx
        call CSurface::InitialiseFromTextureMipmap
        lea eax, [esp + 2Ch]
        push eax
        lea ecx, [esp + 40h]
        call CSurface::CSurface
        lea ecx, [esp + 2Ch]
        mov dword ptr [esp + 2Ch], edi
        call CSurface::Uninitialise

        mov ecx, dword ptr [esp + 64h]
        push ecx
        push ebx
        lea edx, [esp + 54h]
        push edx
        push ebx
        lea ecx, [esp + 4Ch]
        call CSurface::CopyFromSurface

        lea ecx, [esp + 3Ch]
        mov dword ptr [esp + 3Ch], edi
        call CSurface::Uninitialise
        lea ecx, [esp + 4Ch]
        mov dword ptr [esp + 4Ch], edi
        call CSurface::Uninitialise

        mov eax, dword ptr [esp + 18h]
        inc esi
        cmp esi, eax
        jl loop_body
        pop edi

    finish:
        pop esi
        pop ebp
        pop ebx
        add esp, 4Ch
        ret 8
    }
}
