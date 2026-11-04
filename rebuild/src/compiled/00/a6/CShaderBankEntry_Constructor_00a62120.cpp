#include <new>
#include "fable_shader_data_bank.h"
#include "fable_data_input_stream.h"
namespace _STL {
    template<> void vector<unsigned long>::resize(size_t,const unsigned long&);
    template<> void vector<CCharString>::_M_insert_overflow(CCharString*,const CCharString&,
        const __false_type&,size_t,bool);
}
#include "fable_stlport_vector_impl.h"
CShaderBankEntry::CShaderBankEntry(unsigned long type,unsigned long index,CShaderDataBank* bank)
    : ShaderType(type)
{
    unsigned long size=bank->GetEntryDataSize(index);
    CArrayVector<char> bytes(size);
    bank->ReadEntryData(index,0,bytes.GetAsCArray(),size);
    CMemoryDataInputStream stream(bytes.GetAsCArray(),size);
    stream >> index;
    unsigned long words=index/4;
    index=0;
    ShaderCode.resize(words,index);
    stream.Read(ShaderCode.GetAsCArray(),words*4);
    stream >> index;
    for (unsigned long remaining=index;remaining;--remaining)
        ShaderConstants.push_back(stream.ReadNullTerminatedString());
    VSConstantLayout=stream.ReadNullTerminatedString();
}
