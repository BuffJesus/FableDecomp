#include <stdio.h>

struct CCharString {
    char* m_data;
};

static const char* g_last_str = 0;
static int g_last_len = 0;

// Local stand-in for the real ctor at 0x0099ebf0.
CCharString* __fastcall CCharString_ctor(CCharString* self, int /*edx*/, const char* str, int len)
{
    g_last_str = str;
    g_last_len = len;
    self->m_data = (char*)str;
    return self;
}

struct CMirroredTextureHighlightsChunk {
    CCharString GetChunkDescription();
};

CCharString CMirroredTextureHighlightsChunk::GetChunkDescription()
{
    CCharString ret;
    CCharString_ctor(&ret, 0, "MirroredTextureHighlights", -1);
    return ret;
}

int main()
{
    CMirroredTextureHighlightsChunk obj;
    CCharString s = obj.GetChunkDescription();

    int ok = 1;
    if (g_last_len != -1) ok = 0;
    if (g_last_str == 0) ok = 0;
    if (s.m_data != g_last_str) ok = 0;

    if (ok) printf("MIRRTEXHIGHLIGHTS_GETDESC_OK\n");
    else    printf("FAIL str=%p len=%d\n", (void*)s.m_data, g_last_len);
    return ok ? 0 : 1;
}