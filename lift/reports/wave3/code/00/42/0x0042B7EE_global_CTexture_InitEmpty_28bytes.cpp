#include <cstddef>
#include <cstdint>

struct CTexture28StorageOverlay {
    std::uint8_t state;
    std::uint8_t pad_01[3];
    std::uint32_t field_04;
    CTexture28StorageOverlay* next;
    CTexture28StorageOverlay* prev;
    std::uint8_t trailing_pad_10[0x1C - 0x10];
};

static_assert(offsetof(CTexture28StorageOverlay, state) == 0x00);
static_assert(offsetof(CTexture28StorageOverlay, field_04) == 0x04);
static_assert(offsetof(CTexture28StorageOverlay, next) == 0x08);
static_assert(offsetof(CTexture28StorageOverlay, prev) == 0x0C);
static_assert(sizeof(CTexture28StorageOverlay) == 0x1C);

struct CTextureInitEmpty28Overlay {
    CTexture28StorageOverlay* storage;
    std::int32_t field_04;

    void __thiscall Allocator_Alloc_28bytes(std::uint32_t param_3);
};

static_assert(offsetof(CTextureInitEmpty28Overlay, storage) == 0x00);
static_assert(offsetof(CTextureInitEmpty28Overlay, field_04) == 0x04);
static_assert(sizeof(CTextureInitEmpty28Overlay) == 0x08);

CTextureInitEmpty28Overlay* __thiscall CTexture_InitEmpty_28bytes(
    CTextureInitEmpty28Overlay* self,
    std::uint32_t /*param_2*/,
    std::uint32_t param_3)
{
    self->Allocator_Alloc_28bytes(param_3);
    self->field_04 = 0;
    self->storage->state = 0;
    self->storage->field_04 = 0;
    self->storage->next = self->storage;
    self->storage->prev = self->storage;
    return self;
}