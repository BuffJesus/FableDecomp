struct P { virtual bool v0(); };
struct S { char pad[0x2c]; P* f2c; };
bool __fastcall TestPredicate(S* self){ P* p = self->f2c; if(!p) return true; return p->v0(); }