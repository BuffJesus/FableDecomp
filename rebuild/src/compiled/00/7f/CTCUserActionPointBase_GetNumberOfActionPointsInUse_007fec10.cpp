struct S { char pad[0xc]; int f0c; int f10; char pad2[0x24-0x14]; int f24; int M(); };
int S::M(){ return this->f24 - ((this->f10 - this->f0c) >> 2); }