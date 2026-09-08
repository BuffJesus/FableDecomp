#include "engine/CRenderManagerCore.h"

struct IDirect3DDevice9 {
    void** VTable;
    long SetFVF(unsigned long vertexFormat) {
        typedef long (__stdcall *SetFVFMethod)(IDirect3DDevice9*, unsigned long);
        SetFVFMethod method = reinterpret_cast<SetFVFMethod>(VTable[89]);
        return method(this, vertexFormat);
    }
};

struct CRenderManagerCore_SetVertexFormatMethods : CRenderManagerCore {
    void SetVertexFormat(unsigned long vertexFormat);
};

void CRenderManagerCore_SetVertexFormatMethods::SetVertexFormat(
    unsigned long vertexFormat) {
    PD3DDevice->SetFVF(vertexFormat);
}