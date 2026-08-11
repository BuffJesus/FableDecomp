typedef int EAIStateGroupType;

struct CAIStateGroup {
    // slot 8 at vtbl+0x20 -> virtual method index 8
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual void slot5();
    virtual void slot6();
    virtual void slot7();
    virtual EAIStateGroupType GetType();   // slot 8 -> [vtbl+0x20]
    char pad[0x10 - 4];
    unsigned char f10;                      // +0x10
};

struct CAIBrain {
    char pad[0x38];
    CAIStateGroup* f38;                     // +0x38
};

EAIStateGroupType __fastcall GetCurrentStateGroupType(CAIBrain* self)
{
    if (self->f38 != 0 && self->f38->f10 == 0) {
        return self->f38->GetType();
    }
    return 0;
}