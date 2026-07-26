int __fastcall CEngineInternalPrimitive2DSprite_GetRenderLayerMask(const unsigned char* self)
{
    return *reinterpret_cast<const int*>(self + 48);
}