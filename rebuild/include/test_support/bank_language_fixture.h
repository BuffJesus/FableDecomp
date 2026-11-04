#include "fable_retail_banks.h"
#if defined(FABLE_USE_NATIVE_STRINGS)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
fable_i32 g_CCharStringInstanceCount_013BD800 = 0;
char g_FableEmptyNarrowString_0129AAF4[1] = {0};
static void require(bool value) {if (!value) {puts("native bank lookup mismatch"); exit(1);}}
static CCharStringData* dataOf(const CCharString& text)
{return *reinterpret_cast<CCharStringData* const*>(&text);}
#else
#include "test_support/checksum_string_fixture.h"
#endif

namespace NLocalisation { unsigned long FABLE_FASTCALL GetLanguage(); }
unsigned long g_FableLanguage_013B8684;
CBankFileManager g_FableBankFileManager_013CA79C;

// String storage is fixture-managed; the actual CBasicString comparison is
// linked. Bank construction and insertion are fixture boundaries. The three
// tree lookups, alias resolution, BankExists and GetLanguage execute natively.
CWideString::CWideString() {}
CWideString::CWideString(const CWideString&) {}
CWideString::~CWideString() {}
CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
NBankFileManager::CRetailBank::CRetailBank() {}
NBankFileManager::CRetailBank::~CRetailBank() {free(containedBanks_.head_);}

typedef CBankFileManager::CBankInfoMap::CNode DevNode;
typedef CBankFileManager::CBankInfoMap::CEntryNode DevEntry;
typedef CBasicRetailBankAliasMap::CNode AliasNode;
typedef NBankFileManager::CContainedBankMap::CNode RetailNode;

static const char* key(const CCharString& value)
{
    CCharStringData* data = dataOf(value);
    return data ? data->text : "";
}
static const CCharString& keyOf(const DevNode* node)
{return static_cast<const DevEntry*>(node)->value_.first;}
static const CCharString& keyOf(const AliasNode* node) {return node->value_.first;}
static const CCharString& keyOf(const RetailNode* node) {return node->value_.first;}

template<class Node> static void resetHead(Node* head)
{
    head->parent_ = 0;
    head->left_ = head->right_ = head;
}
template<class Node> static void insert(Node* head, Node* node)
{
    node->left_ = node->right_ = 0;
    Node* parent = head;
    Node** link = &head->parent_;
    while (*link)
    {
        parent = *link;
        link = strcmp(key(keyOf(node)), key(keyOf(parent))) < 0 ? &parent->left_ : &parent->right_;
    }
    node->parent_ = parent;
    *link = node;
}

static void run()
{
    CBankFileManager& manager = g_FableBankFileManager_013CA79C;
    DevNode devHead;
    AliasNode aliasHead;
    resetHead(&devHead);
    resetHead(&aliasHead);
    manager.Banks.head_ = &devHead;
    manager.Aliases.head_ = &aliasHead;
    manager.UseRetailBanks = false;
    CWideString empty;
    CBankFileManager::CBankInfo info(empty, empty, empty, empty);
    static const char* const names[] = {
        "TEXT_ENGLISH_MAIN", "TEXT_FRENCH_MAIN", "TEXT_JAPANESE_MAIN",
        "TEXT_GERMAN_MAIN", "TEXT_KOREAN_MAIN", "TEXT_CHINESE_MAIN",
        "TEXT_SPANISH_MAIN", "TEXT_ITALIAN_MAIN"
    };
    static const unsigned long languages[] = {9,12,17,7,18,4,10,16};
    DevEntry* entries[8];
    for (unsigned i = 0; i != 8; ++i)
        entries[i] = new DevEntry(CCharString(names[i], -1), info);
    for (unsigned mask = 0; mask != 256; ++mask)
    {
        resetHead(&devHead);
        unsigned first = 8;
        for (unsigned i = 0; i != 8; ++i)
            if (mask & (1U << i))
            {
                if (first == 8) first = i;
                insert(&devHead, static_cast<DevNode*>(entries[i]));
            }
        g_FableLanguage_013B8684 = 0;
        unsigned long result = NLocalisation::GetLanguage();
        require(result == (first == 8 ? 0 : languages[first]));
        resetHead(&devHead);
        require(NLocalisation::GetLanguage() == result);
        g_FableLanguage_013B8684 = 0;
        require(NLocalisation::GetLanguage() == 0);
    }

    // One alias lookup, including a handle shared by identity with the key.
    AliasNode alias = {false,0,0,0,std::pair<const CCharString,CCharString>(
        CCharString("TEXT_FRENCH_MAIN",-1), CCharString("physical",-1))};
    DevEntry physical(CCharString("physical",-1), info);
    insert(&devHead, static_cast<DevNode*>(&physical));
    insert(&aliasHead, &alias);
    require(manager.BankExists(alias.value_.first));
    require(!manager.BankExists(CCharString("TEXT_ITALIAN_MAIN",-1)));
    g_FableLanguage_013B8684 = 0;
    require(NLocalisation::GetLanguage() == 12);

    // The retail path searches each contained-bank tree and ignores dev banks.
    NBankFileManager::CRetailBank firstBank, secondBank;
    // Raw list storage models borrowed banks: no counted owners are created.
    CBankFileManager::CRetailBankList::CNode* links =
        static_cast<CBankFileManager::CRetailBankList::CNode*>(calloc(3, sizeof(*links)));
    CBankFileManager::CRetailBankList::CNode& listHead = links[0];
    CBankFileManager::CRetailBankList::CNode& firstLink = links[1];
    CBankFileManager::CRetailBankList::CNode& secondLink = links[2];
    manager.RetailBanks.node_ = &listHead;
    listHead.next_ = &firstLink; listHead.prev_ = &secondLink;
    firstLink.next_ = &secondLink; firstLink.prev_ = &listHead;
    secondLink.next_ = &listHead; secondLink.prev_ = &firstLink;
    firstLink.value_.Data = &firstBank;
    secondLink.value_.Data = &secondBank;
    firstLink.value_.Info = secondLink.value_.Info = listHead.value_.Info = 0;
    RetailNode retail = {false,0,0,0,std::pair<const CCharString,NBankFileManager::CContainedBank>(
        CCharString("physical",-1), NBankFileManager::CContainedBank())};
    manager.UseRetailBanks = true;
    require(!manager.BankExists(alias.value_.first));
    insert(secondBank.containedBanks_.head_, &retail);
    require(manager.BankExists(alias.value_.first));
    g_FableLanguage_013B8684 = 0;
    require(NLocalisation::GetLanguage() == 12);
    listHead.next_ = &listHead;
    require(!manager.BankExists(alias.value_.first));

    // Null/same-owner keys exercise the inline string ordering shortcuts.
    resetHead(&aliasHead);
    AliasNode nullAlias;
    insert(&aliasHead, &nullAlias);
    require(manager.Aliases.Find(nullAlias.value_.first) == &nullAlias);
    require(manager.Aliases.Find(CCharString("a",-1)) == &aliasHead);
    free(links);
    for (unsigned i = 0; i != 8; ++i) delete entries[i];
}
int main()
{
    run();
#if defined(FABLE_USE_NATIVE_STRINGS)
    require(g_CCharStringInstanceCount_013BD800 == 0);
    puts("BANK_LANGUAGE_NATIVE_STRINGS_PASS subsets=256 aliases=1 modes=2 charInstances=0");
#else
    require(liveData == 0);
    puts("BANK_LANGUAGE_NATIVE_LOOKUPS_PASS subsets=256 aliases=1 modes=2 liveData=0");
#endif
    return 0;
}

#if !defined(FABLE_BANK_TEST_LANGUAGE)
#include "../../src/compiled/00/41/NLocalisation_GetLanguage_00415070.cpp"
#endif
#if !defined(FABLE_BANK_TEST_EXISTS)
#include "../../src/compiled/00/9a/CBankFileManager_BankExists_009a80b0.cpp"
#endif
#if !defined(FABLE_BANK_TEST_FIND)
#include "../../src/compiled/00/9a/STLBankMaps_Find_009ab4f0.cpp"
#endif
template bool CBasicString<char>::operator<(const CBasicString<char>& other) const;
