#include "rebuild_abi.h"

struct CVertexShaderConstantLayout;

struct CVertexShaderConstantLayoutVectorView
{
    CVertexShaderConstantLayout** begin00;
    CVertexShaderConstantLayout** end04;
    CVertexShaderConstantLayout** capacity08;

    fable_u32 size() const
    {
        return static_cast<fable_u32>(end04 - begin00);
    }

    void resize(
        fable_u32 count,
        CVertexShaderConstantLayout* const& value);
};

struct CShaderRenderManagerRegisterLayoutView
{
    fable_u8 unknown00[0x0C];
    CVertexShaderConstantLayoutVectorView constantLayouts0C;

    void RegisterVertexShaderConstantLayout(
        fable_i32 index,
        CVertexShaderConstantLayout* layout);
};

void CShaderRenderManagerRegisterLayoutView::
RegisterVertexShaderConstantLayout(
    fable_i32 index,
    CVertexShaderConstantLayout* layout)
{
    if (
        static_cast<fable_u32>(index) >=
        constantLayouts0C.size())
    {
        CVertexShaderConstantLayout* empty = 0;
        constantLayouts0C.resize(
            static_cast<fable_u32>(index + 1),
            empty);
    }
    constantLayouts0C.begin00[index] = layout;
}
