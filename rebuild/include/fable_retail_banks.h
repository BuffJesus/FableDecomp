#pragma once

#include <stddef.h>
#include <stdlib.h>
#include <utility>

#include "fable_base_class.h"
#include "fable_counted_pointer.h"
#include "fable_string.h"
#include "fable_threaded_file.h"

// FableWin ?GUseRetailBanks@@3_NA (bool GUseRetailBanks), retail 0x013B8616:
// CWorld::ConvertWorldToStaticMapPath/ConvertWorldToISMThingyPath (0x0049DDD0 /
// 0x0049DEC0) pass it where FableWin passes GUseRetailBanks. fable_gfmain.h
// used throughout the native startup and frontend path as one physical byte.
extern bool GUseRetailBanks;

class CBasicRetailBankAliasMap
{
public:
    typedef std::pair<const CCharString, CCharString> value_type;
    struct CNode
    {
        bool color_;
        CNode* parent_;
        CNode* left_;
        CNode* right_;
        std::pair<const CCharString, CCharString> value_;
    };
    class iterator
    {
    public:
        iterator() {}
        iterator(CNode* node) { node_ = node; }
        iterator(const iterator& other) { node_ = other.node_; }
        value_type* operator->() const { return &node_->value_; }
        bool operator==(const iterator& other) const { return node_ == other.node_; }
        iterator& operator++();
        iterator& operator--();
        CNode* node_;
    };
    iterator end() { return iterator(head_); }
    iterator lower_bound(const CCharString& key) { return iterator(LowerBound(key)); }
    iterator insert(iterator hint, const value_type& value) { return InsertUnique(hint, value); }
    CNode* Find(const CCharString& key) const;
    CNode* LowerBound(const CCharString& key) const;
    iterator InsertNode(CNode* position, CNode* parent,
                        const value_type& value, fable_u32 forceRight);
    std::pair<iterator, bool> InsertUnique(const value_type& value);
    iterator InsertUnique(iterator hint, const value_type& value);
    __forceinline CCharString Resolve(const CCharString& key) const
    {
        CNode* node = Find(key);
        if (node != head_)
            return node->value_.second;
        return key;
    }
    CCharString& operator[](const CCharString& key);
    void Erase(CNode* node);
    void clear()
    {
        if (size_ != 0)
        {
            Erase(head_->parent_);
            head_->left_ = head_;
            head_->parent_ = 0;
            head_->right_ = head_;
            size_ = 0;
        }
    }
    CNode* head_;
    fable_u32 size_;
    fable_u32 compare_;
};

class CBasicRetailBankManager
{
public:
    void SetNavigatorEnabled(bool enabled);
    void AddBankAlias(
        const CCharString& genericName,
        const CCharString& platformName);

    fable_u8 unknown00_[0x14];
    bool navigatorEnabled_;
    fable_u8 unknown15_[3];
    CBasicRetailBankAliasMap aliases_;
};

FABLE_STATIC_ASSERT(
    offsetof(CBasicRetailBankManager, navigatorEnabled_) == 0x14);
FABLE_STATIC_ASSERT(
    offsetof(CBasicRetailBankManager, aliases_) == 0x18);

class CDiskFileWin32;

namespace NBankFileManager
{
    class CRetailBank;
}

// Releases the shared block when the last owner goes (0x9A9BB0's body).
#define FABLE_COUNTED_POINTER_RELEASE() \
    if (Info) \
    { \
        --Info->RefCount; \
        if (Info->RefCount == 0) \
        { \
            Info->DeleteFunc(Info->Data); \
            delete Info; \
        } \
    }

// The retail instantiations OpenRetailBank uses. Their bodies are visible
// here, as the original header templates were, so VC7.1 can see that the
// constructors, operator= and push_back never let a local's address escape;
// that is why retail's OpenRetailBank keeps counted-pointer fields in
// registers across calls. Retail still calls them rather than inlining.
template<>
class CCountedPointer<CDiskFileWin32>
{
public:
    CCountedPointer() : Data(0), Info(0) {}
    // The retail read-open path takes a temporary shared disk owner by value.
    CCountedPointer(const CCountedPointer& other)
        : Data(other.Data), Info(other.Info)
    {
        if (Info) ++Info->RefCount;
    }
    explicit CCountedPointer(CDiskFileWin32* data);          // 0x9A9B60
    ~CCountedPointer();                                       // 0x9A9BB0
    CCountedPointer& operator=(const CCountedPointer& other); // 0x9A9BF0
    CDiskFileWin32* operator->() const { return Data; }

    static void FABLE_FASTCALL DeleteData(void* data);        // 0x9A9030

    CDiskFileWin32* Data;
    CCPPointerInfo* Info;
};

inline CCountedPointer<CDiskFileWin32>::CCountedPointer(CDiskFileWin32* data)
{
    Data = data;
    if (Data)
        Info = new CCPPointerInfo(&DeleteData, Data);
    else
        Info = 0;
}

inline CCountedPointer<CDiskFileWin32>::~CCountedPointer()
{
    FABLE_COUNTED_POINTER_RELEASE()
    Data = 0;
    Info = 0;
}

inline CCountedPointer<CDiskFileWin32>&
CCountedPointer<CDiskFileWin32>::operator=(const CCountedPointer& other)
{
    CDiskFileWin32* data = other.Data;
    CCPPointerInfo* info = other.Info;
    if (Info != info)
    {
        FABLE_COUNTED_POINTER_RELEASE()
        Data = data;
        Info = info;
        if (Info)
            ++Info->RefCount;
    }
    return *this;
}

template<>
class CCountedPointer<NBankFileManager::CRetailBank>
{
public:
    CCountedPointer() : Data(0), Info(0) {}
    CCountedPointer(const CCountedPointer& other)
        : Data(other.Data), Info(other.Info)
    {
        if (Info) ++Info->RefCount;
    }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        NBankFileManager::CRetailBank* data = other.Data;
        CCPPointerInfo* info = other.Info;
        if (Info != info) {
            FABLE_COUNTED_POINTER_RELEASE()
            Data = data;
            Info = info;
            if (Info) ++Info->RefCount;
        }
        return *this;
    }
    explicit CCountedPointer(NBankFileManager::CRetailBank* data); // 0x9A9D10
    ~CCountedPointer();
    NBankFileManager::CRetailBank* operator->() const { return Data; }

    static void FABLE_FASTCALL DeleteData(void* data);        // 0x9A90B0

    NBankFileManager::CRetailBank* Data;
    CCPPointerInfo* Info;
};

inline CCountedPointer<NBankFileManager::CRetailBank>::CCountedPointer(
    NBankFileManager::CRetailBank* data)
{
    Data = data;
    if (Data)
        Info = new CCPPointerInfo(&DeleteData, Data);
    else
        Info = 0;
}

inline CCountedPointer<NBankFileManager::CRetailBank>::~CCountedPointer()
{
    FABLE_COUNTED_POINTER_RELEASE()
    Data = 0;
    Info = 0;
}

#undef FABLE_COUNTED_POINTER_RELEASE

namespace NBankFileManager
{
    struct CContainedBank
    {
        CContainedBank()
            : dataVersion_(0),
              entryCount_(0),
              infoOffset_(0),
              infoSize_(0),
              alignment_(0)
        {
        }

        fable_u32 dataVersion_;
        fable_u32 entryCount_;
        fable_u32 infoOffset_;
        fable_u32 infoSize_;
        fable_u32 alignment_;
    };

    // std::map<CCharString, CContainedBank> as the retail STL lays it out:
    // a malloc'd header node whose parent is the root and whose left/right
    // are the leftmost/rightmost nodes.
    class CContainedBankMap
    {
    public:
        typedef std::pair<const CCharString, CContainedBank> value_type;

        struct CNode
        {
            bool color_;
            CNode* parent_;
            CNode* left_;
            CNode* right_;
            value_type value_;
        };

        class iterator
        {
        public:
            iterator() {}
            iterator(CNode* node) { node_ = node; }
            iterator(const iterator& other) { node_ = other.node_; }
            value_type* operator->() const { return &node_->value_; }
            iterator& operator++();
            iterator& operator--();
            bool operator==(const iterator& other) const
            {
                return node_ == other.node_;
            }

            CNode* node_;
        };

        CContainedBankMap() : head_(0)
        {
            head_ = static_cast<CNode*>(malloc(sizeof(CNode)));
            size_ = 0;
            head_->color_ = false;
            head_->parent_ = 0;
            head_->left_ = head_;
            head_->right_ = head_;
        }

        ~CContainedBankMap()
        {
            if (size_ != 0)
                Erase(head_->parent_);
            if (head_ != 0)
                free(head_);
        }

        iterator end() { return iterator(head_); }
        iterator lower_bound(const CCharString& key)
        {
            return iterator(LowerBound(key));
        }
        iterator insert(iterator hint, const value_type& value)
        {
            return InsertUnique(hint, value);
        }
        CContainedBank& operator[](const CCharString& key);

        CNode* LowerBound(const CCharString& key) const;
        CNode* Find(const CCharString& key) const;
        CNode* CreateNode(const value_type& value);
        iterator InsertNode(CNode* position, CNode* parent,
                            const value_type& value, fable_u32 forceRight);
        void Erase(CNode* node);
        std::pair<iterator, bool> InsertUnique(const value_type& value);
        iterator InsertUnique(iterator hint, const value_type& value);

        CNode* head_;
        fable_u32 size_;
        fable_u32 allocator_;
    };

    class CRetailBank : public CBaseClassNonCopyable
    {
    public:
        CRetailBank();
        virtual ~CRetailBank();

        __forceinline bool ContainsBank(const CCharString& handle) const
        {
            return containedBanks_.Find(handle) != containedBanks_.head_;
        }

        CCountedPointer<CDiskFileWin32> diskFile_;
        CCountedPointer<CThreadedFile> threadedFile_;
        bool useDma_;
        CContainedBankMap containedBanks_;
        CCharString filePath_;
    };
}

FABLE_STATIC_ASSERT(sizeof(NBankFileManager::CContainedBank) == 0x14);
FABLE_STATIC_ASSERT(sizeof(NBankFileManager::CContainedBankMap::CNode) == 0x28);
FABLE_STATIC_ASSERT(sizeof(NBankFileManager::CContainedBankMap) == 0x0C);
FABLE_STATIC_ASSERT(sizeof(NBankFileManager::CRetailBank) == 0x28);
FABLE_STATIC_ASSERT(
    offsetof(NBankFileManager::CRetailBank, diskFile_) == 0x04);
FABLE_STATIC_ASSERT(
    offsetof(NBankFileManager::CRetailBank, threadedFile_) == 0x0C);
FABLE_STATIC_ASSERT(
    offsetof(NBankFileManager::CRetailBank, useDma_) == 0x14);
FABLE_STATIC_ASSERT(
    offsetof(NBankFileManager::CRetailBank, containedBanks_) == 0x18);
FABLE_STATIC_ASSERT(
    offsetof(NBankFileManager::CRetailBank, filePath_) == 0x24);

class CBankFile;
class CDataInputStream;

// The manager's 0x28 map has 0x14-byte nodes containing one CWideString.
class CBankManagerWidePathSet
{
public:
    struct CNode
    {
        bool color_;
        CNode* parent_;
        CNode* left_;
        CNode* right_;
        CWideString value_;
    };
    void Erase(CNode* node);
    void clear()
    {
        if (size_ != 0)
        {
            Erase(head_->parent_);
            head_->left_ = head_;
            head_->parent_ = 0;
            head_->right_ = head_;
            size_ = 0;
        }
    }
    CNode* head_;
    fable_u32 size_;
    fable_u32 compare_;
};

class CBankFileManager : public CBaseClass
{
public:
    CBankFileManager();
    virtual ~CBankFileManager();
    bool BankExists(const CCharString& bankHandle) const;
    class CBankInfo
    {
    public:
        CBankInfo() : RegisteredBank(0) {}
        CBankInfo(
            const CWideString& devBankFilePath,
            const CWideString& retailBankFilePath,
            const CWideString& devHeaderFilePath,
            const CWideString& retailHeaderFilePath)
            : DevBankFilePath(devBankFilePath),
              RetailBankFilePath(retailBankFilePath),
              DevHeaderFilePath(devHeaderFilePath),
              RetailHeaderFilePath(retailHeaderFilePath),
              RegisteredBank(0)
        {
        }

        CWideString DevBankFilePath;
        CWideString RetailBankFilePath;
        CWideString DevHeaderFilePath;
        CWideString RetailHeaderFilePath;
        CBankFile* RegisteredBank;
    };

    // std::map<CCharString, CBankInfo> in the retail (STLport) shape: a
    // malloc'd header node whose parent is the root and whose left/right are
    // the leftmost/rightmost nodes.
    class CBankInfoMap
    {
    public:
        typedef std::pair<const CCharString, CBankInfo> value_type;
        struct CNode
        {
            bool color_;
            CNode* parent_;
            CNode* left_;
            CNode* right_;
        };

        void clear()
        {
            if (size_ != 0)
            {
                Erase(head_->parent_);
                head_->left_ = head_;
                head_->parent_ = 0;
                head_->right_ = head_;
                size_ = 0;
            }
        }

        struct CEntryNode : CNode
        {
            CEntryNode(const CCharString& key, const CBankInfo& value)
                : value_(key, value) {}
            std::pair<const CCharString, CBankInfo> value_;
        };

        class iterator
        {
        public:
            iterator() {}
            iterator(CNode* node) { node_ = node; }
            iterator(const iterator& other) { node_ = other.node_; }
            value_type* operator->() const
            {
                return &static_cast<CEntryNode*>(node_)->value_;
            }
            bool operator==(const iterator& other) const { return node_ == other.node_; }
            iterator& operator++();
            iterator& operator--();
            CNode* node_;
        };

        iterator end() { return iterator(head_); }
        iterator lower_bound(const CCharString& key) { return iterator(LowerBound(key)); }
        iterator insert(iterator hint, const value_type& value) { return InsertUnique(hint, value); }
        iterator InsertNode(CNode* position, CNode* parent,
                            const value_type& value, fable_u32 forceRight);
        std::pair<iterator, bool> InsertUnique(const value_type& value);
        iterator InsertUnique(iterator hint, const value_type& value);

        CBankInfo& operator[](const CCharString& key);  // 0x9AC5D0
        CNode* Find(const CCharString& key) const;
        CNode* LowerBound(const CCharString& key) const;
        void Erase(CNode* node);                         // 0x9AA7C0

        CNode* head_;
        fable_u32 size_;
        fable_u32 compare_;
    };

    // STLport list<CCountedPointer<NBankFileManager::CRetailBank> >: one
    // pointer to a malloc'd circular sentinel node.
    class CRetailBankList
    {
    public:
        struct CNode
        {
            CNode* next_;
            CNode* prev_;
            CCountedPointer<NBankFileManager::CRetailBank> value_;
        };

        // 0x9AAAA0; retail calls it rather than inlining.
        void push_back(
            const CCountedPointer<NBankFileManager::CRetailBank>& value)
        {
            CNode* head = node_;
            CNode* node = static_cast<CNode*>(malloc(sizeof(CNode)));
            if (&node->value_ != 0)
            {
                node->value_.Data = value.Data;
                node->value_.Info = value.Info;
                if (value.Info)
                    ++value.Info->RefCount;
            }
            node->next_ = head;
            node->prev_ = head->prev_;
            head->prev_->next_ = node;
            head->prev_ = node;
        }

        // list::erase(iterator), inline in retail
        // (CBankFileManager::CloseRetailBank 0x009A78D0): unlink, release the
        // counted pointer, free the node.
        void erase(CNode* node)
        {
            CNode* next = node->next_;
            CNode* prev = node->prev_;
            prev->next_ = next;
            next->prev_ = prev;
            node->value_.~CCountedPointer<NBankFileManager::CRetailBank>();
            free(node);
        }

        CNode* node_;
        void clear();
    };

    void OpenRetailBank(
        const CWideString& pathname,
        bool startupPriority);
    // FableWin ?CloseRetailBank@CBankFileManager@@QAEXABVCWideString@@@Z (0x009A78D0).
    void CloseRetailBank(const CWideString& pathname);
    // Ego_r ?GetDevHeaderFilePath@CBankFileManager@@QBE?AVCWideString@@ABVCCharString@@@Z and
    // ?GetRetailHeaderFilePath@...@@QBE?AVCWideString@@ABVCCharString@@@Z, retail 0x009A7E20 / 0x009A7ED0.
    CWideString GetDevHeaderFilePath(const CCharString& bank_handle) const;
    CWideString GetRetailHeaderFilePath(const CCharString& bank_handle) const;
    // Ego_r ?GetDevBankFilePath@CBankFileManager@@QBE?AVCWideString@@ABVCCharString@@@Z, retail 0x009A7CA0.
    CWideString GetDevBankFilePath(const CCharString& bank_handle) const;
    // Ego_r ?LookupBankAlias@CBankFileManager@@QBE?AVCCharString@@ABV2@@Z, retail 0x009A7C60.
    CCharString LookupBankAlias(const CCharString& bank_handle) const;
    // Retail 009A7F80: resolve one alias, then return the first contained bank.
    bool FindContainedBank(const CCharString&, NBankFileManager::CContainedBank&,
        CCountedPointer<NBankFileManager::CRetailBank>&);
    void OpenIniFile(const CWideString& pathname);
    void Close();

private:
    friend class CBankFileManagerTestAccess;
    bool ReadQuotedString(CCharString& text, CDataInputStream& stream);

public:
    CBankInfoMap Banks;
    CRetailBankList RetailBanks;
    bool UseRetailBanks;
    fable_u8 padding15_[3];
    CBasicRetailBankAliasMap Aliases;
    CWideString& BasePath()
    {
        return *reinterpret_cast<CWideString*>(basePathStorage_);
    }
    const CWideString& BasePath() const
    {
        return *reinterpret_cast<const CWideString*>(basePathStorage_);
    }
    fable_u8 basePathStorage_[sizeof(CWideString)];
    CBankManagerWidePathSet WidePaths;
    fable_u32 retailBankVersion_;
};

FABLE_STATIC_ASSERT(sizeof(CBankFileManager::CBankInfo) == 0x14);
FABLE_STATIC_ASSERT(
    offsetof(CBankFileManager, Banks) == 0x04);
FABLE_STATIC_ASSERT(
    offsetof(CBankFileManager, RetailBanks) == 0x10);
FABLE_STATIC_ASSERT(
    offsetof(CBankFileManager, basePathStorage_) == 0x24);
FABLE_STATIC_ASSERT(
    offsetof(CBankFileManager, retailBankVersion_) == 0x34);

FABLE_STATIC_ASSERT(sizeof(CBasicRetailBankAliasMap) == 0x0C);
FABLE_STATIC_ASSERT(offsetof(CBasicRetailBankAliasMap::CNode, value_) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CBankFileManager, UseRetailBanks) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CBankFileManager, Aliases) == 0x18);
FABLE_STATIC_ASSERT(offsetof(CBankFileManager, WidePaths) == 0x28);
