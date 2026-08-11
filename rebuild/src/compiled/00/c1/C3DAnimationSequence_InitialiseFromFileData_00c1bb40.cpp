// C3DAnimationSequence::InitialiseFromFileData (retail 0x00c1bb40)
// Tiny setter: stores (short)(f * scale) into a member at +8.
// Codegen: fld [esp+4]; fmul [scale]; call __ftol; mov [esi+8], ax; ret 4
// No mov eax,1 is emitted, so this is effectively void-returning.

static const float g_animScale = 65535.0f / 360.0f;  // static float at .rdata (relocated fmul operand)

struct C3DAnimationSequence
{
    int   field0;      // +0x00
    int   field4;      // +0x04
    short m_value;     // +0x08

    void InitialiseFromFileData(float f)
    {
        m_value = (short)(f * g_animScale);
    }
};

void InitialiseFromFileData_C3DAnimationSequence(C3DAnimationSequence* self, float f)
{
    self->InitialiseFromFileData(f);
}