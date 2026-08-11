struct CFrameData;

struct XResource {
    void Release(CFrameData* fd);
};

struct CFrameData {
    XResource* res0;   // +0
    XResource* res1;   // +4
    int refCount;      // +8
    void ReleaseTexture();
};

void CFrameData::ReleaseTexture()
{
    if (--this->refCount == 0) {
        this->res0->Release(this);
        this->res1->Release(this);
    }
}