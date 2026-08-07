struct Inner { virtual void d0();virtual void d1();virtual void d2();virtual void d3();virtual void d4();virtual void d5();virtual void d6();virtual void d7();virtual void d8();virtual void d9();virtual void d10(); virtual int tgt(); };
struct S { char pad[4]; Inner* f4; };
int __fastcall GetPThing(S* self){ Inner* p = self->f4; if(!p) return 0; return p->tgt(); }