#include "fable_engine.h"
inline CEngine::CPreloadStats::CPreloadStats()
 :LastFullPreloadTime(0),LastFullPreloadTimedOut(false),
  LastFastPreloadTime(0),LastFastPreloadTimedOut(false) {}
inline CEngineVideoOptions::CEngineVideoOptions()
 :ScreenResolution(0,0),ScreenAspectRatio(4,3),VerticalSync(true),
  LandscapeDetail(ENGINE_VIDEO_OPTION_QUALITY_NORMAL),MeshDetail(ENGINE_VIDEO_OPTION_QUALITY_NORMAL),
  TextureDetail(ENGINE_VIDEO_OPTION_QUALITY_NORMAL),ShadowQuality(ENGINE_VIDEO_OPTION_QUALITY_NORMAL),
  EffectsQuality(ENGINE_VIDEO_OPTION_QUALITY_NORMAL)
{BitsPerPixel=0;RefreshRate=0;Antialiasing=SURFACE_MULTISAMPLE_NONE;}
CEngine::CEngine() :CurrentCamera(0),FrameMemoryPool(0,0x1000)
{IsInitialised=false;}
