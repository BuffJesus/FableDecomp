#pragma once
#include "fable_base_class.h"
#include "fable_stlport_vector.h"
// Names and values from Ego_r.pdb; retail uses 12-byte STLport arrays.
enum EVertexStreamDataType {
 VERTEX_STREAM_DATA_TYPE_FLOAT1=0, VERTEX_STREAM_DATA_TYPE_FLOAT2=1,
 VERTEX_STREAM_DATA_TYPE_FLOAT3=2, VERTEX_STREAM_DATA_TYPE_FLOAT4=3,
 VERTEX_STREAM_DATA_TYPE_D3DCOLOR=4, VERTEX_STREAM_DATA_TYPE_SHORT2=5,
 VERTEX_STREAM_DATA_TYPE_SHORT4=6, VERTEX_STREAM_DATA_TYPE_UVSHORT2=7,
 VERTEX_STREAM_DATA_TYPE_NORMSHORT1=8, VERTEX_STREAM_DATA_TYPE_NORMSHORT2=9,
 VERTEX_STREAM_DATA_TYPE_NORMSHORT3=10, VERTEX_STREAM_DATA_TYPE_NORMSHORT4=11,
 VERTEX_STREAM_DATA_TYPE_NORMPACKED3=12, VERTEX_STREAM_DATA_TYPE_POSPACKED3=13
};
class CVertexStreamFormat : public CBaseClass
{
public:
 class CDataElement { public: unsigned long Register; EVertexStreamDataType DataType; };
 CVertexStreamFormat();
 void AddDataElement(unsigned long,EVertexStreamDataType);
 unsigned long CalculateVertexSize() const;
 static unsigned long FABLE_FASTCALL GetDataTypeSize(EVertexStreamDataType);
 _STL::vector<CDataElement> DataElements;
};
FABLE_STATIC_ASSERT(sizeof(CVertexStreamFormat::CDataElement)==8);
FABLE_STATIC_ASSERT(sizeof(CVertexStreamFormat)==16);
