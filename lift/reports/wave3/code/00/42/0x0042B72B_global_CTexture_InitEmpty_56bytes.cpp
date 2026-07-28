#include <cstddef>
#include <cstdint>

struct Texture56Overlay {
    std::uint8_t field_0;
    std::uint8_t pad_01[3];
    std::uint32_t field_4;
    Texture56Overlay* field_8;
    Texture56Overlay* field_C;
    std::uint8_t pad_10[0x28];
};

static_assert(offsetof(Texture56Overlay, field_0) == 0x00);
static_assert(offsetof(Texture56Overlay, field_4) == 0x04);
static_assert(offsetof(Texture56Overlay, field_8) == 0x08);
static_assert(offsetof(Texture56Overlay, field_C) == 0x0C);
static_assert(sizeof(Texture56Overlay) == 0x38);

struct CTextureInitEmpty56BytesOverlay {
    Texture56Overlay* field_0;
    std::uint32_t field_4;
};

static_assert(offsetof(CTextureInitEmpty56BytesOverlay, field_0) == 0x00);
static_assert(offsetof(CTextureInitEmpty56BytesOverlay, field_4) == 0x04);

void __thiscall Allocator_Alloc_56bytes(CTextureInitEmpty56BytesOverlay* self, std::uint32_t param_3);

CTextureInitEmpty56BytesOverlay* __thiscall CTexture_InitEmpty_56bytes(
    CTextureInitEmpty56BytesOverlay* self,
    std::uint32_t /*param_2*/,
    std::uint32_t param_3)
{
    Allocator_Alloc_56bytes(self, param_3);
    self->field_4 = 0;
    self->field_0->field_0 = 0;
    self->field_0->field_4 = 0;
    self->field_0->field_8 = self->field_0;
    self->field_0->field_C = self->field_0;
    return self;
}