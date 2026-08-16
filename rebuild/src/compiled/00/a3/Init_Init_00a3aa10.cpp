extern void fn_a3a9c0(); extern void fn_a3aa00();
struct C { char pad[0x1c]; int f1c,f20,f24_unused,f28,f2c; char pad2[0x3c-0x30]; void* f3c; void* f40; char pad3[0x54-0x44]; unsigned char f54,f55,f56; void Init(); };
void C::Init(){ this->f1c=0; this->f28=0; this->f20=0; this->f2c=0; this->f3c=(void*)&fn_a3a9c0; this->f40=(void*)&fn_a3aa00; this->f54=0; this->f55=0; this->f56=0; }