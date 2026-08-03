__declspec(naked) void CThingSearchTools()
{
    __asm {
        mov edx, dword ptr [esp+4]
        mov eax, ecx
        mov ecx, dword ptr [esp+8]
        mov dword ptr [eax+8], edx
        mov dword ptr [eax], ecx
        xor edx, edx
        mov dword ptr [eax+0Ch], edx
        mov dword ptr [eax+10h], edx
        mov dword ptr [eax+14h], edx
        mov ecx, dword ptr [ecx+10h]
        mov dword ptr [eax+4], ecx
        ret 8
    }
}