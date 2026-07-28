#include <cstddef>
#include <cstdint>

struct CTexture24NodeOverlay {
    std::uint8_t state_0;
    std::uint8_t pad_01[3];
    std::uint32_t value_04;
    CTexture24NodeOverlay* next_08;
    CTexture24NodeOverlay* prev_0C;
};

static_assert(offsetof(CTexture24NodeOverlay, state_0) == 0x00);
static_assert(offsetof(CTexture24NodeOverlay, value_04) == 0x04);
static_assert(offsetof(CTexture24NodeOverlay, next_08) == 0x08);
static_assert(offsetof(CTexture24NodeOverlay, prev_0C) == 0x0C);

struct CTexture24Overlay {
    CTexture24NodeOverlay* node_00;
    std::uint32_t field_04;
};

static_assert(offsetof(CTexture24Overlay, node_00) == 0x00);
static_assert(offsetof(CTexture24Overlay, field_04) == 0x04);

extern "C" void __thiscall Allocator_Alloc_24bytes(CTexture24Overlay* self, std::uint32_t param_3);

int* __thiscall CTexture_InitEmpty_24bytes(int* self, std::uint32_t /*param_2*/, std::uint32_t param_3) {
    auto* const texture = reinterpret_cast<CTexture24Overlay*>(self);

    Allocator_Alloc_24bytes(texture, param_3);

    texture->field_04 = 0;
    texture->node_00->state_0 = 0;
    texture->node_00->value_04 = 0;
    texture->node_00->next_08 = texture->node_00;
    texture->node_00->prev_0C = texture->node_00;

    return self;
}