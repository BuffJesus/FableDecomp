struct UF2 { char pad[0x2c]; int f2c; };
struct UF { char pad[8]; UF2* f8; char pad2[0x18-0xc]; int f18; };
int __fastcall fn_009fbf70(UF* self){ UF2* p=self->f8; if(p) return p->f2c - self->f18; return -1; }