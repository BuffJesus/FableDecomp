class CCharString;
class CGameScriptInterface;

void __stdcall CGameScriptInterface_AddRumourToCategoryEx(
    CCharString rumourCategory,
    CCharString rumourText);

void __thiscall CGameScriptInterface::AddNewRumourToCategory(
    const CCharString& rumourCategory,
    const CCharString& rumourText) const
{
    __asm
    {
        mov eax, rumourText
        push ecx
        mov ecx, esp
        push eax
        call CCharString::CCharString

        mov edx, rumourCategory
        push ecx
        mov ecx, esp
        push edx
        call CCharString::CCharString

        call CGameScriptInterface_AddRumourToCategoryEx
    }
}