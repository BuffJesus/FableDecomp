#include "fable_display_engine.h"
CDisplayEngine::CDisplayEngine(const CDisplayEngineInit& init)
 :EnginePreloadFlags(1),Component(init.Component),PlayerManager(init.PlayerManager),
  DefinitionManager(const_cast<CGameDefinitionManager*>(init.DefinitionManager)),
  EngineGraphicBank(init.MainGraphicBank),MeshBank(init.MeshBank),Engine3D(0),
  Camera(C3DVector(0,0,4),C3DVector(1,0,0),0.25f,false),DrawGame(true),
  ScreenFadeOutLocked(false),DrawMemoryUse(false),DrawDebugPage(0),
  InitialFadeReadynessCount(30),InitialFadeActive(false),
  ScreenFadeDuration(-1),TimeSinceFadeStarted(0),World(init.World)
{
    OldGammaRamp=0;PreparePrimitives=true;GammaRamp=1;
}
