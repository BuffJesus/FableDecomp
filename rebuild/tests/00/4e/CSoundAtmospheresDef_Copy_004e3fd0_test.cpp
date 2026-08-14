#include <cstdio>

struct CDefClassBase;

static int g_base_called = 0;
static int g_sub_called = 0;
static const void* g_base_src = 0;
static const void* g_sub_src = 0;

struct CSubMember {
    int v;
    void Copy(const CDefClassBase* src){ g_sub_called++; g_sub_src = (const void*)src; }
};

struct CBaseDef {
    void Copy(const CDefClassBase* src){ g_base_called++; g_base_src = (const void*)src; }
};

struct CSoundAtmospheresDef {
    char pad[0x28];
    CSubMember sub;
    void Copy(const CDefClassBase* src);
};

void CSoundAtmospheresDef::Copy(const CDefClassBase* src)
{
    ((CBaseDef*)this)->Copy(src);
    const CDefClassBase* srcSub = (const CDefClassBase*)((char*)src + 0x28);
    this->sub.Copy(srcSub);
}

int main(){
    CSoundAtmospheresDef dst; char srcbuf[0x40];
    dst.Copy((const CDefClassBase*)srcbuf);
    int ok = (g_base_called==1) && (g_sub_called==1)
          && (g_base_src == (const void*)srcbuf)
          && (g_sub_src == (const void*)(srcbuf + 0x28));
    if (ok) printf("COPY_OK\n");
    else printf("COPY_FAIL\n");
    return 0;
}