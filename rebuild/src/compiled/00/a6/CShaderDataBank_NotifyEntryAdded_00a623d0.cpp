#include <new>
#include "fable_shader_data_bank.h"
#include "fable_stlport_vector_impl.h"
void CShaderDataBank::NotifyEntryAdded(unsigned long type,unsigned long index,
    const CArray<char>& data)
{
    while (Shaders.size() <= index) Shaders.push_back(0);
    CShaderBankEntry* entry=0;
    if (type==0 || type==1) entry=new CShaderBankEntry(type,index,this);
    Shaders[index]=entry;
    CBankFile::NotifyEntryAdded(type,index,data);
}
