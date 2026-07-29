extern "C" __declspec(naked) void __fastcall
std_Fill_n_ulong_0066ec31(
    unsigned long* destination, unsigned long count, const unsigned long* value)
{
    __asm
    {
        test edx, edx
        mov eax, ecx
        jbe done

    fill:
        mov ecx, [esp + 4]
        mov ecx, [ecx]
        mov [eax], ecx
        add eax, 4
        dec edx
        jne fill

    done:
        ret 4
    }
}
