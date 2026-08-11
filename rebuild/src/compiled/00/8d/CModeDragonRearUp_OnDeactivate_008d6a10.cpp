// CModeDragonRearUp::OnDeactivate  @ 008d6a10

struct CModeDragonRearUp;

struct SomeState {
    char pad[0x10];
    unsigned char flag;   // +0x10
    // __fastcall member (helper 0x833a10): ecx = this(state), args (int, self)
    void Helper(int a, CModeDragonRearUp* self);
};

struct CModeDragonRearUp {
    void* vfptr;          // +0
    SomeState* state;     // +4
    void OnDeactivate();
};

// base-class deactivate, tail-called (thiscall) -- 0x861d00
void __fastcall DragonRearUpBase_861d00(CModeDragonRearUp* self);

void CModeDragonRearUp::OnDeactivate()
{
    SomeState* s = this->state;
    if (s->flag == 1)
        s->Helper(0, this);
    DragonRearUpBase_861d00(this);
}