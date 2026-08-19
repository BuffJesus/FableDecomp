// Hand the counted pointer at this+0x24 back through an out-param and addref it.
// __fastcall this=ecx, out=stack (ret 4).
#pragma pack(push,1)
struct Def { void* vptr; long rc; };
struct T { char pad[0x24]; Def* def; void GetPBaseDef(Def** out); };
#pragma pack(pop)
void T::GetPBaseDef(Def** out) {
    Def* cur = this->def;
    *out = cur;
    if (cur) ++cur->rc;
}
