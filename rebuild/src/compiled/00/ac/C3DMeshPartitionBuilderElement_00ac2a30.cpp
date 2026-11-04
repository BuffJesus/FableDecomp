// C3DMeshPartitionBuilderElement (Ego_r lib_3d_mesh_file_partition.cpp): the destructor 0x00AC2A30, BuildOutput
// 0x00AC2A80 and Build 0x00AC36D0 (the counted-pointer Resets it calls are instantiated with the element methods). Build copies the triangles in, partitions them (PartitionSpace, limit 1e30), then
// writes a fresh output tree and triangle list and hands both to the caller's counted pointers. BuildOutput copies a
// node's bounds and triangle range; a split node builds its two output children, a leaf appends its triangles.
#include <new>
#include "fable_mesh_partition.h"
#include "fable_stlport_vector_impl.h"

C3DMeshPartitionBuilderElement::~C3DMeshPartitionBuilderElement()
{
    delete SubElements[0];
    delete SubElements[1];
}

void C3DMeshPartitionBuilderElement::BuildOutput(C3DMeshPartitionElement* output, CArray<C3DTriangle2>* triangleList)
{
    output->BoundingBox = BoundingBox;
    output->BoundingSphere = BoundingSphere;
    output->StartTriangle = StartTriangle;
    output->TriangleCount = TriangleCount;
    if (SubElements[0])
    {
        SubElements[0]->BuildOutput(output->BuildSubElement(0), triangleList);
        SubElements[1]->BuildOutput(output->BuildSubElement(1), triangleList);
    }
    else
    {
        for (unsigned long i = 0; i < Triangles.size(); ++i)
            triangleList->push_back(Triangles[i]);
    }
}

void C3DMeshPartitionBuilderElement::Build(const C3DVertex2* vertices, const C3DTriangle2* triangles, long triangleCount,
                                           CCountedPointer<C3DMeshPartitionElement>* tree,
                                           CCountedPointer<CArray<C3DTriangle2> >* triangleList)
{
    for (long i = 0; i < triangleCount; ++i)
        Triangles.push_back(triangles[i]);
    PartitionSpace(vertices, 0, 1e30f);
    C3DMeshPartitionElement* root = new C3DMeshPartitionElement;
    CArray<C3DTriangle2>* list = new CArray<C3DTriangle2>;
    BuildOutput(root, list);
    tree->Reset(root);
    triangleList->Reset(list);
}
