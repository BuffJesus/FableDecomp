#include <cstddef>
#include <functional>

struct CCharString;

struct StdMapNodeOverlay {
    std::byte _pad0[0x04];
    StdMapNodeOverlay* headerRoot; // 0x04
    StdMapNodeOverlay* left;       // 0x08
    StdMapNodeOverlay* right;      // 0x0C
    std::byte keyStorage[1];       // 0x10, start of in-place CCharString
};

static_assert(offsetof(StdMapNodeOverlay, headerRoot) == 0x04);
static_assert(offsetof(StdMapNodeOverlay, left) == 0x08);
static_assert(offsetof(StdMapNodeOverlay, right) == 0x0C);
static_assert(offsetof(StdMapNodeOverlay, keyStorage) == 0x10);

struct StdMapOverlay {
    StdMapNodeOverlay* header;      // 0x00
    std::less<CCharString> keyLess; // 0x08
};

static_assert(offsetof(StdMapOverlay, header) == 0x00);
static_assert(offsetof(StdMapOverlay, keyLess) == 0x08);

int __thiscall StdMap_FindLE(StdMapOverlay* self, CCharString* key)
{
    StdMapNodeOverlay* const header = self->header;
    StdMapNodeOverlay* current = header->headerRoot;
    StdMapNodeOverlay* candidate = header;

    while (current != nullptr) {
        if (self->keyLess(
                *reinterpret_cast<CCharString*>(current->keyStorage),
                *key)) {
            current = current->right;
        } else {
            candidate = current;
            current = current->left;
        }
    }

    if (candidate != header &&
        !self->keyLess(
            *key,
            *reinterpret_cast<CCharString*>(candidate->keyStorage))) {
        return reinterpret_cast<int>(candidate);
    }

    return reinterpret_cast<int>(header);
}