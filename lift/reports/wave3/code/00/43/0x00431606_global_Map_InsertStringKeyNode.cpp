#include <cstddef>
#include <cstdint>

struct MapTreeOverlay {
    std::byte pad00[0x04];
    std::int32_t root;      // 0x04
    std::int32_t sentinel;  // 0x08
};
static_assert(offsetof(MapTreeOverlay, root) == 0x04);
static_assert(offsetof(MapTreeOverlay, sentinel) == 0x08);

struct MapStringNodeOverlay {
    std::byte pad00[0x08];
    std::int32_t left;      // 0x08
    std::int32_t right;     // 0x0C
    void* keyString;        // 0x10
};
static_assert(offsetof(MapStringNodeOverlay, left) == 0x08);
static_assert(offsetof(MapStringNodeOverlay, right) == 0x0C);
static_assert(offsetof(MapStringNodeOverlay, keyString) == 0x10);

struct MapInsertResultOverlay {
    std::int32_t node;      // 0x00
    std::uint8_t inserted;  // 0x04
};
static_assert(offsetof(MapInsertResultOverlay, node) == 0x00);
static_assert(offsetof(MapInsertResultOverlay, inserted) == 0x04);

template <typename T>
struct CBasicString;

template <>
struct CBasicString<char> {
    static long Compare(const char* lhs, const char* rhs);
};

extern "C" std::int32_t RBTree__GetMaxNode(std::int32_t node);
extern "C" std::int32_t* BinaryTree_InsertDigScriptNode(
    void* a1,
    std::int32_t a2,
    std::int32_t a3,
    void* a4,
    std::int32_t a5);

void __thiscall Map_InsertStringKeyNode(
    std::int32_t* this_,
    MapInsertResultOverlay* result,
    void** keyStorage)
{
    bool goLeft = true;
    const auto* const tree = reinterpret_cast<const MapTreeOverlay*>(*this_);

    std::int32_t parent = *this_;
    std::int32_t current = tree->root;

    while (current != 0) {
        const auto* const currentNode =
            reinterpret_cast<const MapStringNodeOverlay*>(current);
        void* const currentKey = currentNode->keyString;
        void* const searchKey = *keyStorage;

        if (currentKey == searchKey) {
            goLeft = false;
        } else if (searchKey == nullptr) {
            goLeft = true;
        } else {
            if (currentKey == nullptr) {
                goLeft = false;
            } else {
                const long compareResult = CBasicString<char>::Compare(
                    static_cast<const char*>(searchKey),
                    static_cast<const char*>(currentKey));
                goLeft = compareResult < 0;
            }
        }

        parent = current;
        if (goLeft) {
            current = currentNode->left;
        } else {
            current = currentNode->right;
        }
    }

    std::int32_t candidate = parent;
    std::int32_t insertParent;

    if (goLeft) {
        insertParent = parent;
        if (parent == tree->sentinel) {
            goto do_insert;
        }
        candidate = RBTree__GetMaxNode(parent);
    }

    {
        void* const searchKey = *keyStorage;
        const auto* const candidateNode =
            reinterpret_cast<const MapStringNodeOverlay*>(candidate);
        void* const candidateKey = candidateNode->keyString;

        if ((searchKey == candidateKey) ||
            ((insertParent = current, candidateKey != nullptr) &&
             ((searchKey == nullptr) ||
              (CBasicString<char>::Compare(
                   static_cast<const char*>(candidateKey),
                   static_cast<const char*>(searchKey)) >= 0)))) {
            result->node = candidate;
            result->inserted = 0;
            return;
        }
    }

do_insert:
    {
        std::int32_t* const insertedNode = BinaryTree_InsertDigScriptNode(
            &keyStorage,
            insertParent,
            parent,
            keyStorage,
            0);
        result->node = *insertedNode;
        result->inserted = 1;
        return;
    }
}