#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <new>

struct ShowOnScreenMessagePayload
{
    CWideString text;               // 0x00
    float x;                        // 0x04
    float y;                        // 0x08
    std::uint8_t color[4];          // 0x0C
    CCharString displayType;        // 0x10
    long preMainMemoryUsed;         // 0x14
};

static_assert(offsetof(ShowOnScreenMessagePayload, x) == 0x04);
static_assert(offsetof(ShowOnScreenMessagePayload, y) == 0x08);
static_assert(offsetof(ShowOnScreenMessagePayload, color) == 0x0C);
static_assert(offsetof(ShowOnScreenMessagePayload, displayType) == 0x10);
static_assert(offsetof(ShowOnScreenMessagePayload, preMainMemoryUsed) == 0x14);
static_assert(sizeof(ShowOnScreenMessagePayload) == 0x18);

struct ShowOnScreenMessageListNode
{
    ShowOnScreenMessageListNode* prev; // 0x00
    ShowOnScreenMessageListNode* next; // 0x04
};

static_assert(sizeof(ShowOnScreenMessageListNode) == 0x08);

struct ShowOnScreenMessageNode
{
    ShowOnScreenMessageListNode links; // 0x00
    ShowOnScreenMessagePayload payload; // 0x08
};

static_assert(offsetof(ShowOnScreenMessageNode, payload) == 0x08);
static_assert(sizeof(ShowOnScreenMessageNode) == 0x20);

struct ShowOnScreenMessageOwnerOverlay
{
    std::byte pad_0000[0x324];
    ShowOnScreenMessageListNode* listHead; // 0x324
};

static_assert(offsetof(ShowOnScreenMessageOwnerOverlay, listHead) == 0x324);

void __thiscall ShowOnScreenMessage_FormatDisplay(
    int thisPtr,
    CWideString* message,
    C2DVector* gfaCoord,
    std::uint8_t* rgba,
    CCharString* displayType)
{
    C2DVector vCoord;
    GFACoordToVCoord(gfaCoord, &vCoord);
    vCoord.x = GFRoundVXToNearestPixel(static_cast<double>(vCoord.x));
    vCoord.y = GFRoundVYToNearestPixel(static_cast<double>(vCoord.y));

    const long preMainMemoryUsed = GFGetPreMainMemoryUsed();

    alignas(ShowOnScreenMessagePayload) std::byte localPayloadStorage[sizeof(ShowOnScreenMessagePayload)];
    auto* const localPayload = reinterpret_cast<ShowOnScreenMessagePayload*>(localPayloadStorage);

    ::new (&localPayload->text) CWideString(*message);
    localPayload->x = vCoord.x;
    localPayload->color[2] = rgba[2];
    localPayload->y = vCoord.y;
    localPayload->color[1] = rgba[1];
    localPayload->color[0] = rgba[0];
    localPayload->color[3] = rgba[3];
    ::new (&localPayload->displayType) CCharString(*displayType);

    auto* const listHead =
        reinterpret_cast<ShowOnScreenMessageOwnerOverlay*>(thisPtr)->listHead;

    localPayload->preMainMemoryUsed = preMainMemoryUsed;

    auto* const node = static_cast<ShowOnScreenMessageNode*>(std::malloc(0x20));
    auto* const nodePayload = reinterpret_cast<ShowOnScreenMessagePayload*>(
        reinterpret_cast<std::uintptr_t>(node) + 0x08);

    if (nodePayload != nullptr)
    {
        nodePayload->ShowOnScreenMessagePayload::operator=(*localPayload);
    }

    ShowOnScreenMessageListNode* const oldFirst = listHead->next;
    node->links.prev = listHead;
    node->links.next = oldFirst;
    oldFirst->prev = &node->links;
    listHead->next = &node->links;

    localPayload->displayType.~CCharString();
    localPayload->text.~CWideString();
}