#include "fable_shader_resource.h"
#include "fable_game_text_bank.h"
#include "fable_stlport_algobase.h"

void CShaderResource::OpenShaderBank(const CShaderDataBank* bank)
{
    ShaderBank = const_cast<CShaderDataBank*>(bank);
    // PDB and retail calls place the CBankFile base at offset zero.
    // The derived bank remains forward-declared until its separate ABI recovery.
    const CBankFile* base = reinterpret_cast<const CBankFile*>(bank);
    const unsigned long count = base->GetMaxEntryIndex() + 1;
    Shaders.resize(count, static_cast<CShaderBase*>(0));
    _STL::fill_n(Shaders.begin(), count, static_cast<CShaderBase*>(0));
}
