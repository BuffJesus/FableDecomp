struct CEngineRetailRenderFlags {
    unsigned char Reserved[0xb8];
    unsigned long SceneRenderingFlags;
    void Enable3DSceneRendering(bool enabled);
};
void CEngineRetailRenderFlags::Enable3DSceneRendering(bool enabled)
{
    SceneRenderingFlags &= ~1UL;
    if (enabled) SceneRenderingFlags |= 1UL;
}