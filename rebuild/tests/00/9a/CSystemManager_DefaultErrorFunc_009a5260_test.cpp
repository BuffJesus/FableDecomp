#include "fable_system_manager_state.h"
#include <stdio.h>
struct CSystemManagerTestAccess {
    static bool Run(ESystemCode code) { return CSystemManager::DefaultErrorFunc(code); }
};
int main() {
    const int codes[] = {0,1,2,3,4,5,-1,0x7fffffff,(-2147483647-1)};
    for(unsigned i=0;i<sizeof(codes)/sizeof(codes[0]);++i)
        if(CSystemManagerTestAccess::Run(static_cast<ESystemCode>(codes[i]))) return 1;
    puts("DEFAULT_ERROR_PASS scenarios=9");return 0;
}
