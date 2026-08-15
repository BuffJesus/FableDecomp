#include <cstdio>

struct Inner {
    unsigned int div_;
    virtual int vf0() { return 0; }
    virtual int vf1() { return 0; }
    virtual unsigned int vf2() { return div_; }
};

struct Holder {
    void* p0;
    Inner* pInner;
};

struct CShaderRenderManager {
    int pad0, pad4, pad8, padC;
    unsigned int b;   // +0x10
    unsigned int a;   // +0x14
    unsigned int c;   // +0x18
    int pad1C;
    Holder* holder;   // +0x20
    unsigned int outResult;
};

// re-implementation of the target logic
void SetShadowFadeTransform_model(CShaderRenderManager* self)
{
    unsigned int sum = self->c + self->a + self->b;
    unsigned int divisor = self->holder->pInner->vf2();
    self->outResult = sum / divisor;
}

int main()
{
    Inner inner; inner.div_ = 7;
    Holder h; h.p0 = 0; h.pInner = &inner;
    CShaderRenderManager m;
    m.b = 10; m.a = 20; m.c = 40; m.holder = &h; m.outResult = 0;

    SetShadowFadeTransform_model(&m);
    // sum = 40 + 20 + 10 = 70; 70 / 7 = 10
    unsigned int expected = (40u + 20u + 10u) / 7u;
    if (m.outResult == expected && expected == 10u) {
        printf("PARITY_OK\n");
    } else {
        printf("FAIL got=%u exp=%u\n", m.outResult, expected);
    }
    return 0;
}