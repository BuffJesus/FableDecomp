struct E { char pad[0x1c]; int v1c; char pad2[0x48-0x20]; };
struct S { char pad[0x20]; E* f20; int* GetLevelScriptName(int i); };
int* S::GetLevelScriptName(int i){ return &this->f20[i].v1c; }