#include "fable_display_viewport.h"

__declspec(naked)
void CDisplayManager::SetViewport(const C2DBoxF&)
{
    __asm
    {
        sub esp, 20h
        mov eax, dword ptr [esp + 24h]
        mov edx, dword ptr [eax + 0ch]
        mov dword ptr [esp + 24h], edx
        fld dword ptr [esp + 24h]
        fistp dword ptr [esp + 0ch]
        mov edx, dword ptr [eax + 08h]
        mov dword ptr [esp + 24h], edx
        fld dword ptr [esp + 24h]
        fistp dword ptr [esp + 08h]
        mov edx, dword ptr [eax + 04h]
        mov dword ptr [esp + 24h], edx
        fld dword ptr [esp + 24h]
        fistp dword ptr [esp + 04h]
        mov eax, dword ptr [eax]
        mov dword ptr [esp + 24h], eax
        fld dword ptr [esp + 24h]
        fistp dword ptr [esp]
        mov edx, dword ptr [esp]
        mov eax, dword ptr [esp + 04h]
        mov dword ptr [esp + 10h], edx
        mov edx, dword ptr [esp + 08h]
        mov dword ptr [esp + 18h], edx
        mov dword ptr [esp + 14h], eax
        mov eax, dword ptr [esp + 0ch]
        lea edx, dword ptr [esp + 10h]
        push edx
        mov dword ptr [esp + 20h], eax
        call CDisplayManager::SetIntegerViewportEndpoint
        add esp, 20h
        ret 4
    }
}
