extern int g_def_004aa8b0;
struct P {
  virtual void* d0(){return 0;}
  virtual void* tgt();
};
struct S { char pad[4]; P* f4; void* M(); };
void* S::M(){ P* p=this->f4; if(!p) return &g_def_004aa8b0; return p->tgt(); }