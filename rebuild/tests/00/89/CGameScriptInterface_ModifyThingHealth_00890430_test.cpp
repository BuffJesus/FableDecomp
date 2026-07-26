#include <cstdio>
struct Thing {
    virtual void s0();
    virtual void s1();
    virtual void s2();
    virtual void s3();
    virtual void s4();
    virtual void s5();
    virtual void s6();
    virtual void s7();
    virtual void s8();
    virtual void s9();
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
    virtual void s50();
    virtual void s51();
    virtual void s52();
    virtual void s53();
    virtual void s54();
    virtual void s55();
    virtual void s56();
    virtual void s57();
    virtual void s58();
    virtual void modHealth(int a, int b);
    unsigned char pad_after_vptr[0x68]; // to +0x6c
    unsigned char flag6c;               // +0x6c
    unsigned char pad1[0x24];           // to +0x91
    unsigned char flag91;               // +0x91
};
struct Gsi {
    virtual void g0();
    virtual void g1();
    virtual void g2();
    virtual void g3();
    virtual void g4();
    virtual void g5();
    virtual void g6();
    virtual void g7();
    virtual void g8();
    virtual void g9();
    virtual void g10();
    virtual Thing* getThing();
};
static int g_called, g_a1, g_a2;
static Thing g_thing;
void Thing::s0(){}
void Thing::s1(){}
void Thing::s2(){}
void Thing::s3(){}
void Thing::s4(){}
void Thing::s5(){}
void Thing::s6(){}
void Thing::s7(){}
void Thing::s8(){}
void Thing::s9(){}
void Thing::s10(){}
void Thing::s11(){}
void Thing::s12(){}
void Thing::s13(){}
void Thing::s14(){}
void Thing::s15(){}
void Thing::s16(){}
void Thing::s17(){}
void Thing::s18(){}
void Thing::s19(){}
void Thing::s20(){}
void Thing::s21(){}
void Thing::s22(){}
void Thing::s23(){}
void Thing::s24(){}
void Thing::s25(){}
void Thing::s26(){}
void Thing::s27(){}
void Thing::s28(){}
void Thing::s29(){}
void Thing::s30(){}
void Thing::s31(){}
void Thing::s32(){}
void Thing::s33(){}
void Thing::s34(){}
void Thing::s35(){}
void Thing::s36(){}
void Thing::s37(){}
void Thing::s38(){}
void Thing::s39(){}
void Thing::s40(){}
void Thing::s41(){}
void Thing::s42(){}
void Thing::s43(){}
void Thing::s44(){}
void Thing::s45(){}
void Thing::s46(){}
void Thing::s47(){}
void Thing::s48(){}
void Thing::s49(){}
void Thing::s50(){}
void Thing::s51(){}
void Thing::s52(){}
void Thing::s53(){}
void Thing::s54(){}
void Thing::s55(){}
void Thing::s56(){}
void Thing::s57(){}
void Thing::s58(){}
void Thing::modHealth(int a,int b){ g_called=1; g_a1=a; g_a2=b; }
void Gsi::g0(){}
void Gsi::g1(){}
void Gsi::g2(){}
void Gsi::g3(){}
void Gsi::g4(){}
void Gsi::g5(){}
void Gsi::g6(){}
void Gsi::g7(){}
void Gsi::g8(){}
void Gsi::g9(){}
void Gsi::g10(){}
Thing* Gsi::getThing(){ return &g_thing; }
void __stdcall CGameScriptInterface_ModifyThingHealth(Gsi* self, int arg1, int arg2)
{
    Thing* t = self->getThing();
    if (t) {
        if (!(t->flag91 & 1)) {
            if (t->flag6c & 2) {
                t->modHealth(arg1, arg2);
            }
        }
    }
}
int main(){
    Gsi gsi;
    // call path: flag91 clear, flag6c bit2 set -> should invoke
    g_thing.flag91=0; g_thing.flag6c=2; g_called=0;
    CGameScriptInterface_ModifyThingHealth(&gsi,111,222);
    if(!g_called||g_a1!=111||g_a2!=222){ std::printf("FAIL call\n"); return 1; }
    // flag91 bit0 set -> skip
    g_thing.flag91=1; g_thing.flag6c=2; g_called=0;
    CGameScriptInterface_ModifyThingHealth(&gsi,1,2);
    if(g_called){ std::printf("FAIL flag91\n"); return 1; }
    // flag6c bit2 clear -> skip
    g_thing.flag91=0; g_thing.flag6c=0; g_called=0;
    CGameScriptInterface_ModifyThingHealth(&gsi,1,2);
    if(g_called){ std::printf("FAIL flag6c\n"); return 1; }
    std::printf("CGameScriptInterface_00890430_TEST PASS\n");
    return 0;
}