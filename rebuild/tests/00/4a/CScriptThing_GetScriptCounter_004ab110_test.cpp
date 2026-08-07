#include <cstdio>
struct P {
  virtual int d0(){return 0;}
  virtual int d1(){return 0;}
  virtual int d2(){return 0;}
  virtual int d3(){return 0;}
  virtual int d4(){return 0;}
  virtual int d5(){return 0;}
  virtual int d6(){return 0;}
  virtual int d7(){return 0;}
  virtual int d8(){return 0;}
  virtual int d9(){return 0;}
  virtual int d10(){return 0;}
  virtual int d11(){return 0;}
  virtual int d12(){return 0;}
  virtual int d13(){return 0;}
  virtual int d14(){return 0;}
  virtual int d15(){return 0;}
  virtual int d16(){return 0;}
  virtual int d17(){return 0;}
  virtual int d18(){return 0;}
  virtual int d19(){return 0;}
  virtual int d20(){return 0;}
  virtual int d21(){return 0;}
  virtual int d22(){return 0;}
  virtual int d23(){return 0;}
  virtual int d24(){return 0;}
  virtual int d25(){return 0;}
  virtual int d26(){return 0;}
  virtual int d27(){return 0;}
  virtual int d28(){return 0;}
  virtual int d29(){return 0;}
  virtual int d30(){return 0;}
  virtual int d31(){return 0;}
  virtual int d32(){return 0;}
  virtual int d33(){return 0;}
  virtual int d34(){return 0;}
  virtual int d35(){return 0;}
  virtual int d36(){return 0;}
  virtual int d37(){return 0;}
  virtual int d38(){return 0;}
  virtual int d39(){return 0;}
  virtual int d40(){return 0;}
  virtual int d41(){return 0;}
  virtual int d42(){return 0;}
  virtual int d43(){return 0;}
  virtual int d44(){return 0;}
  virtual int d45(){return 0;}
  virtual int d46(){return 0;}
  virtual int d47(){return 0;}
  virtual int d48(){return 0;}
  virtual int d49(){return 0;}
  virtual int d50(){return 0;}
  virtual int d51(){return 0;}
  virtual int d52(){return 0;}
  virtual int d53(){return 0;}
  virtual int d54(){return 0;}
  virtual int d55(){return 0;}
  virtual int d56(){return 0;}
  virtual int d57(){return 0;}
  virtual int d58(){return 0;}
  virtual int d59(){return 0;}
  virtual int d60(){return 0;}
  virtual int d61(){return 0;}
  virtual int d62(){return 0;}
  virtual int d63(){return 0;}
  virtual int d64(){return 0;}
  virtual int d65(){return 0;}
  virtual int d66(){return 0;}
  virtual int d67(){return 0;}
  virtual int d68(){return 0;}
  virtual int d69(){return 0;}
  virtual int d70(){return 0;}
  virtual int d71(){return 0;}
  virtual int d72(){return 0;}
  virtual int d73(){return 0;}
  virtual int tgt();
};
struct S { char pad[4]; P* f4; int M(); };
int P::tgt(){ return 42; }
int S::M(){ P* p=this->f4; if(!p) return 0; return p->tgt(); }
int main(){ P pp; S o; o.f4=&pp;
 if(o.M()!=42){ std::printf("004ab110_TEST FAIL\n"); return 1;}
 S o2; o2.f4=0; if(o2.M()!=0){ std::printf("004ab110_TEST FAIL\n"); return 1;}
 std::printf("004ab110_TEST PASS\n"); return 0;}