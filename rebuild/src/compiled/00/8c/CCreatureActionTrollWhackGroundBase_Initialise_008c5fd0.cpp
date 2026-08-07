#include "rebuild_abi.h"
// CCreatureAction_TrollWhackGroundBase::Initialise @ 0x008c5fd0
// push esi; mov esi,ecx; mov eax,[esi]; call [eax+0x144];
// mov ecx,esi; pop esi; jmp base   (vcall then tail-jmp base(this))
struct Fable_TrollInit_008c5fd0
{
    virtual void s00();
    virtual void s01();
    virtual void s02();
    virtual void s03();
    virtual void s04();
    virtual void s05();
    virtual void s06();
    virtual void s07();
    virtual void s08();
    virtual void s09();
    virtual void s0a();
    virtual void s0b();
    virtual void s0c();
    virtual void s0d();
    virtual void s0e();
    virtual void s0f();
    virtual void s10();
    virtual void s11();
    virtual void s12();
    virtual void s13();
    virtual void s14();
    virtual void s15();
    virtual void s16();
    virtual void s17();
    virtual void s18();
    virtual void s19();
    virtual void s1a();
    virtual void s1b();
    virtual void s1c();
    virtual void s1d();
    virtual void s1e();
    virtual void s1f();
    virtual void s20();
    virtual void s21();
    virtual void s22();
    virtual void s23();
    virtual void s24();
    virtual void s25();
    virtual void s26();
    virtual void s27();
    virtual void s28();
    virtual void s29();
    virtual void s2a();
    virtual void s2b();
    virtual void s2c();
    virtual void s2d();
    virtual void s2e();
    virtual void s2f();
    virtual void s30();
    virtual void s31();
    virtual void s32();
    virtual void s33();
    virtual void s34();
    virtual void s35();
    virtual void s36();
    virtual void s37();
    virtual void s38();
    virtual void s39();
    virtual void s3a();
    virtual void s3b();
    virtual void s3c();
    virtual void s3d();
    virtual void s3e();
    virtual void s3f();
    virtual void s40();
    virtual void s41();
    virtual void s42();
    virtual void s43();
    virtual void s44();
    virtual void s45();
    virtual void s46();
    virtual void s47();
    virtual void s48();
    virtual void s49();
    virtual void s4a();
    virtual void s4b();
    virtual void s4c();
    virtual void s4d();
    virtual void s4e();
    virtual void s4f();
    virtual void s50();
    virtual void act();            // vtable +0x144
    void FABLE_FASTCALL base();
    void Run();
};
void Fable_TrollInit_008c5fd0::Run()
{
    this->act();
    this->base();
}