#ifndef FABLE_ENGINE_CGRAPHICFRAME_H
#define FABLE_ENGINE_CGRAPHICFRAME_H
// Canonical original 4B graphic frame and recovered method ABI.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CGraphicFrame = 0x4.
// Retail evidence: 1 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"
#include "fable_graphic_frame_data.h"

class CGraphicFrameData;

#pragma pack(push, 1)
class CGraphicFrame {
public:
    CGraphicFrame() : FrameData(0) {}
    __forceinline ~CGraphicFrame() {
        CGraphicFrameData* data=FrameData;
        if(data) {
            if(--data->ReferenceCount==0) {
                data->~CGraphicFrameData();
                operator delete(data);
            }
            FrameData=0;
        }
    }
    __forceinline const CGraphicFrame& operator=(const CGraphicFrame& other) {
        CGraphicFrameData* data=FrameData;
        if(data && --data->ReferenceCount==0) {
            data->~CGraphicFrameData();
            operator delete(data);
        }
        FrameData=other.FrameData;
        if(FrameData)++FrameData->ReferenceCount;
        return *this;
    }
#ifdef FABLE_GRAPHIC_FRAME_INLINE_UNINITIALISE
#ifdef FABLE_INLINE_PROVIDER_ANCHOR
    __declspec(noinline) inline void UnInitialise();
#else
    inline void UnInitialise();
#endif
#else
    void UnInitialise();
#endif
    bool InitialiseAsSurface(
        const CCountedPointer<CTexture>& texture,
        long width,
        long height,
        bool alpha,
        bool booleanAlpha);
    // Ego_r public ABI; retail 009FCB60 forwards to data initializer 009FCA40.
    bool InitialiseAsSurfaceSection(
        const CCountedPointer<CTexture>& texture,
        float x, float y, long width, long height,
        bool alpha, bool booleanAlpha);
    unsigned long Sizeof() const {return sizeof(*this)+(FrameData?FrameData->Sizeof():0);}
    CGraphicFrameData* FrameData;                            // +0x00
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CGraphicFrame) == 0x4);
FABLE_STATIC_ASSERT(offsetof(CGraphicFrame, FrameData) == 0x0);

#endif // FABLE_ENGINE_CGRAPHICFRAME_H
