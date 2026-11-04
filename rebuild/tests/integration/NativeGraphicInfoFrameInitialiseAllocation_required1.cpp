#include <stdlib.h>
#include <new>
#include "fable_game_text_bank.h"
extern "C" __declspec(noinline) void RequiredAllocationProvider(CResourceBank* bank,CResource& resource){bank->AddResource(resource);}
extern "C" void UnusedAllocationBoundary(){abort();}
int main(int,char**argv){RequiredAllocationProvider(reinterpret_cast<CResourceBank*>(argv),*reinterpret_cast<CResource*>(argv+1));return 0;}
#pragma comment(linker,"/include:?AddResource@CResourceBank@@QAEXAAVCResource@@@Z")

