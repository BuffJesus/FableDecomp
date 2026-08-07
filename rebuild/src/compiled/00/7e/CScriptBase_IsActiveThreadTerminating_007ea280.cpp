struct S { char pad[0xC]; void* f0c; char b10; };
char __fastcall IsActiveThreadTerminating(S* self){ if(!self->f0c) return 0; return self->b10; }