#ifndef FABLETLC_LANDSCAPE_BACKGROUND_LOD_COMPILED_H
#define FABLETLC_LANDSCAPE_BACKGROUND_LOD_COMPILED_H
#include "engine/CEngineLandscapeMap.h"
// Legacy LOD fixture stream view; native stream callers include the canonical
// landscape header directly so this old view cannot override their stream ABI.
class CLandscapeGPUUpdateQueue {};
class CMemoryDataInputStream {
public:
    fable_u8 storage[0x18];
    explicit CMemoryDataInputStream(unsigned char* data);
    ~CMemoryDataInputStream();
};
FABLE_STATIC_ASSERT(sizeof(CMemoryDataInputStream)==0x18);
#endif
