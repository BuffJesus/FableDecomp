struct OV { void** vtbl; };
typedef char (__fastcall *pfo)(OV*);
int __fastcall fn_00c35ea0(OV* self){ char b=((pfo)self->vtbl[1])(self); return b ? 0 : (int)self; }