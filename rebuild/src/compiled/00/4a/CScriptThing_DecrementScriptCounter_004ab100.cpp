// CScriptThing::DecrementScriptCounter @ 0x004ab100
//   mov ecx,[ecx+0x4]   ; this->m_counter
//   test ecx,ecx
//   je  ret
//   mov eax,[ecx]       ; vtable
//   jmp [eax+0x124]     ; tail-call virtual method (ecx=m_counter)
//   ret
__declspec(naked) void DecrementScriptCounter()
{
    __asm
    {
        mov ecx, dword ptr [ecx+0x4]
        test ecx, ecx
        je  done
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax+0x124]
    done:
        ret
    }
}