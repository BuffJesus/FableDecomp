#include "fable_world.h"
#include "fable_thing_manager.h"
#include <stdio.h>
#include <string.h>

struct CWorldFrameAccess : public CWorld
{
    static void Set(long value) { Frame = value; }
    static long Get() { return Frame; }
};

#ifndef WORLD_FRAME_EXTERNAL_PROVIDER
long CWorld::Frame = 0;
#endif

static unsigned long worldStorage[(sizeof(CWorld) + 3) / 4];
static unsigned long managerStorage[16];
static const CThingManager* expectedManager;
static unsigned long checksumValue;
static long nextFrame;
static bool mutateFrame;
static int calls;
static int errors;

unsigned long CThingManager::GetNonLocalThingChecksum() const
{
    if (this != expectedManager) ++errors;
    ++calls;
    if (mutateFrame) CWorldFrameAccess::Set(nextFrame);
    return checksumValue;
}

int main()
{
    const unsigned long checksums[] = {0, 1, 0x80000000UL, 0xffffffffUL};
    const long frames[] = {0, 1, -1, 0x12345678L};
    CWorld* world = reinterpret_cast<CWorld*>(worldStorage);
    CThingManager* manager = reinterpret_cast<CThingManager*>(managerStorage);
    expectedManager = manager;
    memset(worldStorage, 0xa5, sizeof(worldStorage));
    memcpy(reinterpret_cast<unsigned char*>(worldStorage) + 0x50, &manager, 4);
    int cases = 0;
    for (unsigned int i = 0; i < 4; ++i)
    for (unsigned int j = 0; j < 4; ++j)
    for (int mutation = 0; mutation < 2; ++mutation)
    {
        checksumValue = checksums[i];
        CWorldFrameAccess::Set(frames[j]);
        nextFrame = static_cast<long>(frames[j] ^ 0x55aa55aaL);
        mutateFrame = mutation != 0;
        calls = 0;
        errors = 0;
        const long finalFrame = mutateFrame ? nextFrame : frames[j];
        const unsigned long result = world->GetChecksum();
        if (errors || calls != 1 || result != checksumValue + static_cast<unsigned long>(finalFrame) ||
            CWorldFrameAccess::Get() != finalFrame) return 1;
        ++cases;
    }
    CWorldFrameAccess::Set(0);
    printf("WORLD_CHECKSUM_PASS cases=%d\n", cases);
    return 0;
}
