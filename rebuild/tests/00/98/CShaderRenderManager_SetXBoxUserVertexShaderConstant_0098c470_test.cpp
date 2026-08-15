#include <cstdio>

struct CRGBFloatColour { float x, y, z; };

struct IDevice;
typedef void (__stdcall *SetConstFn)(IDevice* self, int reg, void* pData, int count);
struct IDeviceVtbl { void* slots[0x5f]; };
struct IDevice { IDeviceVtbl* vtbl; };

struct CShaderRenderManager {
    char pad[0x1d4];
    IDevice* device;
    void SetXBoxUserVertexShaderConstant(int reg, CRGBFloatColour* colour, int packedIdx);
};

static int   g_reg=-1, g_count=-1;
static float g_data[4]={0,0,0,0};
static IDevice* g_self=0;

static void __stdcall CaptureSet(IDevice* self, int reg, void* pData, int count){
    g_self=self; g_reg=reg; g_count=count;
    float* f=(float*)pData;
    g_data[0]=f[0]; g_data[1]=f[1]; g_data[2]=f[2];
}

void CShaderRenderManager::SetXBoxUserVertexShaderConstant(
    int reg, CRGBFloatColour* colour, int packedIdx)
{
    float local[4];
    void* pdup=&packedIdx;
    local[0]=colour->x; local[1]=colour->y; local[2]=colour->z;
    local[3]=*(float*)pdup;
    IDevice* dev=this->device;
    SetConstFn fn=(SetConstFn)dev->vtbl->slots[0x178/4];
    fn(dev, reg, &local[0], 1);
}

int main(){
    void* vt[0x5f]={0};
    vt[0x178/4]=(void*)&CaptureSet;
    IDeviceVtbl vtbl; for(int i=0;i<0x5f;i++) vtbl.slots[i]=vt[i];
    IDevice dev; dev.vtbl=&vtbl;
    CShaderRenderManager mgr; mgr.device=&dev;
    CRGBFloatColour c; c.x=1.5f; c.y=2.5f; c.z=3.5f;
    mgr.SetXBoxUserVertexShaderConstant(42, &c, 42);
    if(g_self==&dev && g_reg==42 && g_count==1 &&
       g_data[0]==1.5f && g_data[1]==2.5f && g_data[2]==3.5f)
        printf("PARITY_OK\n");
    else
        printf("FAIL r=%d c=%d d=%f,%f,%f\n",g_reg,g_count,g_data[0],g_data[1],g_data[2]);
    return 0;
}