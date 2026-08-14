struct CCharString;

struct Sub {
    int resolve(CCharString* s);  // thiscall: ecx=this, s on stack
};

struct Obj {
    char pad[0x54];
    Sub* p54;
};

struct CTCEnemy;
// call 0x686a80 : thiscall on CTCEnemy, returns Obj*
extern "C" Obj* __fastcall Enemy_GetObj(CTCEnemy* self);

struct CTCEnemy {
    char pad[0x18];
    int f18;
};

void __fastcall SetFaction(CTCEnemy* self, void* edx, CCharString* faction)
{
    Obj* o = Enemy_GetObj(self);
    Sub* s = o->p54;
    self->f18 = s->resolve(faction);
}