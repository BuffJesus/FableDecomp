#include <cstdio>
struct Q { char pad[0x50]; int f50; };
struct P { char pad[0x68]; Q* f68; };
struct S { char pad[0x40]; P* f40; int M(); };
int S::M(){ P* p=this->f40; Q* q=p->f68; int v=q->f50; return v != 0; }
int main(){ Q q; P p; p.f68=&q; S o; o.f40=&p; q.f50=5;
 if(o.M()!=1){ std::printf("00c9cfd0_TEST FAIL\n"); return 1;}
 q.f50=0; if(o.M()!=0){ std::printf("00c9cfd0_TEST FAIL\n"); return 1;}
 std::printf("00c9cfd0_TEST PASS\n"); return 0;}