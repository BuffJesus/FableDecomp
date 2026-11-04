#pragma once
#include "fable_base_class.h"
#include "fable_d3d9_vertex_buffer.h"
enum EVertexType { VertexType_0x144 = 0x144 };
enum EVertexBufferPool { VertexBufferPool_1 = 1 };
// Implicit (virtual, via CBaseClassNonCopyable) destructor: retail ??_GCVertexBufferBase 0x009E2B20 calls
// ~CBaseClassNonCopyable directly, with no CVertexBufferBase vptr store.
class CVertexBufferBase : public CBaseClassNonCopyable {
public:
    unsigned char* Lock(unsigned long flags,unsigned long offset,unsigned long bytes);
    void Unlock();
    bool IsBusy() const;
    unsigned long Sizeof() const;
    IDirect3DVertexBuffer9* Buffer;
    long ByteSize;
    unsigned long VertexType;
    unsigned char VertexStride;
    unsigned char Padding11[3];
    float ScaleX,ScaleY,ScaleZ,ScaleW;
    unsigned long Extra24,Extra28,Extra2c,Extra30;
protected:
    CVertexBufferBase():Buffer(0),ByteSize(0),VertexType(0),VertexStride(0),ScaleX(1.0f),ScaleY(1.0f),ScaleZ(1.0f),ScaleW(1.0f),Extra24(0),Extra28(0),Extra2c(0),Extra30(0) {}
private:
    virtual unsigned char* DoLock(unsigned long flags,unsigned long offset,unsigned long bytes)=0;
    virtual void DoUnlock()=0;
    virtual bool DoIsBusy() const=0;
    virtual unsigned long DoSizeof() const=0;
};
class CVertexBufferWin32 : public CVertexBufferBase {
public:
    CVertexBufferWin32() {Initialise(VertexType_0x144,300,0x1c,8,VertexBufferPool_1);}
    virtual ~CVertexBufferWin32();
    bool Initialise(EVertexType vertexType,long count,long stride,unsigned long flags,EVertexBufferPool pool);
    void Uninitialise();
private:
    virtual unsigned char* DoLock(unsigned long flags,unsigned long offset,unsigned long bytes);
    virtual void DoUnlock();
    virtual bool DoIsBusy() const;
    virtual unsigned long DoSizeof() const;
};
FABLE_STATIC_ASSERT(sizeof(CVertexBufferBase)==0x34);
FABLE_STATIC_ASSERT(sizeof(CVertexBufferWin32)==0x34);
