char __fastcall h_920710(void* nested);
struct RN { void* f0; char* f4; };
char __fastcall fn_00920710(RN* self){ char* p4 = self->f4; void* nested = *(void**)(p4 + 0x18); return h_920710(nested) == 0; }