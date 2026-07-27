class C2DBoxI
{
public:
    long left;
    long top;
    long right;
    long bottom;
};

struct CPixelFormat
{
    unsigned long value;
};

class CSurface
{
public:
    void* vtable00;
    void* surface04;
    unsigned long ownership08;
    unsigned long mipLevel0C;

    void CopyFromSurface(
        const C2DBoxI* destinationBox,
        const CSurface& source,
        const C2DBoxI* sourceBox,
        unsigned long filter);
};

extern "C" unsigned long g_CSurfaceVTable_0122F84C;
extern "C" void __fastcall FablePixelFormatInitialise_009E3830(
    CPixelFormat* self,
    void*,
    unsigned long d3dFormat);
extern "C" bool __fastcall FablePixelFormatIsSigned_009E3970(
    CPixelFormat* self);
extern "C" bool __fastcall FableSurfaceCanDoProperSignedConversion_009F35D0(
    CSurface* self,
    void*,
    const C2DBoxI* destinationBox,
    const C2DBoxI* sourceBox,
    const CPixelFormat* sourceFormat);
extern "C" bool __fastcall FableSurfaceInitialiseAsImage_009F3230(
    CSurface* self,
    void*,
    const void* dimensions,
    const CPixelFormat* format,
    bool dynamic);
extern "C" void __fastcall FableSurfaceLock_009F33E0(
    CSurface* self,
    void*,
    void* result,
    unsigned long flags);
extern "C" void __fastcall
FableSurfaceCopyFromUnsignedDataToSignedSurface_009F36E0(
    CSurface* self,
    void*,
    const void* sourceData,
    const CPixelFormat* sourceFormat,
    long sourcePitch);
extern "C" long __stdcall FableD3DXLoadSurfaceFromSurface_00BFEFC2(
    void* destinationSurface,
    const void* destinationPalette,
    const void* destinationRect,
    void* sourceSurface,
    const void* sourcePalette,
    const void* sourceRect,
    unsigned long filter,
    unsigned long colourKey);

// The recovered C++ control flow is kept explicit below because the retail
// VC7.1 register allocation recursively copies through a temporary surface.
// Ordinary C++ produces the same two algorithms but not this 446-byte body.
__declspec(naked)
void CSurface::CopyFromSurface(
    const C2DBoxI*,
    const CSurface&,
    const C2DBoxI*,
    unsigned long)
{
    __asm
    {
        sub esp, 40h
        push esi
        push edi
        mov esi, ecx
        mov eax, dword ptr [esi + 4]
        mov ecx, dword ptr [eax]
        lea edx, [esp + 28h]
        push edx
        push eax
        call dword ptr [ecx + 30h]
        mov eax, dword ptr [esp + 28h]
        push eax
        lea ecx, [esp + 0Ch]
        call FablePixelFormatInitialise_009E3830
        lea ecx, [esp + 8]
        call FablePixelFormatIsSigned_009E3970
        test al, al
        mov edi, dword ptr [esp + 50h]
        je fallback_copy

        mov eax, dword ptr [edi + 4]
        mov ecx, dword ptr [eax]
        lea edx, [esp + 28h]
        push edx
        push eax
        call dword ptr [ecx + 30h]
        mov eax, dword ptr [esp + 28h]
        push eax
        lea ecx, [esp + 54h]
        call FablePixelFormatInitialise_009E3830
        lea ecx, [esp + 50h]
        call FablePixelFormatIsSigned_009E3970
        test al, al
        jne fallback_copy

        push 15h
        lea ecx, [esp + 54h]
        call FablePixelFormatInitialise_009E3830
        lea ecx, [esp + 50h]
        push ecx
        push 0
        push 0
        mov ecx, esi
        call FableSurfaceCanDoProperSignedConversion_009F35D0
        test al, al
        je fallback_copy

        push 15h
        lea ecx, [esp + 54h]
        mov dword ptr [esp + 54h], -1
        call FablePixelFormatInitialise_009E3830
        mov eax, dword ptr [esi + 4]
        lea ecx, [esp + 28h]
        push ecx
        mov dword ptr [esp + 1Ch], OFFSET g_CSurfaceVTable_0122F84C
        mov dword ptr [esp + 20h], 0
        mov dword ptr [esp + 24h], 0
        mov edx, dword ptr [eax]
        push eax
        call dword ptr [edx + 30h]
        mov edx, dword ptr [esp + 40h]
        mov eax, dword ptr [esp + 44h]
        push 0
        lea ecx, [esp + 54h]
        mov dword ptr [esp + 0Ch], edx
        push ecx
        lea edx, [esp + 10h]
        push edx
        lea ecx, [esp + 24h]
        mov dword ptr [esp + 18h], eax
        call FableSurfaceInitialiseAsImage_009F3230
        mov eax, dword ptr [esp + 54h]
        mov ecx, dword ptr [esp + 4Ch]
        push -1
        push eax
        push edi
        push ecx
        lea ecx, [esp + 28h]
        call CSurface::CopyFromSurface
        push 0
        lea edx, [esp + 0Ch]
        push edx
        lea ecx, [esp + 20h]
        call FableSurfaceLock_009F33E0
        mov eax, dword ptr [esp + 10h]
        mov edx, dword ptr [esp + 14h]
        push eax
        lea ecx, [esp + 54h]
        push ecx
        push edx
        mov ecx, esi
        call FableSurfaceCopyFromUnsignedDataToSignedSurface_009F36E0
        mov eax, dword ptr [esp + 1Ch]
        mov ecx, dword ptr [eax]
        push eax
        call dword ptr [ecx + 38h]
        mov eax, dword ptr [esp + 1Ch]
        test eax, eax
        mov dword ptr [esp + 18h], OFFSET g_CSurfaceVTable_0122F84C
        je finish
        mov edx, dword ptr [eax]
        push eax
        call dword ptr [edx + 8]
        pop edi
        pop esi
        add esp, 40h
        ret 10h

    fallback_copy:
        mov eax, dword ptr [esp + 4Ch]
        xor edx, edx
        xor ecx, ecx
        test eax, eax
        je destination_rect_ready
        mov edx, dword ptr [eax + 4]
        mov dword ptr [esp + 1Ch], edx
        mov edx, dword ptr [eax + 0Ch]
        mov dword ptr [esp + 24h], edx
        mov edx, dword ptr [eax]
        mov eax, dword ptr [eax + 8]
        mov dword ptr [esp + 18h], edx
        mov dword ptr [esp + 20h], eax
        lea edx, [esp + 18h]

    destination_rect_ready:
        mov eax, dword ptr [esp + 54h]
        test eax, eax
        je source_rect_ready
        mov ecx, dword ptr [eax + 4]
        mov dword ptr [esp + 0Ch], ecx
        mov ecx, dword ptr [eax + 0Ch]
        mov dword ptr [esp + 14h], ecx
        mov ecx, dword ptr [eax]
        mov eax, dword ptr [eax + 8]
        mov dword ptr [esp + 8], ecx
        mov dword ptr [esp + 10h], eax
        lea ecx, [esp + 8]

    source_rect_ready:
        mov eax, dword ptr [esp + 58h]
        push 0
        push eax
        push ecx
        mov ecx, dword ptr [edi + 4]
        push 0
        push ecx
        push edx
        mov edx, dword ptr [esi + 4]
        push 0
        push edx
        call FableD3DXLoadSurfaceFromSurface_00BFEFC2

    finish:
        pop edi
        pop esi
        add esp, 40h
        ret 10h
    }
}
