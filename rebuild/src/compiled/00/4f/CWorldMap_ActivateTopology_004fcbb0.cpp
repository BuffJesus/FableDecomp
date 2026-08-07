struct E { char pad[0x26]; unsigned char b26; char pad2[72-0x27]; };
struct S { char pad[0x20]; E* f20; void M(int a0); };
void S::M(int a0){ this->f20[a0].b26 = 1; }