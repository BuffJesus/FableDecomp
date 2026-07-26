#include <cstdio>
struct GBlob { char pad[0xa0]; float thresh; };
struct GHolder { char pad[0xdc]; GBlob* blob; };
struct CTCHeroStats { char pad[0x19c]; float drunk; };
static GBlob g_blob;
static GHolder g_holder;
extern "C" GHolder* GetGlobal(void){ g_holder.blob = &g_blob; return &g_holder; }
bool __fastcall CTCHeroStats_IsDrunk(const CTCHeroStats* self)
{
    GHolder* h = GetGlobal();
    return self->drunk >= h->blob->thresh;
}
int main(){
    CTCHeroStats s; s.drunk = 5.0f;
    g_blob.thresh = 3.0f;
    if(!CTCHeroStats_IsDrunk(&s)){ std::printf("FAIL ge\n"); return 1; }
    g_blob.thresh = 9.0f;
    if(CTCHeroStats_IsDrunk(&s)){ std::printf("FAIL lt\n"); return 1; }
    g_blob.thresh = 5.0f;
    if(!CTCHeroStats_IsDrunk(&s)){ std::printf("FAIL eq\n"); return 1; }
    std::printf("CTCHeroStats_0057a874_TEST PASS\n");
    return 0;
}