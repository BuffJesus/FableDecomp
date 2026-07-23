struct SrcMat { unsigned int m[12]; };
struct CShaderRenderManager {
    char pad[0x230];
    unsigned int dst[12];
    unsigned int f260, f264, f268, f26c;
    char pad2[0x3d8 - 0x270];
    unsigned int flags;
};
void __fastcall CShaderRenderManager_SetViewTransform(CShaderRenderManager* self, int edxpad, const SrcMat* s)
{
    self->dst[0] = s->m[0];
    self->dst[1] = s->m[3];
    self->dst[2] = s->m[6];
    self->dst[3] = s->m[9];
    self->dst[4] = s->m[1];
    self->dst[5] = s->m[4];
    self->dst[6] = s->m[7];
    self->dst[7] = s->m[10];
    self->dst[8] = s->m[2];
    self->dst[9] = s->m[5];
    self->dst[10] = s->m[8];
    self->dst[11] = s->m[11];
    self->f260 = 0;
    self->f264 = 0;
    self->f268 = 0;
    self->f26c = 0x3f800000u;
    unsigned int *p = &self->flags;
    *p |= 0xc990u;
}