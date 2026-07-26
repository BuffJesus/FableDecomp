#include <cstddef>
#include <cstdint>
#include <cstdlib>

struct CMessageEventFilter_IsOfType
{
    std::uint32_t Type;
};

static_assert(sizeof(CMessageEventFilter_IsOfType) == 0x04);

struct CMessageEventObjectOverlay
{
    void** Vftable;
};

static_assert(offsetof(CMessageEventObjectOverlay, Vftable) == 0x00);

struct CMessageEventListEntry
{
    CMessageEventListEntry* Next;
    CMessageEventListEntry* Prev;
    CMessageEventObjectOverlay MessageObject;
};

static_assert(offsetof(CMessageEventListEntry, Next) == 0x00);
static_assert(offsetof(CMessageEventListEntry, Prev) == 0x04);
static_assert(offsetof(CMessageEventListEntry, MessageObject) == 0x08);

struct CMessageEventConstPtrListNode
{
    CMessageEventConstPtrListNode* Next;
    CMessageEventConstPtrListNode* Prev;
    const void* Message;
};

static_assert(offsetof(CMessageEventConstPtrListNode, Next) == 0x00);
static_assert(offsetof(CMessageEventConstPtrListNode, Prev) == 0x04);
static_assert(offsetof(CMessageEventConstPtrListNode, Message) == 0x08);
static_assert(sizeof(CMessageEventConstPtrListNode) == 0x0C);

struct CMessageEventConstPtrList
{
    CMessageEventConstPtrListNode* Head;
};

static_assert(offsetof(CMessageEventConstPtrList, Head) == 0x00);

class CMessageEventManager
{
public:
    std::byte Pad0[0x04];
    CMessageEventListEntry* Field04;

    template <typename TFilter>
    long __thiscall FindAllMessages(TFilter* pFilter, CMessageEventConstPtrList* pMessages);
};

static_assert(offsetof(CMessageEventManager, Field04) == 0x04);

struct CGameScriptInterface_Field04
{
    std::byte Pad0[0x60];
    CMessageEventManager* Field60;
};

static_assert(offsetof(CGameScriptInterface_Field04, Field60) == 0x60);

class CGameScriptInterface
{
public:
    std::byte Pad0[0x04];
    CGameScriptInterface_Field04* Field04;

    void __thiscall RemoveAllCutSceneSkippedMessages() const;
};

static_assert(offsetof(CGameScriptInterface, Field04) == 0x04);

void __thiscall CGameScriptInterface::RemoveAllCutSceneSkippedMessages() const
{
    CMessageEventManager* const messageEventManager = Field04->Field60;

    auto* const foundMessagesHead =
        static_cast<CMessageEventConstPtrListNode*>(std::malloc(sizeof(CMessageEventConstPtrListNode)));
    foundMessagesHead->Next = foundMessagesHead;
    foundMessagesHead->Prev = foundMessagesHead;

    CMessageEventFilter_IsOfType messageFilter{};
    messageFilter.Type = 0x32;

    CMessageEventConstPtrList foundMessages{};
    foundMessages.Head = foundMessagesHead;

    const long messageCount =
        messageEventManager->FindAllMessages<CMessageEventFilter_IsOfType>(&messageFilter, &foundMessages);

    if (messageCount > 0)
    {
        for (CMessageEventConstPtrListNode* foundNode = foundMessagesHead->Next;
             foundNode != foundMessagesHead;
             foundNode = foundNode->Next)
        {
            CMessageEventListEntry* const eventListHead = messageEventManager->Field04;

            for (CMessageEventListEntry* eventEntry = eventListHead->Next;
                 eventEntry != eventListHead;
                 eventEntry = eventEntry->Next)
            {
                if (&eventEntry->MessageObject == foundNode->Message)
                {
                    CMessageEventListEntry* const nextEntry = eventEntry->Next;
                    CMessageEventListEntry* const prevEntry = eventEntry->Prev;

                    prevEntry->Next = nextEntry;
                    nextEntry->Prev = prevEntry;

                    using RawMessageMethod = void(__thiscall*)(void*, int);
                    auto* const messageObject = &eventEntry->MessageObject;
                    reinterpret_cast<RawMessageMethod>(messageObject->Vftable[0])(messageObject, 0);

                    std::free(eventEntry);
                    break;
                }
            }
        }
    }

    CMessageEventConstPtrListNode* listNode = foundMessagesHead->Next;
    while (listNode != foundMessagesHead)
    {
        CMessageEventConstPtrListNode* const nextNode = listNode->Next;
        std::free(listNode);
        listNode = nextNode;
    }

    foundMessagesHead->Next = foundMessagesHead;
    foundMessagesHead->Prev = foundMessagesHead;
    std::free(foundMessagesHead);
}