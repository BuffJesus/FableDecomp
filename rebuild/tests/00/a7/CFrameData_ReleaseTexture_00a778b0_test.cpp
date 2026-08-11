#include <cstdio>

struct CFrameData;

struct XResource {
    int released;
    CFrameData* lastArg;
    void Release(CFrameData* fd);
};

struct CFrameData {
    XResource* res0;   // +0
    XResource* res1;   // +4
    int refCount;      // +8
    void ReleaseTexture();
};

void XResource::Release(CFrameData* fd)
{
    this->released++;
    this->lastArg = fd;
}

void CFrameData::ReleaseTexture()
{
    if (--this->refCount == 0) {
        this->res0->Release(this);
        this->res1->Release(this);
    }
}

int main()
{
    XResource a; a.released = 0; a.lastArg = 0;
    XResource b; b.released = 0; b.lastArg = 0;
    CFrameData fd; fd.res0 = &a; fd.res1 = &b; fd.refCount = 2;

    fd.ReleaseTexture(); // 2 -> 1, no release
    if (a.released != 0 || b.released != 0) { printf("FAIL nrel\n"); return 1; }
    if (fd.refCount != 1) { printf("FAIL rc1\n"); return 1; }

    fd.ReleaseTexture(); // 1 -> 0, release both
    if (a.released != 1 || b.released != 1) { printf("FAIL rel\n"); return 1; }
    if (a.lastArg != &fd || b.lastArg != &fd) { printf("FAIL arg\n"); return 1; }
    if (fd.refCount != 0) { printf("FAIL rc0\n"); return 1; }

    printf("RELEASETEX_OK\n");
    return 0;
}