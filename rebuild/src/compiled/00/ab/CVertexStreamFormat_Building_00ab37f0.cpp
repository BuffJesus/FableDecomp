#include <new>
#include "fable_vertex_stream_format.h"
#include "fable_stlport_vector_impl.h"
__declspec(noinline) unsigned long FABLE_FASTCALL CVertexStreamFormat::GetDataTypeSize(EVertexStreamDataType type)
{
    switch(type)
    {
    case VERTEX_STREAM_DATA_TYPE_FLOAT1: case VERTEX_STREAM_DATA_TYPE_D3DCOLOR:
    case VERTEX_STREAM_DATA_TYPE_SHORT2: case VERTEX_STREAM_DATA_TYPE_UVSHORT2:
    case VERTEX_STREAM_DATA_TYPE_NORMSHORT2: case VERTEX_STREAM_DATA_TYPE_NORMPACKED3:
    case VERTEX_STREAM_DATA_TYPE_POSPACKED3: return 4;
    case VERTEX_STREAM_DATA_TYPE_FLOAT2: case VERTEX_STREAM_DATA_TYPE_SHORT4:
    case VERTEX_STREAM_DATA_TYPE_NORMSHORT4: return 8;
    case VERTEX_STREAM_DATA_TYPE_FLOAT3: return 12;
    case VERTEX_STREAM_DATA_TYPE_FLOAT4: return 16;
    case VERTEX_STREAM_DATA_TYPE_NORMSHORT1: return 2;
    case VERTEX_STREAM_DATA_TYPE_NORMSHORT3: return 6;
    default: return 0;
    }
}


CVertexStreamFormat::CVertexStreamFormat() {}
void CVertexStreamFormat::AddDataElement(unsigned long index,EVertexStreamDataType type)
{
 CDataElement element;
 element.Register=index;element.DataType=type;
 DataElements.push_back(element);
}
unsigned long CVertexStreamFormat::CalculateVertexSize() const
{
 unsigned long size=0;
 for(unsigned long i=0;i<DataElements.size();++i)size+=GetDataTypeSize(DataElements[i].DataType);
 return size;
}

