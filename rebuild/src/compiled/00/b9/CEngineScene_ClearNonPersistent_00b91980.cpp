#include "engine/CEngineScene.h"
#include "engine/CEngineInternalPrimitiveBase.h"
void CEngineScene::ClearNonPersistent()
{
    CEngineInternalPrimitiveBase* primitive=FirstNonPersistentPrimitive;
    while(primitive){
        CEngineInternalPrimitiveBase* next=primitive->NextPrimitive;
        primitive->NextPrimitive=0;
        primitive->RefPrimitive=0;
        primitive->NextLayerMask=0;
        primitive->RefLayerMask=0;
        primitive->Release();
        primitive=next;
    }
    FirstNonPersistentPrimitive=0;
}
