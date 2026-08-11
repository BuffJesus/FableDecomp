// CTCActionUseScriptedHook::SetEntranceAsLocked  @ 0x00729c30
// __fastcall void(CTCActionUseScriptedHook*, bool)

struct Entrance
{
    // method at 0x00a01b50 : __fastcall, returns pointer/bool in eax
    void* Resolve();
    char data[8];   // occupies +0x20..+0x27
};

struct CTCActionUseScriptedHook
{
    char pad0[0x20];
    Entrance m_entrance;   // +0x20
    char m_locked;         // +0x28

    void SetEntranceAsLocked(bool locked);
};

void CTCActionUseScriptedHook::SetEntranceAsLocked(bool locked)
{
    if (this->m_entrance.Resolve())
    {
        this->m_locked = locked;
    }
}