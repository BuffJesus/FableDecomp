#include <new>
#include "fable_vertex_shader_input.h"
// This separately compiled native boundary is AB4260.
namespace _STL {
template<> void vector<D3DVERTEXELEMENT9>::_M_fill_insert(D3DVERTEXELEMENT9*,size_t,const D3DVERTEXELEMENT9&);
}
#include "fable_stlport_vector_impl.h"

// Retail 0129E9CC maps stream types to D3DDECLTYPE; -1 rejects unsupported types.
const long g_VertexStreamD3DTypes_0129E9CC[14]={0,1,2,3,4,6,7,6,-1,-1,-1,-1,-1,-1};

namespace _STL {
// Class-element erase follows the STLport copy loop. The shared default
// erase currently models the POD path only.
template<> inline D3DVERTEXELEMENT9* vector<D3DVERTEXELEMENT9>::erase(D3DVERTEXELEMENT9* first,D3DVERTEXELEMENT9* last)
{
    D3DVERTEXELEMENT9* result=__copy(last,_M_finish,first,random_access_iterator_tag(),(ptrdiff_t*)0);
    _M_finish=result;
    return first;
}
}

static inline unsigned long FABLE_FASTCALL VertexStreamDataTypeSize(EVertexStreamDataType type)
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

void CVertexShaderInput::GenerateD3DDeclaration()
{
    unsigned short offset=0;
    unsigned long output=0;
    unsigned long total=0;
    for(unsigned long stream=0;stream<DataStreams.size();++stream)
        total+=DataStreams[stream].StreamFormat.DataElements.size()+1;
    D3DDeclaration.resize(total);
    D3DVERTEXELEMENT9 element;
    for(stream=0;stream<DataStreams.size();++stream)
    {
        const CStreamInfo& streamInfo=DataStreams[stream];
        const unsigned long count=streamInfo.StreamFormat.DataElements.size();
        for(unsigned long i=0;i<count;++i)
        {
            const CVertexStreamFormat::CDataElement& data=streamInfo.StreamFormat.DataElements[i];
            const EVertexStreamDataType dataType=data.DataType;
            long type=g_VertexStreamD3DTypes_0129E9CC[dataType];
            if(type==-1){D3DDeclaration.clear();return;}
            element.Stream=static_cast<unsigned short>(stream);
            element.Offset=offset;
            element.Type=static_cast<unsigned char>(type);
            element.Method=0;
            element.Usage=5;
            element.UsageIndex=static_cast<unsigned char>(data.Register);
            D3DDeclaration[output++]=element;
            offset=static_cast<unsigned short>(offset+VertexStreamDataTypeSize(dataType));
        }
    }
    const D3DVERTEXELEMENT9 end={0xff,0,17,0,0,0};
    D3DDeclaration[output]=end;
}

