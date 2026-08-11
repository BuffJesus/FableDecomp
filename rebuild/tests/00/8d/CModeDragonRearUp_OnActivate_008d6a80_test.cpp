#include <cstdio>

struct Sub
{
    char pad[0x10];
    char flag;
    int notified;
    void* seen;
    void Notify(int one, void* who) { notified = one; seen = who; }
};

struct CModeDragonRearUp
{
    int   vslot;
    Sub*  sub;
    int   baseCalled;
    void  BaseActivate() { baseCalled = 1; }
    void  OnActivate();
};

void CModeDragonRearUp::OnActivate()
{
    BaseActivate();
    Sub* s = this->sub;
    if (s->flag == 1)
        s->Notify(1, this);
}

int main()
{
    Sub s;
    s.flag = 1; s.notified = 0; s.seen = 0;
    CModeDragonRearUp m;
    m.vslot = 0; m.sub = &s; m.baseCalled = 0;
    m.OnActivate();

    bool ok = m.baseCalled == 1 && s.notified == 1 && s.seen == &m;

    Sub s2;
    s2.flag = 0; s2.notified = 0; s2.seen = 0;
    CModeDragonRearUp m2;
    m2.vslot = 0; m2.sub = &s2; m2.baseCalled = 0;
    m2.OnActivate();
    ok = ok && m2.baseCalled == 1 && s2.notified == 0;

    if (ok) printf("REARUP_ACTIVATE_OK\n");
    else printf("FAIL\n");
    return ok ? 0 : 1;
}