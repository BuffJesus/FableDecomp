// CInputProcessGameBase::IsOtherQuickAccessItemButtonHeld  @ 0x00687c90
// bool __fastcall (CInputProcessGameBase* this, long param)

struct Helper18
{
    int GetValue();          // thiscall, no extra args -> 0x487510
};

struct Helper24
{
    bool Check(long b, int a); // thiscall (b,a) -> 0x446060
};

struct CInputProcessGameBase
{
    char pad0[0x18];
    Helper18* m18;   // +0x18
    char pad1[0x24 - 0x1C];
    Helper24* m24;   // +0x24

    bool IsOtherQuickAccessItemButtonHeld(long param);
};

bool CInputProcessGameBase::IsOtherQuickAccessItemButtonHeld(long param)
{
    return m24->Check(param, m18->GetValue());
}