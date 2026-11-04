#pragma once
#include <stddef.h>
#include "rebuild_abi.h"
#include "fable_stlport_list.h"
class CMovableResource;
class CTexture;
class CVertexBufferWin32;
class CIndexBuffer;
// Bounded retail prefix. EnablePoolAllocation at00BDA070 reads/writes39;
// the donor's81 offset belongs to its larger list implementation.
// FreeDisposalList at00BD9CE0 verifies the three list arrays. The complete
// allocation size and the first16bytes remain unreconciled here.
class CEngineLandscapeMemoryManager {
public:
    void FreeVirtual(void*);
    void SetVirtualBlockMovableResource(void*,CMovableResource*);
    void EnablePoolAllocation(bool);
    void FreeDisposalList();
    unsigned char UnknownPrefix[0x10];
    _STL::list<CTexture*> TextureDisposalList[3];
    _STL::list<CVertexBufferWin32*> VertexBufferDisposalList[3];
    _STL::list<CIndexBuffer*> IndexBufferDisposalList[3];
    unsigned char Unknown34[5];
    bool PoolAllocationEnabled;
};
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeMemoryManager,PoolAllocationEnabled)==0x39);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeMemoryManager,TextureDisposalList)==0x10);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeMemoryManager,VertexBufferDisposalList)==0x1c);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeMemoryManager,IndexBufferDisposalList)==0x28);
