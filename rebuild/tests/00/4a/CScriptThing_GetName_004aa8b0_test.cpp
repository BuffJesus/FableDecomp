#include <cstdio>
int g_def_004aa8b0=0;
struct P {
  virtual void* d0(){return 0;}
  virtual void* tgt();
};
struct S { char pad[4]; P* f4; void* M(); };
void* P::tgt(){ return (void*)0x1234; }
void* S::M(){ P* p=this->f4; if(!p) return &g_def_004aa8b0; return p->tgt(); }
int main(){ P pp; S o; o.f4=&pp;
 if(o.M()!=(void*)0x1234){ std::printf("004aa8b0_TEST FAIL\n"); return 1;}
 S o2; o2.f4=0; if(o2.M()!=&g_def_004aa8b0){ std::printf("004aa8b0_TEST FAIL\n"); return 1;}
 std::printf("004aa8b0_TEST PASS\n"); return 0;}