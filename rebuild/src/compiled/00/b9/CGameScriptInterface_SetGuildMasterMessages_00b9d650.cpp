struct P { char pad[0x27]; unsigned char b27; };
struct A { char pad[4]; P* f4; };
struct S { void M(A* a0, char b); };
void S::M(A* a0, char b){ a0->f4->b27 = b; }