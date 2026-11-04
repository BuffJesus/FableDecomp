// Behaviour fixture for the mesh X chunks' descriptions (provider C3DMeshFileXChunkDescriptions_00aa7010.cpp; it
// defines none of them). Chunks are raw storage driven by qualified calls; the CCharString constructor is a double
// that records the text and the length argument.
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
template <class T>
static bool Says(const char* expected)
{
    g_text = 0; g_length = 0;
    {
        CCharString s = reinterpret_cast<const T*>(storage)->T::GetChunkDescription();
    }
    return g_text && strcmp(g_text, expected) == 0 && g_length == -1;
}

int main()
{
    CHECK(Says<C3DMeshFileXTextureIndexChunk>("Texture Index"));
    CHECK(Says<C3DMeshFileXMappingInfoChunk>("Mapping Info"));
    CHECK(Says<C3DMeshFileXAnimatedPrimitiveChunk>("Animated Primitive"));
    CHECK(Says<C3DMeshFileXBoundingSphereChunk>("Bounding Sphere"));
    CHECK(Says<C3DMeshFileXBoundingBoxChunk>("Bounding Box"));
    CHECK(Says<C3DMeshFileXVertexDataChunk>("Vertex Buffer Data"));
    CHECK(Says<C3DMeshFileXUVTangentChunk>("UV Tangent Vectors"));
    printf(g_fail ? "FAIL\n" : "MESH_X_CHUNK_DESCRIPTIONS_OK\n");
    return g_fail != 0;
}
