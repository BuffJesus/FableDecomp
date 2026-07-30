#include "fable_persist_context.h"

extern "C" fable_u8 g_FableSignedCharType_0122D6C4;
extern "C" void* g_FableTransferSignedCharJumpTable_0040467C[5];

extern "C" void __fastcall FablePersistCheckCrc_004045c0(
    void*,
    void*,
    const char*);
extern "C" void __fastcall FablePersistWriteBool_004045c0(
    void*,
    void*,
    fable_u32);
extern "C" void __fastcall FablePersistTransferBinaryIn_004045c0(
    void*,
    void*,
    void*,
    signed char*);
extern "C" void __fastcall FablePersistTransferObjectSaveText_004045c0(
    void*,
    void*,
    const char*,
    void*);
extern "C" void __fastcall FablePersistTransferObjectLoadText_004045c0(
    void*,
    void*,
    const char*,
    void*);

__declspec(naked) void
CPersistContext::TransferSignedChar(
    const char*,
    signed char&,
    const signed char&)
{
    __asm
    {
        sub esp, 8
        push esi
        push edi
        mov edi, dword ptr [esp + 14h]
        push edi
        mov esi, ecx
        call FablePersistCheckCrc_004045c0
        mov eax, dword ptr [esi + 18h]
        jmp dword ptr [eax * 4 + g_FableTransferSignedCharJumpTable_0040467C]
    }
}

extern "C" __declspec(naked) void
FablePersistSignedCharDefault_004045DB()
{
    __asm
    {
        mov ecx, dword ptr [esp + 1Ch]
        mov dl, byte ptr [ecx]
        mov eax, dword ptr [esp + 18h]
        pop edi
        mov byte ptr [eax], dl
        pop esi
        add esp, 8
        ret 0Ch
    }
}

extern "C" __declspec(naked) void
FablePersistSignedCharBinaryOut_004045EF()
{
    __asm
    {
        mov ecx, dword ptr [esp + 18h]
        xor edx, edx
        mov dl, byte ptr [ecx]
        mov ecx, dword ptr [esi + 28h]
        push edx
        call FablePersistWriteBool_004045c0
        pop edi
        pop esi
        add esp, 8
        ret 0Ch
    }
}

extern "C" __declspec(naked) void
FablePersistSignedCharBinaryIn_00404608()
{
    __asm
    {
        mov eax, dword ptr [esp + 18h]
        mov ecx, dword ptr [esi + 24h]
        push eax
        push ecx
        lea ecx, dword ptr [esp + 20h]
        call FablePersistTransferBinaryIn_004045c0
        pop edi
        pop esi
        add esp, 8
        ret 0Ch
    }
}

extern "C" __declspec(naked) void
FablePersistSignedCharTextSave_00404622()
{
    __asm
    {
        mov edx, dword ptr [esp + 18h]
        lea eax, dword ptr [esp + 8]
        push eax
        push edi
        mov ecx, esi
        mov dword ptr [esp + 10h], offset g_FableSignedCharType_0122D6C4
        mov dword ptr [esp + 14h], edx
        call FablePersistTransferObjectSaveText_004045c0
        pop edi
        pop esi
        add esp, 8
        ret 0Ch
    }
}

extern "C" __declspec(naked) void
FablePersistSignedCharTextLoad_00404647()
{
    __asm
    {
        mov al, byte ptr [esi + 14h]
        test al, al
        mov eax, dword ptr [esp + 18h]
        jz text_default_ready
        mov ecx, dword ptr [esp + 1Ch]
        mov dl, byte ptr [ecx]
        mov byte ptr [eax], dl

    text_default_ready:
        mov dword ptr [esp + 0Ch], eax
        lea eax, dword ptr [esp + 8]
        push eax
        push edi
        mov ecx, esi
        mov dword ptr [esp + 10h], offset g_FableSignedCharType_0122D6C4
        call FablePersistTransferObjectLoadText_004045c0
        pop edi
        pop esi
        add esp, 8
        ret 0Ch
    }
}
