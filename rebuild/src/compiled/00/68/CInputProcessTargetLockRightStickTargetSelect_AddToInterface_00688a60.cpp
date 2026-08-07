struct C { char pad[0x28]; char f28; char f29; void AddToInterface(); };
extern void __fastcall base_ati(C*);
void C::AddToInterface(){ this->f28 = 0; this->f29 = 0; base_ati(this); }