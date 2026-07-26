class CVertexShader& __thiscall
CEngineLandscapeRenderer::GetForegroundSpotLightVertexShader(long index)
{
    struct ForegroundSpotLightVertexShadersOverlay
    {
        std::byte Padding[0x4E8];
        CVertexShader* Begin;
        CVertexShader* End;
    };

    static_assert(offsetof(ForegroundSpotLightVertexShadersOverlay, Begin) == 0x4E8);
    static_assert(offsetof(ForegroundSpotLightVertexShadersOverlay, End) == 0x4EC);

    const auto& shaders =
        *reinterpret_cast<const ForegroundSpotLightVertexShadersOverlay*>(this);

    if ((((reinterpret_cast<long>(shaders.End) - reinterpret_cast<long>(shaders.Begin)) >> 3) - 1U) <=
        static_cast<unsigned long>(index))
    {
        index = ((reinterpret_cast<long>(shaders.End) - reinterpret_cast<long>(shaders.Begin)) >> 3) + -1;
    }

    return *reinterpret_cast<CVertexShader*>(reinterpret_cast<long>(shaders.Begin) + index * 8);
}