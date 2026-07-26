#include "candidates/object_family_compiled.h"

long CObjectFamilyDef::GetRandomObject() const
{
    if (mObjectsBegin != mObjectsEnd)
    {
        unsigned int totalWeight = 0;
        unsigned int index = 0;
        const long count = mObjectsEnd - mObjectsBegin;
        if (count != 0)
        {
            const CObjectFamilyEntry* entry = mObjectsBegin;
            do
            {
                totalWeight += entry->Probability;
                ++index;
                ++entry;
            }
            while (index < (unsigned int)count);
        }

        unsigned long* worldSeed = &gMainGameComponent->WorldSeed;
        const unsigned long nextSeed = *worldSeed * 0x24A1U + 0x24DFU;
        *worldSeed = nextSeed;
        const unsigned long rotatedSeed = GFROR13(nextSeed);
        *worldSeed = rotatedSeed;

        unsigned long randomWeight = 0;
        if (totalWeight != 0)
            randomWeight = rotatedSeed % totalWeight;

        long runningWeight = 0;
        index = 0;
        if (count != 0)
        {
            const CObjectFamilyEntry* entry = mObjectsBegin;
            do
            {
                runningWeight += entry->Probability;
                if ((long)randomWeight < runningWeight)
                    return entry->Object;
                ++index;
                ++entry;
            }
            while (index < (unsigned int)(mObjectsEnd - mObjectsBegin));
        }
    }
    return 0;
}
