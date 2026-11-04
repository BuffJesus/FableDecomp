#include "fable_world.h"
#include "fable_thing_manager.h"
#include <stdio.h>
#include <string.h>

struct ThingNode
{
    ThingNode* Next;
    ThingNode* Previous;
    CThing* Thing;
};

static unsigned long managerStorage[(sizeof(CThingManager) + 3) / 4];
static unsigned long thingStorage[4][0xa0 / 4];
static void* vtable[24];
static ThingNode headA, headB, nodes[3];
static CThing* things[4];
static unsigned long values[4];
static bool mutate;
static int calls, errors;

unsigned long __fastcall ChecksumCallback(const void* self)
{
    int index = -1;
    for (int i = 0; i < 4; ++i)
        if (self == things[i]) index = i;
    if (index < 0) { ++errors; return 0; }
    ++calls;
    if (mutate)
    {
        mutate = false;
        nodes[index].Next = &headB;
        headB.Next = &headA;
        headB.Thing = things[3];
        ThingNode* newHead = &headB;
        memcpy(reinterpret_cast<unsigned char*>(managerStorage) + 0x18, &newHead, 4);
    }
    return values[index];
}

static CThingManager* Prepare(int length, int deadMask, int valuePattern)
{
    memset(managerStorage, 0xa5, sizeof(managerStorage));
    memset(nodes, 0, sizeof(nodes));
    memset(&headA, 0, sizeof(headA));
    memset(&headB, 0, sizeof(headB));
    memset(vtable, 0, sizeof(vtable));
    vtable[23] = reinterpret_cast<void*>(&ChecksumCallback);
    for (int i = 0; i < 4; ++i)
    {
        memset(thingStorage[i], 0xa5, sizeof(thingStorage[i]));
        things[i] = reinterpret_cast<CThing*>(thingStorage[i]);
        void** table = vtable;
        memcpy(thingStorage[i], &table, 4);
        reinterpret_cast<unsigned char*>(thingStorage[i])[0x91] =
            static_cast<unsigned char>((deadMask >> i) & 1);
        values[i] = valuePattern == 0 ? static_cast<unsigned long>(i + 1) :
                    valuePattern == 1 ? 0xffffffffUL - static_cast<unsigned long>(i) :
                    0x80000000UL + static_cast<unsigned long>(i * 17);
    }
    headA.Next = length ? &nodes[0] : &headA;
    for (int i = 0; i < length; ++i)
    {
        nodes[i].Next = i + 1 < length ? &nodes[i + 1] : &headA;
        nodes[i].Thing = things[i];
    }
    ThingNode* firstHead = &headA;
    memcpy(reinterpret_cast<unsigned char*>(managerStorage) + 0x18, &firstHead, 4);
    calls = errors = 0;
    mutate = false;
    return reinterpret_cast<CThingManager*>(managerStorage);
}

int main()
{
    int cases = 0;
    const int lengths[] = {0, 1, 3};
    for (int li = 0; li < 3; ++li)
    for (int deadMask = 0; deadMask < 8; ++deadMask)
    for (int pattern = 0; pattern < 3; ++pattern)
    {
        const int length = lengths[li];
        CThingManager* manager = Prepare(length, deadMask, pattern);
        unsigned long expected = 0;
        int expectedCalls = 0;
        for (int i = 0; i < length; ++i)
            if (!(deadMask & (1 << i))) { expected += values[i]; ++expectedCalls; }
        if (manager->GetNonLocalThingChecksum() != expected || calls != expectedCalls || errors) return 1;
        ++cases;
    }
    for (int pattern = 0; pattern < 8; ++pattern)
    {
        CThingManager* manager = Prepare(3, 0, pattern % 3);
        mutate = true;
        const unsigned long expected = values[0];
        if (manager->GetNonLocalThingChecksum() != expected || calls != 1 || errors || mutate) return 2;
        ++cases;
    }
    printf("THING_MANAGER_CHECKSUM_PASS cases=%d\n", cases);
    return 0;
}
