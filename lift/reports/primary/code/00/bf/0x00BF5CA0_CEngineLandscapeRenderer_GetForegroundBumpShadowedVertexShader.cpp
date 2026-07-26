CVertexShader* __thiscall
CEngineLandscapeRenderer::GetForegroundBumpShadowedVertexShader(long index)
{
    struct ForegroundBumpShadowedVertexShadersOverlay
    {
        std::byte Padding[0x544];
        int* Value;
    };

    struct ArrayBoundsOverlay
    {
        int Begin;
        int End;
    };

    static_assert(offsetof(ForegroundBumpShadowedVertexShadersOverlay, Value) == 0x544);
    static_assert(offsetof(ArrayBoundsOverlay, Begin) == 0x0);
    static_assert(offsetof(ArrayBoundsOverlay, End) == 0x4);

    const auto* const shaders = reinterpret_cast<const ForegroundBumpShadowedVertexShadersOverlay*>(this)->Value;
    const auto* const bounds = reinterpret_cast<const ArrayBoundsOverlay*>(shaders);

    if ((((bounds->End - bounds->Begin) >> 3) - 1U) <= static_cast<unsigned int>(index))
    {
        index = ((bounds->End - bounds->Begin) >> 3) + -1;
    }

    return reinterpret_cast<CVertexShader*>(bounds->Begin + index * 8);
}