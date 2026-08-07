extern void __fastcall dt_helper(void*);
struct S { char pad[0x24]; void* f24; void* f28; };
void __fastcall DetachTarget(S* self){ if(self->f28) dt_helper(reinterpret_cast<char*>(self) + 0x24); }