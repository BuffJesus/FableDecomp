struct E { char pad[0x20]; int v20; char pad2[0x48-0x24]; };
struct S { char pad[0x20]; E* f20; int GetEditMap(int i); };
int S::GetEditMap(int i){ return this->f20[i].v20; }