#pragma optimize("s",on)
struct C3DVector { float x, y, z; };
struct CRightHandedSet { float m[9]; };

// callee at 0x581266 (cdecl): takes (out, rhs, &flag)
void __stdcall GetLookDir_helper(C3DVector* out, CRightHandedSet* rhs, char* flag);

struct CTCCameraPointScripted {
    CTCCameraPointScripted* GetLookDirection(C3DVector* out, CRightHandedSet* rhs);
};

CTCCameraPointScripted* CTCCameraPointScripted::GetLookDirection(C3DVector* out, CRightHandedSet* rhs)
{
    char flag;
    GetLookDir_helper(out, rhs, &flag);
    return this;
}