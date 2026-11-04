// Behaviour fixture for CIndexBuffer construction, Initialise and Lock (provider CIndexBuffer_Construction_00ac1ec0.cpp).
// GFGetSystemManager returns raw storage whose display manager (+0x60) holds a fake D3D device (+0x58; slot 27
// CreateIndexBuffer) and the render manager (+0x08; WaitWhileResourceIsBusy supplied here, recording). The index
// buffer is a fake COM object (slot 11 Lock, slot 13 GetDesc). Checked: the default constructor leaves no D3D buffer
// and no allocated block; the full constructor and Initialise pass length, usage, format and pool to CreateIndexBuffer
// (no shared handle) into PD3DIndexBuffer and keep the length and primitive type; Lock waits for the buffer (timeout 1)
// before locking, passes offset / size / flags through, and with size 0 locks the size from GetDesc; the (inline)
// destructor and the deleting destructor (through the vtable) release the buffer through Uninitialise (supplied here).
#include <stdio.h>
#include <string.h>
#include "fable_index_buffer.h"
#include "fable_d3d9_index_buffer.h"
#include "fable_system_manager.h"
#include "fable_display_viewport.h"

static int g_fail, g_creates, g_waits, g_locks, g_descs, g_step, g_waitStep, g_lockStep;
static UINT g_len; static DWORD g_usage, g_lockFlags; static D3DFORMAT g_format; static D3DPOOL g_pool; static HANDLE* g_shared;
static UINT g_lockOffset, g_lockSize; static const void* g_waitResource; static float g_waitTimeout;
static unsigned char systemStorage[0x64], displayStorage[0x5c], renderStorage[0x40];
static unsigned long deviceStorage[1], bufferStorage[1];
static void* deviceMethods[40]; static void* bufferMethods[16];
static unsigned char g_bytes[64];
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

CSystemManager* FABLE_FASTCALL GFGetSystemManager() { return reinterpret_cast<CSystemManager*>(systemStorage); }
void CRenderManagerCore::WaitWhileResourceIsBusy(IDirect3DResource9* resource, float timeout)
{
    ++g_waits; g_waitStep = ++g_step; g_waitResource = resource; g_waitTimeout = timeout;
}
static HRESULT __stdcall CreateIndexBuffer(IDirect3DDevice9*, UINT length, DWORD usage, D3DFORMAT format, D3DPOOL pool,
                                           IDirect3DIndexBuffer9** out, HANDLE* shared)
{
    ++g_creates; g_len = length; g_usage = usage; g_format = format; g_pool = pool; g_shared = shared;
    *out = reinterpret_cast<IDirect3DIndexBuffer9*>(bufferStorage); return 0;
}
static HRESULT __stdcall BufferLock(IDirect3DIndexBuffer9*, UINT offset, UINT size, void** data, DWORD flags)
{
    ++g_locks; g_lockStep = ++g_step; g_lockOffset = offset; g_lockSize = size; g_lockFlags = flags; *data = g_bytes + offset; return 0;
}
static HRESULT __stdcall BufferGetDesc(IDirect3DIndexBuffer9*, D3DINDEXBUFFER_DESC* desc) { ++g_descs; desc->Size = 48; return 0; }

// The class's vtable is emitted with the constructors; its other slots are not reached.
static int g_uninits; static const void* g_uninitThis;
void CIndexBuffer::Uninitialise() { ++g_uninits; g_uninitThis = this; }
void CIndexBuffer::RelocateData(long, void*, void*, long) {}
bool CMovableResource::CanRelocateData(long) const { return false; }
unsigned long CMovableResource::Verify() const { return 0; }

static unsigned long Word(const void* p, unsigned o) { unsigned long v; memcpy(&v, (const unsigned char*)p + o, 4); return v; }

int main()
{
    deviceMethods[27] = (void*)CreateIndexBuffer; deviceStorage[0] = (unsigned long)deviceMethods;
    bufferMethods[11] = (void*)BufferLock; bufferMethods[13] = (void*)BufferGetDesc; bufferStorage[0] = (unsigned long)bufferMethods;
    *reinterpret_cast<void**>(systemStorage + 0x60) = displayStorage;
    *reinterpret_cast<void**>(displayStorage + 0x58) = deviceStorage;
    *reinterpret_cast<void**>(displayStorage + 0x08) = renderStorage;

    {
        CIndexBuffer b;
        CHECK(Word(&b, 4) == 0 && Word(&b, 0x10) == 0 && g_creates == 0);
        b.Initialise(96, 8, IB_FORMAT_16, IB_POOL_MANAGED, (EPrimitiveType)4);
        CHECK(g_creates == 1 && g_len == 96 && g_usage == 8 && g_format == (D3DFORMAT)101 && g_pool == (D3DPOOL)1 && g_shared == 0);
        CHECK(Word(&b, 4) == (unsigned long)bufferStorage && Word(&b, 8) == 96 && Word(&b, 0xC) == 4);
        CHECK(g_uninits == 0);
    }
    CHECK(g_uninits == 1);
    {
        CIndexBuffer b(64, 0x208, IB_FORMAT_32, IB_POOL_DEFAULT, (EPrimitiveType)5);
        CHECK(g_creates == 2 && g_len == 64 && g_usage == 0x208 && g_format == (D3DFORMAT)102 && g_pool == (D3DPOOL)0);
        CHECK(Word(&b, 4) == (unsigned long)bufferStorage && Word(&b, 8) == 64 && Word(&b, 0xC) == 5 && Word(&b, 0x10) == 0);
        unsigned char* p = b.Lock(0x10, 4, 12);
        CHECK(p == g_bytes + 4 && g_locks == 1 && g_lockOffset == 4 && g_lockSize == 12 && g_lockFlags == 0x10 && g_descs == 0);
        CHECK(g_waits == 1 && g_waitResource == bufferStorage && g_waitTimeout == 1.0f && g_waitStep < g_lockStep);
        p = b.Lock(0, 0, 0);
        CHECK(p == g_bytes && g_descs == 1 && g_lockSize == 48 && g_lockOffset == 0 && g_waits == 2);
    }
    CHECK(g_uninits == 2);
    {
        CMovableResource* r = new CIndexBuffer;
        const void* self = r;
        delete r;
        CHECK(g_uninits == 3 && g_uninitThis == self);
    }
    if (g_fail) { printf("INDEX_BUFFER_CONSTRUCTION_FAIL %d\n", g_fail); return 1; }
    printf("INDEX_BUFFER_CONSTRUCTION_OK\n");
    return 0;
}
