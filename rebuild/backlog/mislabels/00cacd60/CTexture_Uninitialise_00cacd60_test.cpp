#include <stdio.h>
static int g_releases;
struct CTextureResource;
struct CTextureResourceVTable { void* reserved0; void* reserved4; void (__stdcall *Release)(CTextureResource*); };
struct CTextureResource { CTextureResourceVTable* vtable; };
struct CTexture { CTextureResource* resource; void Uninitialise(); };
static void __stdcall MockRelease(CTextureResource*) { ++g_releases; }
void CTexture::Uninitialise() { if (resource != 0) resource->vtable->Release(resource); }
int main() {
    CTextureResourceVTable table = {0, 0, MockRelease}; CTextureResource resource = {&table}; CTexture texture;
    texture.resource = 0; texture.Uninitialise(); if (g_releases != 0) return 1;
    texture.resource = &resource; texture.Uninitialise(); if (g_releases != 1) return 1;
    printf("TEXTURE_UNINITIALISE PASS\n"); return 0;
}