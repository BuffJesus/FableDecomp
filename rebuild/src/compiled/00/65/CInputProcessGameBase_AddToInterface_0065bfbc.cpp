struct IF { char pad[0x98]; int f98; };
void __fastcall h_0065bfbc(IF* self);
void __fastcall fn_0065bfbc(IF* self, int _edx, int arg){ self->f98 = arg; h_0065bfbc(self); }