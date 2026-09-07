struct CParentDefClassBase;

extern "C" void __thiscall CParentDefClassBase_CParentDefClassBase(CParentDefClassBase* self);
extern void* PTR__vector_deleting_destructor__01235274;

struct VertexListOverlay {
    std::byte storage[0x0C];
};
static_assert(sizeof(VertexListOverlay) == 0x0C);

using VertexListInsertVertices_t =
    void(__thiscall*)(VertexListOverlay* self, int count, const void* first, const void* last);

extern VertexListInsertVertices_t VertexListInsertVertices;

struct CParentDefClassBase_Overlay {
    void** vfptr;                    // 0x00
    std::byte gap_04[0x38];          // 0x04
    std::vector<long> vector_3C;     // 0x3C
    std::vector<long> vector_48;     // 0x48
    std::vector<long> vector_54;     // 0x54
    VertexListOverlay vertex_list_60;// 0x60
    std::uint32_t value_6C;          // 0x6C
};

static_assert(offsetof(CParentDefClassBase_Overlay, vector_3C) == 0x3C);
static_assert(offsetof(CParentDefClassBase_Overlay, vector_48) == 0x48);
static_assert(offsetof(CParentDefClassBase_Overlay, vector_54) == 0x54);
static_assert(offsetof(CParentDefClassBase_Overlay, vertex_list_60) == 0x60);
static_assert(offsetof(CParentDefClassBase_Overlay, value_6C) == 0x6C);

CParentDefClassBase* __fastcall CParentDefClassBase_ConstructorWithVectors(CParentDefClassBase* self) {
    auto* const overlay = reinterpret_cast<CParentDefClassBase_Overlay*>(self);

    CParentDefClassBase_CParentDefClassBase(self);
    overlay->vfptr = &PTR__vector_deleting_destructor__01235274;

    ::new (static_cast<void*>(std::addressof(overlay->vector_3C))) std::vector<long>(6, 0L);
    ::new (static_cast<void*>(std::addressof(overlay->vector_48))) std::vector<long>(6, 0L);
    ::new (static_cast<void*>(std::addressof(overlay->vector_54))) std::vector<long>(6, 0L);

    std::uint8_t scratch_byte;
    scratch_byte = 0;
    VertexListInsertVertices(
        reinterpret_cast<VertexListOverlay*>(reinterpret_cast<std::byte*>(self) + 0x60),
        6,
        &scratch_byte,
        &scratch_byte);

    overlay->value_6C = 0;
    return self;
}