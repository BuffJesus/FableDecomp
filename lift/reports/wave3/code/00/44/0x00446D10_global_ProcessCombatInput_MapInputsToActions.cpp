struct CountedInputTypeConstOverlay
{
    void* object;
    int* refCount;
};
static_assert(sizeof(CountedInputTypeConstOverlay) == 0x8);
static_assert(offsetof(CountedInputTypeConstOverlay, object) == 0x0);
static_assert(offsetof(CountedInputTypeConstOverlay, refCount) == 0x4);

static void __cdecl DestroyRange_CountedInputTypeConst(
    CountedInputTypeConstOverlay* first,
    CountedInputTypeConstOverlay* last,
    int* const* allocator_context);

std::uint32_t __thiscall ProcessCombatInput_MapInputsToActions(void* const self, int* const actionRange)
{
    struct ProcessCombatInputOverlay
    {
        std::byte pad_0000[0x7A8];
    };
    static_assert(sizeof(ProcessCombatInputOverlay) == 0x7A8);

    struct ActionMapEntryOverlay
    {
        EGameAction action;
        void* inputTypeObject;
        int* refCount;
    };
    static_assert(sizeof(ActionMapEntryOverlay) == 0xC);
    static_assert(offsetof(ActionMapEntryOverlay, action) == 0x0);
    static_assert(offsetof(ActionMapEntryOverlay, inputTypeObject) == 0x4);
    static_assert(offsetof(ActionMapEntryOverlay, refCount) == 0x8);

    struct ActionMapOverlay
    {
        ActionMapEntryOverlay* LowerBound(const EGameAction* key);
        ActionMapEntryOverlay* UpperBound(const EGameAction* key);
    };

    using RawCompatibilityCall = char(__thiscall*)(void*, void*);

    auto add_ref = [](CountedInputTypeConstOverlay& value) -> void
    {
        if (value.refCount != nullptr) {
            ++*value.refCount;
        }
    };

    auto* currentAction = reinterpret_cast<EGameAction*>(actionRange[0]);
    auto* const actionEnd = reinterpret_cast<EGameAction*>(actionRange[1]);

    CountedInputTypeConstOverlay* bufferBegin = nullptr;
    CountedInputTypeConstOverlay* bufferEnd = nullptr;
    CountedInputTypeConstOverlay* bufferCapacityEnd = nullptr;

    if (currentAction != actionEnd) {
        auto* const actionMap =
            reinterpret_cast<ActionMapOverlay*>(reinterpret_cast<std::byte*>(self) + sizeof(ProcessCombatInputOverlay));

        do {
            EGameAction key = *currentAction;
            ActionMapEntryOverlay* upper = actionMap->UpperBound(&key);
            ActionMapEntryOverlay* entry = actionMap->LowerBound(&key);

            if (entry != upper) {
                do {
                    for (CountedInputTypeConstOverlay* probe = bufferBegin; probe != bufferEnd; ++probe) {
                        const auto vftable = *reinterpret_cast<std::uintptr_t*>(probe->object);
                        const auto compatibilityCall =
                            *reinterpret_cast<RawCompatibilityCall*>(vftable + 0x8);

                        if (compatibilityCall(probe->object, entry->inputTypeObject) != '\0') {
                            DestroyRange_CountedInputTypeConst(bufferBegin, bufferEnd, &actionRange);
                            if (bufferBegin != nullptr) {
                                free(bufferBegin);
                            }
                            return 1;
                        }
                    }

                    if (bufferEnd == bufferCapacityEnd) {
                        const int oldCount = static_cast<int>(bufferEnd - bufferBegin);
                        int growth = 1;
                        const int newCount = oldCount + (oldCount != 0 ? oldCount : growth);

                        auto* const newBuffer =
                            newCount == 0
                                ? nullptr
                                : static_cast<CountedInputTypeConstOverlay*>(
                                      malloc(static_cast<std::size_t>(newCount) * sizeof(CountedInputTypeConstOverlay)));

                        CountedInputTypeConstOverlay* write = newBuffer;
                        if (bufferBegin != bufferEnd) {
                            const auto byteDelta =
                                reinterpret_cast<const std::byte*>(bufferBegin) -
                                reinterpret_cast<const std::byte*>(newBuffer);

                            do {
                                if (write != nullptr) {
                                    const auto* const src =
                                        reinterpret_cast<const CountedInputTypeConstOverlay*>(
                                            reinterpret_cast<const std::byte*>(write) + byteDelta);
                                    write->object = src->object;
                                    write->refCount = src->refCount;
                                    add_ref(*write);
                                }
                                ++write;
                            } while (reinterpret_cast<const CountedInputTypeConstOverlay*>(
                                         reinterpret_cast<const std::byte*>(write) + byteDelta) != bufferEnd);
                        }

                        if (write != nullptr) {
                            write->object = entry->inputTypeObject;
                            write->refCount = entry->refCount;
                            add_ref(*write);
                        }

                        DestroyRange_CountedInputTypeConst(bufferBegin, bufferEnd, &actionRange);
                        if (bufferBegin != nullptr) {
                            free(bufferBegin);
                        }

                        bufferCapacityEnd = newBuffer + newCount;
                        bufferBegin = newBuffer;
                        bufferEnd = write;
                    } else if (bufferEnd != nullptr) {
                        bufferEnd->object = entry->inputTypeObject;
                        bufferEnd->refCount = entry->refCount;
                        add_ref(*bufferEnd);
                    }

                    ++bufferEnd;
                    entry = reinterpret_cast<ActionMapEntryOverlay*>(
                        reinterpret_cast<std::byte*>(entry) + sizeof(ActionMapEntryOverlay));
                } while (entry != upper);
            }

            ++currentAction;
        } while (currentAction != actionEnd);
    }

    DestroyRange_CountedInputTypeConst(bufferBegin, bufferEnd, &actionRange);
    if (bufferBegin != nullptr) {
        free(bufferBegin);
    }
    return 0;
}