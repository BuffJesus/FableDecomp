// CClickable::OnRightUnclicked @ 0x0055ad30, __fastcall (this=ecx)
struct Sub {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4(int);   // slot 4 (offset 0x10) -> [edx+0x10]
};
struct CClickable {
    virtual void s00(); virtual void s01(); virtual void s02(); virtual void s03();
    virtual void s04(); virtual void s05(); virtual void s06(); virtual void s07();
    virtual void s08(); virtual void s09(); virtual void s10(); virtual void s11();
    virtual void s12(); virtual void s13(); virtual void s14(); virtual void s15();
    virtual void s16(); virtual void s17(); virtual void s18(); virtual void s19();
    virtual void s20(); virtual void s21(); virtual void s22(); virtual void s23();
    virtual void s24(); virtual void s25(); virtual void s26(); virtual void s27();
    virtual void s28(); virtual void s29(); virtual void s30(); virtual void s31();
    virtual void s32(); virtual void s33(); virtual void s34(); virtual void s35();
    virtual void s36(); virtual void s37(); virtual void s38(); virtual void s39();
    virtual void s40(); virtual void s41(); virtual void s42(); virtual void s43();
    virtual void s44(); virtual void s45(); virtual void s46(); virtual void s47();
    virtual void s48(int); // slot 48 (offset 0xc0) -> [eax+0xc0]
    Sub  sub;              // embedded subobject at +0x4 (its vtable ptr lives at +0x4)
    char pad[0x180 - 0x8];
    int  arg;              // +0x180
    void OnRightUnclicked();
};
void CClickable::OnRightUnclicked() {
    this->s48(this->arg);
    this->sub.v4(0x1d);
}