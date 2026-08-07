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
  virtual char tgt(int a0);
};
struct S { char pad[4]; P* f4; char M(int a0); };
char P::tgt(int a0){ return 1; }
char S::M(int a0){ P* p=this->f4; if(!p) return 0; return p->tgt(a0); }
int main(){ P pp; S o; o.f4=&pp;
 if(o.M(0)!=1){ std::printf("004aab70_TEST FAIL\n"); return 1;}
 S o2; o2.f4=0; if(o2.M(0)!=0){ std::printf("004aab70_TEST FAIL\n"); return 1;}
 std::printf("004aab70_TEST PASS\n"); return 0;}