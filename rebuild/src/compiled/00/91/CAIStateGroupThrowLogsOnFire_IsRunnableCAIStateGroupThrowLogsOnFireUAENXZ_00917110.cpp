// Reconstruction of ?IsRunnable@CAIStateGroup_ThrowLogsOnFire@@UAE_NXZ @ 0x00917110
struct CAIStateGroup_IdleAtHome;

// The two retail callees at 0x917050 and 0x9170b0 (this-call, return a testable value).
// Model as __fastcall (ecx = self) returning int (retail tests full eax).
int __fastcall subA(CAIStateGroup_IdleAtHome* self);
int __fastcall subB(CAIStateGroup_IdleAtHome* self);

bool __fastcall IsRunnable(CAIStateGroup_IdleAtHome* self)
{
    if (subA(self) != 0 && subB(self) != 0)
        return true;
    return false;
}