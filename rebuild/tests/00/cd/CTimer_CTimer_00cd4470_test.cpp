#include "rebuild_abi.h"
#include <cstdio>
static int g_hit=0; static void *g_arg=0;
struct GMgrV {
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
    virtual void s59();
    virtual void s60();
    virtual void s61();
    virtual void s62();
    virtual void s63();
    virtual void s64();
    virtual void s65();
    virtual void s66();
    virtual void s67();
    virtual void s68();
    virtual void s69();
    virtual void s70();
    virtual void s71();
    virtual void s72();
    virtual void s73();
    virtual void s74();
    virtual void s75();
    virtual void s76();
    virtual void s77();
    virtual void s78();
    virtual void s79();
    virtual void s80();
    virtual void s81();
    virtual void s82();
    virtual void s83();
    virtual void s84();
    virtual void s85();
    virtual void s86();
    virtual void s87();
    virtual void Release(void *p);
};
void GMgrV::s00(){}
void GMgrV::s01(){}
void GMgrV::s02(){}
void GMgrV::s03(){}
void GMgrV::s04(){}
void GMgrV::s05(){}
void GMgrV::s06(){}
void GMgrV::s07(){}
void GMgrV::s08(){}
void GMgrV::s09(){}
void GMgrV::s10(){}
void GMgrV::s11(){}
void GMgrV::s12(){}
void GMgrV::s13(){}
void GMgrV::s14(){}
void GMgrV::s15(){}
void GMgrV::s16(){}
void GMgrV::s17(){}
void GMgrV::s18(){}
void GMgrV::s19(){}
void GMgrV::s20(){}
void GMgrV::s21(){}
void GMgrV::s22(){}
void GMgrV::s23(){}
void GMgrV::s24(){}
void GMgrV::s25(){}
void GMgrV::s26(){}
void GMgrV::s27(){}
void GMgrV::s28(){}
void GMgrV::s29(){}
void GMgrV::s30(){}
void GMgrV::s31(){}
void GMgrV::s32(){}
void GMgrV::s33(){}
void GMgrV::s34(){}
void GMgrV::s35(){}
void GMgrV::s36(){}
void GMgrV::s37(){}
void GMgrV::s38(){}
void GMgrV::s39(){}
void GMgrV::s40(){}
void GMgrV::s41(){}
void GMgrV::s42(){}
void GMgrV::s43(){}
void GMgrV::s44(){}
void GMgrV::s45(){}
void GMgrV::s46(){}
void GMgrV::s47(){}
void GMgrV::s48(){}
void GMgrV::s49(){}
void GMgrV::s50(){}
void GMgrV::s51(){}
void GMgrV::s52(){}
void GMgrV::s53(){}
void GMgrV::s54(){}
void GMgrV::s55(){}
void GMgrV::s56(){}
void GMgrV::s57(){}
void GMgrV::s58(){}
void GMgrV::s59(){}
void GMgrV::s60(){}
void GMgrV::s61(){}
void GMgrV::s62(){}
void GMgrV::s63(){}
void GMgrV::s64(){}
void GMgrV::s65(){}
void GMgrV::s66(){}
void GMgrV::s67(){}
void GMgrV::s68(){}
void GMgrV::s69(){}
void GMgrV::s70(){}
void GMgrV::s71(){}
void GMgrV::s72(){}
void GMgrV::s73(){}
void GMgrV::s74(){}
void GMgrV::s75(){}
void GMgrV::s76(){}
void GMgrV::s77(){}
void GMgrV::s78(){}
void GMgrV::s79(){}
void GMgrV::s80(){}
void GMgrV::s81(){}
void GMgrV::s82(){}
void GMgrV::s83(){}
void GMgrV::s84(){}
void GMgrV::s85(){}
void GMgrV::s86(){}
void GMgrV::s87(){}
void GMgrV::Release(void *p){ g_hit=1; g_arg=p; }
static GMgrV g_inst;
GMgrV *g_timerMgr_cd4470=&g_inst;
struct CTimer { void *m_first; void Dtor(); };
void CTimer::Dtor(){ g_timerMgr_cd4470->Release(m_first); }
int main(){ CTimer t; int x; t.m_first=&x; t.Dtor();
 if(!g_hit||g_arg!=&x){std::printf("B1\n");return 1;}
 std::printf("TM_cd4470_OK\n"); return 0; }