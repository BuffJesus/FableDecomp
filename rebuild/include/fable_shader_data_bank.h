#pragma once
#include "fable_game_text_bank.h"
#include "fable_shader_bank_state.h"
#include "fable_carray.h"
class CShaderDataBank;
class CShaderBankEntry : public CBaseClassNonCopyable
{
public:
    CShaderBankEntry(unsigned long type,unsigned long index,CShaderDataBank* bank);
    unsigned long ShaderType;
    CArrayVector<unsigned long> ShaderCode;
    CArrayVector<CCharString> ShaderConstants;
    CCharString VSConstantLayout;
};
FABLE_STATIC_ASSERT(offsetof(CShaderBankEntry,ShaderCode)==8);
FABLE_STATIC_ASSERT(offsetof(CShaderBankEntry,VSConstantLayout)==0x20);
FABLE_STATIC_ASSERT(sizeof(CShaderBankEntry)==0x24);
// Retail A62020/A62110: development-only parser fields from Ego_r are absent.
class CShaderDataBank : public CBankFile
{
public:
    CShaderDataBank();
    virtual ~CShaderDataBank();
    virtual void Close();
    CShaderBankEntry* GetVertexShader(unsigned long index) const;
    CArrayVector<CShaderBankEntry*> Shaders;
    CShaderBankStateBlock StateBlock;
protected:
    virtual unsigned long GetDataFormatVersion();
    virtual void NotifyEntryAdded(unsigned long,unsigned long,const CArray<char>&);
    virtual void NotifyEntryRemoved(unsigned long);
    virtual void PrepareDataForInclusion(unsigned long,const CArray<CCharString>&,
        unsigned long,CCountedPointer<CBankStateBlock>,CDataOutputStream&,CDataOutputStream&);
private:
    void CloseShaders();
};
FABLE_STATIC_ASSERT(offsetof(CShaderDataBank,Shaders)==0x110);
FABLE_STATIC_ASSERT(offsetof(CShaderDataBank,StateBlock)==0x11c);
FABLE_STATIC_ASSERT(sizeof(CShaderDataBank)==0x128);
