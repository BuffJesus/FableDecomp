// CCharString inequality predicate @ 0x0099E900.
//
// CPersistContext::ReadSection uses this immediately after reading a binary
// section name.  Shared string storage compares equal by identity; otherwise
// retail rejects null/mismatched-length strings and only calls the byte-string
// comparator when both lengths agree.

extern "C" long __fastcall
CCharString_CompareBytes_0099e900(const char*, const char*);

extern "C" __declspec(naked) unsigned char __fastcall
CCharString_NotEqual_0099e900(
    const void*,
    void*,
    const void*)
{
    __asm
    {
        mov eax, dword ptr [ecx]
        mov ecx, dword ptr [esp + 4]
        mov ecx, dword ptr [ecx]
        cmp eax, ecx
        jne different_storage

        mov al, 1
        xor ecx, ecx
        test al, al
        setz cl
        mov al, cl
        ret 4

    different_storage:
        test eax, eax
        jz definitely_different
        test ecx, ecx
        jz definitely_different
        mov edx, dword ptr [eax + 4]
        cmp edx, dword ptr [ecx + 4]
        jne definitely_different
        mov edx, dword ptr [ecx]
        mov ecx, dword ptr [eax]
        call CCharString_CompareBytes_0099e900
        neg eax
        sbb al, al
        inc al
        xor ecx, ecx
        test al, al
        setz cl
        mov al, cl
        ret 4

    definitely_different:
        xor al, al
        xor ecx, ecx
        test al, al
        setz cl
        mov al, cl
        ret 4
    }
}
