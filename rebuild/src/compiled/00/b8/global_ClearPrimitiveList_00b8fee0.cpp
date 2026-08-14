struct CEngineInternalPrimitiveBase
{
    char pad0[0x20];
    void* m20;      // +0x20
    char pad1[0x14];
    CEngineInternalPrimitiveBase* m38; // +0x38
    void* m3c;      // +0x3c
    void* m40;      // +0x40
    void* m44;      // +0x44
};

void __fastcall ClearPrimitiveList(CEngineInternalPrimitiveBase* self)
{
    CEngineInternalPrimitiveBase* p = self;
    while (p != 0)
    {
        CEngineInternalPrimitiveBase* next = p->m38;
        p->m20 = 0;
        p->m38 = 0;
        p->m3c = 0;
        p->m40 = 0;
        p->m44 = 0;
        p = next;
    }
}