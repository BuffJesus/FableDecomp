#include <cstddef>
#include <cstdint>

extern std::uint8_t DAT_0129aaf4;

struct CCRC
{
    static void __fastcall Calc(CCRC* self, void* data, std::uint32_t length);
};

#pragma pack(push, 1)
struct CharStringStorage
{
    void* data;
    std::uint32_t length;
};
#pragma pack(pop)

static_assert(offsetof(CharStringStorage, data) == 0x0);
static_assert(offsetof(CharStringStorage, length) == 0x4);
static_assert(sizeof(CharStringStorage) == 0x8);

void __fastcall CCharString_ComputeCRC32(CharStringStorage** stringStorageRef)
{
    CharStringStorage* stringStorage = *stringStorageRef;
    if (stringStorage == nullptr) {
        CCRC::Calc(nullptr, &DAT_0129aaf4, 0);
        return;
    }

    CCRC::Calc(nullptr, stringStorage->data, stringStorage->length);
}