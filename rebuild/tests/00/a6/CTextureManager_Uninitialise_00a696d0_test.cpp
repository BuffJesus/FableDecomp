#include <stdio.h>
struct CTextureManagerRetail { void* v; bool Initialised; unsigned char state[0x5cb];
 void* TextureBuffer; void Uninitialise(); };
void CTextureManagerRetail::Uninitialise() { Initialised=false;
 if(TextureBuffer!=0) TextureBuffer=0; }
int main() { CTextureManagerRetail manager;
 manager.Initialised=true; manager.TextureBuffer=(void*)1; manager.Uninitialise();
 if(manager.Initialised || manager.TextureBuffer!=0) return 1;
 manager.Initialised=true; manager.TextureBuffer=0; manager.Uninitialise();
 if(manager.Initialised || manager.TextureBuffer!=0) return 2;
 printf("TEXTURE_MANAGER_UNINITIALISE PASS\n"); return 0; }