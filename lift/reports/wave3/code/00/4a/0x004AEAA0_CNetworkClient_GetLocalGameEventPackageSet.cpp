namespace
{
struct LocalGameServerUpdateEvent
{
    std::int32_t Type;                  // +0x00
    std::byte MemoryBufferStorage[0x21]; // +0x04
    std::uint8_t Flag25;               // +0x25
    std::uint8_t UpdateReady;          // +0x26
    std::uint8_t Flag27;               // +0x27
};

static_assert(offsetof(LocalGameServerUpdateEvent, Type) == 0x00);
static_assert(offsetof(LocalGameServerUpdateEvent, MemoryBufferStorage) == 0x04);
static_assert(offsetof(LocalGameServerUpdateEvent, Flag25) == 0x25);
static_assert(offsetof(LocalGameServerUpdateEvent, UpdateReady) == 0x26);
static_assert(offsetof(LocalGameServerUpdateEvent, Flag27) == 0x27);

struct CGameEventPackageFrameOverlay
{
    long Frame;
};

static_assert(offsetof(CGameEventPackageFrameOverlay, Frame) == 0x00);

void __thiscall CMemoryBuffer__Copy(void* memory_buffer, int size, const void* source);
}

bool CNetworkClient::GetLocalGameEventPackageSet(CGameEventPackageSet* package_set)
{
    LocalGameServerUpdateEvent event_storage;
    std::uint32_t copy_source;

    event_storage.MemoryBufferStorage[0] = std::byte{0xFF};
    event_storage.UpdateReady = 0;
    event_storage.UpdateReady =
        static_cast<std::uint8_t>(GameComponent->IsTimeForServerUpdate(LocalFrame));

    if (event_storage.UpdateReady != 0)
    {
        copy_source = Checksum1;
        event_storage.Type = 1;
        event_storage.Flag25 = 0;
        event_storage.Flag27 = 0;
        event_storage.MemoryBufferStorage[0] = std::byte{0xFF};
        CMemoryBuffer__Copy(&event_storage.MemoryBufferStorage[0], 4, &copy_source);

        copy_source = Checksum2;
        CMemoryBuffer__Copy(&event_storage.MemoryBufferStorage[0], 4, &copy_source);

        copy_source = static_cast<std::uint32_t>(LocalFrame);
        CMemoryBuffer__Copy(&event_storage.MemoryBufferStorage[0], 4, &copy_source);

        LocalFrame = LocalFrame + 1;

        CGameEventPackage* const local_event_package = &LocalEventPackage;
        local_event_package->AddEvent(reinterpret_cast<CGameEvent*>(&event_storage));
        reinterpret_cast<CGameEventPackageFrameOverlay*>(local_event_package)->Frame = LocalFrame;

        NHeroInformationScreens::CBase::CBase(reinterpret_cast<CBase*>(package_set));
        CGameEventPackageSet::AddPackage(package_set, local_event_package);
        local_event_package->Clear();
        return true;
    }

    return false;
}