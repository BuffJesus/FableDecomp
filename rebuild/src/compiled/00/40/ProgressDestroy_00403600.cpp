// Native boot progress-display ownership provider, validated against retail and shared lifetime composition.
#include "fable_boot.h"
// ABI view of the object's virtual deleting destructor at slot zero.
class ProgressDeleteInterface{public:virtual ~ProgressDeleteInterface();};
void FABLE_FASTCALL FableDestroyProgressDisplay_00403600(void* object)
{
    delete static_cast<ProgressDeleteInterface*>(object);
}
