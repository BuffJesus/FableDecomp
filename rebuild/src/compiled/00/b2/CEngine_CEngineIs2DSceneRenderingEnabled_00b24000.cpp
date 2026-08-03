class CEngine
{
public:
    char pad_0000[0xb8];
    unsigned int flags_0xb8;

    bool __fastcall Is2DSceneRenderingEnabled(void* /*edx*/);
};

bool __fastcall CEngine::Is2DSceneRenderingEnabled(void* /*edx*/)
{
    return (flags_0xb8 >> 1) & 1;
}