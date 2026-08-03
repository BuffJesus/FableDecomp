// CTCCameraPointScriptedSpline::EditCyclePreviousKeyCamera @ 0x0041B9C0  (54 bytes)
//
// Genuine C++ decompilation. Modeled as a free __fastcall so the compiler emits
// the this-in-ecx idiom (VC7.1 rejects the literal __fastcall keyword on a free
// function). The two masked callees are genuine __fastcall member calls on the
// spline definition owner, so the compiler reproduces the retail idiom:
//   mov ecx, self ; push args right-to-left ; call rel32
//
// Retail behaviour:
//   def = this->def (this+0x08); if def is null -> return.
//   if (this->cachedKey (this+0x18) == def->keyIndex (def+0x2C)) -> return.
//   StepBackward();                                    // this-only masked call
//   def = this->def;                                   // reloaded after call
//   ApplyKey(def->field10, &def->field04);             // two stack args
//   this->cachedKey = this->def->keyIndex;             // re-cache index

struct FableSplineKeyDef_0041B9C0
{
    unsigned int field00;   /* +0x00 */
    unsigned int field04;   /* +0x04 */
    unsigned int field08;   /* +0x08 */
    unsigned int field0C;   /* +0x0C */
    unsigned int field10;   /* +0x10 */
    unsigned int pad14[6];  /* +0x14 .. +0x2B */
    unsigned int keyIndex;  /* +0x2C */
};

struct FableCameraPointScriptedSpline_0041B9C0
{
    unsigned int              pad00[2];   /* +0x00 .. +0x07 */
    FableSplineKeyDef_0041B9C0* def;      /* +0x08 */
    unsigned int              pad0C[3];   /* +0x0C .. +0x17 */
    unsigned int              cachedKey;  /* +0x18 */

    void StepBackward();                              /* masked @ 0x5E05F0 */
    void ApplyKey(unsigned int keyValue, unsigned int* keyRef); /* masked @ 0x5E05D0 */
};

void __fastcall FableCameraSplineEditCyclePreviousKeyCamera_0041B9C0(
    FableCameraPointScriptedSpline_0041B9C0* self)
{
    FableSplineKeyDef_0041B9C0* def = self->def;
    if (def != 0 && self->cachedKey != def->keyIndex)
    {
        self->StepBackward();
        def = self->def;
        self->ApplyKey(def->field10, &def->field04);
        self->cachedKey = self->def->keyIndex;
    }
}