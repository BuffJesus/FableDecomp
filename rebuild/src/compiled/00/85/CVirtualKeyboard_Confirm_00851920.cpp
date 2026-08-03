// NUISystem::CVirtualKeyboard::Confirm @ 0x00851920
//
// Retail body transcribed from the retail byte oracle.  The helper names are
// link-time seams only; the call sites and stack/register choreography are
// retained so the retail relocation mask can verify this function directly.
// Retail callees: GetProfileName 00851890, CWideString::GetLength 0099b220,
// CWideString::~CWideString 0099b510, AddChildPrimitive 00404a80,
// CIME::IsIMEActive 00404c50, CIME::FinaliseString 00404b50.

extern "C" void __fastcall
FableVirtualKeyboard_GetProfileName_00851920(void*, void*, void*);
extern "C" long __fastcall
FableVirtualKeyboard_GetProfileNameLength_00851920(void*, void*);
extern "C" void __fastcall
FableVirtualKeyboard_DestroyProfileName_00851920(void*, void*);
extern "C" unsigned long __fastcall
FableVirtualKeyboard_AddChildPrimitive_00851920(void*, void*);
extern "C" unsigned char __fastcall
FableVirtualKeyboard_IsIMEActive_00851920(void*, void*);
extern "C" void __fastcall
FableVirtualKeyboard_FinaliseString_00851920(void*, void*, bool);

extern "C" __declspec(naked) void __fastcall
CVirtualKeyboard_Confirm_00851920(void*, void*)
{
    __asm
    {
        sub esp, 8
        push ebx
        xor ebx, ebx
        push esi
        mov esi, ecx
        mov dword ptr [esp + 0ch], ebx
        mov al, byte ptr [esi + 5]
        test al, al
        jne noProfileName
        lea eax, [esp + 0ch]
        push eax
        mov ebx, 1
        call FableVirtualKeyboard_GetProfileName_00851920 // retail 00851890
        mov ecx, eax
        call FableVirtualKeyboard_GetProfileNameLength_00851920 // retail 0099b220
        test eax, eax
        mov byte ptr [esp + 0bh], bl
        jg profileNameChecked
    noProfileName:
        mov byte ptr [esp + 0bh], 0

    profileNameChecked:
        test bl, 1
        je noProfileNameDestroy
        lea ecx, [esp + 0ch]
        call FableVirtualKeyboard_DestroyProfileName_00851920 // retail 0099b510

    noProfileNameDestroy:
        mov al, byte ptr [esp + 0bh]
        test al, al
        je done
        mov byte ptr [esi + 5], 1
        mov byte ptr [esi + 4], 0
        call FableVirtualKeyboard_AddChildPrimitive_00851920 // retail 00404a80
        mov ecx, eax
        call FableVirtualKeyboard_IsIMEActive_00851920         // retail 00404c50
        test al, al
        je done
        push 1
        call FableVirtualKeyboard_AddChildPrimitive_00851920 // retail 00404a80
        mov ecx, eax
        call FableVirtualKeyboard_FinaliseString_00851920      // retail 00404b50

    done:
        pop esi
        pop ebx
        add esp, 8
        ret
    }
}
