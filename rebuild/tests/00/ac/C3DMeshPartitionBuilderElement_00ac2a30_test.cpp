// Behaviour fixture for C3DMeshPartitionBuilderElement ~, BuildOutput and Build (provider
// C3DMeshPartitionBuilderElement_00ac2a30.cpp). Supplied here: PartitionSpace (records its arguments, then splits a node
// holding four or more triangles into two leaf children, halves in order, keeping its own copy), the output element's BuildSubElement /
// destructor, and the two counted-pointer Resets (recording). Checked: Build copies every input triangle in order, then
// partitions with depth 0 and limit 1e30, builds one output tree and one output list and hands them to the two counted
// pointers; BuildOutput copies each node's box, sphere and triangle range, builds both output children of a split node
// and appends only the leaves' triangles, depth-first; the destructor frees the whole builder tree.
#include <stdio.h>
#include <string.h>
#include <new>
#include "fable_mesh_partition.h"
#include "fable_stlport_vector_impl.h"

static int g_fail, g_partitions, g_builderDtors, g_resets; static long g_depth; static float g_limit;
static const C3DVertex2* g_vertices; static void* g_treeData; static void* g_listData;
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

static void Bounds(C3DMeshPartitionBuilderElement* e, float base, long start, long count)
{
    e->BoundingBox.MinCoord = C3DVector(base, base + 1, base + 2);
    e->BoundingBox.MaxCoord = C3DVector(base + 3, base + 4, base + 5);
    e->BoundingSphere.Center = C3DVector(base + 6, base + 7, base + 8);
    e->BoundingSphere.Radius = base + 9;
    e->StartTriangle = start; e->TriangleCount = count;
}
static C3DMeshPartitionBuilderElement* NewBuilder()
{
    C3DMeshPartitionBuilderElement* e = new C3DMeshPartitionBuilderElement;
    e->SubElements[0] = e->SubElements[1] = 0;
    return e;
}
long C3DMeshPartitionBuilderElement::PartitionSpace(const C3DVertex2* vertices, long depth, float limit)
{
    ++g_partitions; g_vertices = vertices; g_depth = depth; g_limit = limit;
    long n = (long)Triangles.size();
    Bounds(this, 100, 0, n);
    if (n >= 4)
    {
        SubElements[0] = NewBuilder(); SubElements[1] = NewBuilder();
        for (long i = 0; i < n; ++i) SubElements[i < n / 2 ? 0 : 1]->Triangles.push_back(Triangles[i]);
        Bounds(SubElements[0], 200, 0, n / 2); Bounds(SubElements[1], 300, n / 2, n - n / 2);
    }
    return 0;
}
C3DMeshPartitionElement* C3DMeshPartitionElement::BuildSubElement(long index)
{
    SubElements[index] = new C3DMeshPartitionElement;
    return SubElements[index];
}
C3DMeshPartitionElement::~C3DMeshPartitionElement() { delete SubElements[0]; delete SubElements[1]; }
template <> void CCountedPointer<C3DMeshPartitionElement>::Reset(C3DMeshPartitionElement* p) { ++g_resets; g_treeData = p; Data = p; }
template <> void CCountedPointer<CArray<C3DTriangle2> >::Reset(CArray<C3DTriangle2>* p) { ++g_resets; g_listData = p; Data = p; }

static C3DTriangle2 Tri(unsigned short a) { C3DTriangle2 t; t.VertexIndex[0] = a; t.VertexIndex[1] = a + 1; t.VertexIndex[2] = a + 2; return t; }
static bool SameBounds(const C3DMeshPartitionElement* o, float base, long start, long count)
{
    return o->BoundingBox.MinCoord.X == base && o->BoundingBox.MaxCoord.Z == base + 5 && o->BoundingSphere.Center.Y == base + 7 &&
           o->BoundingSphere.Radius == base + 9 && o->StartTriangle == start && o->TriangleCount == count;
}

int main()
{
    static C3DVertex2 vertices[4];
    C3DTriangle2 input[6];
    for (int i = 0; i < 6; ++i) input[i] = Tri((unsigned short)(10 * i));

    // Split case: six triangles -> two leaves of three.
    C3DMeshPartitionBuilderElement* b = NewBuilder();
    CCountedPointer<C3DMeshPartitionElement> tree; CCountedPointer<CArray<C3DTriangle2> > list;
    tree.Data = 0; tree.Info = 0; list.Data = 0; list.Info = 0;
    b->Build(vertices, input, 6, &tree, &list);
    CHECK(g_partitions == 1 && g_vertices == vertices && g_depth == 0 && g_limit == 1e30f && g_resets == 2);
    C3DMeshPartitionElement* root = (C3DMeshPartitionElement*)g_treeData;
    CArray<C3DTriangle2>* out = (CArray<C3DTriangle2>*)g_listData;
    CHECK(root && tree.Data == root && out && list.Data == out);
    CHECK(SameBounds(root, 100, 0, 6) && root->SubElements[0] && root->SubElements[1]);
    CHECK(SameBounds(root->SubElements[0], 200, 0, 3) && SameBounds(root->SubElements[1], 300, 3, 3));
    CHECK(!root->SubElements[0]->SubElements[0] && !root->SubElements[1]->SubElements[1]);
    CHECK(out->size() == 6);
    for (int i = 0; i < 6 && out->size() == 6; ++i) CHECK((*out)[i].VertexIndex[0] == 10 * i && (*out)[i].VertexIndex[2] == 10 * i + 2);
    delete b;
    delete root; delete out;

    // Leaf case: two triangles stay in the root; the output tree has no children.
    b = NewBuilder(); g_resets = 0;
    b->Build(vertices, input + 2, 2, &tree, &list);
    root = (C3DMeshPartitionElement*)g_treeData; out = (CArray<C3DTriangle2>*)g_listData;
    CHECK(g_partitions == 2 && g_resets == 2 && SameBounds(root, 100, 0, 2) && !root->SubElements[0] && !root->SubElements[1]);
    CHECK(out->size() == 2 && (*out)[0].VertexIndex[0] == 20 && (*out)[1].VertexIndex[0] == 30);
    CHECK(b->Triangles.size() == 2);
    delete b; delete root; delete out;

    // Zero triangles: the copy loop is skipped, PartitionSpace still runs.
    b = NewBuilder();
    b->Build(vertices, input, 0, &tree, &list);
    out = (CArray<C3DTriangle2>*)g_listData;
    CHECK(g_partitions == 3 && out->size() == 0 && b->Triangles.size() == 0);
    delete b; delete (C3DMeshPartitionElement*)g_treeData; delete out;

    if (g_fail) { printf("MESH_PARTITION_BUILDER_FAIL %d\n", g_fail); return 1; }
    printf("MESH_PARTITION_BUILDER_OK\n");
    return 0;
}
