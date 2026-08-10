extern "C" unsigned long __security_cookie_g;
extern "C" unsigned char g_flag_b;

void __declspec(naked) SetStateAfterYesNo()
{
    __asm {
        sub  esp, 0x34
        mov  eax, __security_cookie_g
        push ebx
        mov  dword ptr [esp+0x34], eax
        mov  al, byte ptr g_flag_b
        xor  ebx, ebx
        cmp  al, bl
    }
}