// CShaderRenderManager::SetShadowFadeTransform 0x00c26520
struct Inner {
    virtual int vf0();
    virtual int vf1();
    virtual unsigned int vf2();  // slot 2 -> call [edx+8]
};

struct Holder {
    void* p0;
    Inner* pInner;   // +4
};

struct CShaderRenderManager {
    int pad0;        // +0x00
    int pad4;        // +0x04
    int pad8;        // +0x08
    int padC;        // +0x0C
    unsigned int b;  // +0x10
    unsigned int a;  // +0x14
    unsigned int c;  // +0x18
    int pad1C;       // +0x1C
    Holder* holder;  // +0x20
};

unsigned int __fastcall SetShadowFadeTransform(CShaderRenderManager* self)
{
    unsigned int sum = self->c + self->a + self->b;
    unsigned int divisor = self->holder->pInner->vf2();
    return sum / divisor;
}