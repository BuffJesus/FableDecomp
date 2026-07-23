class CVertexShader& __thiscall
CEngineLandscapeRenderer::GetForegroundBumpShadowedVertexShader(long index)
{
    struct ForegroundBumpShadowedVertexShadersOverlay
    {
        std::byte Padding[0x548];
        int* ArrayData;
    };

    static_assert(offsetof(ForegroundBumpShadowedVertexShadersOverlay, ArrayData) == 0x548);

    const auto& shaders =
        *reinterpret_cast<const ForegroundBumpShadowedVertexShadersOverlay*>(this);
    int* const arrayData = shaders.ArrayData;

    if ((((arrayData[1] - arrayData[0]) >> 3) - 1U) <= static_cast<unsigned long>(index))
    {
        index = ((arrayData[1] - arrayData[0]) >> 3) + -1;
    }

    return *reinterpret_cast<CVertexShader*>(arrayData[0] + index * 8);
}