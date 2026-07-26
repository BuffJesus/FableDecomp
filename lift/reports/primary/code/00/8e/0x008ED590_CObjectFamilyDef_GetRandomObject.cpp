long CObjectFamilyDef::GetRandomObject() const
{
    if (mObjectsBegin != mObjectsEnd) {
        unsigned int totalWeight = 0;
        unsigned int index = 0;
        const std::ptrdiff_t count = (mObjectsEnd - mObjectsBegin);

        if (count != 0) {
            const ObjectFamilyEntry* entry = mObjectsBegin;
            do {
                totalWeight = totalWeight + entry->Weight;
                index = index + 1;
                entry = entry + 1;
            } while (index < static_cast<unsigned int>(count));
        }

        unsigned int* const worldSeed = &gMainGameComponent->WorldSeed;
        const unsigned int nextSeed = *worldSeed * 0x24A1u + 0x24DFu;
        *worldSeed = nextSeed;

        const unsigned int rotatedSeed = GFROR13(nextSeed);
        unsigned int randomWeight = 0;
        *worldSeed = rotatedSeed;

        if (totalWeight != 0) {
            randomWeight = rotatedSeed % totalWeight;
        }

        int runningWeight = 0;
        index = 0;

        if (count != 0) {
            const ObjectFamilyEntry* entry = mObjectsBegin;
            do {
                runningWeight = runningWeight + entry->Weight;
                if (static_cast<int>(randomWeight) < runningWeight) {
                    return entry->ObjectDefIndex;
                }
                index = index + 1;
                entry = entry + 1;
            } while (index < static_cast<unsigned int>(mObjectsEnd - mObjectsBegin));
        }
    }

    return 0;
}