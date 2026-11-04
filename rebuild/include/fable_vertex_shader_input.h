#pragma once
#include "fable_base_class.h"
#include "fable_vertex_stream_format.h"
#include "fable_d3d9_shader.h"
#include "fable_stlport_vector.h"
// Retail constructor AB3B10; STLport arrays are 12 bytes each.
class CVertexShaderInput : public CBaseClass
{
    friend struct CVertexShaderInputTestAccess;
    void GenerateD3DDeclaration();
public:
    class CStreamInfo { public: CStreamInfo(unsigned long index,const CVertexStreamFormat& format):StreamIndex(index),StreamFormat(format) {} unsigned long StreamIndex; CVertexStreamFormat StreamFormat; };
    CVertexShaderInput();
 void AddStream(unsigned long,const CVertexStreamFormat&,bool);
    _STL::vector<CStreamInfo> DataStreams;
    _STL::vector<D3DVERTEXELEMENT9> D3DDeclaration;
};
FABLE_STATIC_ASSERT(sizeof(CVertexShaderInput)==0x1c);

FABLE_STATIC_ASSERT(sizeof(CVertexShaderInput::CStreamInfo)==20);
