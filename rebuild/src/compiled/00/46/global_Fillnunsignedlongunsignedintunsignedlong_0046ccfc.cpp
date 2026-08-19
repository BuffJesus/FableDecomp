extern "C" __declspec(naked) void __fastcall
Fill_n_(unsigned long *destination, unsigned long count, const unsigned long *value)
{
    __asm
    {
        test edx, edx
        mov eax, ecx
        jbe done

    fill:
        test eax, eax
        je skip
        mov ecx, [esp + 4]
        mov ecx, [ecx]
        mov [eax], ecx
    skip:
        add eax, 4
        dec edx
        jne fill

    done:
        ret 4
    }
}