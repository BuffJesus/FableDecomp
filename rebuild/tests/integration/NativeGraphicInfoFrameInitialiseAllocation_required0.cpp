#include <stdlib.h>
#include <new>
#include "fable_graphic_info.h"
extern "C" __declspec(noinline) void RequiredAllocationProvider(void* storage,const CGraphicInfoFrame* frame){::new(storage) CGraphicInfoFrame::CFrameData(frame);}
extern "C" void UnusedAllocationBoundary(){abort();}
int main(int,char**argv){RequiredAllocationProvider(argv,reinterpret_cast<const CGraphicInfoFrame*>(argv+1));return 0;}
#pragma comment(linker,"/include:??0CFrameData@CGraphicInfoFrame@@QAE@PBV1@@Z")
#pragma comment(linker,"/alternatename:??0CBaseClass@@QAE@XZ=_UnusedAllocationBoundary")
#pragma comment(linker,"/alternatename:??0CResource@@QAE@XZ=_UnusedAllocationBoundary")
#pragma comment(linker,"/alternatename:??1CFrameData@CGraphicInfoFrame@@UAE@XZ=_UnusedAllocationBoundary")
#pragma comment(linker,"/alternatename:??1CManagedTexture@@UAE@XZ=_UnusedAllocationBoundary")
#pragma comment(linker,"/alternatename:??3@YAXPAX@Z=_UnusedAllocationBoundary")
#pragma comment(linker,"/alternatename:??_ECFrameData@CGraphicInfoFrame@@UAEPAXI@Z=_UnusedAllocationBoundary")
#pragma comment(linker,"/alternatename:??_ECIVCountedPointeeBase@@UAEPAXI@Z=_UnusedAllocationBoundary")
#pragma comment(linker,"/alternatename:??_ECManagedTexture@@UAEPAXI@Z=_UnusedAllocationBoundary")
#pragma comment(linker,"/alternatename:??_ECManagedTextureOwner@@UAEPAXI@Z=_UnusedAllocationBoundary")
#pragma comment(linker,"/alternatename:??_ECResource@@UAEPAXI@Z=_UnusedAllocationBoundary")
#pragma comment(linker,"/alternatename:?AddResource@CResourceBank@@QAEXAAVCResource@@@Z=_UnusedAllocationBoundary")
#pragma comment(linker,"/alternatename:?DeleteCIVCountedPointeeBase@CIVCountedPointeeBase@@UBEXXZ=_UnusedAllocationBoundary")
#pragma comment(linker,"/alternatename:?GetDebugMemoryInUse@CFrameData@CGraphicInfoFrame@@UBEKXZ=_UnusedAllocationBoundary")
#pragma comment(linker,"/alternatename:?GetUnloadableMemory@CFrameData@CGraphicInfoFrame@@UBEKXZ=_UnusedAllocationBoundary")
#pragma comment(linker,"/alternatename:?IsResourceLoaded@CFrameData@CGraphicInfoFrame@@UBE_NXZ=_UnusedAllocationBoundary")
#pragma comment(linker,"/alternatename:?OnResourceLoaded@CFrameData@CGraphicInfoFrame@@UAEXXZ=_UnusedAllocationBoundary")
#pragma comment(linker,"/alternatename:?ReleaseTexture@CFrameData@CGraphicInfoFrame@@UAEXXZ=_UnusedAllocationBoundary")
#pragma comment(linker,"/alternatename:?RemoveResource@CResourceList@@QAEXAAVCResource@@@Z=_UnusedAllocationBoundary")
#pragma comment(linker,"/alternatename:?ResetDebugMemoryInUse@CFrameData@CGraphicInfoFrame@@UAEXXZ=_UnusedAllocationBoundary")
#pragma comment(linker,"/alternatename:?SetResourceSize@CResource@@QAEXK@Z=_UnusedAllocationBoundary")
#pragma comment(linker,"/alternatename:?Sizeof@CGraphicFrameData@@QBEKXZ=_UnusedAllocationBoundary")
#pragma comment(linker,"/alternatename:?Sizeof@CManagedTexture@@QBEKXZ=_UnusedAllocationBoundary")
#pragma comment(linker,"/alternatename:?Uninitialise@CGraphicFrameData@@QAEXXZ=_UnusedAllocationBoundary")

