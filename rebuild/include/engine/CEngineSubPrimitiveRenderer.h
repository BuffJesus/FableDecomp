#ifndef FABLE_ENGINE_CENGINESUBPRIMITIVERENDERER_H
#define FABLE_ENGINE_CENGINESUBPRIMITIVERENDERER_H
// Original nested types from Ego_r.pdb; retail lists use 12-byte arrays
// and 24-byte entries (donor arrays are 16 bytes). Retail owner is 0x190.
#include <stddef.h>
#include "rebuild_abi.h"
#include "fable_carray.h"
class CEngineSubPrimitiveBase;
enum ERenderPassType;

enum ESubPrimitiveType {
    SUBPRIM_TYPE_OPAQUE,SUBPRIM_TYPE_ALPHA,SUBPRIM_TYPE_2D,SUBPRIM_TYPE_2D_VIEWPORT,
    SUBPRIM_TYPE_GLOW_BUFFER,SUBPRIM_TYPE_DISPLACEMENT_BUFFER,SUBPRIM_TYPE_DISPLACEMENT_STENCIL,
    SUBPRIM_TYPE_BOUNDING_VOLUME,SUBPRIM_TYPE_OUTLINE_STENCIL_SMOOTH,
    SUBPRIM_TYPE_OUTLINE_ALPHA_STENCIL_MASK_SMOOTH,SUBPRIM_TYPE_OUTLINE_STENCIL,
    SUBPRIM_TYPE_STENCIL_MESHES,SUBPRIM_TYPE_POSTWATER_MESHES,
    SUBPRIM_TYPE_OUTLINE_SMOOTH_FILL,SUBPRIM_TYPE_OUTLINE_FILL,SUBPRIM_TYPE_COUNT
};
#pragma pack(push, 1)
class CEngineSubPrimitiveRenderer {
public:
    class CSortElement { public: CSortElement() {} bool operator<(const CSortElement& other) const { return SortValue<other.SortValue; } unsigned long SortValue; CEngineSubPrimitiveBase* SubPrimitive; };
    class CSubPrimitiveList {
    public:
        CArray<CSortElement> Elements;
        ERenderPassType RenderPass,PreRenderPass;
        bool BoundingVolumePass,RequiresStableSort,Sorted,Enable;
    };
private:
    void DeleteList(CArray<CSortElement>&);
    void SortList(CArray<CSortElement>&,bool);
    void RadixSort(CArray<CSortElement>&);
    void SortSingleRadix(const CSortElement*,CSortElement*,unsigned long,unsigned long);
    void RenderList(const CArray<CSortElement>&,ERenderPassType);
    void RenderListTwoPass(const CArray<CSortElement>&,ERenderPassType,ERenderPassType);
public:
    void RenderType(ESubPrimitiveType,bool);
    void ClearType(ESubPrimitiveType);
    unsigned long GetCount(ESubPrimitiveType) const;
    void*         __vftable;                                 // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char _pad_0x04[0x4];                            // +0x04
    unsigned char _pad_0x08[0x4];                            // +0x08
    unsigned char _pad_0x0c[0x4];                            // +0x0c
    float         MaxSortDistance;                           // +0x10
    float         SortDistanceMultiplier;                    // +0x14
    CSubPrimitiveList SubPrimitiveLists[15];                  // +0x18 CSubPrimitiveList[15]
    CArray<CSortElement> SortBuffer;                          // +0x180 CArray<CEngineSubPrimitiveRenderer::CSortElement>
    bool          EnableSolidPass;                           // +0x18c
    bool          EnableAlphaPass;                           // +0x18d
    unsigned char _pad_0x18e[2];                           // +0x18e
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CEngineSubPrimitiveRenderer) == 0x190);
FABLE_STATIC_ASSERT(offsetof(CEngineSubPrimitiveRenderer, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CEngineSubPrimitiveRenderer, MaxSortDistance) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CEngineSubPrimitiveRenderer, SortDistanceMultiplier) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CEngineSubPrimitiveRenderer, SubPrimitiveLists) == 0x18);
FABLE_STATIC_ASSERT(offsetof(CEngineSubPrimitiveRenderer, SortBuffer) == 0x180);
FABLE_STATIC_ASSERT(offsetof(CEngineSubPrimitiveRenderer, EnableSolidPass) == 0x18c);
FABLE_STATIC_ASSERT(offsetof(CEngineSubPrimitiveRenderer, EnableAlphaPass) == 0x18d);


FABLE_STATIC_ASSERT(sizeof(CEngineSubPrimitiveRenderer::CSubPrimitiveList)==24);
FABLE_STATIC_ASSERT(sizeof(CEngineSubPrimitiveRenderer::CSortElement)==8);

// STLport class-element erase uses assignment, not the POD memmove path.
namespace _STL {
#ifdef FABLE_SUBPRIMITIVE_ERASE_DECLARATION_ONLY
template<> void vector<CEngineSubPrimitiveRenderer::CSortElement>::_M_fill_insert(CEngineSubPrimitiveRenderer::CSortElement*,size_t,const CEngineSubPrimitiveRenderer::CSortElement&);
#endif
template<> CEngineSubPrimitiveRenderer::CSortElement* vector<CEngineSubPrimitiveRenderer::CSortElement>::erase(CEngineSubPrimitiveRenderer::CSortElement* first,CEngineSubPrimitiveRenderer::CSortElement* last);
#ifndef FABLE_SUBPRIMITIVE_ERASE_DECLARATION_ONLY
template<> inline CEngineSubPrimitiveRenderer::CSortElement* vector<CEngineSubPrimitiveRenderer::CSortElement>::erase(CEngineSubPrimitiveRenderer::CSortElement* first,CEngineSubPrimitiveRenderer::CSortElement* last)
{
 CEngineSubPrimitiveRenderer::CSortElement* out=first;
 for(ptrdiff_t n=this->_M_finish-last;n>0;--n)*out++=*last++;
 this->_M_finish=out;
 return first;
}
#endif
}

#endif // FABLE_ENGINE_CENGINESUBPRIMITIVERENDERER_H
