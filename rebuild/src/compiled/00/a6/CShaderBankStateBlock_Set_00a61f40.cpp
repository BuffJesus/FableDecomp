// Retail 0x00A61F40. PDB names identify the two shader-bank settings copied here.
struct CShaderBankStateBlock {
    void* __vftable;
    long ShaderTarget;
    bool EnableShaderOptimisation;
    void Set(const CShaderBankStateBlock* pState);
};
void CShaderBankStateBlock::Set(const CShaderBankStateBlock* pState) {
    EnableShaderOptimisation = pState->EnableShaderOptimisation;
    ShaderTarget = pState->ShaderTarget;
}