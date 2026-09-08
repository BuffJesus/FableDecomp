#include "engine/C3DMeshFileSubMeshChunk.h"
#include <cstdio>
#include <cstring>

struct CCharString {
    char* Data;
    int Length;
    CCharString(const char* text, int length);
};
struct C3DMeshFileSubMeshChunk_Methods : C3DMeshFileSubMeshChunk {
    CCharString GetChunkDescription() const;
};

CCharString::CCharString(const char* text, int length)
{
    Data = const_cast<char*>(text);
    Length = length < 0 ? (int)std::strlen(text) : length;
}

CCharString C3DMeshFileSubMeshChunk_Methods::GetChunkDescription() const
{
    return CCharString("SubMesh", -1);
}

int main()
{
    C3DMeshFileSubMeshChunk_Methods chunk;
    CCharString result = chunk.GetChunkDescription();
    if (result.Data && std::strcmp(result.Data, "SubMesh") == 0 && result.Length == 7) {
        std::printf("CHUNKDESC_OK\n");
        return 0;
    }
    return 1;
}
