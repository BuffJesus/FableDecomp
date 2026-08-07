struct F { int f0; int op(unsigned char* a2){ int x=(unsigned char)a2[0x10]; return x==this->f0; } };
int __fastcall fn_006a7100(F* self, int _edx, unsigned char* a2){ return self->op(a2); }