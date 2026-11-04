#include "fable_quick_draw_info.h"
#include "fable_render2d_vertex.h"
// Retail intentionally returns 1 for equal and unordered depths.
extern "C" int __cdecl FableCompareQuickDrawInfoDepth_009d8c30(const void* left,const void* right)
{
 const CQuickDrawTriInfo* a=(const CQuickDrawTriInfo*)left;
 const CQuickDrawTriInfo* b=(const CQuickDrawTriInfo*)right;
 if(a->ZAverage>b->ZAverage)return -1;
 return 1;
}
extern "C" int __cdecl FableCompareQuickDrawVerticesDepth_009d8cb0(const void* left,const void* right)
{
 const CTVertexRHWColSpecTex1* a=(const CTVertexRHWColSpecTex1*)left;
 const CTVertexRHWColSpecTex1* b=(const CTVertexRHWColSpecTex1*)right;
 if((a[2].Z+a[1].Z+a[0].Z)*0.33333334f > (b[2].Z+b[1].Z+b[0].Z)*0.33333334f)return -1;
 return 1;
}
