#include <cstdio>
struct P {
  virtual char d0(){return 0;}
  virtual char d1(){return 0;}
  virtual char d2(){return 0;}
  virtual char d3(){return 0;}
  virtual char d4(){return 0;}
  virtual char d5(){return 0;}
  virtual char d6(){return 0;}
  virtual char d7(){return 0;}
  virtual char d8(){return 0;}
  virtual char tgt(int a0);
};
struct S { char pad[8]; P* fp; char M(int a0); };
char P::tgt(int a0){ return 1; }
char S::M(int a0){ P* p=this->fp; if(!p) return 0; return p->tgt(a0); }
int main(){ P pp; S o; o.fp=&pp;
 if(o.M(0)!=1){ std::printf("007e7340_TEST FAIL\n"); return 1;}
 S o2; o2.fp=0; if(o2.M(0)!=0){ std::printf("007e7340_TEST FAIL\n"); return 1;}
 std::printf("007e7340_TEST PASS\n"); return 0;}