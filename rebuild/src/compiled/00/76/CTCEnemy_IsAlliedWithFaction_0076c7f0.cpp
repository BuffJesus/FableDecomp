// CTCEnemy::IsAlliedWithFaction @ 0076c7f0
struct CFaction;

struct CFactionHolder {
    // the object at CTCEnemy+0x18; has a method IsAlliedWithFaction(CFaction*)
    bool IsAlliedWithFaction(CFaction* faction);
};

struct CTCEnemy {
    char pad[0x18];
    CFactionHolder* m_holder; // +0x18
    bool IsAlliedWithFaction(CFaction* faction);
};

bool CTCEnemy::IsAlliedWithFaction(CFaction* faction)
{
    if (faction != 0) {
        CFactionHolder* h = this->m_holder;
        if (h != 0)
            return h->IsAlliedWithFaction(faction);
    }
    return false;
}