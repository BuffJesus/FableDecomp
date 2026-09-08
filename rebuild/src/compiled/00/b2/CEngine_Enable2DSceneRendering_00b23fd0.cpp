struct CEngineRetailRenderFlags {
    unsigned char Reserved[0xb8];
    unsigned long SceneRenderingFlags;
    void Enable2DSceneRendering(bool enabled);
};
void CEngineRetailRenderFlags::Enable2DSceneRendering(bool enabled)
{
    SceneRenderingFlags &= ~2UL;
    if (enabled) SceneRenderingFlags |= 2UL;
}