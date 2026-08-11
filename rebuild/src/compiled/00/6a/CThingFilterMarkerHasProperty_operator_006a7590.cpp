// CThingMarker: needs a virtual at vtable slot 48 (offset 0xC0).
struct CThingMarker {
    virtual int vf00();
    virtual int vf01();
    virtual int vf02();
    virtual int vf03();
    virtual int vf04();
    virtual int vf05();
    virtual int vf06();
    virtual int vf07();
    virtual int vf08();
    virtual int vf09();
    virtual int vf10();
    virtual int vf11();
    virtual int vf12();
    virtual int vf13();
    virtual int vf14();
    virtual int vf15();
    virtual int vf16();
    virtual int vf17();
    virtual int vf18();
    virtual int vf19();
    virtual int vf20();
    virtual int vf21();
    virtual int vf22();
    virtual int vf23();
    virtual int vf24();
    virtual int vf25();
    virtual int vf26();
    virtual int vf27();
    virtual int vf28();
    virtual int vf29();
    virtual int vf30();
    virtual int vf31();
    virtual int vf32();
    virtual int vf33();
    virtual int vf34();
    virtual int vf35();
    virtual int vf36();
    virtual int vf37();
    virtual int vf38();
    virtual int vf39();
    virtual int vf40();
    virtual int vf41();
    virtual int vf42();
    virtual int vf43();
    virtual int vf44();
    virtual int vf45();
    virtual int vf46();
    virtual int vf47();
    virtual int GetPropertyFlags();   // slot 48 -> [eax+0xC0]
};

struct CThingFilter_MarkerHasProperty {
    int mask;   // +0x00
    bool operator()(CThingMarker* marker);
};

bool CThingFilter_MarkerHasProperty::operator()(CThingMarker* marker)
{
    int m = this->mask;
    return (m & marker->GetPropertyFlags()) != 0;
}