#include <cstdio>
static int g_v=0;
struct C { virtual void v0(){} virtual void v1(){} virtual void v2(){} virtual void v3(){}
          virtual void v4(){} virtual void v5(){} virtual void SetX(bool x){ g_v = x?1:2; } };
struct B { char pad[0x2c]; C* f2c; };
struct A { char pad[0x28]; B* f28; };
struct CGameScriptInterface { char pad[8]; A* f8; void SetEnvironmentalEffectsAlwaysUpdate(bool b); };
void CGameScriptInterface::SetEnvironmentalEffectsAlwaysUpdate(bool b){ f8->f28->f2c->SetX(b); }
int main(){ C c; B b; b.f2c=&c; A a; a.f28=&b; CGameScriptInterface g; g.f8=&a;
 g.SetEnvironmentalEffectsAlwaysUpdate(true);
 if(g_v!=1){ std::printf("00890b90_TEST FAIL\n"); return 1;}
 std::printf("00890b90_TEST PASS\n"); return 0;}