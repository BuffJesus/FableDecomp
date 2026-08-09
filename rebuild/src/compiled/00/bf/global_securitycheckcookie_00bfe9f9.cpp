extern "C" unsigned long __security_cookie;
unsigned long __security_cookie = 0;
extern "C" __declspec(noreturn) void __fastcall report_gsfailure_stub(unsigned long cookie);
__declspec(naked) void __fastcall my__security_check_cookie(unsigned long cookie)
{
    (void)cookie;
    __asm {
        cmp ecx, __security_cookie
        jne bad
        ret
bad:
        jmp report_gsfailure_stub
    }
}
__declspec(naked) void __fastcall report_gsfailure_stub(unsigned long cookie)
{
    (void)cookie;
    __asm { ret }
}