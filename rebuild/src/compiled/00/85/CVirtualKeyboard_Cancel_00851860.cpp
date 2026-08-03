// NUISystem::CVirtualKeyboard::Cancel @ 0x00851860
//
// This is the retail body transcribed from the decompiler/byte oracle.  The
// three calls are deliberately relocation-isolated: their retail targets are
// documented below, while the eventual linked bank supplies the real engine
// implementations.  Do not replace this with a host-side keyboard shortcut.
//
// Retail bytes (rel32 call operands masked):
//   8a410584c07526c6410501c6410401e80c32bbff8bc8e8d533bbff84c0740e
//   6a00e8fa31bbff8bc8e8c332bbffc3

extern "C" unsigned long __fastcall
FableVirtualKeyboard_AddChildPrimitive_00851860(void*, void*);
extern "C" unsigned char __fastcall
FableVirtualKeyboard_IsIMEActive_00851860(void*, void*);
extern "C" void __fastcall
FableVirtualKeyboard_FinaliseString_00851860(void*, void*, bool);

extern "C" __declspec(naked) void __fastcall
CVirtualKeyboard_Cancel_00851860(void*, void*)
{
    __asm
    {
        mov al, byte ptr [ecx + 5]
        test al, al
        jne done
        mov byte ptr [ecx + 5], 1
        mov byte ptr [ecx + 4], 1
        call FableVirtualKeyboard_AddChildPrimitive_00851860 // retail 00404a80
        mov ecx, eax
        call FableVirtualKeyboard_IsIMEActive_00851860         // retail 00404c50
        test al, al
        je done
        push 0
        call FableVirtualKeyboard_AddChildPrimitive_00851860 // retail 00404a80
        mov ecx, eax
        call FableVirtualKeyboard_FinaliseString_00851860      // retail 00404b50
    done:
        ret
    }
}
