extern int g_def_004aa980;
struct P {
  virtual void* d0(){return 0;}
  virtual void* d1(){return 0;}
  virtual void* d2(){return 0;}
  virtual void* d3(){return 0;}
  virtual void* d4(){return 0;}
  virtual void* d5(){return 0;}
  virtual void* tgt();
};
struct S { char pad[4]; P* f4; void* M(); };
void* S::M(){ P* p=this->f4; if(!p) return &g_def_004aa980; return p->tgt(); }