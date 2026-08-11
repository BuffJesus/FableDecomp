#include <cstdio>

struct C3DVector { float x, y, z; };
struct CRightHandedSet { float m[9]; };

static int g_called = 0;
static char* g_flag = 0;
void GetLookDir_helper(C3DVector* out, CRightHandedSet* rhs, char* flag)
{
    g_called = 1;
    g_flag = flag;
    out->x = rhs->m[0];
    out->y = rhs->m[1];
    out->z = rhs->m[2];
}

struct CTCCameraPointScripted {
    CTCCameraPointScripted* GetLookDirection(C3DVector* out, CRightHandedSet* rhs);
};

CTCCameraPointScripted* CTCCameraPointScripted::GetLookDirection(C3DVector* out, CRightHandedSet* rhs)
{
    char flag;
    GetLookDir_helper(out, rhs, &flag);
    return this;
}

int main()
{
    CTCCameraPointScripted cam;
    CRightHandedSet rhs;
    rhs.m[0] = 1.5f; rhs.m[1] = 2.5f; rhs.m[2] = 3.5f;
    C3DVector out;
    CTCCameraPointScripted* r = cam.GetLookDirection(&out, &rhs);
    if (r == &cam && g_called && out.x == 1.5f && out.y == 2.5f && out.z == 3.5f && g_flag != 0)
        printf("LOOKDIR_OK\n");
    else
        printf("LOOKDIR_FAIL\n");
    return 0;
}