struct C { char pad[0x40]; int f40; int DoSizeof(); };
int C::DoSizeof(){ return this->f40 + 0x78; }