// CModeDragonRearUp::OnActivate  @ 008d69f0
struct CModeDragonRearUp;

struct SomeState {
    char pad[0x10];
    unsigned char flag;                        // +0x10
    void Do(int a, CModeDragonRearUp* self);   // thiscall -> 0x833a10
};

struct ModeBase {
    virtual ~ModeBase();     // vptr at +0 -> base subobject occupies 4 bytes
    void BaseOnActivate();   // -> 0x8621a0 (thiscall)
};

struct CModeDragonRearUp : public ModeBase {
    SomeState* state;        // +4
    void OnActivate();
};

void CModeDragonRearUp::OnActivate()
{
    this->BaseOnActivate();
    if (this->state->flag == 1)
        this->state->Do(1, this);
}