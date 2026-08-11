// CCreatureAction_TrollAngryGroundPound::PerformAction  0x008c63a0

struct CSomeState {
    char pad[0x1c8];
    unsigned char flags; // +0x1c8
};

struct CSubObject {
    CSomeState* GetState();   // external -> real call (returns ptr in eax)
};

struct CCreatureAction_TrollAngryGroundPound {
    char pad0[8];
    CSubObject sub;           // subobject at +8

    void DoBase();            // external -> real call on 'this'
    void PerformAction();
};

void CCreatureAction_TrollAngryGroundPound::PerformAction()
{
    this->DoBase();
    CSomeState* p = this->sub.GetState();
    p->flags |= 0x40;
}