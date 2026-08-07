#include <cstdio>
static void* g_arg=0;
struct P { char pad[0x18]; char* f18; };
void* __fastcall gb_sub_00946670(void* p){ g_arg=p; return p; }
struct S { char pad[4]; P* f4; void* M(); };
void* S::M(){ P* p=this->f4; char* q=p->f18; return gb_sub_00946670(q+368); }
int main(){ char buf[0x200]; P p; p.f18=buf; S o; o.f4=&p; o.M();
 if(g_arg!=buf+368){ std::printf("00946670_TEST FAIL\n"); return 1;}
 std::printf("00946670_TEST PASS\n"); return 0;}