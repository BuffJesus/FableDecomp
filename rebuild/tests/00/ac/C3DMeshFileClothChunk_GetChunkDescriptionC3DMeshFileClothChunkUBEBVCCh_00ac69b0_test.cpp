// Behaviour fixture for C3DMeshFileClothChunk::GetChunkDescription (provider
// C3DMeshFileClothChunk_GetChunkDescriptionC3DMeshFileClothChunkUBEBVCCh_00ac69b0.cpp; it does not define it). The chunk
// is raw storage driven by a qualified call; the CCharString constructor is a double that records the text and length.
#include "fable_chunked_file.h"
#include <stdio.h>
#include <string.h>

static int g_fail;
static const char* g_text;
static long g_length;
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

CCharString::CCharString(const char* text, long length) { memset(this, 0, sizeof(*this)); g_text = text; g_length = length; }
CCharString::CCharString(const CCharString&) { memset(this, 0, sizeof(*this)); }
CCharString::~CCharString() {}

static unsigned char storage[0x100];

int main()
{
    {
        CCharString s = reinterpret_cast<const C3DMeshFileClothChunk*>(storage)->C3DMeshFileClothChunk::GetChunkDescription();
    }
    CHECK(g_text && strcmp(g_text, "Cloth") == 0 && g_length == -1);
    printf(g_fail ? "FAIL\n" : "CHUNKDESC_OK\n");
    return g_fail != 0;
}
