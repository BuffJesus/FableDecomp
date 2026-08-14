// CEngineScreenEffectColourFilter::Render  0x00b5af60
struct CEngineComponentRenderParams;

struct CEngineScreenEffectColourFilter {
    char pad0[8];
    unsigned char enabled;   // +8
    char pad9[7];
    float amount;            // +0x10
};

// the global threshold constant compared against (0x129ba3c)
extern const float g_colourFilterThreshold;

// member helper at 0xb5a3f0 (this-call)
void __fastcall CEngineScreenEffectColourFilter_DoRender(CEngineScreenEffectColourFilter* self);

void __fastcall Render(CEngineScreenEffectColourFilter* self, int edx_dummy, CEngineComponentRenderParams const& params)
{
    (void)edx_dummy;
    if (self->enabled) {
        if (self->amount > g_colourFilterThreshold) {
            CEngineScreenEffectColourFilter_DoRender(self);
        }
    }
}