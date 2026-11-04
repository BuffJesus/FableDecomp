#include "fable_engine_init.h"
CEngineInit::CEngineInit()
 :DefinitionManager(0),EngineDef(0),RenderManager(0),World(0),Font(0),
  MainGraphicBank(0),TerrainGraphicBank(0),MeshBank(0)
{
    MemoryInit.LandscapePoolSize=-1;
    MemoryInit.LandscapeVirtualMemoryMinSize=-1;
    MemoryInit.MeshPoolSize=-1;
    MemoryInit.MeshStatsPoolSize=-1;
    MemoryInit.HiresTextureMemory=-1;
    MemoryInit.PhysicsMeshPoolSize=-1;
    MemoryInit.AnimationPoolSize=-1;
    MemoryInit.ClothPoolSize=-1;
    MemoryInit.LandscapePhysicalMemoryRatio=-1.0f;
    MemoryInit.MeshPhysicalMemoryRatio=-1.0f;
    InitFlags=0;
    StaticMapQuality=0x7fffffff;
}
// Other retail callers inline this lifetime. Emit the shared out-of-line
// copy as a pick-any COMDAT, matching the string lifetime providers.
inline __declspec(noinline) CEngineInit::~CEngineInit() {}
namespace {
    void EmitDestructor(CEngineInit* init) { init->~CEngineInit(); }
}
