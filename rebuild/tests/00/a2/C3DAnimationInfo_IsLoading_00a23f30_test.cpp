// Behaviour fixture for C3DAnimationInfo::IsLoading 0x00A23F30, linked against the provider object
// (it does not define IsLoading itself). The objects are built in raw storage at the retail
// offsets the shared header pins: C3DAnimationInfo::Data +0x0C, CAnimationData::FileData +0x30.
#include "engine/C3DAnimationInfo.h"
#include <stdio.h>
#include <string.h>

static unsigned char infoStorage[sizeof(C3DAnimationInfo)];
static unsigned char dataStorage[sizeof(C3DAnimationInfo::CAnimationData)];

static void setPointer(unsigned char* storage, size_t offset, void* value)
{
    memcpy(storage + offset, &value, sizeof(value));
}

int main()
{
    const C3DAnimationInfo* info = reinterpret_cast<const C3DAnimationInfo*>(infoStorage);
    int cases = 0;
    memset(infoStorage, 0, sizeof(infoStorage));
    memset(dataStorage, 0, sizeof(dataStorage));
    if (info->IsLoading()) return 1;
    ++cases;
    setPointer(infoStorage, 0x0c, dataStorage);
    if (info->IsLoading()) return 2;
    ++cases;
    unsigned char fileData[0x10];
    setPointer(dataStorage, 0x30, fileData);
    if (!info->IsLoading()) return 3;
    ++cases;
    setPointer(infoStorage, 0x0c, 0);
    if (info->IsLoading()) return 4;
    ++cases;
    printf("ISLOADING_OK cases=%d\n", cases);
    return 0;
}
