struct Inner { virtual void d0(); virtual void d1(); virtual void d2(); virtual void d3();
               virtual void d4(); virtual void d5(); virtual void d6(); virtual void d7(); virtual void d8();
               virtual int V9(); };
struct S { char pad[0x1c]; Inner* f1c; };
int __fastcall GetLength(S* self){ Inner* p = self->f1c; if(!p) return 0; return p->V9(); }