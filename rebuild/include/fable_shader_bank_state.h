#pragma once
#include "fable_font_state_blocks.h"
class CShaderBankStateBlock : public CBankStateBlock {
public:
    // PDB enum type; this descriptive enumerator names the observed native zero.
    enum EShaderTarget { SHADER_TARGET_NATIVE_DEFAULT=0 };
    struct CStateBlock { EShaderTarget ShaderTarget; bool EnableShaderOptimisation; };
    virtual ~CShaderBankStateBlock() {}
    virtual void GetDefault(CBankStateBlock& output);
    virtual unsigned long CreateCRC();
    CStateBlock State;
protected:
    virtual void Set(const CBankStateBlock* source);
};
FABLE_STATIC_ASSERT(sizeof(CShaderBankStateBlock::CStateBlock)==8);
FABLE_STATIC_ASSERT(sizeof(CShaderBankStateBlock)==12);
