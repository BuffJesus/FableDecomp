// CAnimComponentStrikeSpeedMultiplier::Alloc @ 0x005df8e0
// __fastcall (self in ecx) returning CAnimComponentBase*
// Classic: p = operator new(0xc); if(p){ p[+4]=1; p[0]=vtable; } return p;

typedef unsigned long ULONG;

struct CAnimComponentBase;

struct CAnimComponentStrikeSpeedMultiplier {
    ULONG vtable;   // +0
    ULONG field4;   // +4
    ULONG field8;   // +8
};

extern "C" void * __cdecl fable_op_new(unsigned int size);

// this-call modeled as __fastcall: self arrives in ecx
CAnimComponentBase * __fastcall Alloc(CAnimComponentStrikeSpeedMultiplier * self)
{
    CAnimComponentStrikeSpeedMultiplier * p =
        (CAnimComponentStrikeSpeedMultiplier *)fable_op_new(0xc);
    if (p) {
        p->field4 = 1;
        p->vtable = 0x1255800;
        return (CAnimComponentBase *)p;
    }
    return (CAnimComponentBase *)0;
}