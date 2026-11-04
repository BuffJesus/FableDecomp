// Verified native user-profile ownership provider.
#include "fable_user_profile.h"
#include "fable_boot.h"
void FABLE_FASTCALL FableDestroyUserProfileManager_0040d830(void* object);
void CCountedUserProfileManager::operator=(CUserProfileManager* value)
{
    if(reference){
        --static_cast<FableReferenceCount*>(reference)->owners;
        if(static_cast<FableReferenceCount*>(reference)->owners==0){
            static_cast<FableReferenceCount*>(reference)->destroy(static_cast<FableReferenceCount*>(reference)->object);
            FableOperatorDelete(reference);
        }
    }
    reference=0;
    object=value;
    if(value){
        FableReferenceCount* block=static_cast<FableReferenceCount*>(FableOperatorNew(sizeof(FableReferenceCount)));
        FableReferenceCount* result;
        if(block){block->owners=1;block->destroy=FableDestroyUserProfileManager_0040d830;block->object=object;result=block;}
        else result=0;
        reference=result;
    }
}
