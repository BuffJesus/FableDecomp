#include <cstdio>
struct CCharString {};
static int g_set=0;
struct Inner { virtual void v0(){} virtual void v1(){} virtual void v2(){} virtual void v3(){} virtual void VSet(const CCharString&){ g_set=1; } };
struct S { char pad[4]; Inner* f4; void SetDataString(const CCharString&); };
void S::SetDataString(const CCharString& s){ Inner* p = this->f4; if(p) p->VSet(s); }
int main(){ Inner inr; S o; o.f4=&inr; CCharString cs; o.SetDataString(cs);
 if(!g_set){ std::printf("004aa930_TEST FAIL\n"); return 1;}
 o.f4=0; o.SetDataString(cs);
 std::printf("004aa930_TEST PASS\n"); return 0;}