#include "fable_shader_bank_state.h"
#include <string.h>
void CShaderBankStateBlock::GetDefault(CBankStateBlock& output) {
    CShaderBankStateBlock& block=static_cast<CShaderBankStateBlock&>(output);
    memset(&block.State,0,sizeof block.State);
    block.State.EnableShaderOptimisation=true;
    block.State.ShaderTarget=SHADER_TARGET_NATIVE_DEFAULT;
}
