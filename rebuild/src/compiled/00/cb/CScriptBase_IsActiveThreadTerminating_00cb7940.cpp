struct Inner { char pad[5]; char b5; };
struct S { char pad[0x2c]; Inner* f2c; };
char __fastcall IsActiveThreadTerminating(S* self){ if(!self->f2c) return 0; return self->f2c->b5; }