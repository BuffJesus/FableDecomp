#include "fable_shader_bank_state.h"
#include "fable_crc.h"
#include <string.h>
void CShaderBankStateBlock::Set(const CBankStateBlock* source) {
    const CStateBlock& state=static_cast<const CShaderBankStateBlock*>(source)->State;
    State.EnableShaderOptimisation=state.EnableShaderOptimisation;
    State.ShaderTarget=state.ShaderTarget;
}
unsigned long CShaderBankStateBlock::CreateCRC(){
    return reinterpret_cast<const CCRC*>(this)->Calc(&State,sizeof State);
}
