#include <stdlib.h>
#include <new>
#include "fable_resource_list.h"
extern "C" __declspec(noinline) void RequiredAllocationProvider(void* storage){::new(storage) CResource;}
extern "C" void UnusedAllocationBoundary(){abort();}
int main(int,char**argv){RequiredAllocationProvider(argv);return 0;}
#pragma comment(linker,"/include:??0CResource@@QAE@XZ")
#pragma comment(linker,"/alternatename:??0CBaseClassNonCopyable@@QAE@XZ=_UnusedAllocationBoundary")
#pragma comment(linker,"/alternatename:??1CBaseClassNonCopyable@@UAE@XZ=_UnusedAllocationBoundary")
#pragma comment(linker,"/alternatename:??3@YAXPAX@Z=_UnusedAllocationBoundary")
#pragma comment(linker,"/alternatename:??_ECFailedAllocationHandler@@UAEPAXI@Z=_UnusedAllocationBoundary")
#pragma comment(linker,"/alternatename:??_ECIVCountedPointeeBase@@UAEPAXI@Z=_UnusedAllocationBoundary")
#pragma comment(linker,"/alternatename:??_ECResource@@UAEPAXI@Z=_UnusedAllocationBoundary")
#pragma comment(linker,"/alternatename:??_ECResourceBank@@UAEPAXI@Z=_UnusedAllocationBoundary")
#pragma comment(linker,"/alternatename:??_ECResourceList@@UAEPAXI@Z=_UnusedAllocationBoundary")
#pragma comment(linker,"/alternatename:?DeleteCIVCountedPointeeBase@CIVCountedPointeeBase@@UBEXXZ=_UnusedAllocationBoundary")
#pragma comment(linker,"/alternatename:?HandleFailedAllocation@CResourceList@@UAE_NK@Z=_UnusedAllocationBoundary")
#pragma comment(linker,"/alternatename:?RemoveResource@CResourceList@@QAEXAAVCResource@@@Z=_UnusedAllocationBoundary")

