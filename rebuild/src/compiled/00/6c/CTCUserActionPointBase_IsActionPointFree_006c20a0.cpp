struct S { char pad[0x14]; void** f14; };
bool __fastcall IsActionPointFree(S* self){ void** p = self->f14; return *p != (void*)p; }