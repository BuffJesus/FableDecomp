// CTCEnemy::IsAlliedWithFaction @ 0x0076c7d0
// bool __fastcall(CTCEnemy* this, CFaction* faction)
//   if (!faction) return false;
//   Faction* f = this->m18;         // [ecx+0x18]
//   if (!f) return false;
//   return f->IsAlliedWithFaction(faction);   // tail-call to 0x6b94c0

struct CFaction;

// The object stored at CTCEnemy+0x18; its method lives at 0x6b94c0.
struct FactionHolder {
    bool IsAlliedWithFaction(CFaction* faction);
};

struct CTCEnemy {
    char pad0[0x18];
    FactionHolder* m18;   // +0x18
    bool IsAlliedWithFaction(CFaction* faction);
};

bool CTCEnemy::IsAlliedWithFaction(CFaction* faction)
{
    if (faction != 0)
    {
        FactionHolder* f = this->m18;
        if (f != 0)
            return f->IsAlliedWithFaction(faction);
    }
    return false;
}