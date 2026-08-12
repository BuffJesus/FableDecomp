#include <cstdio>

struct CFaction;

struct CFactionHolder {
    int calls;
    bool ret;
    bool IsAlliedWithFaction(CFaction* faction);
};

struct CTCEnemy {
    char pad[0x18];
    CFactionHolder* m_holder; // +0x18
    bool IsAlliedWithFaction(CFaction* faction);
};

// local definition of the forwarding target so the test links standalone
bool CFactionHolder::IsAlliedWithFaction(CFaction* faction)
{
    this->calls++;
    return this->ret;
}

bool CTCEnemy::IsAlliedWithFaction(CFaction* faction)
{
    if (faction != 0) {
        CFactionHolder* h = this->m_holder;
        if (h != 0)
            return h->IsAlliedWithFaction(faction);
    }
    return false;
}

int main()
{
    CFactionHolder h; h.calls = 0; h.ret = true;
    CTCEnemy e; e.m_holder = &h;
    CFaction* fac = (CFaction*)0x1234;

    bool ok = true;
    // null faction -> false, no forward
    if (e.IsAlliedWithFaction(0) != false) ok = false;
    if (h.calls != 0) ok = false;
    // null holder -> false, no forward
    e.m_holder = 0;
    if (e.IsAlliedWithFaction(fac) != false) ok = false;
    if (h.calls != 0) ok = false;
    // both present, ret true -> forwards, returns true
    e.m_holder = &h;
    if (e.IsAlliedWithFaction(fac) != true) ok = false;
    if (h.calls != 1) ok = false;
    // ret false -> forwards, returns false
    h.ret = false;
    if (e.IsAlliedWithFaction(fac) != false) ok = false;
    if (h.calls != 2) ok = false;

    printf(ok ? "ALLIED_FWD_OK\n" : "ALLIED_FWD_FAIL\n");
    return ok ? 0 : 1;
}