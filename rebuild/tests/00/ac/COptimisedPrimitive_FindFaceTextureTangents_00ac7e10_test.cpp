// Behaviour fixture for QuadricOptimiserInternals::COptimisedPrimitive::FindFaceTextureTangents (provider
// COptimisedPrimitive_FindFaceTextureTangents_00ac7e10.cpp) on raw storage: the primitive's vertex array (+0x10) points
// at retail-stride (0xB8) vertex records (Position +0x00, TextureCoord +0x18). FindTextureTangent is supplied here and
// records its arguments. Checked: it is called twice on the same primitive with the face's three vertex positions in
// face order, first with their texture U coordinates, then with their V coordinates; its results land in the face's
// UTangent and VTangent; the face's indices and edges are untouched.
#include <stdio.h>
#include <string.h>
#include "fable_quadric_optimiser.h"

using namespace QuadricOptimiserInternals;
static int g_fail, g_calls;
static const void* g_this[2]; static const C3DVector* g_p[2][3]; static float g_t[2][3];
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

C3DVector COptimisedPrimitive::FindTextureTangent(const C3DVector& p0, const C3DVector& p1, const C3DVector& p2, float t0,
                                                  float t1, float t2)
{
    int k = g_calls < 2 ? g_calls : 1;
    g_this[k] = this; g_p[k][0] = &p0; g_p[k][1] = &p1; g_p[k][2] = &p2; g_t[k][0] = t0; g_t[k][1] = t1; g_t[k][2] = t2;
    ++g_calls;
    return g_calls == 1 ? C3DVector(1, 2, 3) : C3DVector(-4, -5, -6);
}

static unsigned char g_vertices[6][0xB8];
static unsigned char g_prim[0x20];
static void SetVertex(int i, float u, float v) { float tc[3] = { u, v, 99 }; memcpy(g_vertices[i] + 0x18, tc, 12); }

int main()
{
    memset(g_vertices, 0, sizeof(g_vertices));
    for (int i = 0; i < 6; ++i) SetVertex(i, 10.0f + i, 20.0f + i);
    void* begin = g_vertices; void* end = g_vertices + 6;
    memcpy(g_prim + 0x10, &begin, 4); memcpy(g_prim + 0x14, &end, 4); memcpy(g_prim + 0x18, &end, 4);
    COptimisedPrimitive* prim = reinterpret_cast<COptimisedPrimitive*>(g_prim);

    COptimisedFace face; memset(&face, 0x33, sizeof(face));
    face.VertexIndices[0] = 4; face.VertexIndices[1] = 1; face.VertexIndices[2] = 5;
    COptimisedFace before = face;
    prim->FindFaceTextureTangents(face);

    CHECK(g_calls == 2 && g_this[0] == prim && g_this[1] == prim);
    for (int k = 0; k < 2; ++k)
    {
        CHECK(g_p[k][0] == (const C3DVector*)g_vertices[4] && g_p[k][1] == (const C3DVector*)g_vertices[1] &&
              g_p[k][2] == (const C3DVector*)g_vertices[5]);
    }
    CHECK(g_t[0][0] == 14 && g_t[0][1] == 11 && g_t[0][2] == 15);
    CHECK(g_t[1][0] == 24 && g_t[1][1] == 21 && g_t[1][2] == 25);
    CHECK(face.UTangent.X == 1 && face.UTangent.Y == 2 && face.UTangent.Z == 3);
    CHECK(face.VTangent.X == -4 && face.VTangent.Y == -5 && face.VTangent.Z == -6);
    CHECK(!memcmp(&face, &before, 0x24));

    if (g_fail) { printf("FACE_TANGENTS_FAIL %d\n", g_fail); return 1; }
    printf("FACE_TANGENTS_OK\n");
    return 0;
}
