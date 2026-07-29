struct EventConversation_Overlay {
    struct IndirectCallEntry {
        void** call_table;          // 0x00
        std::uint32_t reserved_04;  // 0x04
    };

    std::byte pad_00[0x1C];
    IndirectCallEntry* array_1_begin;      // 0x1C
    IndirectCallEntry* array_1_end;        // 0x20
    std::byte pad_24[0x1C];
    CFileInstaller::CActiveFile active_file; // 0x40
    IndirectCallEntry* array_2_begin;      // 0x44
    IndirectCallEntry* array_2_end;        // 0x48
    std::byte pad_4C[0x04];
    CCharString string_50;                 // 0x50
    CCharString string_54;                 // 0x54
    void* list_storage;                    // 0x58
};

static_assert(offsetof(EventConversation_Overlay, array_1_begin) == 0x1C);
static_assert(offsetof(EventConversation_Overlay, array_1_end) == 0x20);
static_assert(offsetof(EventConversation_Overlay, active_file) == 0x40);
static_assert(offsetof(EventConversation_Overlay, array_2_begin) == 0x44);
static_assert(offsetof(EventConversation_Overlay, array_2_end) == 0x48);
static_assert(offsetof(EventConversation_Overlay, string_50) == 0x50);
static_assert(offsetof(EventConversation_Overlay, string_54) == 0x54);
static_assert(offsetof(EventConversation_Overlay, list_storage) == 0x58);
static_assert(sizeof(EventConversation_Overlay::IndirectCallEntry) == 0x08);

void __fastcall EventConversation_Destructor(int param_1)
{
    auto* self = reinterpret_cast<EventConversation_Overlay*>(param_1);

    using EventList =
        std::list<CCountedPointer<NEntityEvents::CEventBase>,
                  std::allocator<CCountedPointer<NEntityEvents::CEventBase>>>;
    using EventFilter =
        NEntityEvents::CConvFilterToCP<NEntityEvents::CIsEventOfType>;

    reinterpret_cast<EventList*>(param_1 + 0x58)->remove_if(EventFilter{});

    void* memory = *reinterpret_cast<void**>(param_1 + 0x58);
    if (memory != nullptr) {
        free(memory);
    }

    self->string_54.~CCharString();
    self->string_50.~CCharString();

    EventConversation_Overlay::IndirectCallEntry* end_2 = self->array_2_end;
    for (EventConversation_Overlay::IndirectCallEntry* it = self->array_2_begin; it != end_2; ++it) {
        auto* fn = reinterpret_cast<void(__thiscall*)(EventConversation_Overlay::IndirectCallEntry*, int)>(
            it->call_table[0]);
        fn(it, 0);
    }

    if (self->array_2_begin != nullptr) {
        free(self->array_2_begin);
    }

    CFileInstaller::CActiveFile::OnReadFinished(&self->active_file);

    EventConversation_Overlay::IndirectCallEntry* end_1 = self->array_1_end;
    for (EventConversation_Overlay::IndirectCallEntry* it = self->array_1_begin; it != end_1; ++it) {
        auto* fn = reinterpret_cast<void(__thiscall*)(EventConversation_Overlay::IndirectCallEntry*, int)>(
            it->call_table[0]);
        fn(it, 0);
    }

    if (self->array_1_begin != nullptr) {
        free(self->array_1_begin);
    }
}