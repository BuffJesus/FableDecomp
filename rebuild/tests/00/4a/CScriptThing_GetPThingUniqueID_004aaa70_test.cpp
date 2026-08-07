#include <cstdio>
struct P { long long (__fastcall **v)(P*); };
struct S { char pad[4]; P* f4; long long GetPThingUniqueID(); };
long long S::GetPThingUniqueID(){ P* p=this->f4; if(!p) return 0; return p->v[0x30/4](p); }
static long long __fastcall vfn(P* p){ return 0x1122334455LL; }
int main(){ long long(__fastcall* tbl[0x20])(P*)={0}; tbl[0x30/4]=vfn;
 P pp; pp.v=tbl; S o; o.f4=&pp;
 if(o.GetPThingUniqueID()!=0x1122334455LL){ std::printf("004aaa70_TEST FAIL\n"); return 1;}
 S o2; o2.f4=0; if(o2.GetPThingUniqueID()!=0){ std::printf("004aaa70_TEST FAIL\n"); return 1;}
 std::printf("004aaa70_TEST PASS\n"); return 0;}