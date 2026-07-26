// ?GetAngleXY@CGameScriptThing@@UBEMXZ @ 0x008cff10
// Retail: d9 41 40 c3  =>  fld DWORD PTR [ecx+0x40] ; ret
struct CGameScriptThing {
    char pad[0x40];
    float mAngleXY;   // 0x40
};

float __fastcall CGameScriptThing__GetAngleXY(const CGameScriptThing* this_ptr)
{
    return this_ptr->mAngleXY;
}