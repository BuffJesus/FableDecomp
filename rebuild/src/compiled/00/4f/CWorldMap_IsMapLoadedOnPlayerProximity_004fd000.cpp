struct E { char pad[0x27]; char v27; char pad2[0x48-0x28]; };
struct S { char pad[0x20]; E* f20; char IsMapLoadedOnPlayerProximity(int i); };
char S::IsMapLoadedOnPlayerProximity(int i){ return this->f20[i].v27; }