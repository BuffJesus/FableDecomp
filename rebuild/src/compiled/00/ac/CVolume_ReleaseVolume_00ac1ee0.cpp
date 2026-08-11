struct IResource;
struct IResVtbl {
    void (__stdcall *v0)(IResource*);
    void (__stdcall *v1)(IResource*);
    void (__stdcall *Release)(IResource*); // slot 2 at offset 0x8
};
struct IResource {
    IResVtbl* vtbl;
};

struct CVolume {
    void* pad0;      // +0
    IResource* res;  // +4
    void ReleaseVolume();
};

void CVolume::ReleaseVolume()
{
    IResource* r = this->res;
    if (r) {
        r->vtbl->Release(r);
        this->res = 0;
    }
}