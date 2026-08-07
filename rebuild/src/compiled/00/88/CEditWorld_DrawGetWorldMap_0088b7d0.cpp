struct S { char pad[0x38]; char* f38; };
char* __fastcall DrawGetWorldMap(S* self){ char* p = self->f38; return p ? p + 0xC : 0; }