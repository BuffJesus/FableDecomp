#include <cstdio>

struct C3DVector { float x, y, z; };

struct Mesh {
    char pad[0x108];
    C3DVector m_worldPos;
};

bool __fastcall GetWorldPosition(Mesh* self, int edx_dummy, C3DVector& out)
{
    (void)edx_dummy;
    out = self->m_worldPos;
    return true;
}

int main()
{
    Mesh m;
    m.m_worldPos.x = 1.5f;
    m.m_worldPos.y = -2.25f;
    m.m_worldPos.z = 42.0f;
    C3DVector o;
    o.x = o.y = o.z = 0.0f;
    bool r = GetWorldPosition(&m, 0, o);
    if (r && o.x == 1.5f && o.y == -2.25f && o.z == 42.0f) {
        printf("PARITY_OK\n");
    } else {
        printf("FAIL\n");
    }
    return 0;
}