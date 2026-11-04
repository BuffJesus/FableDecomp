#pragma once
// Original private callback names are unknown. Retail DrawBufferedPolys passes
// these cdecl functions to qsort for info records and three-vertex groups.
extern "C" int __cdecl FableCompareQuickDrawInfoDepth_009d8c30(const void*,const void*);
extern "C" int __cdecl FableCompareQuickDrawVerticesDepth_009d8cb0(const void*,const void*);
