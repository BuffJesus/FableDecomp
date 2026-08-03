struct C3DVector { float x, y, z; };

struct CTCCoopSpirit {
    char pad[0x154];
    C3DVector movement;
};

// Retail is __fastcall: this=ecx, source ptr on stack, ret 4.
// Emulate with __fastcall + an unused edx slot so the real arg stays on the stack.
void __fastcall ApplyMovementVector(CTCCoopSpirit* self, int /*edx_unused*/, const C3DVector* v)
{
    self->movement = *v;
}