class CCharString;
class CGameScriptInterface
{
public:
    __declspec(naked) void DeactivateQuestLater(const CCharString& questName, unsigned long delay) const;
};

__declspec(naked) void CGameScriptInterface::DeactivateQuestLater(const CCharString& questName, unsigned long delay) const
{
    __asm
    {
        mov ecx, dword ptr ds:[0x013B89FC]
        jmp 0x004B0A30
    }
}