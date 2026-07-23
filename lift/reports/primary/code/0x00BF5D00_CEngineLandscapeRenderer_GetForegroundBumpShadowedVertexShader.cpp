CVertexShader& __thiscall
CEngineLandscapeRenderer::GetForegroundBumpShadowedVertexShader(long index)
{
    struct CEngineLandscapeRendererOverlay
    {
        std::byte Padding[0x54C];
        void* ForegroundBumpShadowedVertexShaders;
    };

    struct CArrayOverlay
    {
        CVertexShader* Begin;
        CVertexShader* End;
    };

    static_assert(offsetof(CEngineLandscapeRendererOverlay, ForegroundBumpShadowedVertexShaders) == 0x54C);
    static_assert(offsetof(CArrayOverlay, Begin) == 0x0);
    static_assert(offsetof(CArrayOverlay, End) == 0x4);

    const auto& renderer = *reinterpret_cast<const CEngineLandscapeRendererOverlay*>(this);
    const auto& shaders =
        *reinterpret_cast<const CArrayOverlay*>(renderer.ForegroundBumpShadowedVertexShaders);

    if ((((reinterpret_cast<long>(shaders.End) - reinterpret_cast<long>(shaders.Begin)) >> 3) - 1U) <=
        static_cast<unsigned long>(index))
    {
        index = ((reinterpret_cast<long>(shaders.End) - reinterpret_cast<long>(shaders.Begin)) >> 3) + -1;
    }

    return *reinterpret_cast<CVertexShader*>(reinterpret_cast<long>(shaders.Begin) + index * 8);
}