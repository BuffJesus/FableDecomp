#include <cstdio>

struct CPixelShader { int a, b, c, d; };
struct CSubResource { void* found; };
struct CShaderResource { void* pad0; CSubResource* sub; };

static void* g_last_found;
static CPixelShader* g_last_ret;

// mirrors sub_9ccdc0: given the sub-resource + a name, yields a "found" pointer
static void* find_shader(CSubResource* that, void* name) {
    g_last_found = that->found;
    return that->found;
}

// mirrors sub_a5d510 (self->emit): constructs a CPixelShader into ret from found, returns ret
static CPixelShader* emit(CShaderResource* self, CPixelShader* ret, void* found) {
    g_last_ret = ret;
    ret->a = (int)(long)found;
    ret->b = (int)(long)self;
    ret->c = 0x77;
    ret->d = 0x88;
    return ret;
}

// behaviour model of GetPixelShader
static CPixelShader* GetPixelShader(CShaderResource* self, CPixelShader* ret, void* name) {
    void* found = find_shader(self->sub, name);
    emit(self, ret, found);
    return ret;
}

int main() {
    CSubResource sr; sr.found = (void*)0x1234;
    CShaderResource res; res.sub = &sr; res.pad0 = 0;
    CPixelShader out;
    CPixelShader* rp = GetPixelShader(&res, &out, (void*)0xABCD);

    bool ok = (rp == &out)                 // returns the caller-provided retptr
           && (g_last_ret == &out)         // emit wrote through that same retptr
           && (g_last_found == (void*)0x1234)
           && (out.a == 0x1234)            // found threaded into the shader
           && (out.b == (int)(long)&res)   // self threaded through
           && (out.c == 0x77) && (out.d == 0x88);

    printf(ok ? "PARITY_OK_a5d5f0\n" : "FAIL\n");
    return 0;
}