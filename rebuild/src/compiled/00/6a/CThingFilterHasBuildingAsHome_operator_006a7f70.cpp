// CThingFilter_HasBuildingAsHome::operator()  @ 006a7f70
// bool __fastcall op()(CThingFilter_HasBuildingAsHome* this, CThingAICreature* creature)
//   push esi; mov esi,ecx; mov ecx,[esp+8]; add ecx,0x168; call 0xa01b50
//   sub eax,[esi]; neg;sbb;inc  -> (call(creature+0x168) == this->field0)

struct SomeHomeThing;

// the __fastcall member at 0xa01b50 on the object living at creature+0x168
struct HomeObj {
    int GetHomeId();   // __fastcall, returns int in eax
};

struct CThingAICreature {
    char pad[0x168];
    HomeObj home;      // object embedded at +0x168; add ecx,0x168 = &creature->home
};

struct CThingFilter_HasBuildingAsHome {
    int field0;        // the id to match

    bool operator()(CThingAICreature* creature);
};

bool CThingFilter_HasBuildingAsHome::operator()(CThingAICreature* creature)
{
    return creature->home.GetHomeId() == this->field0;
}