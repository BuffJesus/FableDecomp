#include <cstdio>

struct CSomeState {
    char pad[0x1c8];
    unsigned char flags; // +0x1c8
};

struct CSubObject {
    CSomeState* target;
    CSomeState* GetState() { return target; }
};

struct CCreatureAction_TrollAngryGroundPound {
    char pad0[8];
    CSubObject sub;
    bool baseCalled;

    void DoBase() { baseCalled = true; }
    void PerformAction()
    {
        this->DoBase();
        this->sub.GetState()->flags |= 0x40;
    }
};

int main()
{
    CSomeState st;
    for (int i = 0; i < (int)sizeof(st); ++i) ((unsigned char*)&st)[i] = 0;
    st.flags = 0x81; // has bits 0x80 and 0x01

    CCreatureAction_TrollAngryGroundPound act;
    act.baseCalled = false;
    act.sub.target = &st;

    act.PerformAction();

    bool ok = act.baseCalled && (st.flags == (0x81 | 0x40));
    if (ok) printf("GROUNDPOUND_OK flags=%02x\n", st.flags);
    else    printf("FAIL flags=%02x base=%d\n", st.flags, act.baseCalled);
    return ok ? 0 : 1;
}