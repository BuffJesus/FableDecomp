// CTCInventoryBase::ProcessButtonAReleased @ 0x005bc638
// Exact transcription:
//   mov eax,[ecx+0x1c]
//   dec eax
//   jne  end
//   mov eax,[ecx]
//   push 0
//   call [eax+0xb8]
// end:
//   ret
__declspec(naked) void ProcessButtonAReleased()
{
    __asm {
        mov  eax, dword ptr [ecx+0x1c]
        dec  eax
        jne  end
        mov  eax, dword ptr [ecx]
        push 0
        call dword ptr [eax+0xb8]
    end:
        ret
    }
}