// Behaviour fixture for CBankFileManager::CloseRetailBank (0x009A78D0).
// Links the compiled source object. The retail-bank list is built from real
// malloc'd STLport-shaped nodes; banks are raw storage with a real
// CCharStringData path; ToCharString and ~CCharString are recording
// stand-ins; the comparison is the real CRT _stricmp.
#define private public
#include "fable_retail_banks.h"
#undef private

#include <stdio.h>
#include <string.h>

typedef CBankFileManager::CRetailBankList::CNode Node;

static int g_toChar, g_strDtor;
static const CWideString* g_toCharThis;
static CCharStringData g_nameData;
static char g_nameText[64];

CCharString CWideString::ToCharString() const
{
    ++g_toChar;
    g_toCharThis = this;
    return CCharString(static_cast<const char*>(0), -7);
}

// Stand-ins: the (text, length) form builds the converted name; the copy
// takes the handle without counting.
CCharString::CCharString(const char*, long) : storage_(&g_nameData) {}
CCharString::CCharString(const CCharString& other) : storage_(other.storage_) { const_cast<CCharString&>(other).storage_ = 0; }
char g_FableEmptyNarrowString_0129AAF4[1] = "";

CCharString::CCharString() : storage_(0) {}
CWideString::CWideString() : storage_(0) {}
CWideString::~CWideString() {}
CCharString::~CCharString()
{
    if (storage_ == &g_nameData)
        ++g_strDtor;
}

static int g_deleted[8];
static int g_deleteCount;

static void FABLE_FASTCALL DeleteBank(void* data)
{
    g_deleted[g_deleteCount++] = *static_cast<int*>(data);
}

struct BankStorage
{
    int id;                        // +0x00 (stands in for the vptr slot)
    unsigned char pad[0x20];
    CCharStringData* path;         // +0x24 filePath_
};

static BankStorage g_banks[3];
static CCharStringData g_paths[3];
static char g_pathText[3][64];

static Node* MakeNode(int index, unsigned long refs)
{
    Node* node = static_cast<Node*>(malloc(sizeof(Node)));
    g_banks[index].id = 100 + index;
    g_banks[index].path = &g_paths[index];
    node->value_.Data = reinterpret_cast<NBankFileManager::CRetailBank*>(&g_banks[index]);
    node->value_.Info = new CCPPointerInfo(&DeleteBank, &g_banks[index]);
    node->value_.Info->RefCount = refs;
    return node;
}

static void Link(Node* head, Node** nodes, int count)
{
    Node* prev = head;
    for (int i = 0; i < count; ++i)
    {
        prev->next_ = nodes[i];
        nodes[i]->prev_ = prev;
        prev = nodes[i];
    }
    prev->next_ = head;
    head->prev_ = prev;
}

static void SetText(CCharStringData& data, char* buffer, const char* text)
{
    strcpy(buffer, text);
    data.text = buffer;
    data.unknown04 = (fable_u32)strlen(text);
    data.unknown08 = 0;
    data.flags0C = 0;
    data.owners = 1;
}

int main()
{
    int fail = 0;
    const char* paths[3] = { "Data\\Levels.big", "Data\\Graphics.BIG", "data\\graphics.big" };
    for (int i = 0; i < 3; ++i)
        SetText(g_paths[i], g_pathText[i], paths[i]);

    unsigned char managerRaw[sizeof(CBankFileManager)];
    memset(managerRaw, 0xAB, sizeof(managerRaw));
    CBankFileManager* manager = reinterpret_cast<CBankFileManager*>(managerRaw);
    unsigned long headRaw[sizeof(Node) / 4];
    Node& head = *reinterpret_cast<Node*>(headRaw);
    manager->RetailBanks.node_ = &head;

    // Last owner: the first case-insensitive match is unlinked and released.
    Node* nodes[3] = { MakeNode(0, 1), MakeNode(1, 1), MakeNode(2, 1) };
    Link(&head, nodes, 3);
    SetText(g_nameData, g_nameText, "DATA\\GRAPHICS.big");
    CWideString path;
    manager->CloseRetailBank(path);
    if (g_toChar != 1 || g_toCharThis != &path || g_strDtor != 1) fail |= 1;
    if (head.next_ != nodes[0] || nodes[0]->next_ != nodes[2] || nodes[2]->prev_ != nodes[0] ||
        nodes[2]->next_ != &head || head.prev_ != nodes[2]) fail |= 2;
    if (g_deleteCount != 1 || g_deleted[0] != 101) fail |= 4;
    if (managerRaw[0] != 0xAB || managerRaw[sizeof(managerRaw) - 1] != 0xAB) fail |= 8;

    // Shared bank: unlinked, reference dropped, bank kept alive.
    g_deleteCount = 0;
    CCPPointerInfo* info2 = nodes[2]->value_.Info;
    info2->RefCount = 2;
    manager->CloseRetailBank(path);
    if (head.next_ != nodes[0] || nodes[0]->next_ != &head || head.prev_ != nodes[0]) fail |= 16;
    if (g_deleteCount != 0 || info2->RefCount != 1 || g_strDtor != 2) fail |= 32;

    // No match: list untouched, the name still destroyed.
    SetText(g_nameData, g_nameText, "Data\\Missing.big");
    manager->CloseRetailBank(path);
    if (head.next_ != nodes[0] || nodes[0]->next_ != &head || g_deleteCount != 0 || g_strDtor != 3) fail |= 64;

    // Empty list: nothing to do.
    unsigned long emptyRaw[sizeof(Node) / 4];
    Node& empty = *reinterpret_cast<Node*>(emptyRaw);
    empty.next_ = empty.prev_ = &empty;
    manager->RetailBanks.node_ = &empty;
    manager->CloseRetailBank(path);
    if (empty.next_ != &empty || g_strDtor != 4 || g_toChar != 4) fail |= 128;

    if (fail == 0)
        printf("CBANKFILEMANAGER_CLOSERETAILBANK_PASS\n");
    else
        printf("CBANKFILEMANAGER_CLOSERETAILBANK_FAIL %d\n", fail);
    return 0;
}
