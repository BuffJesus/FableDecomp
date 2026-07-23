CVertexShader& CEngineLandscapeRenderer::GetForegroundSpotLightVertexShader(long index)
{
    auto& shaders = this->ForegroundSpotLightVertexShaders;

    if ((((reinterpret_cast<int>(shaders.end()) - reinterpret_cast<int>(shaders.begin())) >> 3) - 1U) <=
        static_cast<unsigned int>(index))
    {
        index = ((reinterpret_cast<int>(shaders.end()) - reinterpret_cast<int>(shaders.begin())) >> 3) + -1;
    }

    return *reinterpret_cast<CVertexShader*>(
        reinterpret_cast<int>(shaders.begin()) + index * 8);
}