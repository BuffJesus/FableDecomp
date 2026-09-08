#include <stdio.h>
struct CShaderBankStateBlock { void* v; long target; bool optimise;
 void Set(const CShaderBankStateBlock*); };
void CShaderBankStateBlock::Set(const CShaderBankStateBlock* state) {
 optimise=state->optimise; target=state->target; }
int main() { CShaderBankStateBlock from,to; from.target=4; from.optimise=true;
 to.target=0; to.optimise=false; to.Set(&from);
 if(to.target!=4 || !to.optimise) return 1;
 printf("SHADER_STATE_SET PASS\n"); return 0; }