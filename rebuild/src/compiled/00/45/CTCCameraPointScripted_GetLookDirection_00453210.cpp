#pragma optimize("y",off)
#pragma optimize("s",on)
struct C3DVector { float x, y, z; };
struct CRightHandedSet { float m[9]; };

void __stdcall GetLookDirHelper(C3DVector* out, CRightHandedSet* rhs, unsigned char* pb);

struct CTCCameraPointScripted {
    CTCCameraPointScripted* GetLookDirection(C3DVector* out, CRightHandedSet* rhs);
};

CTCCameraPointScripted* CTCCameraPointScripted::GetLookDirection(C3DVector* out, CRightHandedSet* rhs)
{
    unsigned char local;
    GetLookDirHelper(out, rhs, &local);
    return this;
}