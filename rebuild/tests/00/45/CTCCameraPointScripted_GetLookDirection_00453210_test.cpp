#include <cstdio>
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

void __stdcall GetLookDirHelper(C3DVector* out, CRightHandedSet* rhs, unsigned char* pb) {
    out->x = rhs->m[6];
    out->y = rhs->m[7];
    out->z = rhs->m[8];
    *pb = 0;
}

int main() {
    CRightHandedSet rhs;
    for (int i = 0; i < 9; ++i) rhs.m[i] = (float)i;
    CTCCameraPointScripted cam;
    C3DVector v;
    CTCCameraPointScripted* r = cam.GetLookDirection(&v, &rhs);
    if (r == &cam && v.x == 6.0f && v.y == 7.0f && v.z == 8.0f)
        printf("LOOKDIR_OK\n");
    else
        printf("FAIL\n");
    return (r == &cam && v.x == 6.0f && v.y == 7.0f && v.z == 8.0f) ? 0 : 1;
}