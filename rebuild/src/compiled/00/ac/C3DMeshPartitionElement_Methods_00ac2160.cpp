// C3DMeshPartitionElement (Ego_r lib_3d_mesh_file_partition.cpp): the destructor 0x00AC2100, BuildSubElement 0x00AC2160, ReadFromFile 0x00AC21E0,
// WriteToFile 0x00AC27A0, Sizeof 0x00AC2930, the scalar deleting destructor 0x00AC37B0, plus the partition chunk's
// WriteToFile 0x00AC2920 / ReadFromFile 0x00AC2970 and the CCountedPointer<C3DMeshPartitionElement> DeleteData
// 0x00AC3810 / Reset 0x00AC38A0 they instantiate, and the CCountedPointer<CArray<C3DTriangle2> > Reset 0x00AC3A40 /
// DeleteData 0x00AC3A10 that C3DMeshPartitionBuilderElement::Build calls. A node stores its bounding box and sphere, its triangle range and,
// when it is split, two child nodes; the file form writes the children depth-first after a flag byte.
#include "fable_mesh_partition.h"
#include "fable_chunked_file.h"
#include "fable_data_input_stream.h"
#include "fable_data_output_stream.h"

C3DMeshPartitionElement::~C3DMeshPartitionElement()
{
    delete SubElements[0];
    delete SubElements[1];
    StartTriangle = 0;
    TriangleCount = 0;
}

C3DMeshPartitionElement* C3DMeshPartitionElement::BuildSubElement(long index)
{
    delete SubElements[index];
    SubElements[index] = new C3DMeshPartitionElement;
    return SubElements[index];
}

void C3DMeshPartitionElement::ReadFromFile(CDataInputStream& stream)
{
    delete SubElements[0];
    delete SubElements[1];
    SubElements[0] = 0;
    SubElements[1] = 0;

    C3DVector minimum;
    minimum.X = stream.ReadFloat();
    minimum.Y = stream.ReadFloat();
    minimum.Z = stream.ReadFloat();
    C3DVector maximum;
    maximum.X = stream.ReadFloat();
    maximum.Y = stream.ReadFloat();
    maximum.Z = stream.ReadFloat();
    BoundingBox.MinCoord = minimum;
    BoundingBox.MaxCoord = maximum;

    C3DVector centre;
    centre.X = stream.ReadFloat();
    centre.Y = stream.ReadFloat();
    centre.Z = stream.ReadFloat();
    float radius = stream.ReadFloat();
    BoundingSphere.Center = centre;
    BoundingSphere.Radius = radius;

    StartTriangle = stream.ReadValue<long>();
    TriangleCount = stream.ReadValue<long>();
    if (stream.ReadEBOOL())
    {
        SubElements[0] = new C3DMeshPartitionElement;
        SubElements[1] = new C3DMeshPartitionElement;
        SubElements[0]->ReadFromFile(stream);
        SubElements[1]->ReadFromFile(stream);
    }
}

void C3DMeshPartitionElement::WriteToFile(CDataOutputStream& stream) const
{
    stream.WriteFloat(BoundingBox.PeekMinCoord().X);
    stream.WriteFloat(BoundingBox.PeekMinCoord().Y);
    stream.WriteFloat(BoundingBox.PeekMinCoord().Z);
    stream.WriteFloat(BoundingBox.PeekMaxCoord().X);
    stream.WriteFloat(BoundingBox.PeekMaxCoord().Y);
    stream.WriteFloat(BoundingBox.PeekMaxCoord().Z);
    stream.WriteFloat(BoundingSphere.GetCenter().X);
    stream.WriteFloat(BoundingSphere.GetCenter().Y);
    stream.WriteFloat(BoundingSphere.GetCenter().Z);
    stream.WriteFloat(BoundingSphere.GetRadius());
    stream.WriteSLONG(StartTriangle);
    stream.WriteSLONG(TriangleCount);
    if (SubElements[0])
    {
        stream.WriteEBOOL(true);
        SubElements[0]->WriteToFile(stream);
        SubElements[1]->WriteToFile(stream);
    }
    else
    {
        stream.WriteEBOOL(false);
    }
}

void C3DMeshFilePartitionChunk::WriteToFile(CDataOutputStream& stream, CChunkedFileSaver&) const
{
    PartitionTree->WriteToFile(stream);
}

unsigned long C3DMeshPartitionElement::Sizeof() const
{
    return (SubElements[0] ? SubElements[0]->Sizeof() + 4 : 4) + (SubElements[1] ? SubElements[1]->Sizeof() + 4 : 4);
}

template<typename T>
void CCountedPointer<T>::Reset(T* data)
{
    Release();
    Info = 0;
    Data = data;
    if (Data)
        Info = new CCPPointerInfo(&DeleteData, Data);
}

void C3DMeshFilePartitionChunk::ReadFromFile(CDataInputStream& stream, unsigned long, CChunkedFileLoader&)
{
    C3DMeshPartitionElement* tree = new C3DMeshPartitionElement;
    tree->ReadFromFile(stream);
    PartitionTree.Reset(tree);
}

template void CCountedPointer<C3DMeshPartitionElement>::Reset(C3DMeshPartitionElement* data);
template void CCountedPointer<C3DMeshPartitionElement>::DeleteData(void*);
template void CCountedPointer<CArray<C3DTriangle2> >::Reset(CArray<C3DTriangle2>* data);
template void CCountedPointer<CArray<C3DTriangle2> >::DeleteData(void*);
