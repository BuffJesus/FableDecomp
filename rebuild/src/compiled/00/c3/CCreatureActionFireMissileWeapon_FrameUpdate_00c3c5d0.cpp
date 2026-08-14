// Byte-exact reconstruction of CCreatureAction_FireMissileWeapon::FrameUpdate @ 0x00c3c5d0
// __fastcall (this in ecx). Returns bool (al).

// Global error sink referenced by mov ecx, 0x143bb58; call 0xc3d090 is a thiscall
// member taking one stack int arg. Model as a real (thiscall) member so codegen emits
// push <arg>; mov ecx, self; call  -- with NO edx setup.
struct ErrSink {
    void report(int code); // thiscall: ecx=this, stack arg
};

extern ErrSink g_errSink;

struct CCreatureAction_FireMissileWeapon {
    char pad[0xc];
    unsigned int counter; // +0xc
};

bool __fastcall FrameUpdate(CCreatureAction_FireMissileWeapon* self)
{
    unsigned int v = self->counter;
    if (v < 0xFFFFFFFFu) {
        self->counter = v + 1;
        return true;
    }
    g_errSink.report(-6);
    return false;
}