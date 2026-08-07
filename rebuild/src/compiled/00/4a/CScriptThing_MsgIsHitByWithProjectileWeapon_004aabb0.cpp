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
  virtual char tgt(int a0, int a1);
};
struct S { char pad[4]; P* f4; char M(int a0, int a1); };
char S::M(int a0, int a1){ P* p=this->f4; if(!p) return 0; return p->tgt(a0, a1); }