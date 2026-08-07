struct P { char pad[0x2c]; int f2c; char pad3[0x34-0x30]; int f34; };
struct S { char pad[8]; P* f8; char pad2[0x18-0xc]; int f18; void M(); };
void S::M(){ P* p=this->f8; if(p) this->f18 = p->f2c - p->f34; }