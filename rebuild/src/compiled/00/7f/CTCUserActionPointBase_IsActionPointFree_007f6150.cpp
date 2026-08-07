struct S { char pad[0x48]; void** f48; };
bool __fastcall IsActionPointFree(S* self){ void** p = self->f48; return *p != (void*)p; }