#include <cstdio>

struct CRGBFloatColour { float x, y, z, w; };

struct Device;
struct DeviceVtbl {
    char pad[0x178];
    void (__stdcall *SetConstant)(Device* self, long index, const float* data, long count);
};
struct Device { DeviceVtbl* vtbl; };
struct CShaderRenderManager { char pad[0x1d4]; Device* device; };

static long g_index;
static long g_count;
static float g_data[4];
static Device* g_self;

static void __stdcall RecordConstant(Device* self, long index, const float* data, long count) {
    g_self = self; g_index = index; g_count = count;
    for (int i = 0; i < 4; ++i) g_data[i] = data[i];
}

void __fastcall SetXBoxUserVertexShaderConstant(CShaderRenderManager* self, int, long index, CRGBFloatColour* colour) {
    float buf[4];
    buf[0] = colour->x; buf[1] = colour->y; buf[2] = colour->z; buf[3] = colour->w;
    Device* d = self->device;
    d->vtbl->SetConstant(d, index - 0x60, buf, 1);
}

int main() {
    DeviceVtbl vt; vt.SetConstant = RecordConstant;
    Device dev; dev.vtbl = &vt;
    CShaderRenderManager mgr; mgr.device = &dev;
    CRGBFloatColour c = { 1.5f, -2.0f, 3.25f, 4.0f };

    SetXBoxUserVertexShaderConstant(&mgr, 0, 0x70, &c);

    bool ok = g_self == &dev && g_index == (0x70 - 0x60) && g_count == 1 &&
              g_data[0] == 1.5f && g_data[1] == -2.0f && g_data[2] == 3.25f && g_data[3] == 4.0f;
    if (ok) printf("PARITY_OK_989ba0\n");
    else printf("FAIL idx=%ld cnt=%ld d=%f %f %f %f\n", g_index, g_count, g_data[0], g_data[1], g_data[2], g_data[3]);
    return 0;
}