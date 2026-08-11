// CThingFilter_CanBeUsedAsCover::operator()
// Retail 0x006a7ab0: checks a flag on the CThing arg, then virtual-dispatches.

struct CThing
{
    // vtable ptr at +0; virtual method at slot 64 (offset 0x100) returns bool.
    virtual bool vf00();
    virtual bool vf01();
    virtual bool vf02();
    virtual bool vf03();
    virtual bool vf04();
    virtual bool vf05();
    virtual bool vf06();
    virtual bool vf07();
    virtual bool vf08();
    virtual bool vf09();
    virtual bool vf10();
    virtual bool vf11();
    virtual bool vf12();
    virtual bool vf13();
    virtual bool vf14();
    virtual bool vf15();
    virtual bool vf16();
    virtual bool vf17();
    virtual bool vf18();
    virtual bool vf19();
    virtual bool vf20();
    virtual bool vf21();
    virtual bool vf22();
    virtual bool vf23();
    virtual bool vf24();
    virtual bool vf25();
    virtual bool vf26();
    virtual bool vf27();
    virtual bool vf28();
    virtual bool vf29();
    virtual bool vf30();
    virtual bool vf31();
    virtual bool vf32();
    virtual bool vf33();
    virtual bool vf34();
    virtual bool vf35();
    virtual bool vf36();
    virtual bool vf37();
    virtual bool vf38();
    virtual bool vf39();
    virtual bool vf40();
    virtual bool vf41();
    virtual bool vf42();
    virtual bool vf43();
    virtual bool vf44();
    virtual bool vf45();
    virtual bool vf46();
    virtual bool vf47();
    virtual bool vf48();
    virtual bool vf49();
    virtual bool vf50();
    virtual bool vf51();
    virtual bool vf52();
    virtual bool vf53();
    virtual bool vf54();
    virtual bool vf55();
    virtual bool vf56();
    virtual bool vf57();
    virtual bool vf58();
    virtual bool vf59();
    virtual bool vf60();
    virtual bool vf61();
    virtual bool vf62();
    virtual bool vf63();
    virtual bool CanBeUsedAsCoverImpl(); // slot 64, offset 0x100
    char pad[0x6c - 4];
    unsigned char flags; // +0x6c
};

struct CThingFilter_CanBeUsedAsCover
{
    bool operator()(CThing *t);
};

bool CThingFilter_CanBeUsedAsCover::operator()(CThing *t)
{
    if (t->flags & 4)
        return t->CanBeUsedAsCoverImpl();
    return false;
}