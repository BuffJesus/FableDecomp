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
  virtual char d9(){return 0;}
  virtual char d10(){return 0;}
  virtual char d11(){return 0;}
  virtual char d12(){return 0;}
  virtual char d13(){return 0;}
  virtual char d14(){return 0;}
  virtual char d15(){return 0;}
  virtual char d16(){return 0;}
  virtual char d17(){return 0;}
  virtual char d18(){return 0;}
  virtual char d19(){return 0;}
  virtual char d20(){return 0;}
  virtual char d21(){return 0;}
  virtual char d22(){return 0;}
  virtual char d23(){return 0;}
  virtual char d24(){return 0;}
  virtual char d25(){return 0;}
  virtual char d26(){return 0;}
  virtual char d27(){return 0;}
  virtual char d28(){return 0;}
  virtual char d29(){return 0;}
  virtual char d30(){return 0;}
  virtual char d31(){return 0;}
  virtual char d32(){return 0;}
  virtual char d33(){return 0;}
  virtual char d34(){return 0;}
  virtual char d35(){return 0;}
  virtual char d36(){return 0;}
  virtual char d37(){return 0;}
  virtual char d38(){return 0;}
  virtual char d39(){return 0;}
  virtual char d40(){return 0;}
  virtual char d41(){return 0;}
  virtual char d42(){return 0;}
  virtual char d43(){return 0;}
  virtual char d44(){return 0;}
  virtual char d45(){return 0;}
  virtual char d46(){return 0;}
  virtual char d47(){return 0;}
  virtual char d48(){return 0;}
  virtual char d49(){return 0;}
  virtual char d50(){return 0;}
  virtual char d51(){return 0;}
  virtual char d52(){return 0;}
  virtual char d53(){return 0;}
  virtual char tgt();
};
struct S { char pad[4]; P* f4; char M(); };
char P::tgt(){ return 1; }
char S::M(){ P* p=this->f4; if(!p) return 0; return p->tgt(); }
int main(){ P pp; S o; o.f4=&pp;
 if(o.M()!=1){ std::printf("004aaec0_TEST FAIL\n"); return 1;}
 S o2; o2.f4=0; if(o2.M()!=0){ std::printf("004aaec0_TEST FAIL\n"); return 1;}
 std::printf("004aaec0_TEST PASS\n"); return 0;}