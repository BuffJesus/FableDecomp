void CWorld::EAMoveSpirit(const CGameEvent* event)
{
    using InterfaceMap = CVectorMap<
        ETCInterfaceType,
        CTCBase*,
        CKeyPairCompareLess<ETCInterfaceType, CTCBase*>
    >;

    struct GameEventMoveSpiritOverlay
    {
        std::byte pad00[0x4];
        std::int8_t morphType;
    };
    static_assert(offsetof(GameEventMoveSpiritOverlay, morphType) == 0x4);

    struct ParticleMorphEntryOverlay
    {
        std::byte pad00[0x38];
        std::int8_t spiritEnabledFlag;
        std::byte pad39[0x44 - 0x39];
        InterfaceMap interfaces;
    };
    static_assert(offsetof(ParticleMorphEntryOverlay, spiritEnabledFlag) == 0x38);
    static_assert(offsetof(ParticleMorphEntryOverlay, interfaces) == 0x44);

    struct EventPacketBufferOverlay
    {
        std::byte opaque00[0x20];
        std::uint32_t opaque20;
        std::uint32_t opaque24;
        std::uint32_t opaque28;
    };
    static_assert(sizeof(EventPacketBufferOverlay) == 0x2C);

    struct RawVectorOverlay
    {
        std::uint32_t xBits;
        std::uint32_t yBits;
        std::uint32_t zBits;
    };
    static_assert(sizeof(RawVectorOverlay) == sizeof(C3DVector));

    using ResolveMorphHandleFn = void* (__stdcall*)(int);
    using MorphHandleValidFn = int (__thiscall*)(void*);
    using GetMorphEntryFn = ParticleMorphEntryOverlay* (__thiscall*)(void*);
    using CopyToBufferFn = void (__thiscall*)(const CGameEvent*, EventPacketBufferOverlay*);
    using CircularBufferReadFn = void (__thiscall*)(EventPacketBufferOverlay*, int, void*);

    auto* const resolveMorphHandle =
        reinterpret_cast<ResolveMorphHandleFn>(0x0049D840);
    auto* const morphHandleValid =
        reinterpret_cast<MorphHandleValidFn>(0x00489DB0);
    auto* const getMorphEntry =
        reinterpret_cast<GetMorphEntryFn>(0x00489DA0);
    auto* const copyToBuffer =
        reinterpret_cast<CopyToBufferFn>(0x009F1730);
    auto* const circularBufferRead =
        reinterpret_cast<CircularBufferReadFn>(0x009F1850);

    const auto* const moveEvent =
        reinterpret_cast<const GameEventMoveSpiritOverlay*>(event);

    void* const morphHandle =
        resolveMorphHandle(static_cast<int>(moveEvent->morphType));

    if (morphHandleValid(morphHandle) != 0)
    {
        auto* const entry = getMorphEntry(morphHandle);
        if (entry->spiritEnabledFlag < 0)
        {
            ETCInterfaceType interfaceType = static_cast<ETCInterfaceType>(199);
            int* interfaceIt = InterfaceMap::LowerBound(&entry->interfaces, &interfaceType);
            int* const interfaceEnd = entry->interfaces.End;

            if (interfaceIt == interfaceEnd ||
                interfaceType < *reinterpret_cast<ETCInterfaceType*>(interfaceIt))
            {
                interfaceIt = interfaceEnd;
            }

            auto* const coopSpirit =
                reinterpret_cast<CTCCoopSpirit*>(interfaceIt[1]);

            EventPacketBufferOverlay packetBuffer;
            packetBuffer.opaque20 = 0;
            packetBuffer.opaque24 = 0;
            packetBuffer.opaque28 = 0;

            copyToBuffer(event, &packetBuffer);

            std::uint32_t rawX;
            std::uint32_t rawY;
            circularBufferRead(&packetBuffer, 4, &rawX);
            circularBufferRead(&packetBuffer, 4, &rawY);

            RawVectorOverlay movementBits;
            movementBits.xBits = rawX;
            movementBits.yBits = rawY;
            movementBits.zBits = 0;

            CTCCoopSpirit::ApplyMovementVector(
                coopSpirit,
                reinterpret_cast<C3DVector*>(&movementBits));
        }
    }
}