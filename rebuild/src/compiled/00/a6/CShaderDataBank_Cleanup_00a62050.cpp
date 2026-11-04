#include "fable_shader_data_bank.h"
unsigned long CShaderDataBank::GetDataFormatVersion() { return 107; }
void CShaderDataBank::CloseShaders() {
    for (unsigned long i=0;i<Shaders.size();++i) delete Shaders[i];
    Shaders.clear();
}
void CShaderDataBank::Close() { CloseShaders(); CBankFile::Close(); }
void CShaderDataBank::NotifyEntryRemoved(unsigned long index) {
    delete Shaders[index];
    Shaders[index]=0;
}
void CShaderDataBank::PrepareDataForInclusion(unsigned long,const CArray<CCharString>&,
    unsigned long,CCountedPointer<CBankStateBlock>,CDataOutputStream&,CDataOutputStream&) {}
