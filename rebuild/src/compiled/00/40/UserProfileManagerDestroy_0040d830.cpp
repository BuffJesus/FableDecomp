// Verified native user-profile ownership provider.
#include "fable_boot.h"
// ABI view of the object's virtual deleting destructor at slot zero.
class UserProfileManagerDeleteInterface{public:virtual ~UserProfileManagerDeleteInterface();};
void FABLE_FASTCALL FableDestroyUserProfileManager_0040d830(void* object)
{
    delete static_cast<UserProfileManagerDeleteInterface*>(object);
}

