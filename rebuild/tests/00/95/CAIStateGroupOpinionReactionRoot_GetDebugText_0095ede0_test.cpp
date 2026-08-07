#include <cstdio>
struct CCharString; struct Elem40 { char _[40]; };
static Elem40 g_dbgtable[4];
struct S { char pad[0x18]; unsigned long f18; };
static const CCharString* f(S* self){ return (const CCharString*)&g_dbgtable[self->f18]; }
int main(){ S o; o.f18=2; if((char*)f(&o)!=(char*)&g_dbgtable[2]){ std::printf("0095ede0_TEST FAIL\n"); return 1;}
 std::printf("0095ede0_TEST PASS\n"); return 0;}