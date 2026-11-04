#include <stdlib.h>
#include <new>
#include "fable_base_class.h"
extern "C" __declspec(noinline) void RequiredAllocationProvider(void* storage){::new(storage) CBaseClass;}
extern "C" void UnusedAllocationBoundary(){abort();}
int main(int,char**argv){RequiredAllocationProvider(argv);return 0;}
#pragma comment(linker,"/include:??0CBaseClass@@QAE@XZ")
#pragma comment(linker,"/alternatename:??3@YAXPAX@Z=_UnusedAllocationBoundary")
#pragma comment(linker,"/alternatename:??_ECBaseClass@@UAEPAXI@Z=_UnusedAllocationBoundary")
#pragma comment(linker,"/alternatename:??_ECBaseClassNonCopyable@@UAEPAXI@Z=_UnusedAllocationBoundary")

