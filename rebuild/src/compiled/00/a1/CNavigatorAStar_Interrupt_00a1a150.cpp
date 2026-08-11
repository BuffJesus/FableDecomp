// CNavigatorAStar::Interrupt  @ 00a1a150
// __fastcall void Interrupt(CNavigatorAStar* this)

struct SubObject {
    // The engine calls a __fastcall method on (SubObject*)((char*)base + 0x2c)
    // passing the CNavigatorAStar* as a stack argument.
    void Notify(void* nav);
};

struct CNavigatorAStar {
    char pad0[0x19];
    unsigned char flag19;      // +0x19
    char pad1[0x1c - 0x1a];
    char* sub;                 // +0x1c  (pointer to a block; method lives at +0x2c)
    char pad2[0x88 - 0x20];
    int field88;               // +0x88

    void Interrupt();
};

void CNavigatorAStar::Interrupt()
{
    this->flag19 = 1;
    this->field88 = 0;
    // sub method at offset +0x2c within the block pointed to by +0x1c,
    // receiving 'this' as its argument.
    SubObject* s = (SubObject*)(this->sub + 0x2c);
    s->Notify(this);
}