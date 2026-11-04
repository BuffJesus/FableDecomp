#include "engine/CEngineSubPrimitiveRenderer.h"
#include "fable_stlport_algo.h"
#include "fable_stlport_tree.h"
void CEngineSubPrimitiveRenderer::SortList(CArray<CSortElement>& elements,bool stable)
{
 if(elements.empty())return;
 if(stable)_STL::__stable_sort(elements.begin(),elements.end(),(CSortElement*)0,(ptrdiff_t*)0,_STL::less<CSortElement>());
 else if(elements.size()<256)_STL::sort(elements.begin(),elements.end());
 else RadixSort(elements);
}
