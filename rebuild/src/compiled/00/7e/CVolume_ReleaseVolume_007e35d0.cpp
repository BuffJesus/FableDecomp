// CVolume::ReleaseVolume  0x007e35d0
struct IVolumeManager {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void ReleaseHandle(int handle); // vtable slot +0x14
};

// global singleton pointer at 0x13b8394
IVolumeManager** const g_volumeManager = (IVolumeManager**)0x013b8394;

struct CVolume {
    int f0;
    int f4;
    int f8;
    int handle; // +0xc
};

void __fastcall ReleaseVolume(CVolume* self)
{
    int h = self->handle;
    if (h != 0) {
        (*g_volumeManager)->ReleaseHandle(h);
        self->handle = 0;
    }
}