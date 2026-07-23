struct CGPUScratchBufferTextureManager;
struct CEngine;
extern char* g_engineSingleton;

CGPUScratchBufferTextureManager* CEngine_GetScratchBufferTextureManager(CEngine* self)
{
    (void)self;
    return (CGPUScratchBufferTextureManager*)(g_engineSingleton + 0x3190);
}