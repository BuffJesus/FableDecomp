#include <stdio.h>
#include <string.h>

#include "fable_counted_pointer.h"

class CIPhysicsFacesSegmentBase
{
public:
    virtual ~CIPhysicsFacesSegmentBase();
};

typedef CCountedPointer<const CIPhysicsFacesSegmentBase> CSegmentPointer;

namespace
{
    // Every heap block comes from this pool so the fixture can observe the
    // control-block allocation and both deletes Reset performs.
    unsigned char g_pool[8][32];
    unsigned int g_poolUsed;
    bool g_failNextAllocation;
    unsigned int g_newCalls;
    unsigned int g_lastNewSize;
    void* g_deleted[4];
    unsigned int g_deleteCount;
    unsigned int g_segmentDestructions;
    const void* g_destroyedSegment;

    class CSegment : public CIPhysicsFacesSegmentBase
    {
    public:
        virtual ~CSegment()
        {
            ++g_segmentDestructions;
            g_destroyedSegment = this;
        }
    };

    void ResetObservations()
    {
        g_failNextAllocation = false;
        g_newCalls = 0;
        g_lastNewSize = 0;
        memset(g_deleted, 0, sizeof(g_deleted));
        g_deleteCount = 0;
        g_segmentDestructions = 0;
        g_destroyedSegment = 0;
    }

    bool CheckSharedOwnerReplacement()
    {
        CSegment* oldSegment = new CSegment;
        CSegment* newSegment = new CSegment;
        CCPPointerInfo oldInfo(0, oldSegment);
        oldInfo.RefCount = 2;
        CSegmentPointer pointer;
        pointer.Data = oldSegment;
        pointer.Info = &oldInfo;
        ResetObservations();

        pointer.Reset(newSegment);

        return
            oldInfo.RefCount == 1 &&
            g_segmentDestructions == 0 &&
            g_deleteCount == 0 &&
            g_newCalls == 1 &&
            g_lastNewSize == sizeof(CCPPointerInfo) &&
            pointer.Data == newSegment &&
            pointer.Info != 0 &&
            pointer.Info != &oldInfo &&
            pointer.Info->RefCount == 1 &&
            pointer.Info->Data == newSegment &&
            pointer.Info->DeleteFunc != 0;
    }

    bool CheckLastOwnerRelease()
    {
        // Build the owner through Reset so the stored DeleteFunc is the
        // reconstruction's own, then drop the last reference.
        CSegment* segment = new CSegment;
        CSegmentPointer pointer;
        pointer.Data = 0;
        pointer.Info = 0;
        pointer.Reset(segment);
        CCPPointerInfo* info = pointer.Info;
        ResetObservations();

        pointer.Reset(0);

        return
            info != 0 &&
            info->RefCount == 0 &&
            g_segmentDestructions == 1 &&
            g_destroyedSegment == segment &&
            g_deleteCount == 2 &&
            g_deleted[0] == segment &&
            g_deleted[1] == info &&
            g_newCalls == 0 &&
            pointer.Data == 0 &&
            pointer.Info == 0;
    }

    bool CheckAllocationFailure()
    {
        CSegment* segment = new CSegment;
        CSegmentPointer pointer;
        pointer.Data = 0;
        pointer.Info = 0;
        ResetObservations();
        g_failNextAllocation = true;

        pointer.Reset(segment);

        return
            g_newCalls == 1 &&
            pointer.Data == segment &&
            pointer.Info == 0;
    }
}

CIPhysicsFacesSegmentBase::~CIPhysicsFacesSegmentBase()
{
}

void* operator new(unsigned int size)
{
    ++g_newCalls;
    g_lastNewSize = size;
    if (g_failNextAllocation)
    {
        g_failNextAllocation = false;
        return 0;
    }
    if (g_poolUsed >= 8 || size > sizeof(g_pool[0]))
    {
        return 0;
    }
    return g_pool[g_poolUsed++];
}

void operator delete(void* block)
{
    if (g_deleteCount < 4)
    {
        g_deleted[g_deleteCount] = block;
    }
    ++g_deleteCount;
}

int main()
{
    const bool passed =
        CheckSharedOwnerReplacement() &&
        CheckLastOwnerRelease() &&
        CheckAllocationFailure();
    if (!passed)
    {
        printf("FABLETLC_PHYSICS_FACES_COUNTED_RESET FAIL\n");
        return 1;
    }

    printf("FABLETLC_PHYSICS_FACES_COUNTED_RESET PASS\n");
    return 0;
}
