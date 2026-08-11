// CModeDragonRearUp::OnActivate  @ 008d6a80
// push esi; mov esi,ecx; call 0x8621a0(this)
// ecx=[esi+4]; if ([ecx+0x10]==1) { (obj)->m(1, this); }

struct Sub
{
    char pad[0x10];
    char flag;      // +0x10
    void Notify(int one, void* who);
};

struct CModeDragonRearUp
{
    int   vslot;    // +0x00 (padding to reach +4)
    Sub*  sub;      // +0x04
    void  BaseActivate();
    void  OnActivate();
};

void CModeDragonRearUp::OnActivate()
{
    BaseActivate();
    Sub* s = this->sub;
    if (s->flag == 1)
        s->Notify(1, this);
}