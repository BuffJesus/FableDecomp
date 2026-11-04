#include "fable_engine.h"
#include "fable_engine_internal.h"
#include "engine/CEngineWorldMap.h"
void CEngine::CloseStaticMapFile(){NEngineInternal::GPEngineWorldMap->CloseStaticMapFile();}
void CEngine::SetStaticMapFileForUse(const CWideString& path){NEngineInternal::GPEngineWorldMap->SetStaticMapFileForUse(path);}
void CEngine::SetWorld(const CIDrawWorld* world){NEngineInternal::GPEngineWorldMap->SetWorld(world);}
void CEngine::AddMap(long x,long y){NEngineInternal::GPEngineWorldMap->AddMap(x,y);}
void CEngine::RemoveMap(long id){NEngineInternal::GPEngineWorldMap->DeleteMap(id);}
void CEngine::SetMapAsLoaded(long id){NEngineInternal::GPEngineWorldMap->OnGameMapLoaded(id);}
void CEngine::SetMapAsUnloaded(long id){NEngineInternal::GPEngineWorldMap->OnGameMapUnloaded(id);}
void CEngine::SetMapVisibility(long id,bool visible){NEngineInternal::GPEngineWorldMap->SetMapVisibility(id,visible);}
void CEngine::SetMapVisibilityAllMaps(bool visible){NEngineInternal::GPEngineWorldMap->SetMapVisibilityAllMaps(visible);}
