struct C { char pad[0x28]; int f28; int f2c; void Initialise(); };
extern void __fastcall base_init(C*);
void C::Initialise(){ this->f28 = 0; this->f2c = 0; base_init(this); }